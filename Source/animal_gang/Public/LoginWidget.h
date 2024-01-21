// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UEditableText* edit_roomName;
	
	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class USlider* slider_playerCount;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UTextBlock* text_sliderCount;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UButton* btn_CreateSession;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UScrollBox* sb_roomListBox;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UButton* btn_FindSession;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UWidgetSwitcher* ws_SessionUISwitch;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UButton* btn_CreateSelection;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_FindSelection;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UButton* btn_BackFromCreate;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category=MySettings)
	class UButton* btn_BackFromFind;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UEditableText* edit_userName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Next;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	//class UComboBoxString* cbs_selectMesh;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	//class UComboBoxString* cbs_selectColor;

	UPROPERTY(EditAnywhere, Category=MySettings)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidget;

	UFUNCTION()
	void OnClickCreateButton();

	UFUNCTION()
	void OnSliderMoved(float value);

	UFUNCTION()
	void OnClickCreateSelection();
	
	UFUNCTION()
	void OnClickFindSelection();

	UFUNCTION()
	void OnClickFindButton();

	UFUNCTION()
	void OnClickNextButton();

	UFUNCTION()
	void AddRoomSlot(struct FSessionSlotInfo slotInfo);

	UFUNCTION()
	void ChangeButtonActivation(bool bIsActivation);

	/*UFUNCTION()
	void OnMeshItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType);*/

	UFUNCTION()
	void OnColorItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void BacktoFirstCanvas();

private:
	UPROPERTY()
	class UNetGameInstance* gi;

	UPROPERTY()
	class USessionInfoWidget* sessionSlot;

	int32 currentMeshItem;
	FLinearColor currentColorItem;
	
	void SwitchCanvas(int32 index);
};
