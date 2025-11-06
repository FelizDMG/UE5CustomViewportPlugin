//© www.felizdmg.com - All Rights Reserved.
#include "CustomHUD.h"
#include "Misc/Paths.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/NamedSlot.h"
#include "Widgets/SWindow.h"

void ACustomHUD::SetViewportOverlay(TSubclassOf<UUserWidget> widgetClass)
{
	if (OverlayWidget) {
		OverlayWidget->RemoveFromParent();
		OverlayWidget = nullptr;
	}

	WidgetSlots.Empty();
	OverlayWidget = OverlayWidgetCache.FindRef(widgetClass);
	if (OverlayWidget) {
		RefreshWidgetSlots(OverlayWidget);
	}
	else if (widgetClass) {
		OverlayWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), widgetClass);
		if (!OverlayWidget) { return; }
		OverlayWidgetCache.Add(widgetClass, OverlayWidget);

		FString assetPath = TEXT("/UI");
		TArray<FName> slots;
		OverlayWidget->GetSlotNames(slots);
		for (auto slot : slots) {
			WidgetSlots.Add(slot, OverlayWidget);
			// Load user widgets when slot name ends with Panel from ./Panels/slotName.uasset
			auto slotName = slot.ToString();
			if (!slotName.EndsWith(TEXT("Panel"))) { continue; }
			auto slotPath = assetPath / TEXT("Panels");
		}
	}
	if (OverlayWidget) {
		OverlayWidget->AddToViewport();
		bShowOverlays = true;
	}
}

UUserWidget* ACustomHUD::GetViewportOverlay() const
{ return OverlayWidget; }

void ACustomHUD::RefreshWidgetSlots(UUserWidget* parent)
{
	if (!parent) { return; }

	TFunction<void(UWidget*)> AddWidgetSlots = ([&](UWidget * widget) {
		auto userWidget = Cast<UUserWidget>(widget);
		if (widget && widget->IsA<UNamedSlot>()) {
			const auto& slotName = widget->GetFName();
			WidgetSlots.Add(slotName, parent);
			userWidget = Cast<UUserWidget>(parent->GetContentForSlot(slotName));
		}
		if (userWidget && userWidget->WidgetTree) {
			UUserWidget* last_parent = parent;
			parent = userWidget;
			userWidget->WidgetTree->ForEachWidget(AddWidgetSlots);
			parent = last_parent;
		}
		});

	parent->WidgetTree->ForEachWidget(AddWidgetSlots);
}