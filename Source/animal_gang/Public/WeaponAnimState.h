// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WeaponAnimState.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API UWeaponAnimState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	void Received_NotifyBegin();
	void Received_NotifyTick();

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	class ASY_BasePlayer* player;

	UPROPERTY()
	class AWea_Hammer* hammer;

	/*UPROPERTY()
	class AWea_Hammer* magicstick;*/

	
};
