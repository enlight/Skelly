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
#include "SkellyPoseEditorViewportClient.h"

namespace Skelly {

FPoseEditorViewportClient::FPoseEditorViewportClient(FPreviewScene* inPreviewScene)
	: FEditorViewportClient(inPreviewScene)
{
	EngineShowFlags.Game = 0;
	EngineShowFlags.SetSnap(false);
	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.CompositeEditorPrimitives = true;

	SetRealtime(true);
	if (GEditor->PlayWorld)
	{
		// disable real-time mode when playing in the editor
		SetRealtime(false, true);
	}

	// TODO: create floor, sky, fog, light, reflections
}

void FPoseEditorViewportClient::SetSkeletalMeshPreviewComponent(
	UDebugSkelMeshComponent* inPreviewComponent
)
{
	inPreviewComponent->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	inPreviewComponent->bCanHighlightSelectedSections = true;
	_skeletalMeshPreviewComponent = inPreviewComponent;
}

void FPoseEditorViewportClient::Draw(const FSceneView* inView, FPrimitiveDrawInterface* inPDI)
{
	FEditorViewportClient::Draw(inView, inPDI);

	// TODO: the skeletal mesh is already drawn as part of the preview scene,
	//       but any additional bits and pieces should be drawn here
}

} // namespace Skelly
