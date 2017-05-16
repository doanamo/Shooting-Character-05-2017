// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

using UnrealBuildTool;

public class Game : ModuleRules
{
	public Game(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
