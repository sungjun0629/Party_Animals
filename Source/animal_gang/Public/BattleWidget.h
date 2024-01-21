// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	class ASY_BasePlayer* player;

	// 상단에 sessionName Bind 해주기
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	//class UTextBlock* text_userName;

	
};
