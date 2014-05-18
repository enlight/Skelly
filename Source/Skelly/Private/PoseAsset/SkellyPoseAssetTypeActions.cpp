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
#include "SkellyPoseAssetTypeActions.h"
#include "SkellyPose.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

namespace Skelly {

FText FPoseAssetTypeActions::GetName() const
{
	return LOCTEXT("Skelly.FPoseAssetTypeActions", "Skelly Pose");
}

UClass* FPoseAssetTypeActions::GetSupportedClass() const
{
	return USkellyPose::StaticClass();
}

FColor FPoseAssetTypeActions::GetTypeColor() const
{
	return FColor(80, 123, 72);
}

void FPoseAssetTypeActions::OpenAssetEditor(
	const TArray<UObject*>& inObjects, TSharedPtr<IToolkitHost> editWithinLevelEditor
)
{
	const EToolkitMode::Type toolkitMode = editWithinLevelEditor.IsValid() ?
		EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	// TODO: Open up the pose editor for each of the passed in objects
}

uint32 FPoseAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Animation;
}

} // namespace Skelly

#undef LOCTEXT_NAMESPACE
