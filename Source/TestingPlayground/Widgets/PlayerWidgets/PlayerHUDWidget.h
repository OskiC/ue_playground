#pragma once
#include "Blueprint/UserWidget.h"

#include "PlayerHUDWidget.generated.h"

UCLASS()
class UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowTooltip(const FText& ToolTipText);
	
	void HideTooltip();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class FTextBlock> TooltipText;
};
