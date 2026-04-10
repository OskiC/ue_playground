#pragma once
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "AttributeHealthSet.generated.h"

UCLASS()
class UAttributeHealthSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeHealthSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, Health)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	
	ATTRIBUTE_ACCESSORS_BASIC(UAttributeHealthSet, MaxHealth)
};
