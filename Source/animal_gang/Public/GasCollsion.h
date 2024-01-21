// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasCollsion.generated.h"

UCLASS()
class ANIMAL_GANG_API AGasCollsion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasCollsion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UBoxComponent* boxComp;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UParticleSystemComponent* particle;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	bool bisOverlapped = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float curtime = 0;

	UPROPERTY(EditAnywhere, Category = Settings)
	float delaytime = 120.0f;

	UPROPERTY(EditAnywhere, Category = Settings)
	float particleScale = 0.01f;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class ASY_BasePlayer* player;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	};
