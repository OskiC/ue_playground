#include "HealthBarWorldWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"

void UHealthBarWorldWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(GetOwningPlayerState());
	if (IsValid(PS))
	{
		UAttributeHealthSet* AttributeHealthSet =  PS->GetAttributeHealthSet();
		if (IsValid(AttributeHealthSet))
		{
			AttributeHealthSet->OnHealthChanged.AddDynamic(this, &UHealthBarWorldWidget::OnHealthUpdated);
		
			OnHealthUpdated(AttributeHealthSet->GetHealth(), AttributeHealthSet->GetHealth(), AttributeHealthSet->GetMaxHealth());
		}
	}
	
}

void UHealthBarWorldWidget::OnHealthUpdated(float OldValue, float NewValue, float MaxHealth)
{
	FString HealthText = FString::Printf(TEXT("%.2f / %.2f"),
		NewValue,
		MaxHealth
	);
	
	HealthLiteral->SetText(FText::FromString(HealthText));
	HealthBar->SetPercent(NewValue / MaxHealth);
	
	// [TODO] : Add calculation where we will display damage
	// Thinking: this only works for CustomPlayerState so there will be needed change of abstractions
}