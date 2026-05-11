#include "TestingPlaygroundUtils.h"

#include "GameplayEffect.h"

bool UTestingPlaygroundUtils::TestingPlayground_IsDurationGESpec(FGameplayEffectSpecHandle Handle)
{
	if (Handle.Data && Handle.Data->Def)
	{
		return Handle.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant;
	}
	
	return false;
}

// bool UTestingPlaygroundUtils::TestingPlayground_IsValidActiveGESpec(FActiveGameplayEffectHandle Handle)
// {
// 	
// }