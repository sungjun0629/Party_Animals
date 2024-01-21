// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wea_Gun.generated.h"

UCLASS()
class ANIMAL_GANG_API AWea_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWea_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UStaticMeshComponent* bodyMesh;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UProjectileMovementComponent* ProjectComp;

	UPROPERTY(Replicated, EditAnywhere)
	class UMaterialInstance* newMat1;

	UPROPERTY()
	int32 gunStunTime = 5.0f;

	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerOnGunDamage(class ASY_BasePlayer* player);

	UFUNCTION(NetMulticast, Reliable)
	void MultiOnGunDamage(class ASY_BasePlayer* player);
};
