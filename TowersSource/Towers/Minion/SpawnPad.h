// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Complain.h"
#include "SpawnPad.generated.h"

#define SPAWNCOUNTER 100

class APlayerPlatform;

UCLASS()
class TOWERS_API ASpawnPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int nCountdown;
	bool bCountingDown;

	UStaticMeshComponent* Light25;
	UStaticMeshComponent* Light50;
	UStaticMeshComponent* Light75;
	UStaticMeshComponent* Light100;

	AController* pMyPlayer;
	APlayerPlatform* pMyPlatform;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MinionSpawner")
	void StartCountdown(AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "MinionSpawner")
	void StopCountdown(AActor* otherActor);

	void SetValidPlayer(AController* player);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SetLightVisible(int number, bool visible);

	void SetPlatform(APlayerPlatform* platform);
	APlayerPlatform* GetPlatform();
};
