// Fill out your copyright notice in the Description page of Project Settings.

#include "Mechanisms/Mechanism.h"
#include "Kismet/GameplayStatics.h"

// We need to include a character class
#include "SuperIndustry/SuperIndustryCharacter.h"

// Sets default values
AMechanism::AMechanism()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMechanism::BeginPlay() 
{ 
	Super::BeginPlay();
}

// Called every frame
void AMechanism::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	ThisDeltaTime = DeltaTime;
}

int AMechanism::AcceptEnergy(int EnergyToAdd)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%i"), EnergyToAdd));
	if (*energy + EnergyToAdd < *maxEnergy)
	{
		*energy += EnergyToAdd;
		return 0;
	}
	
	else
	{
		EnergyToAdd = EnergyToAdd - (*maxEnergy - *energy);
		*energy = *maxEnergy;
		
		return EnergyToAdd;
	}
}

void AMechanism::GiveEnergy(TArray<AMechanism*> ConnectedMechanisms)
{
	if (canGiveEnergy)
	{
		for (AMechanism* Mechanism : ConnectedMechanisms)
		{
			EnergyToGive = ((int)*energy)*ThisDeltaTime / ConnectedMechanisms.Num();
			if (EnergyToGive > *maxGivePerTick)
			{
				EnergyToGive = *maxGivePerTick*ThisDeltaTime;
			}

			*energy -= EnergyToGive;
			*energy += Mechanism->AcceptEnergy(EnergyToGive);
		}
	}
}

void AMechanism::SetPause(bool pause) { }

bool AMechanism::Work()
{
	if (IsPaused == false)
	{
		if (*energy - *powerConsumption * ThisDeltaTime > 0)
		{
			*energy -= *powerConsumption * ThisDeltaTime;
			return true;
		}
	}
	return false;
}

TArray<FString> AMechanism::GetStatus()
{
	currentStatus.Empty(); // Clear array

	currentStatus.Emplace(*mechanismTitle); // Title
	currentStatus.Emplace(FString::SanitizeFloat(*energy / *maxEnergy)); // Energy percentage
	
	return currentStatus;
}

void AMechanism::ShowWidget() 
{
	Cast<ASuperIndustryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Disable_Input();
	Cast<ASuperIndustryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->CenterCursor();
}

void AMechanism::HideWidget() 
{
	Cast<ASuperIndustryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Enable_Input();
} 