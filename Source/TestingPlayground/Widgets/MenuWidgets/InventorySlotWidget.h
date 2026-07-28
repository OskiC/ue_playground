#pragma once

#include "../../Components/InventoryComponents/ItemData/ItemsData.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"

#include "InventorySlotWidget.generated.h"

UCLASS()
class UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupSlot(int32 SlotIndex, EPanelType PanelType, const struct FInventoryItemSlot& SlotData, UTexture2D* GhostIcon = nullptr);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountText;

	int32 MyInventoryIndex;
	EPanelType MyPanelType;
};

UCLASS()
class UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int32 SourceIndex = 0;
	TObjectPtr<class UInventorySlotWidget> SourceWidget = nullptr;
	EPanelType PanelSource;
};