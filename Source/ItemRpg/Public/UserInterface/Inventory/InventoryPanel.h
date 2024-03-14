// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UTextBlock;
class UWrapBox;
class UInventoryItemSlot;
class UInventoryComponent;
class AItemRpgCharacter;

UCLASS()
class ITEMRPG_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryWrapBox;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightInfo;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;
	
	UPROPERTY()
	AItemRpgCharacter* PlayerCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryReference;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	UFUNCTION()
	void RefreshInventory();

protected:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	
	void SetInfoText() const;
};
