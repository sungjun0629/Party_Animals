// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameModeBase.h"
#include <GameFramework/PlayerStart.h>
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/TargetPoint.h>

AActor* ABattleGameModeBase::ChoosePlayerStart_Implementation(AController* player)
{
	// 플레이어 스타트 액터 4개로 제한
	TArray<APlayerStart*> playerStarts;
	playerStarts.SetNum(4);

	for (TActorIterator<APlayerStart> it(GetWorld()); it; ++it)
	{
		APlayerStart* ps = *it;

		// 만일, 플레이어 스타트 액터의 태그가 "First"라면 홀수 번째 플레이어 스타트로 반환한다.
		// 그렇지 않다면, 짝수 번째 플레이어 스타트로 반환한다.
		if (ps->ActorHasTag(FName("First")))
		{
			playerStarts[0] = ps;
		}
		else if (ps->ActorHasTag(FName("Seconde")))
		{
			playerStarts[1] = ps;
		}
		else if (ps->ActorHasTag(FName("Third")))
		{
			playerStarts[2] = ps;
		}
		else
		{
			playerStarts[3] = ps;
		}
	}

	APlayerStart* chooseStart = playerStarts[callNumber];
	callNumber = (callNumber + 1) % playerStarts.Num();
	return chooseStart;
}

void ABattleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	//{
	//	AActor* searchActor = *it;
	//	if (searchActor->GetName().Contains("TargetPoint"))
	//	{
	//		targetPoints.Add(searchActor);
	//	}
	//}

	//Weapons.SetNum(4);
	//for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	//{
	//	AActor* searchWeapon = *it;
	//	if (searchWeapon->GetName().Contains("BP_Wea"))
	//	{
	//		Weapons.Add(searchWeapon);
	//	}
	//}

	///*Weapons.Add(gun);
	//Weapons.Add(bow);
	//Weapons.Add(magicstick);
	//Weapons.Add(hammer);*/

	//for (int32 i = 0; i < 2; i++)
	//{
	//	if (Weapons[i] != nullptr)
	//	{
	//	int32 drawTarget = FMath::RandRange(0, targetPoints.Num() - 1);
	//	int32 drawWeapon = FMath::RandRange(0, Weapons.Num() - 1);

	//	FActorSpawnParameters param;
	//	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//	GetWorld()->SpawnActor<AActor>(targetPoints[drawTarget]->GetActorLocation(), targetPoints[drawTarget]->GetActorRotation(), param);

	//	targetPoints.RemoveAt(drawTarget);
	//	Weapons.RemoveAt(drawWeapon);
	//	}
	//}
}