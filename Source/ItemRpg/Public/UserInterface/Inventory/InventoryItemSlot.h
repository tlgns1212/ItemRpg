// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UInventoryToolTip;
class UDragItemVisual;
class UItemBase;
/**
 * 
 */
UCLASS()
class ITEMRPG_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

protected:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryToolTip> ToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UTextBlock* ItemQuantity;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
