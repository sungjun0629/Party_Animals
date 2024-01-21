// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "NetGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "SessionInfoWidget.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/ComboBox.h>
#include "Components/ComboBoxString.h"

void ULoginWidget::NativeConstruct()
{
	gi = GetGameInstance<UNetGameInstance>();

	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateButton);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnSliderMoved);
	btn_CreateSelection->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateSelection);
	btn_FindSelection->OnClicked.AddDynamic(this, &ULoginWidget::OnClickFindSelection);
	btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickFindButton);
	btn_BackFromCreate->OnClicked.AddDynamic(this, &ULoginWidget::BacktoFirstCanvas);
	btn_BackFromFind->OnClicked.AddDynamic(this, &ULoginWidget::BacktoFirstCanvas);
	btn_Next->OnClicked.AddDynamic(this, &ULoginWidget::OnClickNextButton);
	/*cbs_selectMesh->OnSelectionChanged.AddDynamic(this, &ULoginWidget::OnMeshItemSelected);
	cbs_selectColor->OnSelectionChanged.AddDynamic(this, &ULoginWidget::OnColorItemSelected);*/

	if (gi != nullptr)
	{
		gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddRoomSlot);
		gi->onFindButtonActivation.AddDynamic(this, &ULoginWidget::ChangeButtonActivation);
	}
}

void ULoginWidget::OnClickCreateButton()
{
	if (gi != nullptr && !edit_roomName->GetText().IsEmpty())
	{
		gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
	}
}

void ULoginWidget::OnSliderMoved(float value)
{
	text_sliderCount->SetText(FText::AsNumber((int32)value));
}

void ULoginWidget::OnClickCreateSelection()
{
	SwitchCanvas(2);
}

void ULoginWidget::OnClickFindSelection()
{
	SwitchCanvas(3);
	OnClickFindButton();
}

void ULoginWidget::OnClickFindButton()
{
	if (gi != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnClickFindButton"));
		gi->FindOtherSession();
		sb_roomListBox->ClearChildren();
	}
}

void ULoginWidget::OnClickNextButton()
{
	if (!edit_userName->GetText().IsEmpty())
	{
		gi->mySessionName = edit_userName->GetText().ToString();
		gi->playerInitInfo.userName = edit_userName->GetText().ToString();
		//gi->playerInitInfo.Set(edit_userName->GetText().ToString(), currentMeshItem, currentColorItem);
		SwitchCanvas(1);
	}
}

void ULoginWidget::AddRoomSlot(struct FSessionSlotInfo slotInfo)
{
	// 슬롯 위젯을 생성한다.
	sessionSlot = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidget);

	if (sessionSlot != nullptr)
	{
		// 슬롯 위젯의 각 요소에 슬롯 정보를 넣는다.
		sessionSlot->text_RoomName->SetText(FText::FromString(slotInfo.roomName));
		sessionSlot->text_HostName->SetText(FText::FromString(slotInfo.hostName));
		sessionSlot->text_PlayerCount->SetText(FText::FromString(slotInfo.playerCount));
		sessionSlot->text_PingSpeed->SetText(FText::AsNumber(slotInfo.pingSpeed));
		sessionSlot->sessionIndex = slotInfo.sessionIndex;

		// 생성한 슬롯 위젯을 스크롤 박스에 자식으로 추가한다.
		sb_roomListBox->AddChild(sessionSlot);

	}
}

void ULoginWidget::ChangeButtonActivation(bool bIsActivation)
{
	btn_FindSession->SetIsEnabled(bIsActivation);
}

//void ULoginWidget::OnMeshItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
//{
//	if (SelectionType == ESelectInfo::OnMouseClick || ESelectInfo::OnKeyPress)
//	{
//		if (SelectedItem == "Croco")
//		{
//			currentMeshItem = 0;
//		}
//		else if (SelectedItem == "Corgi")
//		{
//			currentMeshItem = 1;
//		}
//		else if (SelectedItem == "Otter")
//		{
//			currentMeshItem = 2;
//		}
//		else if (SelectedItem == "Dino")
//		{
//			currentMeshItem = 3;
//		}
//	}
//}

void ULoginWidget::OnColorItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Red")
	{
		currentColorItem = FLinearColor::Red;
	}
	else if (SelectedItem == "Green")
	{
		currentColorItem = FLinearColor::Green;
	}
	else if (SelectedItem == "Blue")
	{
		currentColorItem = FLinearColor::Blue;
	}
}

void ULoginWidget::BacktoFirstCanvas()
{
	ws_SessionUISwitch->SetActiveWidgetIndex(0);
}

void ULoginWidget::SwitchCanvas(int32 index)
{
	ws_SessionUISwitch->SetActiveWidgetIndex(index);
}
