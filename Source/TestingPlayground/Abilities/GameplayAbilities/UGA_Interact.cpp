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
		AActor* TargetActor = Cast<AActor>(TargetObj);

		if (!TargetActor)
		{
			if (UActorComponent* TargetComp = Cast<UActorComponent>(TargetObj))
			{
				TargetActor = TargetComp->GetOwner();
			}
		}

		if (TargetActor && ActorInfo->AvatarActor.IsValid())
		{
			if (ActorInfo->IsNetAuthority())
			{
				const float MaxInteractDistance = 350.f;
				const float Distance = ActorInfo->AvatarActor->GetDistanceTo(TargetActor);

				if (Distance > MaxInteractDistance)
				{
					UE_LOG(LogTemp, Warning, TEXT("Interaction failed: Target too far away (Distance: %f)"), Distance);
					EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
					return;
				}
			}

			if (IInteractableInterface* InteractableTarget = Cast<IInteractableInterface>(TargetObj))
			{
				InteractableTarget->OnInteract(Cast<APawn>(ActorInfo->AvatarActor.Get()));
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}