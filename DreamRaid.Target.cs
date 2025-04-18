using UnrealBuildTool;
using System.Collections.Generic;

public class DreamRaidTarget : TargetRules
{
    public DreamRaidTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        CppStandard = CppStandardVersion.Cpp20;
        
        bOverrideBuildEnvironment = true;

        ExtraModuleNames.AddRange(new string[] { "DreamRaid" } );
    }
}
