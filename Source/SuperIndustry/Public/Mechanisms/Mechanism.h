// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mechanism.generated.h"

UCLASS()
class SUPERINDUSTRY_API AMechanism : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMechanism();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float ThisDeltaTime;

	// Making variables
	FString* mechanismTitle; // Mechanism title =)
	float* energy; // Current energy (unit of energy - SEP Superindustry Energy Points)
	float* maxEnergy;
	bool* canGiveEnergy;
	int EnergyToGive; // SEP to give for 1 mechanism
	int* maxGivePerTick; // Max SEP giving for 1 mechanism per tick
	int* powerConsumption; // SEP consumption per tick
	bool* IsPaused;

	TArray<FString> currentStatus; // mechanismTitle, energy
	

	// Making functions
	UFUNCTION(BlueprintCallable) virtual void ShowWidget();
	UFUNCTION(BlueprintCallable) virtual void HideWidget();

	// Interacting
	virtual int AcceptEnergy(int EnergyToAdd);
	virtual void GiveEnergy(TArray<AMechanism*> ConnectedMechanisms);
	virtual void SetPause(bool pause);
	virtual bool Work();

	// Get mechanismTitle, energy
	UFUNCTION(BlueprintCallable) virtual TArray<FString> GetStatus();
};
