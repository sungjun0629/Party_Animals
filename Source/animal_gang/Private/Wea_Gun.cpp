// Fill out your copyright notice in the Description page of Project Settings.


#include "Wea_Gun.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Net/UnrealNetwork.h"
#include "SY_BasePlayer.h"

// Sets default values
AWea_Gun::AWea_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	SetRootComponent(bodyMesh);
	bodyMesh->SetRelativeScale3D(FVector(0.15f));
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ConstructorHelpers::FObjectFinder<UStaticMesh> Temp(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	if (Temp.Succeeded())
	{
		bodyMesh->SetStaticMesh(Temp.Object);
	}

	ProjectComp = CreateDefaultSubobject<UProjectileMovementComponent>("Project Comp");
	ProjectComp->InitialSpeed = 1000;
	ProjectComp->MaxSpeed = 1000;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWea_Gun::BeginPlay()
{
	Super::BeginPlay();
	bodyMesh->OnComponentBeginOverlap.AddDynamic(this, &AWea_Gun::OnOverlap);
}

// Called every frame
void AWea_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWea_Gun::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (ASY_BasePlayer* player = Cast<ASY_BasePlayer>(OtherActor))
	{
		ServerOnGunDamage(player);
	}
	Destroyed();
}


void AWea_Gun::ServerOnGunDamage_Implementation(class ASY_BasePlayer* player)
{
	player->bGunDamaged = true;
	player->ServerDamaged(gunStunTime);
	MultiOnGunDamage(player);
}

void AWea_Gun::MultiOnGunDamage_Implementation(class ASY_BasePlayer* player)
{
	if (newMat1 != nullptr) 
	{
		player->GetMesh()->SetSimulatePhysics(true);
		player->GetMesh()->SetOverlayMaterial(newMat1);
	}
}

void AWea_Gun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWea_Gun, newMat1);
}