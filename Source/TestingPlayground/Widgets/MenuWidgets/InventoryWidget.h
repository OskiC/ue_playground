#pragma once

#include "Blueprint/UserWidget.h"
#include <GameplayEffectTypes.h>

#include "InventoryWidget.generated.h"

UCLASS()
class UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;
	void OnAttributeValueChanged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void HandleSlotDrop(EPanelType SourcePanel, int32 SourceIndex, EPanelType TargetPanel, int32 TargetIndex);

protected:
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* EquipmentPanel;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* AttributeBox;

    UPROPERTY()
    TObjectPtr<class UInventoryComponent> InventoryComponent;

    UPROPERTY()
    TObjectPtr<class UEquipmentComponent> EquipmentComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<class UAttributeUIWidget> AttributeWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int32 Columns = 5;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	class UAttributeUIDataAsset* AttributeUIDataAsset = nullptr;

private:
    void FetchCharacterInfromation();

	UFUNCTION()
    void PopulateInventoryGrid();

	UFUNCTION()
	void PopulateEquipmentGrid();

	UFUNCTION()
	void PopulateAttributeBox();
};