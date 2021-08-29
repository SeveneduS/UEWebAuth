// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class WEBAUTHUE5_API AMainMenuHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitializeComponents() override;
};
