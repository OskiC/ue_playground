#pragma once
#include "GameFramework/HUD.h"

#include "CustomHUD.generated.h"

UCLASS()
class ACustomHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void RefreshWidget();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<class UPlayerHUDWidget> PlayerHUDWidget = nullptr;
};
