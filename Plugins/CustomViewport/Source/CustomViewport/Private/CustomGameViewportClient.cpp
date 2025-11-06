//© www.felizdmg.com - All Rights Reserved.
#include "CustomGameViewportClient.h"
#include "Custom3DInstance.h"

#include "SceneView.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"

//Breaks local multiplayer
void UCustomGameViewportClient::Init(
	struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice /*= true*/)
{
	if (auto CustomInstance = Cast<UCustom3DInstance>(OwningGameInstance)) {
		bCustom3DEditor = true;
	}
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
}

void UCustomGameViewportClient::SetSplitScreenViewport(const struct FPerPlayerSplitscreenData& ScreenDim, int32 ScreenIndex)
{
	for (auto i = 0; i < SplitscreenInfo.Num(); ++i) {
		if (ScreenIndex < SplitscreenInfo[i].PlayerData.Num()) {
			SplitscreenInfo[i].PlayerData[ScreenIndex] = ScreenDim;
		}
	}
}

void UCustomGameViewportClient::LayoutPlayers()
{
	if (!bCustom3DEditor) { return Super::LayoutPlayers(); }
	
	auto& players = GetOuterUEngine()->GetGamePlayers(this);
	ActiveSplitscreenType = players.Num() > 1 ? ESplitScreenType::FourPlayer_Grid : ESplitScreenType::None;

	const ESplitScreenType::Type SplitType = GetCurrentSplitscreenConfiguration();
	for (int32 i = 0; i < players.Num(); ++i)
	{
		if (SplitType < SplitscreenInfo.Num() && i < SplitscreenInfo[SplitType].PlayerData.Num())
		{
			players[i]->Size.X   = SplitscreenInfo[SplitType].PlayerData[i].SizeX;
			players[i]->Size.Y   = SplitscreenInfo[SplitType].PlayerData[i].SizeY;
			players[i]->Origin.X = SplitscreenInfo[SplitType].PlayerData[i].OriginX;
			players[i]->Origin.Y = SplitscreenInfo[SplitType].PlayerData[i].OriginY;
		}
		else
		{
			players[i]->Size.X   = 0.f;
			players[i]->Size.Y   = 0.f;
			players[i]->Origin.X = 0.f;
			players[i]->Origin.Y = 0.f;
		}
	}
}