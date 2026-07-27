#pragma once

#include "ItemData/ItemsData.h"

#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentUpdated);

UCLASS()
class UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool EquipItem(class UItemInstance* ItemToEquip);

	int32 GetItemSlotCount();
	TArray<FEquipItemSlot> GetEquipmentSlots() const;

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentUpdated OnEquipmentUpdated;

protected:
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_EquippedItems, Category = "Equipment")
	TArray<FEquipItemSlot> EquippedItems;

	UFUNCTION()
	void OnRep_EquippedItems();
};