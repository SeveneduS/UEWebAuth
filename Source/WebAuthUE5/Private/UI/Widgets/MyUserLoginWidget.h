// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include <Runtime/Online/HTTP/Public/Interfaces/IHttpRequest.h>

#include "MyUserLoginWidget.generated.h"

// FWD Declaration
class UButton;
class UCheckBox;
class UIngameMenuWidget;
class UEditableTextBox;

/**
 * User Login State
 */
enum class ELoginState {
	None,
	OK,
	Fail
};

/**
 * Login Dialog
 */
UCLASS(abstract)
class UMyUserLoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Login Dialog Result */
	ELoginState result;

	void Login(struct FLoginRequest credentials_);
	void LoginResponse(FHttpRequestPtr request_, FHttpResponsePtr response_, bool bWasSuccessful_);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UIngameMenuWidget* m_ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* LoginButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* CancelButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* RegisterButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* ResetPasswordButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UCheckBox* RememberCredentialsCheckBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* LoginTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* PasswordTextBox;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnLogin();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnCancel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnRegister();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnPasswordReset();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnShow(UIngameMenuWidget* parent_);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnHide();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnToggle(bool value);

private:
	/* hold imput */
	bool canInteract;

	FHttpModule* Http;

	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString hash_, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestWithRoute(FString url_);
	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetRequest(FString url_);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString url_, FString bodyJSON_);

	void Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	bool ResponseIsValid(FHttpResponsePtr response_, bool bWasSuccessful_);

#pragma region FromToJSON
	template <typename StructType>
	static void StructToJSON(StructType FilledStruct, FString& StringOutput);

	template <typename StructType>
	static void StructFromJSON(FHttpResponsePtr Response, StructType& StructOutput);
#pragma region
};
