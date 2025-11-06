//© www.felizdmg.com - All Rights Reserved.
#pragma once

#include "Engine/GameViewportClient.h"
#include "CustomGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMVIEWPORT_API UCustomGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

	// Change viewport dimensions of each player's split-screen
	void SetSplitScreenViewport(const struct FPerPlayerSplitscreenData &ScreenDim, int32 ScreenIndex=0);

	// Overridden to repurpose splitscreen viewports as in-game editor viewports
	virtual void LayoutPlayers() override;

private:
	bool bCustom3DEditor;
	TMap<FKey, ULocalPlayer*> PressedMouseButtons;

	FIntPoint LocalMousePos; // GetMousePos() with valid values outside viewport (relative to top-left)
	virtual void MouseMove(FViewport* InViewport, int32 X, int32 Y) override { LocalMousePos = { X, Y }; }
	virtual void CapturedMouseMove(FViewport* InViewport, int32 X, int32 Y) override { LocalMousePos = { X, Y }; }
};
