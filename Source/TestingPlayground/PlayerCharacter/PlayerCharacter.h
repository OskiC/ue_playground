#pragma once
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	
	FVector GetInteractionStart() const;
	FVector GetInteractionDirection() const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PossessedBy(AController* NewController) override;

	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> CameraComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> JumpAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> MoveAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> LookAction = nullptr;
	
	// I'm not sure if that's a correct way but for now lets leave it
	UPROPERTY()
	TObjectPtr<class UCustomAbilitySystemComponent> AbilitySystemComponent = nullptr;
};
