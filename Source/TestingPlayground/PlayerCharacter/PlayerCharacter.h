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
	void OnInteractPressed(const struct FInputActionValue& Value);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

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
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> DebugGameplayEffectAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> InteractAction = nullptr;
	
	// I'm not sure if that's a correct way but for now lets leave it
	UPROPERTY()
	TObjectPtr<class UCustomAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Debug Game Effect")
	TSubclassOf<class UGameplayEffect> DebugGameplayEffect;
	
	UPROPERTY(EditAnywhere, Category = "Debug Game Effect")
	float DebugGameplayEffectLevel = 1.f;
	
private:
	void CallDebugGameplayEffect();

	UFUNCTION(Server, Reliable)
	void Server_CallDebugGameplayEffect();
};
