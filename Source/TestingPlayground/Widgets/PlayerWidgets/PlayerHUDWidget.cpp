#include "PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"

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
	
	HealthBar->SetPercent(NewValue / MaxHealth);
}