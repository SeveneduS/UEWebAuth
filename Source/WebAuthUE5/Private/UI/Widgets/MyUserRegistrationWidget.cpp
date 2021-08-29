// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/MyUserRegistrationWidget.h"
#include "WebAuthUE5/WebAuthUE5.h"
#include "Components/EditableTextBox.h"
#include "WebAuthDataTypes.h"
#include "Async/Async.h"
#include <regex>

#include "UI/Menues/IngameMenuWidget.h"


void UMyUserRegistrationWidget::Register(FRegistrationRequest credentials_)
{
	FString ContentJsonString;
	StructToJSON<FRegistrationRequest>(credentials_, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest(UeWebAuth::Net::URL_REGUSR, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UMyUserRegistrationWidget::RegisterResponse);
	Send(Request);
}

void UMyUserRegistrationWidget::RegisterResponse(FHttpRequestPtr request_, FHttpResponsePtr response_, bool bWasSuccessful_)
{
	if (!ResponseIsValid(response_, bWasSuccessful_))
	{
		canInteract = true;
		return;
	}

	FLoginResponse LoginResponse;
	StructFromJSON<FLoginResponse>(response_, LoginResponse);

	UE_LOG(LogTemp, Warning, TEXT("User Successesfuly registred with email: %d"), LoginResponse.id);

	/* on register request success */
	if (m_ParentWidget && !asyncInProgress)
	{
		m_ParentWidget->ShowLogin(EMailTextBox->Text.ToString(), PasswordTextBox->Text.ToString());

		m_ParentWidget->HideRegister();
		return;
	}
}

void UMyUserRegistrationWidget::OnRegister()
{
	if (!canInteract) { return; }

	canInteract = false;

	auto uName = LoginTextBox->Text.ToString();
	auto email = EMailTextBox->Text.ToString();
	auto psswd = PasswordTextBox->Text.ToString();
	auto cpsswd = PasswordconfirmationTextBox->Text.ToString();

	const std::regex uNamePattern("(\"[^\"]*\")|('[^\r]*)(\r\n)?.{4,17}");
	const std::regex emailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	const std::regex psswdPattern("(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,27}");

	uName = uName.IsEmpty() ? email : uName;
	uName = uName.TrimStart();
	psswd = psswd.TrimStart();

	auto isValidUName = std::regex_match(TCHAR_TO_UTF8(*uName), uNamePattern);
	auto isValidEmail = std::regex_match(TCHAR_TO_UTF8(*email), emailPattern);
	auto isValidPsswd = std::regex_match(TCHAR_TO_UTF8(*psswd), psswdPattern);

	FString notifMSG = email;

	if (isValidPsswd &&
		isValidEmail &&
		isValidUName &&
		psswd.Equals(cpsswd))
	{
		FRegistrationRequest rReq;
		rReq.userName = uName;
		rReq.email = email;
		rReq.password = psswd;

		if (m_ParentWidget)
		{
			FString errorMsg = FString::Printf(TEXT("TRYING TO REGISTER USER %s"), *email);
		}

		Register(rReq);

		return;
	}
	else
	{
		/* print errors into status bar */
		if (m_ParentWidget)
		{
			FString errorMsg = FString::Printf(TEXT("PLEASE CHECK YOUR INPUT %s"), *notifMSG);
		}
	}

	canInteract = true;
}

void UMyUserRegistrationWidget::OnCancel()
{
	if (m_ParentWidget)
	{
		canInteract = false;
		//delete(Http);
		m_ParentWidget->ShowLogin("", "");
		m_ParentWidget->HideRegister();
	}
}

void UMyUserRegistrationWidget::OnShow(UIngameMenuWidget* parent_)
{
	canInteract = true;
	asyncInProgress = false;

	result = ERegisterState::None;

	if (!m_ParentWidget) { m_ParentWidget = parent_; }

	Http = &FHttpModule::Get();

	this->SetVisibility(ESlateVisibility::Visible);
}

void UMyUserRegistrationWidget::OnHide()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UMyUserRegistrationWidget::SetRegistrationHash(FString hash_, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->SetHeader(RegistrationHeader, hash_);
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyUserRegistrationWidget::RequestWithRoute(FString url_)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url_);
	SetRequestHeaders(Request);
	return Request;
}

void UMyUserRegistrationWidget::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->SetHeader(TEXT("User-Agent"), TEXT("X-UeWebAuth-Agent"));
	request_->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	request_->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyUserRegistrationWidget::PostRequest(FString url_, FString jsonString)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(url_);
	Request->SetVerb("POST");
	Request->SetContentAsString(jsonString);
	return Request;
}

void UMyUserRegistrationWidget::RegMultiplyAsync(FRegistrationRequest areq_)
{
	if (asyncInProgress) { return; }

	asyncInProgress = true;

	// benchmark - 1000 number of users to register
	for (int i = 0; i < 1000; i++) {
		FRegistrationRequest rrReq;
		rrReq.userName = areq_.userName;
		rrReq.email = areq_.email;
		rrReq.password = areq_.password;

		int ii = i + FMath::RandRange(0, 255);

		rrReq.userName = rrReq.userName.Append(FString::FromInt(ii));
		rrReq.email = rrReq.email.Append(FString::FromInt(ii));
		rrReq.password = rrReq.password.Append(FString::FromInt(ii));

		Register(rrReq);
		FPlatformProcess::Sleep(0.001F);
	}

	asyncInProgress = false;
}

void UMyUserRegistrationWidget::Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->ProcessRequest();
}

bool UMyUserRegistrationWidget::ResponseIsValid(FHttpResponsePtr response_, bool bWasSuccessful_)
{
	if (!bWasSuccessful_ || !response_.IsValid())
	{
		return false;
	}

	auto rCode = response_->GetResponseCode();
	FString rBody = response_->GetContentAsString();

	if (EHttpResponseCodes::IsOk(rCode) &&
		rBody.Contains("REGISTERED SUCCESSFULLY"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned code: %d"), rCode);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), rCode);
		return false;
	}
}

template<typename StructType>
inline void UMyUserRegistrationWidget::StructToJSON(StructType FilledStruct, FString& StringOutput)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(),
		&FilledStruct,
		StringOutput,
		0, 0);
}

template<typename StructType>
inline void UMyUserRegistrationWidget::StructFromJSON(FHttpResponsePtr Response, StructType& StructOutput)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}
