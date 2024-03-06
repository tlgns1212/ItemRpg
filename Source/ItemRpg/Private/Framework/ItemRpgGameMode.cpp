// Copyright Epic Games, Inc. All Rights Reserved.

// #include "ItemRpgGameMode.h"
#include "ItemRpg/Public/Framework/ItemRpgGameMode.h"

// #include "ItemRpgCharacter.h"
#include "ItemRpg/Public/Player/ItemRpgCharacter.h"
#include "UObject/ConstructorHelpers.h"

AItemRpgGameMode::AItemRpgGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
