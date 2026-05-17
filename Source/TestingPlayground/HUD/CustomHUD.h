#pragma once
#include "GameFramework/HUD.h"

#include "CustomHUD.generated.h"

UCLASS()
class ACustomHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void RefreshWidget();
	void ToggleGameMenu();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<class UPlayerHUDWidget> PlayerHUDWidget = nullptr;

	UPROPERTY(EditDefaultSonly, Category = "Widgets")
	TSubclassOf<class UGameMenuWidget> GameMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<class UGameMenuWidget> GameMenuWidget = nullptr;
};
