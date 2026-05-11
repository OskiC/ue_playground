#pragma once
#include "InteractionComponentBase.h"

#include "PickupInteractionComponent.generated.h"

UCLASS(ClassGroup=(PLAYGROUND), meta=(BlueprintSpawnableComponent))
class UPickupInteractionComponent : public UInteractionComponentBase
{
	GENERATED_BODY()
	
public:
	UPickupInteractionComponent();

	virtual void OnInteract(APawn* Instigator) override;
};