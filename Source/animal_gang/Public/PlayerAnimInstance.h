// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMAL_GANG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = settings)
	bool isPunch = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = settings)
	bool hasBow = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = settings)
	bool hasGun = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = settings)
	float degree = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = settings)
	float speed = 0;

	// 노티파이_드롭킥
	UFUNCTION()
	void AnimNotify_DropKickStart();

	UFUNCTION()
	void AnimNotify_DropKickEnd();

	// 노티파이_헤딩
	UFUNCTION()
	void AnimNotify_HeaderStart();

	UFUNCTION()
	void AnimNotify_HeaderEnd();

	// 노티파이_구르기
	UFUNCTION()
	void AnimNotify_RollStart();
	UFUNCTION()
	void AnimNotify_RollEnd();


private:
	UPROPERTY()
	class ASY_BasePlayer* player;

	UPROPERTY()
	class UCharacterMovementComponent* movementComp;

	float CalculateDir(FVector velocity, FRotator rot);
};
