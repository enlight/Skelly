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
#include "SSkellySkeletonTree.h"

#define LOCTEXT_NAMESPACE "Skelly.SkeletonTreeWidget"

namespace Skelly {

class FBoneTreeItem : public TSharedFromThis<FBoneTreeItem>
{
public:
	FName BoneName;
	FBoneTreeItemWeakPtr Parent;
	TArray<FBoneTreeItemPtr> Children;

	FBoneTreeItem(const FName& inBoneName) : BoneName(inBoneName) {}

	FText GetToolTipText() const
	{
		if (Parent.IsValid())
		{
			return FText::Format(
				LOCTEXT("BoneItemToolTip", "Parent Bone: {0}"),
				FText::FromName(Parent.Pin()->BoneName)
			);
		}
		return FText::GetEmpty();
	}
};

void SSkeletonTree::Construct(const FArguments& inArgs)
{
	OnSelectionChanged = inArgs._OnSelectionChanged;

	ChildSlot
	[
		SAssignNew(_treeView, SBoneTreeView)
		.SelectionMode(ESelectionMode::Single)
		.TreeItemsSource(&_rootBoneItems)
		// get child items for any given parent item
		.OnGetChildren(this, &SSkeletonTree::TreeView_OnGetChildren)
		// generate a widget for each item
		.OnGenerateRow(this, &SSkeletonTree::TreeView_OnGenerateRow)
		.OnSelectionChanged(this, &SSkeletonTree::TreeView_OnSelectionChanged)
	];
}

void SSkeletonTree::Populate(const FReferenceSkeleton& inReferenceSkeleton)
{
	_rootBoneItems.Empty();
	_allBoneItems.Empty(inReferenceSkeleton.GetNum());

	for (int32 boneIndex = 0; boneIndex < inReferenceSkeleton.GetNum(); ++boneIndex)
	{
		FName boneName = inReferenceSkeleton.GetBoneName(boneIndex);
		int32 parentBoneIndex = inReferenceSkeleton.GetParentIndex(boneIndex);
		TSharedRef<FBoneTreeItem> newBone = MakeShareable(new FBoneTreeItem(boneName));
		if (parentBoneIndex == INDEX_NONE)
		{
			_rootBoneItems.Add(newBone);
		}
		else if (parentBoneIndex >= 0)
		{
			// assume that the skeleton is well-formed such that the parent bone always has a lower
			// index than any of its child bones
			check(parentBoneIndex < boneIndex);
			if (parentBoneIndex < boneIndex)
			{
				newBone->Parent = _allBoneItems[parentBoneIndex];
				_allBoneItems[parentBoneIndex]->Children.Add(newBone);
			}
		}
		_allBoneItems.Add(newBone);
		// fully expand the tree by default
		_treeView->SetItemExpansion(newBone, true);
	}

	_treeView->RequestTreeRefresh();
}

void SSkeletonTree::GetSelectedBoneNames(TArray<FName>& outBoneNames) const
{
	auto selectedItems = _treeView->GetSelectedItems();
	for (const auto& item : selectedItems)
	{
		outBoneNames.Add(item->BoneName);
	}
}

void SSkeletonTree::SetSelectedBoneNames(const TArray<FName>& inBoneNames)
{
	_treeView->ClearSelection();

	for (const auto& boneItem : _allBoneItems)
	{
		for (const auto& boneName : inBoneNames)
		{
			if (boneItem->BoneName == boneName)
			{
				_treeView->SetItemSelection(boneItem, true);
				// if only one bone is selected make sure the user can see it
				if (inBoneNames.Num() == 1)
				{
					_treeView->RequestScrollIntoView(boneItem);
				}
				break;
			}
		}
		// no need to keep going once all the required bones have been selected
		if (_treeView->GetNumItemsSelected() == inBoneNames.Num())
		{
			break;
		}
	}
}

TSharedRef<ITableRow> SSkeletonTree::TreeView_OnGenerateRow(
	FBoneTreeItemPtr inItem, const TSharedRef<STableViewBase>& inOwnerTable
)
{
	return
		SNew(STableRow<FBoneTreeItemPtr>, inOwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromName(inItem->BoneName))
				.ToolTipText(inItem->GetToolTipText())
			]
		];
}

void SSkeletonTree::TreeView_OnGetChildren(
	FBoneTreeItemPtr inParent, TArray<FBoneTreeItemPtr>& outChildren
)
{
	if (inParent.IsValid())
	{
		outChildren = inParent->Children;
	}
}

void SSkeletonTree::TreeView_OnSelectionChanged(
	FBoneTreeItemPtr inSelection, ESelectInfo::Type inSelectInfo
)
{
	// only invoke the delegate if the user explicitly selected a bone in the tree view,
	// otherwise an endless loop may occur when SSkeletonTree attempts to change the selection
	// programmatically
	if (inSelectInfo != ESelectInfo::Direct)
	{
		OnSelectionChanged.ExecuteIfBound();
	}
}

} // namespace Skelly

#undef LOCTEXT_NAMESPACE
