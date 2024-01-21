// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API ABattleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;

	//UFUNCTION(Server, Unreliable)
	//void SpawnWeapon();

	UPROPERTY(EditAnywhere, Category = Settings)
	TArray<AActor*> targetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
	TArray<AActor*> Weapons;
	
	//UPROPERTY(EditAnywhere, Category = Settings)
	//TSubclassOf<class AWea_Hammer> hammer;

	//UPROPERTY(EditAnywhere, Category = Settings)
	//TSubclassOf<class AWea_Hammer> magicstick;

	//UPROPERTY(EditAnywhere, Category = Settings)
	//TSubclassOf<class AWea_BowActor> bow;

	//UPROPERTY(EditAnywhere, Category = Settings)
	//TSubclassOf<class AWea_Bow> gun;

private:
	int32 callNumber = 0;
};
