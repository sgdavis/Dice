// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerPlatform.h"
#include "Tower.generated.h"

class AMinion;

#define NORMALFIRINGDELAY 50

UCLASS()
class TOWERS_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();
	void SetPlatform(APlayerPlatform* platform);
	APlayerPlatform* GetPlatform();

	void FireProjectile();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireProjectile();

	UStaticMeshComponent* GetFiringPoint();

	void SetFiringDelay(int delay);

	AMinion* GetTarget() { return pCurrentTarget; }
	void ClearTarget() { pCurrentTarget = NULL; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APlayerPlatform* pPlatform;

	AMinion* pCurrentTarget;

	UStaticMeshComponent* pSwivelPoint;
	UStaticMeshComponent* pFiringPoint;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void TrackTarget(FVector targetLocation);

	UPROPERTY(Replicated)
	int firingDelay;
	int firingDelayCounter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
