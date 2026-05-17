#include "GameMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(InventoryWidgetButton))
	{
		InventoryWidgetButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnInventoryClicked);
	}

	if (IsValid(OptionsWidgetButton))
	{
		OptionsWidgetButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnOptionsClicked);
	}

	if (IsValid(QuitGameButton))
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnQuitGameClicked);
	}
}

void UGameMenuWidget::OnInventoryClicked()
{
	if (IsValid(MenuSwitcher))
	{
		MenuSwitcher->SetActiveWidgetIndex(0);
	}
}

void UGameMenuWidget::OnOptionsClicked()
{
	if (IsValid(MenuSwitcher))
	{
		MenuSwitcher->SetActiveWidgetIndex(1);
	}
}

void UGameMenuWidget::OnQuitGameClicked()
{
#if WITH_EDITOR
	if (GEditor && GEditor->PlayWorld)
	{
		GEditor->RequestEndPlayMap();
		return;
	}
#endif

	FGenericPlatformMisc::RequestExit(false);
}