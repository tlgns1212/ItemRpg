// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}


UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* ItemIn)
{
	// check if in the input item has valid weight
	if(FMath::IsNearlyZero(ItemIn->GetItemStackWeight()) || ItemIn->GetItemStackWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight value"), ItemIn->TextData.Name));
	}

	// will the item weight overflow weight capacity
	if(InventoryTotalWeight + ItemIn->GetItemStackWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit"), ItemIn->TextData.Name));
	}

	// adding one more item would overflow slot capacity
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Inventory slots are full"), ItemIn->TextData.Name));
	}

	AddNewItem(ItemIn, 1);
	return FItemAddResult::AddedAll(1, FText::Format(
			FText::FromString("Successfully added {1} {0} to the inventory."), ItemIn->TextData.Name, 1));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if(RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		return 0;
	}
	int32 AmountToDistribute = RequestedAddAmount;

	// 인벤토리에 이미 아이템이 있는지 확인, 덜찬걸로
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	while(ExistingItemStack)
	{
		// 아이템이 꽉찰려면 몇개 더 필요한지
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// 인벤토리 무게 기준 몇개를 더 넣을 수 있는지
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// 인벤토리의 무게를 초과하지 않으면 더 추가 가능
		if(WeightLimitAddAmount > 0)
		{
			// 인벤토리 총 무게랑 ExistingItem 개수 조정
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// 변경 가능한 숫자 조정
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// 현재 무게에 하나 더 추가했을 때 총 무게를 넘기면 멈추기
			if(InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if(WeightLimitAddAmount <= 0)
		{
			if(AmountToDistribute != RequestedAddAmount)
			{
				// 반복문 돌리다가 무게가 꽉 차면
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if(AmountToDistribute <= 0)
		{
			// 모든 아이템이 이미 존재하는 아이템들에 골고루 분배되는 경우
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// 다른 PartialStack이 있는지 확인 후 반복문 나갈지 계속 돌릴지 확인
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// partial stack 더이상 찾을 수 없음, 새로운 Stack 생성
	if(InventoryContents.Num() + 1 <=InventorySlotsCapacity)
	{
		// 무게를 기준으로 추가할 수 있는 정도
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if(WeightLimitAddAmount > 0)
		{
			// 무게 때문에 다 넣지 못할 경우
			if(WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// 부분적으로만 인벤토리에 추가되었으므로 Copy를 생성
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// 다 넣었을 경우 인벤토리에 추가
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
		
		return RequestedAddAmount - AmountToDistribute;
	}

	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if(GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable items
		if(!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. Get Owner() check somehow failed"));
}


void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if(Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy of is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splitting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}

