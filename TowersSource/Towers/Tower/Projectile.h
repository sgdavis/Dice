// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Complain.h"
#include "Projectile.generated.h"

UCLASS()
class TOWERS_API AProjectile : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void OnImpact(AActor* otherActor);

	void SetStats(int damage, int speed);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
		float movementSpeed;
	UPROPERTY(Replicated)
		int damageValue;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
