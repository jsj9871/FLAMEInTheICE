// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Combat : ModuleRules
{
	public Combat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "GameplayTasks", "AIModule", "HeadMountedDisplay" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "ArenaBattleSetting"});
	}
}
