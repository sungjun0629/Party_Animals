// Fill out your copyright notice in the Description page of Project Settings.


#include "Wea_Bow.h"
#include <Components/ArrowComponent.h>
#include "Components/BoxComponent.h"
#include <Components/StaticMeshComponent.h>
#include "Wea_Gun.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "SY_BasePlayer.h"

// Sets default values
AWea_Bow::AWea_Bow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetBoxExtent(FVector(32));
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	bodyMesh->SetupAttachment(RootComponent);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	arrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow");
	arrowComp->SetupAttachment(RootComponent);
	arrowComp->SetRelativeLocation(FVector(0, 90, 9));
	arrowComp->SetRelativeRotation(FRotator(0, 90, 0));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWea_Bow::BeginPlay()
{
	Super::BeginPlay();
	bodyMesh->OnComponentBeginOverlap.AddDynamic(this, &AWea_Bow::OnOverlap);
	
	pawn = Cast<ASY_BasePlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASY_BasePlayer::StaticClass()));

	//pawn->onAttackedDelegate.AddDynamic(this, &AWea_Bow::ServerShoot);
}

// Called every frame
void AWea_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void AWea_Bow::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("gun attack"));
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWea_Gun* bullet = GetWorld()->SpawnActor<AWea_Gun>(iceBulletFac, arrowComp->GetComponentTransform(), params);
	
}

void AWea_Bow::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWea_Bow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWea_Bow, attack);
}