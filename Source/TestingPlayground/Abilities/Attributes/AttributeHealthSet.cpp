#include "AttributeHealthSet.h"

UAttributeHealthSet::UAttributeHealthSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UAttributeHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange attribute: %s"), *Attribute.AttributeName);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}