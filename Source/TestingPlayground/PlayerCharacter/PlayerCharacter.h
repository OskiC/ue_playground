#pragma once
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	
	FVector GetInteractionStart() const;
	FVector GetInteractionDirection() const;

protected:
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
};
