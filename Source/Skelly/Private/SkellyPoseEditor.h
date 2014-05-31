//-------------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2014 Vadim Macagon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-------------------------------------------------------------------------------
#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "PreviewScene.h"

class USkellyPose;
class USkeletalMesh;
class UDebugSkelMeshComponent;

namespace Skelly {

/** Editor for USkellyPose assets. */
class FPoseEditor : public FAssetEditorToolkit, public FEditorUndoClient, public FGCObject
{
public: // FAssetEditorToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

public: // FEditorUndoClient interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

public: // FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& collector) override;

public:
	FPoseEditor();
	~FPoseEditor();
	/** Called on module startup. Initialize state shared by all pose editor instances. */
	static void Startup();
	/** Called on module shutdown. Cleanup state shared by all pose editor instances. */
	static void Shutdown();

	void InitPoseEditor(
		EToolkitMode::Type toolkitMode, TSharedPtr<IToolkitHost>& editWithinLevelEditor,
		USkellyPose* poseToEdit
	);

	FPreviewScene& GetPreviewScene();

private:
	TSharedRef<FTabManager::FLayout> GenerateDefaultStandaloneLayout();
	void SetPreviewSkeletalMesh(USkeletalMesh* inPreviewSkeletalMesh);
	void SetSelectedBoneNames(const TArray<FName>& inBoneNames);
	void GetSelectedBoneNames(TArray<FName>& outBoneNames) const;

private: // event handlers
	TSharedRef<SDockTab> OnSpawnSkeletonTab(const FSpawnTabArgs& args);
	TSharedRef<SDockTab> OnSpawnViewportTab(const FSpawnTabArgs& args);
	TSharedRef<SDockTab> OnSpawnDetailsTab(const FSpawnTabArgs& args);

	void SkeletonTree_OnSelectionChanged();
	void Viewport_OnSelectionChanged();

private:
	// the pose currently being edited
	USkellyPose* _currentPose;

	FText _skeletonTabTitle;
	FText _viewportTabTitle;
	FText _detailsTabTitle;

	// the scene to be displayed in the viewport
	FPreviewScene _previewScene;

	// the viewport that displays the preview scene
	TSharedPtr<class SPoseEditorViewport> _viewport;

	// the viewport's client, for convenience
	TWeakPtr<class FPoseEditorViewportClient> _viewportClientWeakPtr;

	// the component that will draw the current pose
	UDebugSkelMeshComponent* _skeletalMeshPreviewComponent;

	// the skeleton tree displayed on the skeleton tab
	TSharedPtr<class SSkeletonTree> _skeletonTree;

private:
	static FName PoseEditorAppName;
};

inline FPreviewScene& FPoseEditor::GetPreviewScene()
{
	return _previewScene;
}

} // namespace Skelly