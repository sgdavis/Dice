// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Complain.h"
#include "Bullet.h"
#include "SpaaaceGameMode.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    speed = 3;
    firing = false;

    firingDelay = 50;
    firingTimer = 0;

    SetActorEnableCollision(true);

    bIsAIShip = false;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();	

    TArray<UStaticMeshComponent*> allMeshes;
    GetComponents<UStaticMeshComponent>(allMeshes);

    FString meshName;
    for (int i = 0; i < allMeshes.Num(); i++)
    {
        meshName = allMeshes[i]->GetFName().ToString();
        if (meshName.Compare("Body") == 0)
        {
            pBody = allMeshes[i];
            BodyRotation = pBody->GetComponentRotation();
        }
        else if (meshName.Left(11).Compare("FiringPoint") == 0)
        {
            FiringPoints.Add(allMeshes[i]);
        }
    }
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (firing)
    {
        if (firingTimer-- <= 0)
        {
            FireWeapon();
            firingTimer = firingDelay;
        }
    }
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShip::MovingForwardBack(float axisValue)
{
    FVector Destination = GetActorLocation() + axisValue * GetShipSpeed(axisValue) * GetActorForwardVector();
    SetActorLocation(Destination, true);
}

void AShip::MovingLeftRight(float axisValue)
{
    FVector Destination = GetActorLocation() + axisValue * GetBankSpeed(axisValue) * GetActorRightVector();
    SetActorLocation(Destination, true);
    HandleBankAngle(axisValue);
}

void AShip::HandleBankAngle(float axisValue)
{
    if (axisValue > 0)
    {
        if (Roll >= 0)
        {
            Roll += 1;
        }
        else
        {
            Roll += 3;
        }

        if (Roll > BANK_ROTATION)
        {
            Roll = BANK_ROTATION;
        }
    }
    else if (axisValue < 0)
    {
        if (Roll <= 0)
        {
            Roll -= 1;
        }
        else
        {
            Roll -= 3;
        }

        if (Roll < -BANK_ROTATION)
        {
            Roll = -BANK_ROTATION;
        }
    }
    else
    {
        if (Roll > 0)
        {
            Roll -= FMath::Min(Roll, 3.0f);
        }
        else if (Roll < 0)
        {
            Roll += FMath::Min(Roll*-1, 3.0f);
        }
    }

    pBody->SetRelativeRotation(BodyRotation + FRotator(0, 0, Roll), true);
}

float AShip::GetShipSpeed(float axisValue)
{
    return speed;
}

float AShip::GetBankSpeed(float axisValue)
{
    float ret = 0.0f;

    if (axisValue > 0)
    {
        if (Roll >= 0)
        {
            ret = speed;
        }
        else
        {
            ret = speed * (1.0f - (Roll / -45.0f));
        }
    }
    else if (axisValue < 0)
    {
        if (Roll <= 0)
        {
            ret = speed;
        }
        else
        {
            ret = speed * (1.0f - (Roll / 45.0f));
        }
    }

    return ret;
}

void AShip::FireWeapon()
{
    SelectWeapon();
    SpawnBullet();
}

void AShip::WasHit(int damage)
{
    ASpaaaceGameMode* GM = GetWorld()->GetAuthGameMode<ASpaaaceGameMode>();
    if (GM)
    {
        GM->PlayerWasHit();
    }
}

void AShip::SpawnBullet()
{
    UClass* projectileSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Bullet_BP.Bullet_BP_C'"));
    if (projectileSpawn)
    {
        UWorld* pWorld = GetWorld();
        if (pWorld)
        {
            FActorSpawnParameters parameters;
            parameters.Owner = this;

            ABullet* newProjectile = pWorld->SpawnActor<ABullet>(projectileSpawn, GetWeaponLocation(), GetWeaponDirection());
            newProjectile->SetIsAIFired(bIsAIShip);
            newProjectile->SetSpeed(600.0f);
        }
    }
}

void AShip::SelectWeapon()
{
    currentFiringPoint++;
    if (currentFiringPoint == FiringPoints.Num())
    {
        currentFiringPoint = 0;
    }
}

FVector AShip::GetWeaponLocation()
{
    return FiringPoints[currentFiringPoint]->GetComponentLocation();
}

FRotator AShip::GetWeaponDirection()
{
    return FiringPoints[currentFiringPoint]->GetComponentRotation();
}