// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Complain.h"
#include "Equipment/RPGEquipment.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "RPGCreature.generated.h"

UCLASS()
class RPG_API ARPGCreature : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    int nStrength;
    int nIntelligence;
    int nDexterity;

    ARPGEquipment* pWeapon;
    ARPGEquipment* pBoots;
    ARPGEquipment* pOffhand;

    UStaticMeshComponent* pWeaponSpawnPoint;
    UStaticMeshComponent* pOffhandSpawnPoint;
    UStaticMeshComponent* pBootsSpawnPoint;

    bool bStartingJump;
    bool bStartingSwing;
    bool bStartingBlock;
    bool bStartingRightTurn;
    bool bStartingLeftTurn;
    bool bStartingForward;
    bool bStartingBackUp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetCreatureStates(int startingJump, int startingSwing, int startingBlock, int startingRightTurn, int startingLeftTurn, int startingForward, int startingBackUp);

    void WasHit();
};
