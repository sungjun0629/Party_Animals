// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "Components/TimelineComponent.h"
#include "SY_BasePlayer.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackSignature, ASY_BasePlayer*, player);


UCLASS()
class ANIMAL_GANG_API ASY_BasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASY_BasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class UCameraComponent* followCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class UPhysicsConstraintComponent* physicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class UPhysicalAnimationComponent* physicalAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class UPhysicsHandleComponent* physicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	class UWidgetComponent* infoWidget;

	// enhanced input
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* imc;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GrabOthersAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeadingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropKickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GetWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LTurnCameraAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RTurnCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BackTurnCameraAction;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = settings)
	FName bodyName = FName("spine_03");

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bisHolding = false;

	UPROPERTY(Replicated)
	bool bisGrabing = false;

	UPROPERTY(Replicated)
	FVector RagdollLoc;

	UPROPERTY(Replicated, BlueprintReadWrite)
	class UPrimitiveComponent* RagdollTarget;

	UPROPERTY(Replicated)
	bool bisBow = false;

	UPROPERTY(Replicated)
	bool bisGun = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bisHammer = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bisMagicStick = false;

	UPROPERTY(Replicated)
	bool bisHeading = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bisPunching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool bisDropKick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool bisStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool bisRagDoll = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = settings)
	bool bGunDamaged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool bIsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool canIncreaseStamina = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool AttempToAttach = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool FailToAttach = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = settings)
	bool Attached = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float stamina = 100.0f;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float fogTime = 0;

	UPROPERTY(Replicated)
	FString myName = "Test Name";

	UPROPERTY(Replicated)
	int32 myMeshNumber = 0;

	UPROPERTY(Replicated)
	FLinearColor myMeshColor = FLinearColor::White;

	/*UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = delegate)
	FAttackSignature onAttackedDelegate;*/

	UPROPERTY(Replicated, EditAnywhere)
	class UMaterialInstance* newMat = NULL;
	
	// 무기 클래스 변경해주기
	UPROPERTY(Replicated)
	class AActor* heldItem;

	// 아이스건
	UPROPERTY(Replicated)
	class AWea_Bow* gun;

	UPROPERTY(Replicated)
	class AWea_Gun* bullet;

	// 매직스틱
	UPROPERTY(Replicated)
	class AWea_BowActor* bow;

	UPROPERTY(Replicated)
	class AWea_Arrow* arrow;

	// 매직스틱
	UPROPERTY(Replicated)
	class AWea_Hammer* magicStick;

	// 해머
	UPROPERTY(Replicated)
	class AWea_Hammer* hammer;
	
	// 나이아가라 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* nia_comp;

	// 타임라인
	UPROPERTY(EditDefaultsOnly, Category= settings)
	class UCurveFloat* rollCurve;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UBattleWidget> battleWidget;

	UPROPERTY()
	class UBattleWidget* battle_UI;

	UPROPERTY()
	class UPlayerInfoWidget* info_UI;

	// 캐릭터 스켈레탈 메쉬 경로 받아서 붙여주기
	// Dino, Corgi, Croco, Otter 순서로
	UPROPERTY(EditAnywhere, Category = settings)
	TArray<FString> meshPathList = {TEXT("/Game/Anim/Dino5/Dino_5"), TEXT("/Game/SY/Character/Corgi_4"), TEXT("Game/SY/Character/Crocodile_4"), TEXT("/Game/Anim/Otter7/Otter_7")};

	// anim montage
	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* roll_montage;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* dropkick_montage;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* heading_montage;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* strongPunch_montage;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* hammer_montage;

	UPROPERTY(EditDefaultsOnly, Category=Anim)
	class UAnimMontage* grab_montage;

public:
	struct FTimeline rollTimeline;
	
	UFUNCTION(BlueprintCallable)
	void SetRagdoll();
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void ServerSetDieRagdoll();

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void MultiSetDieRagdoll(FPhysicalAnimationData physicalAnimationData);

	// 이동, 점프
	void Move(const FInputActionValue& value);
	void Jump();
	
	// 스프린트
	void Sprint();
	void SprintStop();

	// 데미지 받는 함수
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDamaged(int32 value);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MultiDamaged(int32 value);

	UFUNCTION(Server, Reliable)
	void ServerRecoverDamage();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiRecoverDamage();


	// 구르기
	UFUNCTION(Server, Unreliable)
	void ServerStartRoll();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiStartRoll();
	//
	//UFUNCTION(Server, Unreliable)
	//void ServerEndRoll();

	//UFUNCTION(NetMulticast, Unreliable)
	//void MultiEndRoll();

	UFUNCTION()
	void OnRoll(float Output);

	//UFUNCTION()
	//void FinishRoll();

	// 헤딩
	UFUNCTION(Server, Unreliable)
	void ServerHeading();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiHeading();
	
	// 드롭킥
	UFUNCTION(Server, Unreliable)
	void ServerDropKick();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiDropKick();

	UFUNCTION(NetMulticast, Reliable)
	void TurnOnCollisionWhileMontagePlay(const bool isDropkick);

	// 무기 줍기
	UFUNCTION(Server, Reliable)
	void ServerGetWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiGetWeapon(const FHitResult result);

	// 무기 놓기
	UFUNCTION(Server, Reliable)
	void ServerDropWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDropWeapon(const FHitResult hitResult);

	// 카메라 시점 변화
	void LTurnCamera();
	void RTurnCamera();
	void BackTurnCamera();

	UFUNCTION()
	void Attack();

	// 펀치 공격
	UFUNCTION()
	void PunchAttack();

	UFUNCTION(Server, Reliable)
	void ServerPunchAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MultiPunchAttack(const FHitResult& hitResult);

	// 아이스건 공격
	UFUNCTION()
	void GunAttack();

	UFUNCTION(Server, Reliable)
	void ServerGunAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MultiGunAttack();


	// 활 공격
	UFUNCTION()
	void BowAttack();

	UFUNCTION(Server, Reliable)
	void ServerBowAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MultiBowAttack();


	// 해머 공격
	UFUNCTION()
	void HammerAttack();

	UFUNCTION(Server, Reliable)
	void ServerHammerAttack();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MultiHammeraAttack();


	// 매직스틱 공격
	UFUNCTION()
	void MagicStickAttack();

	UFUNCTION(Server, Reliable)
	void ServerGrabOthers();

	UFUNCTION(NetMulticast, Reliable)
	void MultiGrabOthers(const FHitResult& hitResult);

	/*UFUNCTION(Server, Reliable)
	void ServerUpdateRagdollLoc();

	UFUNCTION(NetMulticast, Reliable)
	void MultiUpdateRagollLoc();*/

	UFUNCTION(Server, Unreliable)
	void ServerGrabOthersEnd();

	UFUNCTION(NetMulticast, Reliable)
	void MultiGrabOthersEnd();

	UFUNCTION(Server, Unreliable)
	void ServerIsRagdollUpdate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiIsRagdollUpdate(const class ASY_BasePlayer* player);

	UFUNCTION()
	bool HoldingCheck();

	UFUNCTION(Server, Unreliable)
	void ServerSetName(const FString& name);

	// 초기 정보 넘기기
	UFUNCTION(Server, Unreliable)
	void ServerSetInitInfo(struct FPlayerInitInfo initInfo);
	
	// 캐릭터 이름, 메쉬, 컬러 설정
	void InitializePlayer();

	UPROPERTY()
	class USkeletalMesh* selectedMesh;

	UFUNCTION(Server, Unreliable)
	void ServerReadytoStart();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiReadytoStart();
};
