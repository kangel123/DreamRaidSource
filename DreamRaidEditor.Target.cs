using UnrealBuildTool;
using System.Collections.Generic;

public class DreamRaidEditorTarget : TargetRules
{
    public DreamRaidEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;

        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        CppStandard = CppStandardVersion.Cpp20;


        ExtraModuleNames.AddRange(new string[] { "DreamRaid" });
    }
}
