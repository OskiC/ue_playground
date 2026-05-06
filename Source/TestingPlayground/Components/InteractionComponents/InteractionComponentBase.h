#pragma once
#include "TestingPlayground/Interfaces/InteractableInterface.h"

#include "InteractionComponentBase.generated.h"

UCLASS(Abstract)
class UInteractionComponentBase : public USceneComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteract(APawn* Instigator) override;
	virtual FText GetInteractionTooltip() const override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "InteractionComponent")
	FText TooltipText;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bUseAttachParentAsMesh = true;

	UPROPERTY()
	TObjectPtr<UMeshComponent> HighlightMesh;
};