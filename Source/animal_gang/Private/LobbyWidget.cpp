// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "SY_BasePlayer.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include "NetGameInstance.h"
#include <UMG/Public/Components/Image.h>

void ULobbyWidget::NativeConstruct()
{
	player = Cast<ASY_BasePlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASY_BasePlayer::StaticClass()));

	gi = GetGameInstance<UNetGameInstance>();

	//gameStart->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickContinue);
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

void ULobbyWidget::OnClickContinue()
{
	gi->playerInitInfo.meshNumber = currentMeshNum;
	player->ServerReadytoStart();
}