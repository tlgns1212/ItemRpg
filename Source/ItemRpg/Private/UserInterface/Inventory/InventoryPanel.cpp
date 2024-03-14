// Fill out your copyright notice in the Description page of Project Settings.


// game
#include "UserInterface/Inventory/InventoryPanel.h"
#include "Components/InventoryComponent.h"
#include "Player/ItemRpgCharacter.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"


// engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AItemRpgCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue{
	FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity())};

	const FString CapacityInfoValue{
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
			+ FString::FromInt(InventoryReference->GetSlotsCapacity())};
	
	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UInventoryPanel::RefreshInventory()
{
	if(InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();

		for(UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}
	}
	SetInfoText();
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if(ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on Inventory Panel."));

		// true를 반환하면 현재 위젯에서 drop operation을 멈추고
		return true;
	}
	// false를 반환하면 현재 위젯을 건너뛰고 겹쳐져 있는 다음 위젯으로 drop operation을 넘긴다
	return false;
}
