#pragma once

#include "InteractionComponentBase.generated.h"

UCLASS(Abstract)
class UInteractionComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void OnInteractionBegin();
	virtual void OnInteractionEnd();
	
	UPROPERTY(EditAnywhere, Category = "InteractionComponent", meta = (AllowedClasses = "StaticMesh,SkeletalMesh"))
	TObjectPtr<UObject> InteractionMesh;
};