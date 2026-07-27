#pragma once

#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"

UCLASS()
class UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupSlot(int32 SlotIndex, const struct FInventoryItemSlot& SlotData, UTexture2D* GhostIcon = nullptr);

protected:
	UFUNCTION()
	void OnSlotClicked();

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountText;

	int32 MyInventoryIndex;
};