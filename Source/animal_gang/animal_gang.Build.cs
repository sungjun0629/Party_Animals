// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class animal_gang : ModuleRules
{
	public animal_gang(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam", "UMG", "Slate", "SlateCore", "Niagara" });
	}
}
