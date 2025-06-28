// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PBPTarget : TargetRules
{
	public PBPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("PBP");
        //bUseLoggingInShipping = true;
        //bUseChecksInShipping = true;
        //bDisableDebugInfo = false;
        //LinkType = TargetLinkType.Monolithic;
        //bUsePDBFiles = true;
    }
}
