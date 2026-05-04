#pragma once
#include "Blueprint/UserWidget.h"

#include "PlayerHUDWidget.generated.h"

UCLASS()
class UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InteractableTooltipText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthLiteral;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
private:
	UFUNCTION()
	void OnHealthUpdated(float OldValue, float NewValue, float MaxHealth);
	
	UFUNCTION()
	void OnTooltipUpdated(FText IncomingTooltipText);
};