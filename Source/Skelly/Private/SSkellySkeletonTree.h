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

struct FReferenceSkeleton;

namespace Skelly {

typedef TSharedPtr<class FBoneTreeItem> FBoneTreeItemPtr;
typedef TWeakPtr<class FBoneTreeItem> FBoneTreeItemWeakPtr;

/** A tree view that displays all available asset types grouped by asset categories. */
class SSkeletonTree : public SCompoundWidget
{
public:
	DECLARE_DELEGATE(FOnSelectionChanged)

public:
	SLATE_BEGIN_ARGS(SSkeletonTree) {}
		SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& inArgs);

	/** Populate the tree view with bones from the given skeleton. */
	void Populate(const FReferenceSkeleton& inReferenceSkeleton);
	/** Get the names of all the bones currently selected in the tree view. */
	void GetSelectedBoneNames(TArray<FName>& outBoneNames) const;
	/** Set the bones that should be selected in the tree view. */
	void SetSelectedBoneNames(const TArray<FName>& inBoneNames);

protected:
	/** Invoked when the selection in the tree view changes. */
	FOnSelectionChanged OnSelectionChanged;

private:
	// called by TreeView to generate a table row for the given item
	TSharedRef<ITableRow> TreeView_OnGenerateRow(
		FBoneTreeItemPtr inItem, const TSharedRef<STableViewBase>& inOwnerTable
	);

	// called by TreeView to obtain child items for the given parent item
	void TreeView_OnGetChildren(
		FBoneTreeItemPtr inParent, TArray<FBoneTreeItemPtr>& outChildren
	);

	// called by TreeView when selection changes
	void TreeView_OnSelectionChanged(FBoneTreeItemPtr inSelection, ESelectInfo::Type inSelectInfo);

private:
	typedef STreeView<FBoneTreeItemPtr> SBoneTreeView;
	TSharedPtr<SBoneTreeView> _treeView;
	
	// top-level items
	TArray<FBoneTreeItemPtr> _rootBoneItems;

	// all items in the tree (for easy searching)
	TArray<TSharedRef<FBoneTreeItem>> _allBoneItems;
};

} // namespace Skelly