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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, Health)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, MaxHealth)
	
	UPROPERTY(BlueprintAssignable)
	FAttributeHealthChanged OnHealthChanged;
};