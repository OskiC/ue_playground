#pragma once

#include "CustomPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractedObjectHovered, FText, TooltipText);

UCLASS()
class ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UObject* GetCurrentHoveredTarget() const;
	
	void Interact();
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractedObjectHovered OnObjectHovered;
	
protected:
	UPROPERTY(EditAnywhere)
	float InteractLineTraceLength = 300.f;
	
private:
	void SendTrace();
	
	UPROPERTY()
	TScriptInterface<class IInteractableInterface> CurrentHoveredTarget = nullptr;
};
