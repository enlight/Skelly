#Skelly (Terminated)

An Unreal Engine 4 plugin for creating semi-procedural character animations.

##Overview

Creating key-framed animations is tedious and time consuming, for me anyway. The aim of this plugin is to allow simple in-editor animation authoring by providing the user with the ability to create character poses, and to interpolate between those poses using various procedural methods at runtime. Yes, you still need to create key-frames, but fewer than usual, and you can do it in the editor. This is all very experimental!

Here's some inspiration: [Animation Bootcamp: An Indie Approach to Procedural Animation](http://www.gdcvault.com/play/1020583/Animation-Bootcamp-An-Indie-Approach)

##Installation

###Using the Binary UE4 Release
When using a binary release of UE4 you can extract a binary release of the plugin (if available) to either of the following locations:
>```<UE4 Installation Location>/<Engine Version>/Engine/Plugins/Editor/Skelly/```
>
>```<Your Project Location>/Plugins/Editor/Skelly/```

If you extract the plugin binaries into your project's plugins directory it will only be available for that project.

Alternatively, you can either clone or extract the plugin source to your project's plugins directory, which is covered next. Note that placing the plugin source into the engine plugins directory probably won't work because I don't think the binary UE4 release is configured to build engine plugins from source (but I haven't tried yet).

###Using the GitHub UE4 Release
If you'd like the plugin to be available for all your UE4 projects you need to clone or extract the plugin source to:
>```<UE4 Clone Location>/Engine/Plugins/Editor/Skelly/```

Then follow these steps on Windows (adjust as needed on other OSes):

1. Run **GenerateProjectFiles.bat** in the UE4 source directory.
2. Open the generated **UE4.sln** Visual Studio solution file and build it.
3. Launch the Unreal Editor, open any project, and follow the instructions in the next section.

Alternatively, if you only want to make the plugin available for a single project clone or extract the plugin source to:

>```<Your Project Location>/Plugins/Editor/Skelly/```

Then follow these steps on Windows (adjust as needed on other OSes):

1. Right-click on the **.uproject** file in Windows Explorer (e.g. **MyProject.uproject**) in your root project directory and select **Generate Visual Studio Files**.
2. Open the generated Visual Studio solution file (e.g. **MyProject.sln**) and build it.
3. Launch the Unreal Editor, open the project you've just built, and follow the instructions in the next section.

Note that your existing project must have a **Source** subdirectory with a couple of **.Target.cs** files in it, if it doesn't you may need to follow the steps in the **Building from Scratch** section below and then copy the built plugin into your project(s).

###Editor Configuration
Once you've got a binary version of the plugin (either by building or downloading) follow these steps:

1. Open **Window->Plugins** from the main menu of the Unreal Editor.
2. Navigate to the **Built-in/Editor/Animation** or the **Installed/Editor/Animation** sub-category, you should see the **Skelly** plugin in the list.
3. Enable the plugin and restart the editor if requested to do so.

##Building from Scratch

The following steps explain how to build the plugin as part of a new (mostly) empty project, in case you hit any issues while attempting to build it as part of an existing project, or as an engine plugin. 

1. Create a new **Basic Code (C++)** project from the UE4 editor, e.g. **MyProject**, close the editor.
2. Create a new subdirectory called **Plugins** in your root project directory, e.g. **MyProject/Plugins**.
3. Clone or extract the code for this plugin into a subdirectory within the **Plugins** directory, e.g. **MyProject/Plugins/Skelly**.
4. On Windows right-click on the **.uproject** file (e.g. **MyProject.uproject**) in your root project directory and select **Generate Visual Studio Files**.
5. On Windows open the generated Visual Studio solution file (e.g. **MyProject.sln**) and build it.
6. Launch the UE4 editor and open the project you created in step 1.
7. Open **Window->Plugins** from the main menu, navigate to the **Installed/Editor** category and you should see the **Skelly** plugin in the list.

If you'd like to build this plugin within an existing project just skip step 1, note that your existing project must have a **Source** subdirectory with a couple of **.Target.cs** files in it.