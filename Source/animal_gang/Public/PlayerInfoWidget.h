// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class ASY_BasePlayer* player;

public:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_userName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = MySetting)
	class UImage* img_readyCheck;
};
