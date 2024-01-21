// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wea_Hammer.generated.h"

UCLASS()
class ANIMAL_GANG_API AWea_Hammer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWea_Hammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class UStaticMeshComponent* bodyMesh;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
	class UBoxComponent* capComp;*/

	UPROPERTY(EditAnywhere, Category = Settings)
	class ASY_BasePlayer* pawn;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* hammer_montage;




};