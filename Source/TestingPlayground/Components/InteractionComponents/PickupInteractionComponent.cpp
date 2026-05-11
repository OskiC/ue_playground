#include "PickupInteractionComponent.h"

UPickupInteractionComponent::UPickupInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPickupInteractionComponent::OnInteract(APawn* Instigator)
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("destroyed parent: %s"), *GetOwner()->GetName());
		GetOwner()->Destroy();
	}
}