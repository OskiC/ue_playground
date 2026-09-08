#include "AttributeUIWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UAttributeUIWidget::SetupAttributeSlot(UTexture2D* Icon, FText Name, float AttributeVal)
{
	if (!Icon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Icon not found in AttributeDataAsset"));
		return;
	}

	AttributeIcon->SetBrushFromTexture(Icon);
	AttributeName->SetText(Name);
	AttributeValue->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), AttributeVal)));
}