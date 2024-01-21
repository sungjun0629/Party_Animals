// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BattleMapScript.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API ABattleMapScript : public ALevelScriptActor
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	/*UPROPERTY(EditAnywhere, Category = settings)
	TArray<class AActor*> target*/
};
