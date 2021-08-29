// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "MyUserRegistrationWidget.generated.h"


class UButton;
class UCheckBox;
class UIngameMenuWidget;
class UEditableTextBox;

/**
 * User Registration State
 */
enum class ERegisterState {
	None,
	OK,
	Fail
};

/**
 * Register a New User Dialog
 */
UCLASS(abstract)
class UMyUserRegistrationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Register User Dialog Result */
	ERegisterState result;

	void Register(struct FRegistrationRequest credentials_);
	void RegisterResponse(FHttpRequestPtr request_, FHttpResponsePtr response_, bool bWasSuccessful_);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UIngameMenuWidget* m_ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* RegisterButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* CancelButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* LoginTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* EMailTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UEditableTextBox* PasswordconfirmationTextBox;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnRegister();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnCancel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnShow(UIngameMenuWidget* parent_);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnHide();

private:
	/* hold imput */
	bool canInteract;

	FHttpModule* Http;

	FString RegistrationHeader = TEXT("Registration");
	void SetRegistrationHash(FString hash_, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestWithRoute(FString url_);
	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString url_, FString bodyJSON_);

	bool asyncInProgress;
	void RegMultiplyAsync(FRegistrationRequest areq_);
	void Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_);

	bool ResponseIsValid(FHttpResponsePtr response_, bool bWasSuccessful_);

#pragma region FromToJSON
	template <typename StructType>
	void StructToJSON(StructType FilledStruct, FString& StringOutput);

	template <typename StructType>
	void StructFromJSON(FHttpResponsePtr Response, StructType& StructOutput);
#pragma region

}; // End Of MyUserRegisterClass
