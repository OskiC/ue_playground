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
	
	class UAttributeHealthSet* GetAttributeHealthSet();
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCustomAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<class UAttributeHealthSet> HealthSet = nullptr;
	
	virtual void BeginPlay() override;
};
