#pragma once

#include "Blueprint/UserWidget.h"

#include "HealthBarWorldWidget.generated.h"

UCLASS()
class UHealthBarWorldWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthLiteral;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

private:
	UFUNCTION()
	void OnHealthUpdated(float OldValue, float NewValue, float MaxHealth);
};