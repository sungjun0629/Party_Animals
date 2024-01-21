// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wea_Bow.generated.h"

UCLASS()
class ANIMAL_GANG_API AWea_Bow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWea_Bow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UBoxComponent* boxComp;
	
	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UStaticMeshComponent* bodyMesh;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UArrowComponent* arrowComp;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	TSubclassOf <class AWea_Gun> iceBulletFac;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class ASY_BasePlayer* pawn;

	UFUNCTION()
	void Shoot();

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	float coolTime = 0.8f;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION(Server, Reliable)
	//void ServerGrabWeapon(ASY_BasePlayer* player);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastGrabWeapon(ASY_BasePlayer* player);

 	UPROPERTY(Replicated)
 	int32 attack = 5;
 	float reload = 1;

};
