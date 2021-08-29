// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "UI/Menues/IngameMenuWidget.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	// Check if the Asset is assigned in the blueprint.
	if (wMainMenu == nullptr)
	{
		//UMG
		static ConstructorHelpers::FClassFinder<UUserWidget> m_UMGbp(TEXT("/Game/Widgets/IngameMenuWidget"));

		if (m_UMGbp.Succeeded())
		{
			wMainMenu = m_UMGbp.Class;
		}
	}
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/* init components here */
	PlayerCameraManagerClass = APlayerCameraManager::StaticClass();

	if (GetWorld())
	{
		APlayerController* m_PC = GetWorld()->GetFirstPlayerController();

		m_PC->bShowMouseCursor = true;
		m_PC->bEnableClickEvents = true;
		m_PC->bEnableMouseOverEvents = true;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wMainMenu)
	{
		// Create the widget and store it.
		m_MainMenu = CreateWidget<UIngameMenuWidget>(GetWorld(), wMainMenu);
		
		//let add it to the view port
		if (m_MainMenu) 
		{
			m_MainMenu->AddToViewport(1);
		}
	}

	if (ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		//Build menu only after game is initialized
		m_MainMenu->SetOwner(LocalPlayer);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}

void AMyPlayerController::SetupView()
{
}

void AMyPlayerController::ShowMainMenu()
{
}

void AMyPlayerController::OnEsc()
{
}
