#pragma once

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

UCLASS()
class UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleSlotDrop(EPanelType SourcePanel, int32 SourceIndex, EPanelType TargetPanel, int32 TargetIndex);

protected:
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* EquipmentPanel;

    UPROPERTY()
    TObjectPtr<class UInventoryComponent> InventoryComponent;

    UPROPERTY()
    TObjectPtr<class UEquipmentComponent> EquipmentComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int32 Columns = 5;

private:
    void FetchCharacterInfromation();

	UFUNCTION()
    void PopulateInventoryGrid();

	UFUNCTION()
	void PopulateEquipmentGrid();
};