#pragma once

#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

UCLASS()
class UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

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
    int32 Columns = 4;

private:
    void FetchCharacterInfromation();

	UFUNCTION()
    void PopulateInventoryGrid();

	UFUNCTION()
	void PopulateEquipmentGrid();

};