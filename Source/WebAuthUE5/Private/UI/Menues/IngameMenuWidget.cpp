// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menues/IngameMenuWidget.h"
#include "WebAuthUE5/WebAuthUE5.h"

#include "UI/Widgets/MyQuitGameWidget.h"
#include "UI/Widgets/MyUserLoginWidget.h"
#include "UI/Widgets/MyUserRegistrationWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UIngameMenuWidget::Initialize()
{
	HideAll();
	ShowLogin("", "");

	return Super::Initialize();
}


int32 UIngameMenuWidget::GetOwnerUserIndex() const
{
	return m_PlayerOwner ? m_PlayerOwner->GetControllerId() : 0;
}

void UIngameMenuWidget::OnMenuGoBack(UINT32 menuIDX_ = 0)
{
}

void UIngameMenuWidget::CloseSubMenu()
{
	OnMenuGoBack();
}

void UIngameMenuWidget::SetOwner(ULocalPlayer* playerOwner_)
{
	isShown = false;

	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	m_PlayerOwner = playerOwner_;

	//todo:  don't create ingame menus for remote players.
	// _GameInstance = nullptr;
	if (m_PlayerOwner)
	{
		// _GameInstance = Cast<UGameInstance>(m_PlayerOwner->GetGameInstance());
	}
	else
	{
		// _GameInstance = Cast<UGameInstance>(GetGameInstance());
	}

	if (this == NULL) { return; }

	int32 const OwnerUserIndex = GetOwnerUserIndex();
}

void UIngameMenuWidget::ToggleGameMenu()
{
	if (GEngine) // DBG
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UIngameMenuWidget::ToggleGameMenu"));
	}

	//if UWidget Created
	if (this == NULL) { return; }

	/** check for a valid user index.
	 * could be invalid if the user signed out, in which case the
	 * 'please connect your control' ui should be up anyway.
	 * in-game menu needs a valid userindex for many OSS calls. */
	if (GetOwnerUserIndex() == -1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Trying to toggle in-game menu for invalid userid"));
		return;
	}

	if (isShown)
	{
		OnBack();
		return;
	}


	if (!isShown)
	{
		Show();
	}
	else
	{
		//Start hiding animation
		//	GameMenuWidget->HideMenu();
		//	if (PCOwner)
		//	{
		//		// Make sure viewport has focus
		//		FSlateApplication::Get().SetAllUserFocusToGameViewport();

		//	}
	}
}

bool UIngameMenuWidget::GetIsGameMenuUp() const
{
	return isShown;
}

void UIngameMenuWidget::OnBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("UIngameMenuWidget::OnBack()"));
	Hide();
}

void UIngameMenuWidget::OnLogin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UIngameMenuWidget::OnLogin()"));

	/*if (!_GameInstance)
	{
		_GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	}

	if (_GameInstance) {

		_GameInstance->SetAsAuthorized();*/
	//}

	HideLogin();
}

void UIngameMenuWidget::Show()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("UIngameMenuWidget::Show()"));

	int32 const OwnerUserIndex = GetOwnerUserIndex();

	isShown = true;
}

void UIngameMenuWidget::Hide()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UIngameMenuWidget::Hide()"));

	isShown = false;

	if (GEngine && GEngine->GameViewport)
	{
		HideAllLoggedIn();
	}
}

void UIngameMenuWidget::HideAll()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UIngameMenuWidget::HideAll()"));

	HideLogin();
	HideRegister();
	HideQuit();
}

void UIngameMenuWidget::HideAllLoggedIn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UIngameMenuWidget::HideAllLogedIn()"));

	HideRegister();
	HideQuit();
}

void UIngameMenuWidget::ShowRegister()
{
	if (hudRegisterWidget)
	{
		hudRegisterWidget->SetVisibility(ESlateVisibility::Visible);
		hudRegisterWidget->OnShow(this);
	}
}

void UIngameMenuWidget::HideRegister()
{
	if (hudRegisterWidget)
	{
		hudRegisterWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIngameMenuWidget::ShowLogin(const FString username_, const FString password_)
{
	if (hudLoginWidget)
	{
		hudLoginWidget->SetVisibility(ESlateVisibility::Visible);

		if (hudLoginWidget->LoginTextBox) 
		{
			hudLoginWidget->LoginTextBox->Text = FText::FromString(username_);
		}

		if (hudLoginWidget->PasswordTextBox)
		{
			hudLoginWidget->PasswordTextBox->Text = FText::FromString(password_);
		}

		hudLoginWidget->OnShow(this);
	}
}

void UIngameMenuWidget::HideLogin()
{
	if (hudLoginWidget)
	{
		hudLoginWidget->OnHide();
		hudLoginWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIngameMenuWidget::ShowQuit()
{
	HideAllLoggedIn();
	hudQuitWidget->OnShow(this);
}

void UIngameMenuWidget::HideQuit()
{
	if (hudQuitWidget)
	{
		hudQuitWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIngameMenuWidget::CancelQuitGame()
{
	ShowLogin("", "");
}

