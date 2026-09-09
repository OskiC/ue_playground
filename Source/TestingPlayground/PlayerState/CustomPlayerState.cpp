#include "CustomPlayerState.h"

#include "AbilitySystemComponent.h"
#include "TestingPlayground/Abilities/CustomAbilitySystemComponent.h"
#include "TestingPlayground/Abilities/Attributes/AttributeHealthSet.h"
#include "TestingPlayground/Abilities/Attributes/CoreStatsSet.h"

ACustomPlayerState::ACustomPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	NetUpdateFrequency = 100.f;
	
	HealthSet = CreateDefaultSubobject<UAttributeHealthSet>(TEXT("HealthSet"));
	CoreStatsSet = CreateDefaultSubobject<UCoreStatsSet>(TEXT("CoreStatsSet"));
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

UCoreStatsSet* ACustomPlayerState::GetCoreStatsSet()
{
	return CoreStatsSet;
}