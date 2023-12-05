// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanisms/Mechanism.h"
#include "PocketStotage.generated.h"

UCLASS()
class SUPERINDUSTRY_API APocketStotage : public AMechanism
{
	GENERATED_BODY()

public:
	APocketStotage();

protected:
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;
	float MyDeltaTime;

	// Making variables
	float MyEnergy = 0;
	float MyMaxEnergy = 500;
	FString MyMechanismTitle = FString(TEXT("Pocket storage"));
	
	bool ThisCanGiveEnergy = false;
	int MyPowerConsumption = 20;
	
	TArray<FString> currentStatus;

	// Mechanism widget
	UPROPERTY() // UPROPERTY, because UE delete variables
	FSoftClassPath WidgetClassRef = TEXT("WidgetBlueprint'/Game/Blueprints/HUD/Mechanisms/PocketStorage/PocketStorageHUD.PocketStorageHUD_C'"); // Widget BP class reference in a string
	UPROPERTY()
	UClass* WidgetClass;
	UPROPERTY()
	UUserWidget* Widget; // Widget for adding to viewport
	
	// Making functions

	// Interacting
	int AcceptEnergy(int) override;
	UFUNCTION(BlueprintCallable) bool Work() override;

	void ShowWidget() override;
	void HideWidget() override;

	TArray<FString> GetStatus() override;
};

