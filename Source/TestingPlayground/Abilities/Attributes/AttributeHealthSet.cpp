#include "AttributeHealthSet.h"

UAttributeHealthSet::UAttributeHealthSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}


// if someday there will be possibility to change maxhealth, check so we can clamp it :)
void UAttributeHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange attribute: %s"), *Attribute.AttributeName);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UAttributeHealthSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
	UE_LOG(LogTemp, Display, TEXT("PostChange: Attribute %s, OldVal: %f, NewVal: %f"), *Attribute.AttributeName, OldValue, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(OldValue, NewValue, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth, NewValue);
	}
}