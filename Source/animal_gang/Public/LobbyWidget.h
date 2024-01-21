// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UButton* gameStart;

	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	//class UButton* ct2;

	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	//class UButton* ct3;

	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	//class UButton* ct4;

	UPROPERTY(BlueprintReadWrite, Category = settings)
	class ASY_BasePlayer* player;

	UPROPERTY()
	class UNetGameInstance* gi;

	UPROPERTY(BlueprintReadWrite, Category = settings)
	int32 currentMeshNum = 0;

	UFUNCTION(BlueprintCallable)
	void OnClickContinue();
};
