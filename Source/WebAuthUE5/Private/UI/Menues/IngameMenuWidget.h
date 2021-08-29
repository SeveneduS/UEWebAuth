// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "IngameMenuWidget.generated.h"

// class UGameInstance;

class UButton;
// class UMyGameInstance;
class UMyUserLoginWidget;
class UMyUserRegistrationWidget;
class UMyQuitGameWidget;

/** * * * * * * * * * * *
 * InGame Menu widget	*
 * * * * * * * * * * * **/
UCLASS(abstract)
class UIngameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;

public:
	/** Cancel Quit Game Dialog */
	UFUNCTION(Category = "Function")
	void CancelQuitGame();

	/** sets owning player controller */
	UFUNCTION(BlueprintCallable, Category = "Function")
	void SetOwner(ULocalPlayer* playerOwner_);

	/** toggles in game menu */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void ToggleGameMenu();

	/** is game menu currently active? */
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool GetIsGameMenuUp() const;

	/** switching back */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void OnBack();

	/** ON LOGIN HANDLER */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void OnLogin();

	/** show the menu main menu content */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void Show();

	/** hide the menu */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void Hide();

	/** hide all views */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void HideAll();

	/** hide all views while logged in */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void HideAllLoggedIn();

	/** show registration screen*/
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void ShowRegister();

	/** hide registration screen */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void HideRegister();

	/** show login screen */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void ShowLogin(const FString username_, const FString password_);

	/** hide login screen */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void HideLogin();

	/** show the quit game screen */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void ShowQuit();

	/** hide the quit game screen */
	UFUNCTION(BlueprintCallable, Category = "Function")
	virtual void HideQuit();

protected:

#pragma region WIDGETS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UMyUserLoginWidget* hudLoginWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UMyUserRegistrationWidget* hudRegisterWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UMyQuitGameWidget* hudQuitWidget;

#pragma endregion

	/** Owning player controller */
	ULocalPlayer* m_PlayerOwner;

	/** get current user index out of PlayerOwner */
	int32 GetOwnerUserIndex() const;

	/** called when going back to previous menu */
	virtual void OnMenuGoBack(UINT32 menuIDX_);

	/** goes back in menu structure */
	virtual void CloseSubMenu();

private:
	/** if ingame menu is currently shown */
	bool isShown;

	// EOnlineGameMode::Type CurrentGameMode;

	// UMyGameInstance* _GameInstance;

public:
	// UMyGameInstance* GetGameInstance() const { return _GameInstance; }
};
