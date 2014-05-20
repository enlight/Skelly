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

#include "SkellyPoseFactory.generated.h"

/** Factory that creates USkellyPose instances. */
UCLASS(MinimalAPI)
class USkellyPoseFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	/** The skeleton to be associated with a newly constructed pose. */
	UPROPERTY()
	class USkeleton* Skeleton;

public: // UFactory interface
	virtual FText GetDisplayName() const override;
	//virtual FName GetNewAssetThumbnailOverride() const override;
	virtual uint32 GetMenuCategories() const override;
	/** 
	 * Display a configuration dialog to allow the user to specify initial values for some
	 * of the properties of a new pose instance.
	 */
	virtual bool ConfigureProperties() override;
	/** Create and return a new pose instance. */
	virtual UObject* FactoryCreateNew(
		UClass* objectClass, UObject* inParent, FName objectName, EObjectFlags objectFlags, 
		UObject* contextObject, FFeedbackContext* feedbackContext
	) override;

private:
	/** Called by the asset picker when a skeleton asset is selected. */
	void OnSkeletonSelected(const class FAssetData& selectedAssetData);

private:
	TSharedPtr<class SWindow> _assetPickerWindow;
};
