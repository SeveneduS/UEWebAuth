// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WebAuthUE5 : ModuleRules
{
	public WebAuthUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"Http",
			"InputCore",
			"Json",
			"JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{ 
			"Slate", 
			"SlateCore",
			"OnlineSubsystem" 
		});

	}
}
