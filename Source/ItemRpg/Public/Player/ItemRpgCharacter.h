// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/InteractionInterface.h"
#include "ItemRpgCharacter.generated.h"

class UTimelineComponent;
class UItemBase;
class UInventoryComponent;
class AItemRpgHUD;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config=Game)
class AItemRpgCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	bool bAiming;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	AItemRpgCharacter();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

protected:
	//===============================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================
	UPROPERTY()
	AItemRpgHUD* HUD;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Inventory Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	/** Aim Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;
	
	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	// timeline properties for camera aiming transition
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector AimingCameraLocation;

	TObjectPtr<UTimelineComponent> AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category="Character | Aim Timeline")
	UCurveFloat* AimingCameraCurve;
	
	//===============================================================================
	// FUNCTIONS
	//===============================================================================
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void ToggleMenu();

	void StartAiming();
	void StopAiming();
	UFUNCTION()
	void UpdateCameraTimeline(float TimelineValue) const;
	UFUNCTION()
	void CameraTimelineEnd();
	
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};

