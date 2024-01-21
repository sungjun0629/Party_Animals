// Fill out your copyright notice in the Description page of Project Settings.


#include "Wea_BowActor.h"
#include <Components/ArrowComponent.h>
#include "Components/BoxComponent.h"
#include <Components/StaticMeshComponent.h>
#include "Net/UnrealNetwork.h"
#include "Wea_Arrow.h"

// Sets default values
AWea_BowActor::AWea_BowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetBoxExtent(FVector(32));
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	bodyMesh->SetupAttachment(RootComponent);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	arrowBody = CreateDefaultSubobject<UStaticMeshComponent>("ArrowBody");
	arrowBody->SetupAttachment(bodyMesh);

	arrowHead = CreateDefaultSubobject<UStaticMeshComponent>("ArrowHead");
	arrowHead->SetupAttachment(arrowBody);

	arrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow");
	arrowComp->SetupAttachment(RootComponent);
	arrowComp->SetRelativeLocation(FVector(0, 90, 9));
	arrowComp->SetRelativeRotation(FRotator(0, 90, 0));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWea_BowActor::BeginPlay()
{
	Super::BeginPlay();
	bodyMesh->OnComponentBeginOverlap.AddDynamic(this, &AWea_BowActor::OnOverlap);
}

// Called every frame
void AWea_BowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWea_BowActor::Fire()
{
	arrowHead->SetVisibility(false);
	arrowBody->SetVisibility(false);
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//float yaw = arrowComp->GetComponentRotation().Yaw * 90;
	//FRotator rot = FRotator(arrowComp->GetComponentRotation().Pitch, yaw, arrowComp->GetComponentRotation().Roll);

	AWea_Arrow* arrow = GetWorld()->SpawnActor<AWea_Arrow>(arrowFac, arrowComp->GetComponentLocation(), arrowComp->GetComponentRotation(), params);

	FTimerHandle reloadHandle;

	GetWorldTimerManager().SetTimer(reloadHandle, this, &AWea_BowActor::SetArrow, reload, false);

}

void AWea_BowActor::SetArrow()
{
	arrowHead->SetVisibility(true);
	arrowBody->SetVisibility(true);
}

/*id AWea_BowActor::ServerFire_Implementation(class ASY_BasePlayer* player)
{
	if (arrow != nullptr)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWea_Arrow* arrowMesh = GetWorld()->SpawnActor<AWea_Arrow>(arrow, bodyMesh->GetSocketLocation(FName("Bow")), bodyMesh->GetSocketRotation(FName("Bow")), params);
		
		arrow -> SetOwner(player);
		player->

		
	}
	else
	{
		player->
	}
}*/

void AWea_BowActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWea_BowActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWea_BowActor, attack);
}