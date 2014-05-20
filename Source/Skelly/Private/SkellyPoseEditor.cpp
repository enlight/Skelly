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

#include "SkellyPrivatePCH.h"
#include "SkellyPoseEditor.h"
#include "WorkspaceMenuStructureModule.h"
#include "PoseAsset/SkellyPose.h"

#define LOCTEXT_NAMESPACE "Skelly.PoseEditor"

namespace Skelly {

namespace PoseEditorTabID
{
	const FName Skeleton(TEXT("Skeleton"));
	const FName Viewport(TEXT("Viewport"));
	const FName Details(TEXT("Details"));
}

FName FPoseEditor::PoseEditorAppName("PoseEditorApp");

void FPoseEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(tabManager);

	auto group = WorkspaceMenu::GetMenuStructure().GetAssetEditorCategory();

	tabManager->RegisterTabSpawner(
		PoseEditorTabID::Skeleton, FOnSpawnTab::CreateSP(this, &FPoseEditor::OnSpawnSkeletonTab)
	)
	.SetDisplayName(_skeletonTabTitle)
	.SetGroup(group);

	tabManager->RegisterTabSpawner(
		PoseEditorTabID::Viewport, FOnSpawnTab::CreateSP(this, &FPoseEditor::OnSpawnViewportTab)
	)
	.SetDisplayName(_viewportTabTitle)
	.SetGroup(group);

	tabManager->RegisterTabSpawner(
		PoseEditorTabID::Details, FOnSpawnTab::CreateSP(this, &FPoseEditor::OnSpawnDetailsTab)
	)
	.SetDisplayName(_detailsTabTitle)
	.SetGroup(group);
}

void FPoseEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
	tabManager->UnregisterTabSpawner(PoseEditorTabID::Skeleton);
	tabManager->UnregisterTabSpawner(PoseEditorTabID::Viewport);
	tabManager->UnregisterTabSpawner(PoseEditorTabID::Details);

	FAssetEditorToolkit::UnregisterTabSpawners(tabManager);
}

FName FPoseEditor::GetToolkitFName() const
{
	return FName("SkellyPoseEditor");
}

FText FPoseEditor::GetBaseToolkitName() const
{
	return LOCTEXT("BaseToolkitName", "Pose Editor");
}

FString FPoseEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Pose ").ToString();
}

FLinearColor FPoseEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

void FPoseEditor::AddReferencedObjects(FReferenceCollector& collector)
{
	collector.AddReferencedObject(_currentPose);
}

FPoseEditor::FPoseEditor()
	: _currentPose(nullptr)
	, _skeletonTabTitle(LOCTEXT("SkeletonTab", "Skeleton"))
	, _viewportTabTitle(LOCTEXT("ViewportTab", "Viewport"))
	, _detailsTabTitle(LOCTEXT("DetailsTab", "Details"))
{
}

void FPoseEditor::InitPoseEditor(
	EToolkitMode::Type toolkitMode, TSharedPtr<IToolkitHost>& editWithinLevelEditor,
	USkellyPose* poseToEdit
)
{
	_currentPose = poseToEdit;

	TSharedRef<FTabManager::FLayout> defaultStandaloneLayout = 
		FTabManager::NewLayout("Standalone_SkellyPoseEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split // toolbar at top of window
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(FAssetEditorToolkit::GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split // content area
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.9f)
				->SetOrientation(Orient_Horizontal)
				->Split // skeleton tree
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2f)
					->AddTab(PoseEditorTabID::Skeleton, ETabState::OpenedTab)
				)
				->Split // viewport
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->SetHideTabWell(true)
					->AddTab(PoseEditorTabID::Viewport, ETabState::OpenedTab)
				)
				->Split // details
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2f)
					->AddTab(PoseEditorTabID::Details, ETabState::OpenedTab)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(
		toolkitMode, editWithinLevelEditor, PoseEditorAppName, defaultStandaloneLayout, true, true, 
		poseToEdit
	);
}

TSharedRef<SDockTab> FPoseEditor::OnSpawnSkeletonTab(const FSpawnTabArgs& args)
{
	return SNew(SDockTab)
		.Label(_skeletonTabTitle);
}

TSharedRef<SDockTab> FPoseEditor::OnSpawnViewportTab(const FSpawnTabArgs& args)
{
	return SNew(SDockTab)
		.Label(_viewportTabTitle);
}

TSharedRef<SDockTab> FPoseEditor::OnSpawnDetailsTab(const FSpawnTabArgs& args)
{
	return SNew(SDockTab)
		.Label(_detailsTabTitle);
}

} // namespace Skelly

#undef LOCTEXT_NAMESPACE
