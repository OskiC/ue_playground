#pragma once
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "CoreStatsSet.generated.h"

UCLASS()
class UCoreStatsSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCoreStatsSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, MaxStamina)

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, meta = (HideFromModifiers))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Stamina)

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, MaxMana)

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, meta = (HideFromModifiers))
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Mana)

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Armor)

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Strength)

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Intelligence)

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Dexterity)

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS_BASIC(UCoreStatsSet, Luck)

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldLuck);
};