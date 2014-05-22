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

#include "SkellyPose.generated.h"

/** Stores a skeletal mesh pose. */
UCLASS(notplaceable)
class USkellyPose : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void SetSkeleton(USkeleton* inSkeleton);
	USkeleton* GetSkeleton();

private:
	/** Pose in Bone Space */
	UPROPERTY()
	FA2Pose Pose;

	/** The skeleton this pose is based on. */
	UPROPERTY(AssetRegistrySearchable, Category=Animation, VisibleAnywhere)
	class USkeleton* Skeleton;

	// TODO: UAnimationAsset stores the skeleton guid and updates animation sequences when the
	//       guid changes, the guid changes when the skeleton hierarchy changes so this is
	//       probably something that should be handled for poses too. Will probably deal with this
	//       when implementing skeleton replacement for poses.
};

inline USkeleton* USkellyPose::GetSkeleton()
{
	return Skeleton;
}
