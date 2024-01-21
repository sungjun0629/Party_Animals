// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimState.h"
#include "SY_BasePlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Wea_Hammer.h"

void UWeaponAnimState::Received_NotifyBegin()
{
	

	//magicstick = Cast<AWea_Hammer>(UGameplayStatics::GetActorOfClass(GetWorld(), AWea_Hammer::StaticClass()));

}

void UWeaponAnimState::Received_NotifyTick()
{

}

void UWeaponAnimState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	 
	player = Cast<ASY_BasePlayer>(MeshComp);

	hammer = Cast<AWea_Hammer>(UGameplayStatics::GetActorOfClass(GetWorld(), AWea_Hammer::StaticClass()));

	if (player && player->bisHammer)
	{
		FVector start = hammer->bodyMesh->GetSocketLocation(FName("headbase"));
		FVector end = hammer->bodyMesh->GetSocketLocation(FName("headend"));

		TArray<AActor*> ActorsToIgnore;
		FCollisionQueryParams params;
		FHitResult hitResult;

		bool bhit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 30.0f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true);

		if (bhit)
		{
			if (ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(hitResult.GetActor()))
			{
				hitActor->ServerDamaged(5);
			}
		}


		FVector startpos = hammer->bodyMesh->GetSocketLocation(FName("stickbase"));
		FVector endpos = hammer->bodyMesh->GetSocketLocation(FName("stickend"));

		TArray<AActor*> ActorsToIgnore2;
		FHitResult hitResult2;

		bool bhit2 = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startpos, endpos, 30.0f, TraceTypeQuery1, false, ActorsToIgnore2, EDrawDebugTrace::ForDuration, hitResult2, true);

		if (bhit2)
		{
			if (ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(hitResult2.GetActor()))
			{
				hitActor->ServerDamaged(3);
			}
		}
	}

	else if (player && player->bisMagicStick)
	{
		FVector start = hammer->bodyMesh->GetSocketLocation(FName("headbase"));
		FVector end = hammer->bodyMesh->GetSocketLocation(FName("headend"));

		TArray<AActor*> ActorsToIgnore;
		FCollisionQueryParams params;
		FHitResult hitResult;

		bool bhit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 30.0f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true);

		if (bhit)
		{
			if (ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(hitResult.GetActor()))
			{
				hitActor->ServerDamaged(5);
			}
		}


		FVector startpos = hammer->bodyMesh->GetSocketLocation(FName("stickbase"));
		FVector endpos = hammer->bodyMesh->GetSocketLocation(FName("stickend"));

		TArray<AActor*> ActorsToIgnore2;
		FHitResult hitResult2;

		bool bhit2 = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startpos, endpos, 30.0f, TraceTypeQuery1, false, ActorsToIgnore2, EDrawDebugTrace::None, hitResult2, true);

		if (bhit2)
		{
			if (ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(hitResult2.GetActor()))
			{
				hitActor->ServerDamaged(3);
			}
		}
	}


}
