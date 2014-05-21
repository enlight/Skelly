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
#include "SEditorViewportToolBarCameraMenu.h"
#include "EditorViewportCommands.h"
#include "SEditorViewport.h"

// not a typo, all localized strings in this file are from the level viewport toolbar
#define LOCTEXT_NAMESPACE "LevelViewportToolBar"

namespace Skelly {

void SEditorViewportToolBarCameraMenu::Construct(
	const FArguments& inArgs, TSharedPtr<SEditorViewport> inViewport
)
{
	_viewportWeakPtr = inViewport;

	SEditorViewportToolbarMenu::Construct(
		SEditorViewportToolbarMenu::FArguments()
		.ParentToolBar(inArgs._ParentToolBar)
		.Label(this, &SEditorViewportToolBarCameraMenu::GetMenuLabel)
		.LabelIcon(this, &SEditorViewportToolBarCameraMenu::GetMenuLabelIcon)
		.OnGetMenuContent(this, &SEditorViewportToolBarCameraMenu::GenerateMenuContent)
	);
}

FText SEditorViewportToolBarCameraMenu::GetMenuLabel() const
{
	FText label = LOCTEXT("CameraMenuTitle_Default", "Camera");

	TSharedPtr<FEditorViewportClient> viewportClient;
	if (_viewportWeakPtr.IsValid())
	{
		viewportClient = _viewportWeakPtr.Pin()->GetViewportClient();
	}
	
	if (viewportClient.IsValid())
	{
		switch (viewportClient->ViewportType)
		{
			case LVT_Perspective:
				label = LOCTEXT("CameraMenuTitle_Perspective", "Perspective");
				break;

			case LVT_OrthoXY:
				label = LOCTEXT("CameraMenuTitle_Top", "Top");
				break;

			case LVT_OrthoYZ:
				label = LOCTEXT("CameraMenuTitle_Side", "Side");
				break;

			case LVT_OrthoXZ:
				label = LOCTEXT("CameraMenuTitle_Front", "Front");
				break;
		}
	}

	return label;
}

const FSlateBrush* SEditorViewportToolBarCameraMenu::GetMenuLabelIcon() const
{
	FName icon = NAME_None;

	TSharedPtr<FEditorViewportClient> viewportClient;
	if (_viewportWeakPtr.IsValid())
	{
		viewportClient = _viewportWeakPtr.Pin()->GetViewportClient();
	}

	if (viewportClient.IsValid())
	{
		switch (viewportClient->ViewportType)
		{
			case LVT_Perspective:
				icon = FName("EditorViewport.Perspective");
				break;

			case LVT_OrthoXY:
				icon = FName("EditorViewport.Top");
				break;

			case LVT_OrthoYZ:
				icon = FName("EditorViewport.Side");
				break;

			case LVT_OrthoXZ:
				icon = FName("EditorViewport.Front");
				break;
		}
	}

	return FEditorStyle::GetBrush(icon);
}

TSharedRef<SWidget> SEditorViewportToolBarCameraMenu::GenerateMenuContent() const
{
	const bool bInShouldCloseWindowAfterMenuSelection = true;
	TSharedPtr<SEditorViewport> viewportPtr(_viewportWeakPtr.Pin());
	FMenuBuilder menuBuilder(
		bInShouldCloseWindowAfterMenuSelection,
		viewportPtr.IsValid() ? viewportPtr->GetCommandList() : nullptr
	);

	if (viewportPtr.IsValid())
	{
		// view types
		menuBuilder.AddMenuEntry(FEditorViewportCommands::Get().Perspective);
		menuBuilder.BeginSection(
			"LevelViewportCameraType_Ortho", LOCTEXT("CameraTypeHeader_Ortho", "Othographic")
		);
		menuBuilder.AddMenuEntry(FEditorViewportCommands::Get().Top);
		menuBuilder.AddMenuEntry(FEditorViewportCommands::Get().Side);
		menuBuilder.AddMenuEntry(FEditorViewportCommands::Get().Front);
		menuBuilder.EndSection();
	}
	
	return menuBuilder.MakeWidget();
}


} // namespace Skelly