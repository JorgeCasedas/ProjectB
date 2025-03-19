// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PBP : ModuleRules
{
	public PBP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "GameplayAbilities", "Steamworks", "OnlineSubsystemUtils" });
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks" });
    }
}
