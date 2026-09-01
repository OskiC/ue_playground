#pragma once

#include "../../Components/InventoryComponents/ItemData/ItemsData.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"

#include "InventorySlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemDropSignature, EPanelType, SourcePanel, int32, SourceIndex, EPanelType, TargetPanel, int32, TargetIndex);

UCLASS()
class UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupSlot(int32 SlotIndex, EPanelType PanelType, const struct FInventoryItemSlot& SlotData, UTexture2D* GhostIcon = nullptr);

	UPROPERTY(BlueprintAssignable)
	FOnItemDropSignature OnItemDropped;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
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