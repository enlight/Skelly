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
#include "SSkellyPoseEditorViewportShowMenu.h"
#include "SkellyPoseEditorViewportCommands.h"
#include "SEditorViewport.h"

#define LOCTEXT_NAMESPACE "PoseEditorViewport"

namespace Skelly {

void SPoseEditorViewportShowMenu::Construct(
	const FArguments& inArgs, TSharedRef<SEditorViewport> inViewport,
	TSharedRef<SViewportToolBar> inParentToolBar
)
{
	_viewportWeakPtr = inViewport;
	
	SEditorViewportToolbarMenu::Construct(
		SEditorViewportToolbarMenu::FArguments()
		.ParentToolBar(inParentToolBar)
		.Cursor(EMouseCursor::Default)
		.Label(LOCTEXT("ShowMenuTitle", "Show"))
		.OnGetMenuContent(this, &SPoseEditorViewportShowMenu::GenerateMenuContent)
	);
}

TSharedRef<SWidget> SPoseEditorViewportShowMenu::GenerateMenuContent() const
{
	const bool bInShouldCloseWindowAfterMenuSelection = true;
	TSharedPtr<SEditorViewport> viewportPtr(_viewportWeakPtr.Pin());
	FMenuBuilder menuBuilder(
		bInShouldCloseWindowAfterMenuSelection,
		viewportPtr.IsValid() ? viewportPtr->GetCommandList() : nullptr
	);

	const auto& commands = FPoseEditorViewportCommands::Get();
	if (viewportPtr.IsValid())
	{
		menuBuilder.AddMenuEntry(commands.ShowBones);
	}

	return menuBuilder.MakeWidget();
}

} // namespace Skelly

#undef LOCTEXT_NAMESPACE
