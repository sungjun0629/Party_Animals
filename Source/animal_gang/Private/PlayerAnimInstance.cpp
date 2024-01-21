// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "SY_BasePlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	player = Cast<ASY_BasePlayer>(GetOwningActor());

	if (player != nullptr)
	{
		movementComp = player->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player != nullptr && movementComp != nullptr)
	{
		speed = movementComp->Velocity.Size2D();
		degree = CalculateDir(movementComp->Velocity, player->GetActorRotation());

		isPunch = player->bisPunching;
		hasBow = player->bisBow;
		hasGun = player->bisGun;
	}

}

void UPlayerAnimInstance::AnimNotify_DropKickStart()
{
	player->bisDropKick = true;
}

void UPlayerAnimInstance::AnimNotify_DropKickEnd()
{
	player->bisDropKick = false;
}

void UPlayerAnimInstance::AnimNotify_HeaderStart()
{
	player->bisHeading = true;
}

void UPlayerAnimInstance::AnimNotify_HeaderEnd()
{
	player->bisHeading = false;
}

void UPlayerAnimInstance::AnimNotify_RollStart()
{
	player->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void UPlayerAnimInstance::AnimNotify_RollEnd()
{
	player->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

// ���� �ӵ� �����κ��� ������ ���ϴ� �Լ�
float UPlayerAnimInstance::CalculateDir(FVector velocity, FRotator rot)
{
	if (velocity.IsNearlyZero())
	{
		return 0.0f;
	}

	FVector forwardVector = FRotationMatrix(rot).GetUnitAxis(EAxis::X);
	FVector rightVector = FRotationMatrix(rot).GetUnitAxis(EAxis::Y);
	FVector speedVector = velocity.GetSafeNormal2D();

	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(forwardVector, speedVector)));

	float rightDot = FVector::DotProduct(rightVector, speedVector);
	if (rightDot < 0)
	{
		angle *= -1.0f;
	}

	return angle;
}

