#include "CustomHUD.h"

#include "Blueprint/UserWidget.h"
#include "TestingPlayground/Widgets/PlayerWidgets/PlayerHUDWidget.h"

void ACustomHUD::RefreshWidget()
{
	if (!IsValid(PlayerHUDWidget) && PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetOwningPlayerController(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
}
