// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperIndustryGameMode.h"
#include "SuperIndustryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

ASuperIndustryGameMode::ASuperIndustryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));

	static ConstructorHelpers::FObjectFinder<UClass> HUDFinder(TEXT("Blueprint'/Game/Blueprints/HUD/MainHUD.MainHUD_C'"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	HUDClass = (UClass*)HUDFinder.Object;
}
