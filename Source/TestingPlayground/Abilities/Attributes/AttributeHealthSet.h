#pragma once
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "AttributeHealthSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeHealthChanged, float, OldHealth, float, NewHealth, float, MaxHealth);

UCLASS()
class UAttributeHealthSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeHealthSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, meta = (HideFromModifiers))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, Health)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, MaxHealth)

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, Damage)
	
	UPROPERTY(BlueprintAssignable)
	FAttributeHealthChanged OnHealthChanged;
};