#include "UGA_Interact.h"
#include <TestingPlayground/Interfaces/InteractableInterface.h>


UGA_Interact::UGA_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData && TriggerEventData->OptionalObject)
	{
		UObject* TargetObj = const_cast<UObject*>(TriggerEventData->OptionalObject.Get());

		if (IInteractableInterface* InteractableTarget = Cast<IInteractableInterface>(TargetObj))
		{
			InteractableTarget->OnInteract(Cast<APawn>(ActorInfo->AvatarActor.Get()));
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}