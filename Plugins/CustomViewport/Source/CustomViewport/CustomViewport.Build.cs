//© www.felizdmg.com - All Rights Reserved.

using UnrealBuildTool;

public class CustomViewport : ModuleRules
{
	public CustomViewport(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{ "UMG", "Slate", "SlateCore" }
			);
		
	}
}
