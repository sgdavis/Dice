// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Complain.h"
#include "Bullet.generated.h"

UCLASS()
class SPAAACE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void SetSpeed(float newSpeed) { speed = newSpeed; }
    void SetDamage(float newDamage) { damage = newDamage; }

    UFUNCTION(BlueprintCallable, Category = "Bullet Functions")
        void HitSomething(AActor* otherActor, UActorComponent* otherComponent);

    float speed;
    float lifetime;
    int damage;

    bool bIsAIFired;
    void SetIsAIFired(bool fired) { bIsAIFired = fired; }
};
