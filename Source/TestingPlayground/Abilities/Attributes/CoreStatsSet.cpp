#include "CoreStatsSet.h"

#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>

UCoreStatsSet::UCoreStatsSet()
{
	InitMana(100.f);
	InitMaxMana(100.f);

	InitStamina(100.f);
	InitMaxStamina(100.f);

	InitArmor(0.f);
	InitStrength(10.f);
	InitIntelligence(10.f);
	InitDexterity(10.f);
	InitLuck(1.f);
}

void UCoreStatsSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UCoreStatsSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreStatsSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(UCoreStatsSet, MaxMana);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreStatsSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(UCoreStatsSet, MaxStamina);
	DOREPLIFETIME(UCoreStatsSet, Armor);
	DOREPLIFETIME(UCoreStatsSet, Strength);
	DOREPLIFETIME(UCoreStatsSet, Intelligence);
	DOREPLIFETIME(UCoreStatsSet, Dexterity);
	DOREPLIFETIME(UCoreStatsSet, Luck);
}

void UCoreStatsSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Mana, OldMana);
}

void UCoreStatsSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, MaxMana, OldMaxMana);
}

void UCoreStatsSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Stamina, OldStamina);
}

void UCoreStatsSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, MaxStamina, OldMaxStamina);
}

void UCoreStatsSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Armor, OldArmor);
}

void UCoreStatsSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Strength, OldStrength);
}

void UCoreStatsSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Intelligence, OldIntelligence);
}

void UCoreStatsSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Dexterity, OldDexterity);
}

void UCoreStatsSet::OnRep_Luck(const FGameplayAttributeData& OldLuck)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreStatsSet, Luck, OldLuck);
}