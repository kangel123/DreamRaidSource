// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DreamRaid : ModuleRules
{
	public DreamRaid(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slate", "SlateCore", "InputCore", "Niagara" ,"UMG"});
        
        PublicIncludePaths.AddRange(new string[] { "DreamRaid/Public/Boss","DreamRaid/Public/Boss/BossData",
            "DreamRaid/Public/Player", "DreamRaid/Public/Data",
            "DreamRaid/Public/Weapon","DreamRaid/Public/Interface" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
        
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
