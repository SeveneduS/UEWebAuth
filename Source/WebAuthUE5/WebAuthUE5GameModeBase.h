// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WebAuthUE5GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AWebAuthUE5GameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

public:
#pragma region  AGameModeBase interface
	/** skip it, menu doesn't require player start or pawn */
	virtual void RestartPlayer(class AController* player) override;

	/** Returns game session class to use */
	// virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
#pragma endregion
	
};
