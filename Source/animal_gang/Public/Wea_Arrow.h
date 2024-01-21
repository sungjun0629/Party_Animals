// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wea_Arrow.generated.h"

UCLASS()
class ANIMAL_GANG_API AWea_Arrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWea_Arrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UStaticMeshComponent* arrowMesh;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UProjectileMovementComponent* ProjectComp;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UBoxComponent* boxComp;

	UPROPERTY()
	int32 arrowStunTime = 1.0f;

	UPROPERTY()
	float curtime = 0;

	UFUNCTION()	
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	UFUNCTION()
	void ResetArrow();
};
