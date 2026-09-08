#pragma once

#include "Blueprint/UserWidget.h"

#include "AttributeUIWidget.generated.h"

UCLASS()
class UAttributeUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupAttributeSlot(class UTexture2D* Icon, FText Name, float AttributeVal);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* AttributeIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttributeName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttributeValue;
};