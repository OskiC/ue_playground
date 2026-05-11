#include "AttributeHealthSet.h"

#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>

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

void UAttributeHealthSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.f, MaxHealthValue);
		
		if (OldHealthValue != NewHealthValue)
		{
			SetHealth(NewHealthValue);
		}
		
		SetDamage(0.f);
	}
}

void UAttributeHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAttributeHealthSet, Health);
	DOREPLIFETIME(UAttributeHealthSet, MaxHealth);
}

void UAttributeHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeHealthSet, Health, OldValue);

	const float OldHealth = OldValue.GetCurrentValue();
	const float NewHealth = GetHealth();
	const float CurrentMaxHealth = GetMaxHealth();

	OnHealthChanged.Broadcast(OldHealth, NewHealth, CurrentMaxHealth);
}

void UAttributeHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeHealthSet, MaxHealth, OldValue);

	const float CurrentHealth = GetHealth();
	const float OldMaxHealth = OldValue.GetCurrentValue();
	const float NewMaxHealth = GetMaxHealth();

	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth, NewMaxHealth);
}