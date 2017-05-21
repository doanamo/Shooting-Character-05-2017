// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

using UnrealBuildTool;

public class Game : ModuleRules
{
	public Game(TargetInfo Target)
	{
		PrivateIncludePaths.AddRange(new string[]
		{
			 "Game/Character",
			 "Game/Gameplay",
		});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
