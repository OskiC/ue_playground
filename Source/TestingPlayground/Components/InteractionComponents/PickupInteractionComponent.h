#pragma once

#include "InteractionComponentBase.h"
#include <TestingPlayground/Components/InventoryComponents/ItemData/ItemsData.h>

#include "PickupInteractionComponent.generated.h"

UCLASS(ClassGroup=(PLAYGROUND), meta=(BlueprintSpawnableComponent))
class UPickupInteractionComponent : public UInteractionComponentBase
{
	GENERATED_BODY()
	
public:
	UPickupInteractionComponent();

	virtual void OnInteract(APawn* Instigator) override;

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FStartingItemDef ItemDefined{};

	UPROPERTY()
	FInventoryItemSlot ItemPayload{};
};