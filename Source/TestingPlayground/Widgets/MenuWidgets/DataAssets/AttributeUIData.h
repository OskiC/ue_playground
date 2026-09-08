#pragma once

#include "AttributeUIData.generated.h"

USTRUCT(BlueprintType)
struct FAttributeUIData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere)
	FText AttributeName;

	UPROPERTY(EditAnywhere)
	class UTexture2D* AttributeIcon = nullptr;
};

UCLASS()
class UAttributeUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FAttributeUIData> AttributesToShow;
};