// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TargetVector : ModuleRules
{
	public TargetVector(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemEOS", "AITestSuite", "AITestSuite", "AITestSuite"});
	}
}
