// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameEditorTarget : TargetRules
{
	public GameEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
	)
	{
		OutExtraModuleNames.AddRange(new string[] { "Game" });
	}
}
