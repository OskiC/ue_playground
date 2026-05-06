#pragma once

#include "InteractableInterface.generated.h"

UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void OnHoverBegin() = 0;
	virtual void OnHoverEnd() = 0;
	virtual void OnInteract(APawn* Instigator) = 0;
	virtual FText GetInteractionTooltip() const = 0;
};