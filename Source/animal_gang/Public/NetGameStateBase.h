// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API ANetGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	TArray<class APlayerState*> GetPlayerArrayByScore();
};
