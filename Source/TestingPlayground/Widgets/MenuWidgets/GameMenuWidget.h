#pragma once

#include "Blueprint/UserWidget.h"

#include "GameMenuWidget.generated.h"

UCLASS()
class UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* InventoryWidgetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsWidgetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

	UFUNCTION()
	void OnInventoryClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnQuitGameClicked();

private:
};