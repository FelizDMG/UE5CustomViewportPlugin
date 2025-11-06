//© www.felizdmg.com - All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CustomHUD.h"
#include "Custom3DHUD.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMVIEWPORT_API ACustom3DHUD : public ACustomHUD
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CustomViewport")
	virtual void SetViewportOverlay(TSubclassOf<UUserWidget> widgetClass) override;

	virtual void DrawHUD() override;

	// Updates viewport dimensions for "View" slots of layout widget
	UFUNCTION(BlueprintCallable, Category = "CustomViewport")
	void UpdateViewports();

	UFUNCTION(BlueprintGetter, Category = Default)
	bool GetSelectionEnabled();

	UFUNCTION(BlueprintSetter, Category = Default)
	void SetSelectionEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintGetter, Category = Default)
	bool GetViewportsEnabled();

	UFUNCTION(BlueprintSetter, Category = Default)
	void SetViewportsEnabled(bool bEnabled);

private:
	// Allows selection management to be disabled (for initial placement mode, OnSelectionClicked is still called)
	UPROPERTY(BlueprintGetter = GetSelectionEnabled, BlueprintSetter = SetSelectionEnabled, Category = "CustomViewport")
	bool bSelectionEnabled;

	// Used to disable input and rendering on all viewports (useful for overlapping modal dialogs)
	UPROPERTY(BlueprintGetter = GetViewportsEnabled, BlueprintSetter = SetViewportsEnabled, Category = "CustomViewport")
	bool bViewportsEnabled;
};
