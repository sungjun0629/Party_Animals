// Fill out your copyright notice in the Description page of Project Settings.


#include "GasCollsion.h"
#include <Components/BoxComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "Net/UnrealNetwork.h"	

// Sets default values
AGasCollsion::AGasCollsion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = boxComp;
	boxComp->SetRelativeScale3D(FVector(5));

	particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Gas"));
	particle->SetupAttachment(RootComponent);
	particle->SetRelativeScale3D(FVector(0.2f));
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> temp (TEXT("/Game/SJ/FogTest/LocalFog_P"));

	if (temp.Succeeded())
	{
		particle->SetTemplate(temp.Object);
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void AGasCollsion::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AGasCollsion::OnBeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AGasCollsion::OnEndOverlap);

	//player = Cast<ASY_BasePlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASY_BasePlayer::StaticClass()));

}

// Called every frame
void AGasCollsion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	curtime += DeltaTime;


	if (curtime >= delaytime)
	{
		boxComp->SetRelativeScale3D(boxComp->GetRelativeScale3D() + DeltaTime * 1.7f);
		particle->SetRelativeScale3D(boxComp->GetRelativeScale3D() * particleScale);
	}

	if (player != nullptr && bisOverlapped)
	{
		player->fogTime += DeltaTime;
	}

}

void AGasCollsion::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bisOverlapped = true;

	player = Cast<ASY_BasePlayer>(OtherActor);
}

void AGasCollsion::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bisOverlapped = false;
	player->fogTime = 0;
}

void AGasCollsion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGasCollsion, curtime);
}