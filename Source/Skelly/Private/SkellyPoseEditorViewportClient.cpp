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
#include "PreviewScene.h"
#include "AnimGraphDefinitions.h"

/** Hit proxy for bones to allow selection of bones in a viewport using a mouse. */
struct HSkellyBoneProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	FName BoneName;

	explicit HSkellyBoneProxy(const FName& inBoneName) : BoneName(inBoneName) {}
};

IMPLEMENT_HIT_PROXY(HSkellyBoneProxy, HHitProxy);

namespace Skelly {

FPoseEditorViewportClient::FPoseEditorViewportClient(FPreviewScene* inPreviewScene)
	: FEditorViewportClient(inPreviewScene)
	, _widgetMode(FWidget::WM_Rotate)
	, _bWidgetIsBeingManipulated(false)
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

	UpdatePreviewSceneSetup();
	FocusViewportOnPreviewComponent();
}

void FPoseEditorViewportClient::Tick(float inDeltaSeconds)
{
	FEditorViewportClient::Tick(inDeltaSeconds);

	if (!GIntraFrameDebuggingGameThread)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, inDeltaSeconds);
	}
}

void FPoseEditorViewportClient::Draw(const FSceneView* inView, FPrimitiveDrawInterface* inPDI)
{
	// draw the scene contents, the debug skeletal mesh component will draw the mesh at this point
	FEditorViewportClient::Draw(inView, inPDI);

	// draw bones and other extra bits and pieces
	if (_skeletalMeshPreviewComponent.IsValid() && _skeletalMeshPreviewComponent->SkeletalMesh)
	{
		if (_skeletalMeshPreviewComponent->bDisplayBones)
		{
			TArray<FTransform> worldTransforms;
			TArray<FLinearColor> colors;
			ComputeBoneWorldTransformAndColor(worldTransforms, colors);
			DrawBones(inPDI, worldTransforms, colors);
		}
	}
}

void FPoseEditorViewportClient::ProcessClick(
	FSceneView& inView, HHitProxy* inHitProxy, FKey inKey, EInputEvent inEvent, 
	uint32 inHitX, uint32 inHitY
)
{
	if (inHitProxy)
	{
		if (inHitProxy->IsA(HSkellyBoneProxy::StaticGetType()))
		{
			auto boneProxy = static_cast<HSkellyBoneProxy*>(inHitProxy);
			SelectBone(boneProxy->BoneName);
		}
	}
	else // user didn't click on anything selectable
	{
		ClearSelection();
	}
}

void FPoseEditorViewportClient::TrackingStarted(const struct FInputEventState& InInputState, bool bIsDraggingWidget, bool bNudge)
{
	const auto boneIndex = GetSelectedBoneIndex();
	if ((boneIndex != INDEX_NONE) && bIsDraggingWidget)
	{
		_bWidgetIsBeingManipulated = true;
	}
}

void FPoseEditorViewportClient::TrackingStopped()
{
	if (_bWidgetIsBeingManipulated)
	{
		_bWidgetIsBeingManipulated = false;
	}
	Invalidate();
}

bool FPoseEditorViewportClient::InputWidgetDelta(
	FViewport* inViewport, EAxisList::Type inCurrentAxis, 
	FVector& inDrag, FRotator& inRot, FVector& inScale
)
{
	bool bInputHandled = false;

	if (!_bWidgetIsBeingManipulated
		|| (inCurrentAxis == EAxisList::None)
		|| !_skeletalMeshPreviewComponent.IsValid())
	{
		return bInputHandled;
	}

	bInputHandled = true;
	const auto boneIndex = GetSelectedBoneIndex();
	if (boneIndex != INDEX_NONE)
	{
		const auto boneName = _skeletalMeshPreviewComponent->GetBoneName(boneIndex);
		auto& skeletalControl = _skeletalMeshPreviewComponent->PreviewInstance->ModifyBone(boneName);
		// by default ModifyBone() sets up the skeletal control to replace rotation and translation
		// so scaling must be enabled separately
		skeletalControl.ScaleMode = EBoneModificationMode::BMM_Replace;
		skeletalControl.ScaleSpace = EBoneControlSpace::BCS_BoneSpace;

		FTransform controlTransform(
			skeletalControl.Rotation, skeletalControl.Translation, skeletalControl.Scale
		);
		// If A and B are FTransform(s) and v is an FVector then 
		//    (A * B).TransformVector(v) == B.TransformVector(A.TransformVector(v))
		// => v x (A x B) = (v x A) x B
		//
		// Now consider the following transforms
		// W: bone to world, S: skeletal control, P: parent bone transform, C: component to world
		//
		// Get the bone space to world space transform W = S x P x C
		FTransform worldTransform = _skeletalMeshPreviewComponent->GetBoneTransform(boneIndex);
		// Invert the bone space to world space transform to obtain the world space to bone space 
		// transform, however the resulting transform may not be the base bone space transform 
		// because the skeletal control transform may have been applied previously, so remove the 
		// previously applied skeletal control transform to get the base bone space transform.
		// Note: the following deduction may not be 100% correct.
		//    W^-1     = (S x P x C)^-1
		//             = C^-1 x P^-1 x S^-1 
		// => W^-1 x S = C^-1 x P^-1 x S^-1 x S // undo previous skeletal control transform
		// => W^-1 x S = C^-1 x P^-1            // to obtain the base bone space transform
		FTransform baseTransform = worldTransform.InverseSafe() * controlTransform;

		const bool bShouldRotate = (_widgetMode == FWidget::WM_Rotate);
		const bool bShouldTranslate = (_widgetMode == FWidget::WM_Translate);
		const bool bShouldScale = (_widgetMode == FWidget::WM_Scale);

		if (bShouldRotate)
		{
			FVector axis;
			float angle;
			inRot.Quaternion().ToAxisAndAngle(axis, angle);
			// convert rotation axis from world space to bone space
			const auto boneSpaceAxis = baseTransform.TransformVector(axis);
			FQuat deltaQuat(boneSpaceAxis, angle);

			skeletalControl.Rotation = (controlTransform * FTransform(deltaQuat)).Rotator();
		}

		if (bShouldTranslate)
		{
			skeletalControl.Translation += baseTransform.TransformVector(inDrag);
		}

		if (bShouldScale)
		{
			skeletalControl.Scale += inScale;
		}

		Invalidate();
	}
	
	return bInputHandled;
}

void FPoseEditorViewportClient::SetWidgetMode(FWidget::EWidgetMode inNewMode)
{
	_widgetMode = inNewMode;
	Invalidate();
}

bool FPoseEditorViewportClient::CanSetWidgetMode(FWidget::EWidgetMode inNewMode) const
{
	return true;
}

FWidget::EWidgetMode FPoseEditorViewportClient::GetWidgetMode() const
{
	// the transform widget should only be displayed if a single bone is selected
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		if (_skeletalMeshPreviewComponent->BonesOfInterest.Num() == 1)
		{
			return _widgetMode;
		}
	}
	return FWidget::WM_None;
}

FVector FPoseEditorViewportClient::GetWidgetLocation() const
{
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		// place the transform widget at the location of the currently selected bone 
		// (in world space)
		if (_skeletalMeshPreviewComponent->BonesOfInterest.Num() == 1)
		{
			return _skeletalMeshPreviewComponent->GetBoneMatrix(GetSelectedBoneIndex())
				.GetOrigin();
		}
	}
	return FVector::ZeroVector;
}

FMatrix FPoseEditorViewportClient::GetWidgetCoordSystem() const
{
	if (GetWidgetCoordSystemSpace() == COORD_Local)
	{
		if (_skeletalMeshPreviewComponent.IsValid())
		{
			if (_skeletalMeshPreviewComponent->BonesOfInterest.Num() == 1)
			{
				return _skeletalMeshPreviewComponent->GetBoneMatrix(GetSelectedBoneIndex())
					.RemoveTranslation();
			}
		}
	}
	return FMatrix::Identity;
}

void FPoseEditorViewportClient::SetWidgetCoordSystemSpace(ECoordSystem inNewCoordSystem)
{
	GEditorModeTools().SetCoordSystem(inNewCoordSystem);
	Invalidate();
}

ECoordSystem FPoseEditorViewportClient::GetWidgetCoordSystemSpace() const
{
	return (_widgetMode == FWidget::WM_Scale) ? COORD_Local : GEditorModeTools().GetCoordSystem();
}

void FPoseEditorViewportClient::OnShowBones()
{
	auto previewComponent = _skeletalMeshPreviewComponent.Get();
	if (previewComponent)
	{ 
		previewComponent->bDisplayBones = !previewComponent->bDisplayBones;
		previewComponent->MarkRenderStateDirty();
		// force the viewport to redraw immediately after the user checks/unchecks this option in
		// the menu, otherwise the viewport will only be redrawn when the menu is dismissed (which
		// is not what the user would expect to happen)
		Invalidate();
	}
}

bool FPoseEditorViewportClient::IsShowingBones() const
{
	return _skeletalMeshPreviewComponent.IsValid() && _skeletalMeshPreviewComponent->bDisplayBones;
}

void FPoseEditorViewportClient::SelectBone(const FName& inBoneName)
{
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		_skeletalMeshPreviewComponent->BonesOfInterest.Empty();
		
		const auto boneIndex = 
			_skeletalMeshPreviewComponent->SkeletalMesh->Skeleton->GetReferenceSkeleton()
			.FindBoneIndex(inBoneName);

		if (boneIndex != INDEX_NONE)
		{
			_skeletalMeshPreviewComponent->BonesOfInterest.Add(boneIndex);
		}

		OnSelectionChanged.ExecuteIfBound();
	}
}

void FPoseEditorViewportClient::ClearSelection()
{
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		_skeletalMeshPreviewComponent->BonesOfInterest.Empty();

		OnSelectionChanged.ExecuteIfBound();
	}
}

void FPoseEditorViewportClient::ComputeBoneWorldTransformAndColor(
	TArray<FTransform>& outWorldTransforms, TArray<FLinearColor>& outColors
) const
{
	const UDebugSkelMeshComponent* previewComponent = _skeletalMeshPreviewComponent.Get();
	if (!previewComponent || !previewComponent->SkeletalMesh)
	{
		return;
	}

	auto& spaceBases = previewComponent->SpaceBases;

	// preallocate the required space in the output arrays
	if (outWorldTransforms.Num() < spaceBases.Num())
	{
		outWorldTransforms.AddUninitialized(spaceBases.Num() - outWorldTransforms.Num());
	}
	if (outColors.Num() < spaceBases.Num())
	{
		outColors.AddUninitialized(spaceBases.Num() - outColors.Num());
	}

	for (auto boneIndex : previewComponent->RequiredBones)
	{
		const int32 parentBoneIndex = 
			previewComponent->SkeletalMesh->RefSkeleton.GetParentIndex(boneIndex);

		outWorldTransforms[boneIndex] = spaceBases[boneIndex] * previewComponent->ComponentToWorld;

		if (previewComponent->BonesOfInterest.Contains(boneIndex))
		{
			outColors[boneIndex] = FLinearColor(1.0f, 0.34f, 0.0f, 1.0f);
		}
		else
		{
			// by default the root bone will be red, and any other bones will be white
			outColors[boneIndex] = (parentBoneIndex >= 0) ? FLinearColor::White : FLinearColor::Red;
		}
	}
}

void FPoseEditorViewportClient::DrawBones(
	FPrimitiveDrawInterface* inPDI, const TArray<FTransform>& inWorldTransforms,
	const TArray<FLinearColor>& inColors
)
{
	const UDebugSkelMeshComponent* previewComponent = _skeletalMeshPreviewComponent.Get();
	if (!previewComponent || !previewComponent->SkeletalMesh)
	{
		return;
	}

	for (auto boneIndex : previewComponent->RequiredBones)
	{
		const int32 parentBoneIndex =
			previewComponent->SkeletalMesh->RefSkeleton.GetParentIndex(boneIndex);

		FVector boneBasePos, boneTipPos;

		if (parentBoneIndex >= 0) // regular bone
		{
			boneBasePos = inWorldTransforms[parentBoneIndex].GetLocation();
			boneTipPos = inWorldTransforms[boneIndex].GetLocation();
		}
		else // root bone
		{
			boneBasePos = FVector::ZeroVector;
			boneTipPos = inWorldTransforms[boneIndex].GetLocation();
		}

		auto boneHitProxy = new HSkellyBoneProxy(
			previewComponent->SkeletalMesh->RefSkeleton.GetBoneName(boneIndex)
		);
		const float boneTipRadius = 1.0f;
		const FVector boneTipToBase = (boneBasePos - boneTipPos);
		// DrawWireCone() calls this 'cone radius'
		const float coneLength = boneTipToBase.Size();
		const float coneAngle = FMath::RadiansToDegrees(FMath::Atan(boneTipRadius / coneLength));
		TArray<FVector> coneVerts;

		// TODO: add non-wireframe bone drawing, perhaps by making the body mesh translucent
		inPDI->SetHitProxy(boneHitProxy);
		DrawWireSphere(inPDI, boneTipPos, inColors[boneIndex], boneTipRadius, 10, SDPG_Foreground);
		DrawWireCone(
			inPDI, FRotationMatrix::MakeFromX(boneTipToBase) * FTranslationMatrix(boneTipPos),
			coneLength, coneAngle, 4, inColors[boneIndex], SDPG_Foreground, coneVerts
		);
		// this will automatically destroy boneHitProxy since it's not referenced by anything else
		inPDI->SetHitProxy(nullptr);
	}
}

void FPoseEditorViewportClient::UpdatePreviewSceneSetup()
{
	if (_skeletalMeshPreviewComponent.IsValid() && _skeletalMeshPreviewComponent->SkeletalMesh)
	{
		// TODO: move the floor to the bottom of the mesh's bounding box
	}
}

void FPoseEditorViewportClient::FocusViewportOnPreviewComponent()
{
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		auto bounds = _skeletalMeshPreviewComponent->CalcBounds(FTransform::Identity);
		FocusViewportOnBox(bounds.GetBox(), true);
		FEditorViewportClient::Invalidate();
	}
}

int32 FPoseEditorViewportClient::GetSelectedBoneIndex() const
{
	if (_skeletalMeshPreviewComponent.IsValid())
	{
		if (_skeletalMeshPreviewComponent->BonesOfInterest.Num() == 1)
		{
			return _skeletalMeshPreviewComponent->BonesOfInterest[0];
		}
	}
	return INDEX_NONE;
}

} // namespace Skelly
