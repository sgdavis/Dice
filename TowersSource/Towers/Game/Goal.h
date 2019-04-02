// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerPlatform.h"
#include "Goal.generated.h"

UCLASS()
class TOWERS_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	UFUNCTION(BlueprintCallable, Category = "Goal")
	void OnDamageTaken(AActor* otherActor);
	void SetPlatform(APlayerPlatform* platform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int nHitPoints;
	APlayerPlatform* pPlatform;

	UStaticMeshComponent* HP1Sphere;
	UStaticMeshComponent* HP2Sphere;
	UStaticMeshComponent* HP3Sphere;
	UStaticMeshComponent* HP4Sphere;
	UStaticMeshComponent* HP5Sphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
