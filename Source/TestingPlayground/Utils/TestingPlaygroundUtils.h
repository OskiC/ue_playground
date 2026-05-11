#pragma once
#include "GameplayEffectTypes.h"

#include "TestingPlaygroundUtils.generated.h"

UCLASS()
class UTestingPlaygroundUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "OwnUtils")
	static bool TestingPlayground_IsDurationGESpec(FGameplayEffectSpecHandle Handle);
	
	// UFUNCTION(BlueprintCallable, Category = "OwnUtils")
	// static bool TestingPlayground_IsValidActiveGESpec(FActiveGameplayEffectHandle Handle);
};
