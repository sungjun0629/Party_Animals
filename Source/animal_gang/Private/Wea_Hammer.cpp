// Fill out your copyright notice in the Description page of Project Settings.


#include "Wea_Hammer.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"
#include "SY_BasePlayer.h"
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AWea_Hammer::AWea_Hammer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*capComp = CreateDefaultSubobject<UBoxComponent>("Sphere");
	RootComponent = capComp;
	capComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	capComp->SetSimulatePhysics(true);
	capComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	capComp->SetBoxExtent(FVector(30, 30, 70));*/


	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	//RootComponent = bodyMesh;
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bodyMesh->SetSimulatePhysics(true);
	bodyMesh->SetRelativeScale3D(FVector(1));
	//bodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWea_Hammer::BeginPlay()
{
	Super::BeginPlay();

	pawn = Cast<ASY_BasePlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASY_BasePlayer::StaticClass()));

	//pawn->onAttackedDelegate.AddDynamic(this, &AWea_Hammer::Hit);
}

// Called every frame
void AWea_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}