// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGCreature.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

// Sets default values
ARPGCreature::ARPGCreature()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    nStrength = 1;
    nIntelligence = 1;
    nDexterity = 1;

    pWeapon = NULL;
    pBoots = NULL;
    pOffhand = NULL;

    bStartingJump = false;
    bStartingSwing = false;
    bStartingBlock = false;
    bStartingRightTurn = false;
    bStartingLeftTurn = false;
    bStartingForward = false;
    bStartingBackUp = false;
}

// Called when the game starts or when spawned
void ARPGCreature::BeginPlay()
{
	Super::BeginPlay();

    if (pWeaponSpawnPoint == NULL)
    {
        TSet<UActorComponent*> components = GetComponents();
        for (UActorComponent* component : components)
        {
            if (pWeaponSpawnPoint == NULL && component->GetFName().IsEqual("Weapon"))
            {
                pWeaponSpawnPoint = Cast<UStaticMeshComponent>(component);
            }
            if (pOffhandSpawnPoint == NULL && component->GetFName().IsEqual("Offhand"))
            {
                pOffhandSpawnPoint = Cast<UStaticMeshComponent>(component);
            }
            if (pBootsSpawnPoint == NULL && component->GetFName().IsEqual("Boots"))
            {
                pBootsSpawnPoint = Cast<UStaticMeshComponent>(component);
            }
        }
    }

    UClass* baseItem = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/BaseItem_BP.BaseItem_BP_C'"));
    if (baseItem)
    {
        UWorld* pWorld = GetWorld();
        if (pWorld)
        {
            FActorSpawnParameters parameters;
            parameters.Owner = this;

            //pWeapon = pWorld->SpawnActor<ARPGEquipment>(baseItem, pWeaponSpawnPoint->GetComponentLocation(), pWeaponSpawnPoint->GetComponentRotation());
            //pOffhand = pWorld->SpawnActor<ARPGEquipment>(baseItem, pOffhandSpawnPoint->GetComponentLocation(), pOffhandSpawnPoint->GetComponentRotation());
            pBoots = pWorld->SpawnActor<ARPGEquipment>(baseItem, pBootsSpawnPoint->GetComponentLocation(), pBootsSpawnPoint->GetComponentRotation());

            //pWeapon->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
            //pOffhand->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
            pBoots->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
        }
    }
    UClass* shieldItem = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Shield_BP.Shield_BP_C'"));
    if (shieldItem)
    {
        UWorld* pWorld = GetWorld();
        if (pWorld)
        {
            FActorSpawnParameters parameters;
            parameters.Owner = this;

            pOffhand = pWorld->SpawnActor<ARPGEquipment>(shieldItem, pOffhandSpawnPoint->GetComponentLocation(), pOffhandSpawnPoint->GetComponentRotation());
            CLIENTCOMPLAINT2("1X %f", pOffhandSpawnPoint->GetComponentLocation().X);
            CLIENTCOMPLAINT2("1Y %f", pOffhandSpawnPoint->GetComponentLocation().Y);
            CLIENTCOMPLAINT2("1Z %f", pOffhandSpawnPoint->GetComponentLocation().Z);
            CLIENTCOMPLAINT2("2X %f", pOffhand->GetActorLocation().X);
            CLIENTCOMPLAINT2("2Y %f", pOffhand->GetActorLocation().Y);
            CLIENTCOMPLAINT2("2Z %f", pOffhand->GetActorLocation().Z);

            pOffhand->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
        }
    }
    UClass* spearItem = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Spear_BP.Spear_BP_C'"));
    if (spearItem)
    {
        UWorld* pWorld = GetWorld();
        if (pWorld)
        {
            FActorSpawnParameters parameters;
            parameters.Owner = this;

            pWeapon = pWorld->SpawnActor<ARPGEquipment>(spearItem, pWeaponSpawnPoint->GetComponentLocation(), pWeaponSpawnPoint->GetComponentRotation());

            pWeapon->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
        }
    }
}

// Called every frame
void ARPGCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bStartingForward)
    {
        AddMovementInput(GetActorForwardVector(), 1.0f, true);
    }
    if (bStartingBackUp)
    {
        AddMovementInput(GetActorForwardVector() * -1, 1.0f, true);
    }
    if (bStartingLeftTurn)
    {
        AddActorLocalRotation(FRotator(0.0f, -1.5f, 0.0f));
    }
    if (bStartingRightTurn)
    {
        AddActorLocalRotation(FRotator(0.0f, 1.5f, 0.0f));
    }
    if (pWeapon)
    {
        pWeapon->Update(DeltaTime, this);
    }
    if (pOffhand)
    {
        pOffhand->Update(DeltaTime, this);
        /*CLIENTCOMPLAINT2("X %f", pOffhand->GetActorLocation().X);
        CLIENTCOMPLAINT2("Y %f", pOffhand->GetActorLocation().Y);
        CLIENTCOMPLAINT2("Z %f", pOffhand->GetActorLocation().Z);*/
    }
    /*if (pBoots)
    {
        pBoots->Update(DeltaTime, this);
    }*/
}

// Called to bind functionality to input
void ARPGCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCreature::SetCreatureStates(int startingJump, int startingSwing, int startingBlock, int startingRightTurn, int startingLeftTurn, int startingForward, int startingBackUp)
{
    if (!bStartingJump && startingJump == 1)
    {
        if (pBoots)
        {
            pBoots->Crouch(nStrength, nIntelligence, nDexterity);
        }
        bStartingJump = true;
    }
    else if (bStartingJump && startingJump == 0)
    {
        if (pBoots)
        {
            pBoots->Jump(nStrength, nIntelligence, nDexterity);
        }
        bStartingJump = false;
    }

    if (!bStartingSwing && startingSwing == 1)
    {
        if (pWeapon)
        {
            pWeapon->PullBack(nStrength, nIntelligence, nDexterity);
        }
        bStartingSwing = true;
    }
    else if (bStartingSwing && startingSwing == 0)
    {
        if (pWeapon)
        {
            pWeapon->Swing(nStrength, nIntelligence, nDexterity);
        }
        bStartingSwing = false;
    }

    if (!bStartingBlock && startingBlock == 1)
    {
        if (pOffhand)
        {
            pOffhand->RaiseShield(nStrength, nIntelligence, nDexterity);
        }
        bStartingBlock = true;
    }
    else if (bStartingBlock && startingBlock == 0)
    {
        if (pOffhand)
        {
            pOffhand->LowerShield(nStrength, nIntelligence, nDexterity);
        }
        bStartingBlock = false;
    }

    if (!bStartingRightTurn && startingRightTurn == 1)
    {
        bStartingRightTurn = true;
    }
    else if (bStartingRightTurn && startingRightTurn == 0)
    {
        bStartingRightTurn = false;
    }

    if (!bStartingLeftTurn && startingLeftTurn == 1)
    {
        bStartingLeftTurn = true;
    }
    else if (bStartingLeftTurn && startingLeftTurn == 0)
    {
        bStartingLeftTurn = false;
    }

    if (!bStartingForward && startingForward == 1)
    {
        bStartingForward = true;
    }
    else if (bStartingForward && startingForward == 0)
    {
        bStartingForward = false;
    }

    if (!bStartingBackUp && startingBackUp == 1)
    {
        bStartingBackUp = true;
    }
    else if (bStartingBackUp && startingBackUp == 0)
    {
        bStartingBackUp = false;
    }
}

void ARPGCreature::WasHit()
{
    CLIENTCOMPLAINT("OUCH");
}