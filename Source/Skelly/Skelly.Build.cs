namespace UnrealBuildTool.Rules
{
	public class Skelly : ModuleRules
	{
		public Skelly(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
			);

			PrivateIncludePaths.AddRange(
				new string[] {
                    "Skelly/Private"
					// ... add other private include paths required here ...
				}
			);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add other public dependencies that you statically link with here ...
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "CoreUObject",
                    "Engine",
                    "Slate",
                    "UnrealEd",
                    "AssetTools",
                    "WorkspaceMenuStructure",
                    "ContentBrowser",
                    "EditorStyle",
                    "InputCore",
                    "AnimGraph",
                    "PropertyEditor"
					// ... add private dependencies that you statically link with here ...
				}
			);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
			);
		}
	}
}