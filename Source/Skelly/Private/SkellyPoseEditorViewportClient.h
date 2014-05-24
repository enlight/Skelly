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

#include "EditorViewportClient.h"

class FPreviewScene;
class UDebugSkelMeshComponent;
class FSceneView;
class HHitProxy;

namespace Skelly {

class FPoseEditorViewportClient : public FEditorViewportClient
{
public:
	FPoseEditorViewportClient(FPreviewScene* inPreviewScene = nullptr);

	void SetSkeletalMeshPreviewComponent(UDebugSkelMeshComponent* inPreviewComponent);

public: // FEditorViewportClient interface
	virtual void Tick(float inDeltaSeconds);
	virtual void Draw(const FSceneView* inView, FPrimitiveDrawInterface* inPDI) override;
	virtual void ProcessClick(
		FSceneView& inView, HHitProxy* inHitProxy, FKey inKey, EInputEvent inEvent, 
		uint32 inHitX, uint32 inHitY
	) override;

public: // event handlers for the viewport toolbar (bound by the viewport)
	void OnShowBones();
	bool IsShowingBones() const;

public:
	void SelectBone(const FName& inBoneName);
	void ClearSelection();

private:
	void ComputeBoneWorldTransformAndColor(
		TArray<FTransform>& outWorldTransforms, TArray<FLinearColor>& outColors
	) const;
	void DrawBones(
		FPrimitiveDrawInterface* inPDI, const TArray<FTransform>& inWorldTransforms, 
		const TArray<FLinearColor>& inColors
	);
	void UpdatePreviewSceneSetup();
	void FocusViewportOnPreviewComponent();

private:
	TWeakObjectPtr<UDebugSkelMeshComponent> _skeletalMeshPreviewComponent;
};

} // namespace Skelly