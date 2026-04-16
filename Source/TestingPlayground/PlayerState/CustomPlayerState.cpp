#include "CustomPlayerState.h"

#include "AbilitySystemComponent.h"
#include "TestingPlayground/Abilities/CustomAbilitySystemComponent.h"
#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"

ACustomPlayerState::ACustomPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	HealthSet = CreateDefaultSubobject<UAttributeHealthSet>(TEXT("HealthSet"));
}

UAbilitySystemComponent* ACustomPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;	
}

void ACustomPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState i AbilitySystemComp gotowe?"));
	}
}

UAttributeHealthSet* ACustomPlayerState::GetAttributeHealthSet()
{
	return HealthSet;
}