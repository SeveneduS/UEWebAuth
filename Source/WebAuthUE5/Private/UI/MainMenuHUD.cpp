// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUD.h"

AMainMenuHUD::AMainMenuHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AMainMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()) 
	{
		APlayerController* _PController = GetWorld()->GetFirstPlayerController();

		if (_PController) 
		{
			_PController->bShowMouseCursor = true;
			_PController->bEnableClickEvents = true;
			_PController->bEnableMouseOverEvents = true;
		}
	}
}