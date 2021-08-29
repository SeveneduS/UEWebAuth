// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/MyUserLoginWidget.h"
#include "WebAuthUE5/WebAuthUE5.h"

#include "WebAuthDataTypes.h"
#include "Helpers/JsonHelper.h"

#include "Components/EditableTextBox.h"

#include <regex>

#include "UI/Menues/IngameMenuWidget.h"


void UMyUserLoginWidget::OnShow(UIngameMenuWidget* parent_ = nullptr)
{
	canInteract = true;

	result = ELoginState::None;

	if (!m_ParentWidget) { m_ParentWidget = parent_; }

	if (Http == nullptr)
	{
		Http = &FHttpModule::Get();
	}

	this->SetVisibility(ESlateVisibility::Visible);
}

void UMyUserLoginWidget::OnHide()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UMyUserLoginWidget::OnToggle(bool value)
{
}

/* login */
void UMyUserLoginWidget::Login(FLoginRequest credentials_)
{
	FString ContentJsonString;
	StructToJSON<FLoginRequest>(credentials_, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest(UeWebAuth::Net::URL_AUTHSRV_LOGIN, ContentJsonString);

	Request->OnProcessRequestComplete().BindUObject(this, &UMyUserLoginWidget::LoginResponse);

	Send(Request);
}

void UMyUserLoginWidget::LoginResponse(FHttpRequestPtr request_, FHttpResponsePtr response_, bool bWasSuccessful_)
{
	if (!ResponseIsValid(response_, bWasSuccessful_))
	{
		canInteract = true;
		return;
	}

	FLoginResponse LoginResponse;
	StructFromJSON<FLoginResponse>(response_, LoginResponse);

	UE_LOG(LogTemp, Warning, TEXT("Id is: %d"), LoginResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("Name is: %s"), *LoginResponse.name);

	/* on login atempt success */
	 m_ParentWidget->OnLogin();//mve to on success login
}

void UMyUserLoginWidget::OnLogin()
{
	if (canInteract)
	{
		canInteract = false;

		auto login = LoginTextBox->Text.ToString();
		auto psswd = PasswordTextBox->Text.ToString();

		login = login.TrimStart();

		const std::regex uNamePattern("(\"[^\"]*\")|('[^\r]*)(\r\n)?.{4,17}");
		const std::regex emailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		const std::regex psswdPattern("(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,27}");

		/** ToDo:
		 * login must be valid uNamePattern || emailPattern
		 */
		auto isValidLogin = true; // std::regex_match(TCHAR_TO_UTF8(*login), emailPattern);
		auto isValidPsswd = true; // std::regex_match(TCHAR_TO_UTF8(*psswd), psswdPattern);

		FString errorSource = login;

		if (isValidLogin && isValidPsswd)
		{
			FLoginRequest lReq;
			lReq.login = login;
			lReq.password = psswd;

			if (m_ParentWidget)
			{
				FString errorMsg = FString::Printf(TEXT("TRYING TO LOGIN USER %s"), *login);
			}

			Login(lReq);

			return;
		}
		else
		{
			/* print errors into status bar */
			if (m_ParentWidget)
			{
				FString errorMsg = FString::Printf(TEXT("PLEASE CHECK YOUR INPUT %s"), *errorSource);
			}
		}

		canInteract = true;
	}
}

void UMyUserLoginWidget::OnCancel()
{
	/** ToDo:
	 * Check If client loged in and if not show quit dialog
	 * Otherwise display main menu
	 */
	if (canInteract && m_ParentWidget)
	{
		canInteract = false;

		m_ParentWidget->ShowQuit();
		m_ParentWidget->HideLogin();
	}
}

void UMyUserLoginWidget::OnRegister()
{
	if (canInteract && m_ParentWidget)
	{
		canInteract = false;

		m_ParentWidget->ShowRegister();
		m_ParentWidget->HideLogin();
	}
}

void UMyUserLoginWidget::OnPasswordReset()//done
{
	if (canInteract)
	{
		canInteract = false;

		FPlatformProcess::LaunchURL(*UeWebAuth::Net::URL_RESETPWD, nullptr, nullptr);

		canInteract = true;
	}
}

void UMyUserLoginWidget::SetAuthorizationHash(FString hash_, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->SetHeader(AuthorizationHeader, hash_);
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyUserLoginWidget::RequestWithRoute(FString url_)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url_);
	SetRequestHeaders(Request);
	return Request;
}

void UMyUserLoginWidget::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->SetHeader(TEXT("User-Agent"),		TEXT("X-UeWebAuth-Agent"));
	request_->SetHeader(TEXT("Content-Type"),	TEXT("application/json"));
	request_->SetHeader(TEXT("Accepts"),		TEXT("application/json"));
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyUserLoginWidget::GetRequest(FString url_)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(url_);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyUserLoginWidget::PostRequest(FString url_, FString bodyJSON_)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(url_);
	Request->SetVerb("POST");
	Request->SetContentAsString(bodyJSON_);
	return Request;
}

void UMyUserLoginWidget::Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& request_)
{
	request_->ProcessRequest();
}

bool UMyUserLoginWidget::ResponseIsValid(FHttpResponsePtr response_, bool bWasSuccessful_)
{
	if (!bWasSuccessful_ || !response_.IsValid())
	{
		return false;
	}

	if (EHttpResponseCodes::IsOk(response_->GetResponseCode()))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HTTP Response returned error code: %d"), response_->GetResponseCode());
		return false;
	}
}


/////////////// Json templates
template<typename StructType>
void UMyUserLoginWidget::StructToJSON(StructType FilledStruct, FString& StringOutput)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(),
		&FilledStruct,
		StringOutput,
		0, 0);
}

template<typename StructType>
void UMyUserLoginWidget::StructFromJSON(FHttpResponsePtr Response, StructType& StructOutput)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString,
		&StructOutput,
		0, 0);
}
