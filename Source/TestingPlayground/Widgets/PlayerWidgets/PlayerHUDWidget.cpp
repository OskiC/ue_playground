#include "PlayerHUDWidget.h"

#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::NativeConstruct()
{
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(GetOwningPlayerState());
	if (IsValid(PS))
	{
		UAttributeHealthSet* AttributeHealthSet =  PS->GetAttributeHealthSet();
		if (IsValid(AttributeHealthSet))
		{
			AttributeHealthSet->OnHealthChanged.AddDynamic(this, &UPlayerHUDWidget::OnHealthUpdated);
		
			OnHealthUpdated(AttributeHealthSet->GetHealth(), AttributeHealthSet->GetHealth(), AttributeHealthSet->GetMaxHealth());
		}
	}
}

void UPlayerHUDWidget::OnHealthUpdated(float OldValue, float NewValue, float MaxHealth)
{
	FString HealthText = FString::Printf(TEXT("%.2f / %.2f"),
		NewValue,
		MaxHealth
	);
	
	HealthLiteral->SetText(FText::FromString(HealthText));
	HealthBar->SetPercent(NewValue / MaxHealth);
}

void UPlayerHUDWidget::ShowTooltip(const FText& TooltipText)
{
	UE_LOG(LogTemp, Warning, TEXT("Tooltip: %s"), *TooltipText.ToString());
}

void UPlayerHUDWidget::HideTooltip()
{
	UE_LOG(LogTemp, Warning, TEXT("Tooltip hidden"));	
}