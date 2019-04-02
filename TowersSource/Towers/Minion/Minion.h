// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Complain.h"
#include "Net/UnrealNetwork.h"
#include "Minion.generated.h"

class APlayerPlatform;

UCLASS()
class TOWERS_API AMinion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinion();

	UFUNCTION(BlueprintCallable, Category = "Minion")
		void OnDamageDelivered(AActor* otherActor);

	void OnDamageTaken(int amount);

	int GetDamage() { return damageValue; }
	int GetMovementSpeed() { return movementSpeed; }
	int GetHitPoints() { return hitPoints; }
	int GetReward() { return rewardGold; }

	void SetStats(int damage, int speed, int hp, int reward);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPlatform(APlayerPlatform* platform) { pMyPlatform = platform; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
		int damageValue;
	UPROPERTY(Replicated)
		int hitPoints;
	UPROPERTY(Replicated)
		float movementSpeed;
	UPROPERTY(Replicated)
		int rewardGold;

	APlayerPlatform* pMyPlatform;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
