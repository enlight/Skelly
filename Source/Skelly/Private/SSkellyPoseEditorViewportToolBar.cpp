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
#include "SSkellyPoseEditorViewportToolBar.h"
#include "EditorStyle.h"
#include "SEditorViewport.h"
#include "EditorViewportCommands.h"
#include "SEditorViewportCameraMenu.h"
#include "SEditorViewportViewMenu.h"
#include "STransformViewportToolbar.h"

namespace Skelly {

void SPoseEditorViewportToolBar::Construct(
	const FArguments& inArgs, TSharedPtr<SEditorViewport> inViewport
)
{
	const float menuPadding = 2.0f;
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
		.ForegroundColor(FEditorStyle::GetSlateColor("DefaultForeground"))
		[
			SNew(SHorizontalBox)
			// left-most drop-down arrow menu
			/*
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(menuPadding)
			[
				SNew(SEditorViewportToolbarMenu)
				.ParentToolBar(SharedThis(this))
				.Image("EditorViewportToolBar.MenuDropdown")
				.OnGetMenuContent(this, &SPoseEditorViewportToolBar::GeneratePrimaryMenu)
			]
			*/
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(menuPadding)
			[
				SNew(SEditorViewportCameraMenu, inViewport.ToSharedRef(), SharedThis(this))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(menuPadding)
			[
				SNew(SEditorViewportViewMenu, inViewport.ToSharedRef(), SharedThis(this))
			]
			+ SHorizontalBox::Slot()
			.Padding(menuPadding)
			.HAlign(HAlign_Right)
			[
				SNew(STransformViewportToolBar)
				.Viewport(inViewport)
				.CommandList(inViewport->GetCommandList())
			]
		]
	];

	SViewportToolBar::Construct(SViewportToolBar::FArguments());
}
/*
TSharedRef<SWidget> SPoseEditorViewportToolBar::GeneratePrimaryMenu() const
{

}
*/
} // namespace Skelly
