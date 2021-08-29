// Copyright Epic Games, Inc. All Rights Reserved.


#include "WebAuthUE5GameModeBase.h"
#include "Player/MyPlayerController.h"
#include "UI/MainMenuHUD.h"

AWebAuthUE5GameModeBase::AWebAuthUE5GameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = AMainMenuHUD::StaticClass();
}

void AWebAuthUE5GameModeBase::RestartPlayer(class AController* player)
{
	// don't restart
}

//TSubclassOf<AGameSession> AWebAuthUE5GameModeBase::GetGameSessionClass() const
//{
//	return TSubclassOf<AGameSession>();
//}
