#pragma once

#include "CustomPlayerController.generated.h"

UCLASS()
class ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
};
