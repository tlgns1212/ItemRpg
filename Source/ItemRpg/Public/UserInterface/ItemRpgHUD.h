// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ItemRpgHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
/**
 * 
 */
UCLASS()
class ITEMRPG_API AItemRpgHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	
	bool bIsMenuVisible;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	AItemRpgHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowCrosshair();
	void HideCrosshair();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	virtual void BeginPlay() override;
};
