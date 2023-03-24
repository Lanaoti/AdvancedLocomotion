// Copyright 2018 CQUnreal. All Rights Reserved.

using UnrealBuildTool;

public class AdvancedLocomotionGraph : ModuleRules
{
	public AdvancedLocomotionGraph(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicIncludePaths.AddRange(
			new string[] {

			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
				"Engine",
                "AnimGraphRuntime",
                "AdvancedLocomotion"
			}
		);


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {

            }
        );


        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd",
                    "BlueprintGraph",
                    "AnimGraph",
                }
            );
        }


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
		);
	}
}
