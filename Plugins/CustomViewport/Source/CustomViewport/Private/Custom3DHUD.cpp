//© www.felizdmg.com - All Rights Reserved.
#include "Custom3DHUD.h"
#include "CustomGameViewportClient.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/NamedSlot.h"
#include "Layout/Geometry.h"
#include "Slate/SGameLayerManager.h"

ACustom3DHUD::ACustom3DHUD(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void ACustom3DHUD::SetViewportOverlay(TSubclassOf<UUserWidget> widgetClass)
{
	auto viewportWidget = GetViewportOverlay();
	if (viewportWidget && viewportWidget->GetClass() == widgetClass) { return; }
	Super::SetViewportOverlay(widgetClass);
	UpdateViewports();
}

void ACustom3DHUD::DrawHUD()
{

	Super::DrawHUD();
	UpdateViewports();
}

void ACustom3DHUD::UpdateViewports()
{
	auto viewport = Cast<UCustomGameViewportClient>(GetWorld()->GetGameViewport());
	auto viewportWidget = GetViewportOverlay();
	if (!viewport || !viewportWidget) { return; }
	auto viewportLayout = viewport->GetGameLayerManager();
	if (!viewportLayout.IsValid()) { return; }
	auto viewportRegion = viewportLayout->GetViewportWidgetHostGeometry().GetLayoutBoundingRect();
	auto maxViews = FMath::Min<int32>(viewport->MaxSplitscreenPlayers, 4);
	TArray<FPerPlayerSplitscreenData> viewDims;
	viewDims.AddDefaulted(maxViews);

	// Size viewports for slots named View[0-3]
	int32 numViewSlots = 0;
	viewportWidget->WidgetTree->ForEachWidget([&](UWidget * widget) {
		if (widget && widget->IsA<UNamedSlot>()) {
			auto slotName = widget->GetFName().ToString();
			if (!slotName.Contains("3DViewport")) { return; }
			slotName.RemoveFromStart("3DViewport");
			int32 slotID = FCString::Atoi(*slotName);
			for (auto player : GetGameInstance()->GetLocalPlayers())
			{
				auto viewId = slotID;
				if (!IsValid(GetGameInstance()->GetLocalPlayerByIndex(slotID)))
				{
					UGameplayStatics::CreatePlayer(GetWorld(), slotID, true);
				}
				auto playerId = FMath::Clamp<int32>(viewId, 0, maxViews);

				// TODO: Update dimensions during Widget::Tick instead of dependence on CachedGeometry
				auto viewRegion = widget->GetCachedGeometry().GetLayoutBoundingRect();
				if (viewRegion.IsEmpty()) { return; }

				auto origin = (viewRegion.GetTopLeft() - viewportRegion.GetTopLeft()) / viewportRegion.GetSize();
				auto sizexy = (viewRegion.GetBottomRight() - viewRegion.GetTopLeft()) / viewportRegion.GetSize();
				viewDims[playerId].OriginX = origin.X;
				viewDims[playerId].OriginY = origin.Y;
				viewDims[playerId].SizeX = sizexy.X;
				viewDims[playerId].SizeY = sizexy.Y;
				viewport->SetSplitScreenViewport(viewDims[playerId], playerId);
			}
		}
		});
}

bool ACustom3DHUD::GetSelectionEnabled()
{
	auto pc = GetOwningPlayerController();
	return pc ? pc->bEnableClickEvents : true;
}

void ACustom3DHUD::SetSelectionEnabled(bool bEnabled)
{
	for (auto localPlayer : GetGameInstance()->GetLocalPlayers()) {
		if (auto pc = localPlayer->GetPlayerController(GetWorld())) {
			pc->bEnableClickEvents = bEnabled;
		}
	}
}

bool ACustom3DHUD::GetViewportsEnabled()
{
	auto viewport = Cast<UGameViewportClient>(GetWorld()->GetGameViewport());
	return viewport ? !viewport->IgnoreInput() : true;
}

void ACustom3DHUD::SetViewportsEnabled(bool bEnabled)
{
	if (auto viewport = Cast<UGameViewportClient>(GetWorld()->GetGameViewport())) {
		viewport->SetIgnoreInput(!bEnabled);
	}
}