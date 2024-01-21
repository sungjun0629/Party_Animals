// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wea_BowActor.generated.h"

UCLASS()
class ANIMAL_GANG_API AWea_BowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWea_BowActor();

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
	class UStaticMeshComponent* arrowHead;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UStaticMeshComponent* arrowBody;

	UPROPERTY(VisibleAnywhere, Category = Settings)
	class UArrowComponent* arrowComp;
	
	UPROPERTY(EditDefaultsOnly, Category = Setting)
	TSubclassOf <class AWea_Arrow> arrowFac;

	//UFUNCTION(Server, Unreliable)
	//void ServerFire(class ASY_BasePlayer* player);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void SetArrow();

	float reload = 0.4;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

 	UPROPERTY(Replicated)
	int32 attack = 0.8;
 

	
};
