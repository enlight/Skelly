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

#include "SkellyBone.generated.h"

/** 
 * Exposes bone-specific data as properties that can be accessed via the reflection system.
 * 
 * Currently this is used to enable auto-generation of bone-specific property detail views via
 * the PropertyEditor module.
 *
 * @note This class is marked as transient because it shouldn't be persisted as an asset or
 *       part thereof, all the data exposed by the properties in this class is already stored in
 *       some form elsewhere.
 */
UCLASS(Transient, noteditinlinenew, hidedropdown, NotBlueprintType, NotBlueprintable)
class USkellyBone : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category=Object)
	FName BoneName;

	UPROPERTY(VisibleAnywhere, Category=Object)
	FName ParentBoneName;

	UPROPERTY(EditInstanceOnly, Category=Object)
	FTransform BoneTransform;
};
