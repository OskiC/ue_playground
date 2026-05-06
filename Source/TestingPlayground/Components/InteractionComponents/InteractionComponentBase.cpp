#include "InteractionComponentBase.h"

void UInteractionComponentBase::BeginPlay()
{
	Super::BeginPlay();
	if (bUseAttachParentAsMesh)
	{
		HighlightMesh = Cast<UMeshComponent>(GetAttachParent());
	}
}

void UInteractionComponentBase::OnHoverBegin() 
{
	if (IsValid(HighlightMesh))
	{
		HighlightMesh->SetRenderCustomDepth(true);
	}
}

void UInteractionComponentBase::OnHoverEnd()
{
	
}

void UInteractionComponentBase::OnInteract(APawn* Instigator)
{
	
}

FText UInteractionComponentBase::GetInteractionTooltip() const
{
	return TooltipText;
}