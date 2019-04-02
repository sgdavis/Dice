// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TowersPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TowersCharacter.h"
#include "Engine/World.h"
#include "Game/TowerGameStateBase.h"
#include "Game/TowersGameMode.h"

ATowersPlayerController::ATowersPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bHasShowHUD = false;
	HUDTemplate = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/HUD_BP.HUD_BP_C'"));
	bHasRegisteredPlayerListCallback = false;
	playerIndex = -1;
	MovementDestination = FVector(0, 0, 0);
}

void ATowersPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	if (HUDTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !bHasShowHUD)
	{
		ShowHUD(HUDTemplate);
	}
}

void ATowersPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// HACK because sometimes (often?) Tick() runs (WAY) before BeginPlay(), or even before all the assigned-in-Blueprint variables have populated...
	// This appears to be an Unreal issue, not a GDK issue, as I ran into this in Vanilla Shooter as well.
	if (HUDTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !bHasShowHUD)
	{
		ShowHUD(HUDTemplate);
	}

	if (HUDTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !bHasRegisteredPlayerListCallback)
	{
		if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
		{
			// Register a listener between the GameState's ready list and the login's update function.
			FSGPlayerListUpdatedDelegate UpdatePlayerListCallback;
			UpdatePlayerListCallback.BindUObject(TowerHUD, &UTowerHUD::UpdatePlayerList);
			GS->RegisterPlayerListListener(UpdatePlayerListCallback);

			bHasRegisteredPlayerListCallback = true;

			GS->SetLocalPlayerController(this);
		}
	}

	if (playerIndex == -1)
	{
		RequestPlayerID();
	}
}

void ATowersPlayerController::ShowHUD(TSubclassOf<UTowerHUD> NewWidgetClass)
{
	if (TowerHUD != NULL)
	{
		TowerHUD->RemoveFromViewport();
		TowerHUD = NULL;
	}

	if (NewWidgetClass != NULL)
	{
		TowerHUD = CreateWidget<UTowerHUD>(GetWorld(), NewWidgetClass);
		if (TowerHUD != NULL)
		{
			TowerHUD->AddToViewport();
			TowerHUD->SetOwnerPlayerController(this);
			bHasShowHUD = true;
		}
	}
}

void ATowersPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor && MovementDestination != FVector(0,0,0))
	{
		MoveToDestination();
	}
}

void ATowersPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	if (GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy)
	{
		CLIENTCOMPLAINT(TEXT("SETTING INPUT"));

		InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATowersPlayerController::OnSetDestinationPressed);
		InputComponent->BindAction("SetDestination", IE_Released, this, &ATowersPlayerController::OnSetDestinationReleased);

		//InputComponent->BindAction("DebugButton", IE_Pressed, this, &ATowersPlayerController::TempRequestSpawn);
	}
}

void ATowersPlayerController::MoveToDestination()
{
	SetNewMoveDestination(MovementDestination);
}

void ATowersPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			Cast<ATowersCharacter>(MyPawn)->MoveCharacter(DestLocation - MyPawn->GetActorLocation());
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
		else
		{
			bMoveToMouseCursor = false;
			// We hit something, move there
			MovementDestination = FVector(0,0,0);
		}
	}
}

void ATowersPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		bMoveToMouseCursor = true;
		// We hit something, move there
		MovementDestination = Hit.ImpactPoint;
	}
}

void ATowersPlayerController::OnSetDestinationReleased()
{
}

bool ATowersPlayerController::TeleportPlayer_Validate(FVector location)
{
	return true;
}

void ATowersPlayerController::TeleportPlayer_Implementation(FVector location)
{
	GetPawn()->SetActorLocation(location);
}

bool ATowersPlayerController::IsReadyToTeleport()
{
	ATowersCharacter* pCharacter = Cast<ATowersCharacter>(GetPawn());
	return pCharacter->IsReadyToTeleport();
}

bool ATowersPlayerController::SetPlayerIndex_Validate(int index)
{
	return true;
}

void ATowersPlayerController::SetPlayerIndex_Implementation(int index)
{
	playerIndex = index;
}

bool ATowersPlayerController::RequestPlayerID_Validate()
{
	return true;
}

void ATowersPlayerController::RequestPlayerID_Implementation()
{
	ProvidePlayerID(playerIndex);
}

void ATowersPlayerController::ProvidePlayerID_Implementation(int id)
{
	playerIndex = id;
}

bool ATowersPlayerController::RequestReward_Validate(int reward)
{
	return true;
}

void ATowersPlayerController::RequestReward_Implementation(int reward)
{
	ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();
	if (GM)
	{
		GM->RequestReward(playerIndex, reward);
	}
}

//==========SPAWN==========
void ATowersPlayerController::RequestSpawn()
{
	ServerRequestSpawn();
}

bool ATowersPlayerController::ServerRequestSpawn_Validate()
{
	return true;
}

void ATowersPlayerController::ServerRequestSpawn_Implementation()
{
	ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();
	if (GM)
	{
		GM->RequestSpawn(playerIndex);
	}
}

//==========UPGRADES==========
void ATowersPlayerController::RequestUpgrade(int upgradeIndex)
{
	ServerRequestUpgrade(upgradeIndex);
}

bool ATowersPlayerController::ServerRequestUpgrade_Validate(int upgradeIndex)
{
	return true;
}

void ATowersPlayerController::ServerRequestUpgrade_Implementation(int upgradeIndex)
{
	ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();
	if (GM)
	{
		GM->RequestUpgrade(playerIndex, upgradeIndex);
	}
}