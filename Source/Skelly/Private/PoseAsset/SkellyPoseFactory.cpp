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
#include "SkellyPoseFactory.h"
#include "IAssetTypeActions.h"
#include "SkellyPose.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "SkellyPoseFactory"

USkellyPoseFactory::USkellyPoseFactory(const FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SupportedClass = USkellyPose::StaticClass();
	bCreateNew = true;
	//bEditAfterNew = true;
}

FText USkellyPoseFactory::GetDisplayName() const
{
	return LOCTEXT("SkellyPoseFactoryDesc", "Skelly Pose");
}

/*
FName USkellyPoseFactory::GetNewAssetThumbnailOverride() const
{
	// TODO
}
*/

uint32 USkellyPoseFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Animation;
}

bool USkellyPoseFactory::ConfigureProperties()
{
	Skeleton = nullptr;

	// create an asset picker that filters out everything but skeletons
	FAssetPickerConfig assetPickerConfig;
	assetPickerConfig.Filter.ClassNames.Add(USkeleton::StaticClass()->GetFName());
	assetPickerConfig.Filter.bRecursiveClasses = true;
	assetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	assetPickerConfig.ThumbnailScale = 0.25f;
	assetPickerConfig.OnAssetSelected.BindUObject(this, &USkellyPoseFactory::OnSkeletonSelected);

	IContentBrowserSingleton& contentBrowser =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();

	_assetPickerWindow = SNew(SWindow)
		.Title(LOCTEXT("ConfigurePropertiesDlgTitle", "Pick Skeleton"))
		.ClientSize(FVector2D(500.0f, 600.0f))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			[
				contentBrowser.CreateAssetPicker(assetPickerConfig)
			]
		];

	// display the asset picker as a modal window and block until it's closed
	GEditor->EditorAddModalWindow(_assetPickerWindow.ToSharedRef());
	// at this point the actual window has either been destroyed or queued for destruction,
	// in either case there's no longer any need to keep the wrapper SWindow alive
	_assetPickerWindow.Reset();

	return Skeleton != nullptr;
}

UObject* USkellyPoseFactory::FactoryCreateNew(UClass* objectClass, UObject* inParent, FName objectName, EObjectFlags objectFlags, UObject* contextObject, FFeedbackContext* feedbackContext)
{
	auto pose = CastChecked<USkellyPose>(
		StaticConstructObject(objectClass, inParent, objectName, objectFlags)
	);
	return pose;
}

void USkellyPoseFactory::OnSkeletonSelected(const class FAssetData& selectedAssetData)
{
	Skeleton = Cast<USkeleton>(selectedAssetData.GetAsset());
	_assetPickerWindow->RequestDestroyWindow();
}

#undef LOCTEXT_NAMESPACE