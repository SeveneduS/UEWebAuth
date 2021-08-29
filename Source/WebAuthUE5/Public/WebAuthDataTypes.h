// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WebAuthDataTypes.generated.h"

#pragma region : LOGIN
/* Login Request */
USTRUCT()
struct FLoginRequest
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString login;
	UPROPERTY() FString password;

	FLoginRequest() {}
};

/* Login Request Response */
USTRUCT()
struct FLoginResponse
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() int id = 0;
	UPROPERTY() FString name;
	UPROPERTY() FString hash;

	FLoginResponse() {}
};
#pragma endregion

#pragma region : REGISTER
/* Register Request */
USTRUCT()
struct FRegistrationRequest
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() FString userName;
	UPROPERTY() FString email;
	UPROPERTY() FString password;

	FRegistrationRequest() {}
};

/* Register Request Response */
USTRUCT()
struct FRegistrationResponse
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY() int id = 0;
	UPROPERTY() FString name;
	UPROPERTY() FString hash;

	FRegistrationResponse() {}
};
#pragma endregion
