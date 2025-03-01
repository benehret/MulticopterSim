/*
* MulticopterSim.Target.cs: Target script for MulticopterSim
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

using UnrealBuildTool;
using System.Collections.Generic;

public class MulticopterSimTarget : TargetRules
{
	public MulticopterSimTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { 
                "MainModule", 
                "HackflightModule", 
                "SocketModule" 
                } );
	}
}
