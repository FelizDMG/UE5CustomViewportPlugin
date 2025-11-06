//© www.felizdmg.com - All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

UCLASS()
class CUSTOMVIEWPORT_API ACustomHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void SetViewportOverlay(TSubclassOf<UUserWidget> widgetClass);

	UFUNCTION(BlueprintCallable, Category = "CustomViewport")
	UUserWidget* GetViewportOverlay() const;
	
private:
	UUserWidget* OverlayWidget;
	UPROPERTY()
	TMap<TSubclassOf<UUserWidget>, UUserWidget*> OverlayWidgetCache;
	TMap<FName, UUserWidget*> WidgetSlots;
	void RefreshWidgetSlots(UUserWidget* parent);

	FVector2D ScaleXY;
};
