#pragma once
#include "AttributeSet.h"

#include "AttributeHealthSet.generated.h"

UCLASS()
class UAttributeHealthSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
};
