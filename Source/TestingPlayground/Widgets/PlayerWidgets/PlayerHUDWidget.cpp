#include "PlayerHUDWidget.h"

#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"
#include "TestingPlayground/PlayerCharacter/CustomPlayerController.h"

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
		
		ACustomPlayerController* CustomPlayerController = Cast<ACustomPlayerController>(PS->GetPlayerController());
		if (IsValid(CustomPlayerController))
		{
			CustomPlayerController->OnObjectHovered.AddDynamic(this, &UPlayerHUDWidget::OnTooltipUpdated);
			
			OnTooltipUpdated(FText::GetEmpty());
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

void UPlayerHUDWidget::OnTooltipUpdated(FText IncomingTooltipText)
{
	InteractableTooltipText->SetText(IncomingTooltipText);	
}