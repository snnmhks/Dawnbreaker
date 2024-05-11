// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CD2GAS : ModuleRules
{
	public CD2GAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
        PublicDependencyModuleNames.AddRange(new string[] { "CD2", "GameplayAbilities", "GameplayTags", "GameplayTasks" });

        PublicIncludePaths.AddRange(new string[] { "CD2GAS", "CD2",
            "/UE_5.3/Engine/Source/Runtime/GameplayTags/Classes",
            "/UE_5.3/Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public",
            "/Users/snnmh/Desktop/CD2/Source/CD2GAS"
        });
    }
}
