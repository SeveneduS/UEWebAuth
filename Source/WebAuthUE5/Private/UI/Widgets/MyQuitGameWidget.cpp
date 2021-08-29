// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/MyQuitGameWidget.h"
#include "WebAuthUE5/WebAuthUE5.h"
#include "UI/Menues/IngameMenuWidget.h"


void UMyQuitGameWidget::OnAccept()
{
	if (canInteract)
	{
		canInteract = false;

		auto const PCOwner = GetOwningPlayer()
			? Cast<APlayerController>(GetOwningPlayer())
			: nullptr;

		if (PCOwner)
		{
			PCOwner->ConsoleCommand("quit");
		}
		else
		{
			FGenericPlatformMisc::RequestExit(false);
		}
	}
}

void UMyQuitGameWidget::OnReject()
{
	if (m_ParentWidget && canInteract)
	{
		canInteract = false;

		/* if you are not loged in */
		/*if (m_ParentWidget->GetGameInstance()->IsAuthorized())
		{
			m_ParentWidget->CancelQuitGame();
		}
		else*/
		{
			m_ParentWidget->ShowLogin("", "");
		}

		m_ParentWidget->HideQuit();
	}
}

void UMyQuitGameWidget::OnShow(UIngameMenuWidget* parent_)
{
	canInteract = true;

	if (!m_ParentWidget) { m_ParentWidget = parent_; }

	this->SetVisibility(ESlateVisibility::Visible);
}

void UMyQuitGameWidget::OnHide()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
