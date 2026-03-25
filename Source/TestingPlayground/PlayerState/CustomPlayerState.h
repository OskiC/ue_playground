#pragma once
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "CustomPlayerState.generated.h"

UCLASS()
class ACustomPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACustomPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCustomAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	virtual void BeginPlay() override;
};
