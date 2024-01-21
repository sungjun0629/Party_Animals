// Fill out your copyright notice in the Description page of Project Settings.


#include "Wea_Arrow.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Components/BoxComponent.h"
#include "SY_BasePlayer.h"

// Sets default values
AWea_Arrow::AWea_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent> ("Box Component");
	RootComponent = boxComp;
	boxComp -> SetRelativeLocation(FVector(-1, 10, 0));
	boxComp -> SetRelativeRotation(FRotator(0, 0, 0));
	boxComp -> SetRelativeScale3D(FVector(0.5f, 0.1f, 0.1f));
	boxComp -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	
	arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");

	//arrowMesh->SetRelativeRotation(FRotator(-90, 0, 0));
	arrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	arrowMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> Temp(TEXT("/Script/Engine.StaticMesh'/Game/H/Weapon/bow/BOW0/Arrow/arrow_1.arrow_1'"));
	if (Temp.Succeeded())
	{
		arrowMesh->SetStaticMesh(Temp.Object);
	}

	ProjectComp = CreateDefaultSubobject<UProjectileMovementComponent>("Project Comp");
	ProjectComp->InitialSpeed = 1000;
	ProjectComp->MaxSpeed = 1000;
}

// Called when the game starts or when spawned
void AWea_Arrow::BeginPlay()
{
	Super::BeginPlay();
	arrowMesh->OnComponentBeginOverlap.AddDynamic(this, &AWea_Arrow::OnOverlap);
}

// Called every frame
void AWea_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	curtime += DeltaTime;
}

void AWea_Arrow::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASY_BasePlayer* player = Cast<ASY_BasePlayer>(OtherActor);
	if (player != nullptr)
	{
		//player->bGunDamaged = true;
		player->ServerDamaged(arrowStunTime);
		ProjectComp->StopMovementImmediately();
		ProjectComp->ProjectileGravityScale = 0;
		AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);

		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle resetTime;

		GetWorldTimerManager().SetTimer(resetTime, this, &AWea_Arrow::ResetArrow, 4.0f, false);
	}

}

void AWea_Arrow::ResetArrow()
{
	Destroy();
}

