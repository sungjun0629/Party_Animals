// Fill out your copyright notice in the Description page of Project Settings.


#include "SY_BasePlayer.h"
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <PhysicsEngine/PhysicsConstraintComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "NetPlayerController.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <Engine/EngineTypes.h>
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"	// 네트워크 때 꼭 필요
#include "BattleWidget.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "PlayerInfoWidget.h"
#include "NetGameInstance.h"
#include <UMG/Public/Components/TextBlock.h>
#include "Wea_Bow.h"
#include "Wea_Hammer.h"
#include "Wea_Gun.h"
#include <Components/ArrowComponent.h>
#include "Wea_BowActor.h"
#include <Components/BoxComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/TextBlock.h>
#include "NetGameInstance.h"
#include <Kismet/KismetMathLibrary.h>


// Sets default values
ASY_BasePlayer::ASY_BasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

	physicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("physicsConst"));
	physicsConstraint->SetupAttachment(RootComponent);
	physicsConstraint->SetAngularSwing1Limit(ACM_Limited, 25.0f);
	physicsConstraint->SetAngularSwing2Limit(ACM_Limited, 25.0f);
	physicsConstraint->SetAngularTwistLimit(ACM_Limited, 25.0f);

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeRotation(FRotator(-45, 0, 0));
	cameraBoom->TargetArmLength = 500.0f;
	cameraBoom->bUsePawnControlRotation = true;
	cameraBoom->bInheritPitch = false;
	cameraBoom->bInheritYaw = false;
	cameraBoom->bInheritRoll = false;
	cameraBoom->bDoCollisionTest = false;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -73), FRotator(0, 270, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	physicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));
	physicalAnimation->StrengthMultiplyer = 10.0f;

	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->PerchRadiusThreshold = 15.0f;
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	nia_comp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("iceeffect"));
	nia_comp->SetupAttachment(GetMesh());

	bUseControllerRotationYaw = false;
	bReplicates = true;

	// 플레이어 인포 위젯 컴포넌트로 붙여주기
	infoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerInfoWidget"));
	infoWidget->SetupAttachment(GetMesh());

	/*FTimerHandle initHandler;
	GetWorldTimerManager().SetTimer(initHandler, this, &ASY_BasePlayer::InitializePlayer, 0.5f, false);*/

}

// Called when the game starts or when spawned
void ASY_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SetRagdoll();

	// enhanced input 
	ANetPlayerController* playercontroller = Cast<ANetPlayerController>(GetController());
	if (playercontroller)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playercontroller->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc, 0);
		}
	}
	
	// 메인 위젯 (battle widget) 생성하기
	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		battle_UI = CreateWidget<UBattleWidget>(GetWorld(), battleWidget);
		if (battle_UI != nullptr)
		{
			battle_UI->AddToViewport();
		}
		ServerSetName(GetGameInstance<UNetGameInstance>()->mySessionName);
	}

	// 게임 인스턴스에서 초기값 받아오기
	ServerSetInitInfo(GetGameInstance<UNetGameInstance>()->playerInitInfo);

	// 플레이어 인포 위젯 생성하기
	if (infoWidget != nullptr)
	{
		info_UI = Cast<UPlayerInfoWidget>(infoWidget->GetWidget());
		
	}

	// 타임 라인 등록하기
	FOnTimelineFloat onProgressRoll;
	onProgressRoll.BindUFunction(this, FName("OnRoll"));
	FOnTimelineEvent onFinishRoll;
	onFinishRoll.BindUFunction(this, FName("FinishRoll"));

	// 캐릭터 초기화 지연 실행
	FTimerHandle initHandler;
	GetWorldTimerManager().SetTimer(initHandler, this, &ASY_BasePlayer::InitializePlayer, 0.5f, false);

	// 타임라인에 따라 roll 실행하기
	rollTimeline.AddInterpFloat(rollCurve, onProgressRoll);
	rollTimeline.SetTimelineFinishedFunc(onFinishRoll);
	rollTimeline.SetTimelineLength(1.0f);
	rollTimeline.SetLooping(false);
	
	//// 무기 받기
	//gun = Cast<AWea_Bow>(UGameplayStatics::GetActorOfClass(GetWorld(), AWea_Bow::StaticClass()));
	//bow = Cast<AWea_BowActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWea_BowActor::StaticClass()));
}

// Called every frame
void ASY_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fogTime >= 2.0f)
	{
		ServerSetDieRagdoll();
		GetCharacterMovement()->Deactivate();
	}

	if (canIncreaseStamina)
	{
		stamina += 1.0f;
		if (stamina >= 100)
		{
			canIncreaseStamina = false;
		}
		else
		{
			canIncreaseStamina = true;
		}
	}
	
	if(bisRagDoll) ServerIsRagdollUpdate();
	if(bisDropKick) TurnOnCollisionWhileMontagePlay(true);
	if(bisHeading) TurnOnCollisionWhileMontagePlay(false);
	//if (AttempToAttach)
	//{
	//	FVector startPos = GetMesh()->GetSocketLocation(FName("hand_l_socket"));
	//	FVector endPos = startPos;
	//	TArray<AActor*> actorsToIgnore;
	//	FHitResult hitResult;

	//	bool bhit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startPos, endPos, 50.f, ETraceTypeQuery::TraceTypeQuery3, false, actorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);

	//	if (bhit)
	//	{

	//		AWea_Hammer* Hammer = Cast<AWea_Hammer>(hitResult.GetActor());
	//		if (Hammer) {

	//			Hammer->AttempToAttach = true;
	//			
	//			FTimerHandle TimerHandle;
	//			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	//				{

	//				}, 0.1, false);
	//			if (FailToAttach) {
	//				physicsConstraint->SetConstrainedComponents(GetMesh(), "spine_03", hitResult.GetComponent(), "None");
	//				UE_LOG(LogTemp, Warning, TEXT("AttachToCons"));
	//				AttempToAttach = false;
	//				Attached = true;
	//				//bisHolding = true;
	//				//FailToAttach =false;
	//			}
	//			else {
	//			}

	//		}
	//	}
		//Montage 끝나면 알려준다.
		//AttempToAttach = false;
		//Hammer->AttempToAttach = false;
	//}
}

// Called to bind functionality to input
void ASY_BasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::Move);
		playerInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::Jump);
		playerInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::Sprint);
		playerInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASY_BasePlayer::SprintStop);
		playerInput->BindAction(GrabOthersAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerGrabOthers);
		playerInput->BindAction(GrabOthersAction, ETriggerEvent::Completed, this, &ASY_BasePlayer::ServerGrabOthersEnd);
		playerInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ASY_BasePlayer::Attack);
		playerInput->BindAction(RollAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerStartRoll);
		playerInput->BindAction(HeadingAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerHeading);
		playerInput->BindAction(DropKickAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerDropKick);
		playerInput->BindAction(GetWeaponAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerGetWeapon);
		playerInput->BindAction(DropWeaponAction, ETriggerEvent::Started, this, &ASY_BasePlayer::ServerDropWeapon);
		playerInput->BindAction(LTurnCameraAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::LTurnCamera);
		playerInput->BindAction(RTurnCameraAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::RTurnCamera);
		playerInput->BindAction(BackTurnCameraAction, ETriggerEvent::Triggered, this, &ASY_BasePlayer::BackTurnCamera);
	}
}

void ASY_BasePlayer::SetRagdoll()
{
	FPhysicalAnimationData physicalAnimationData;
	physicalAnimationData.bIsLocalSimulation = true;
	physicalAnimationData.OrientationStrength = 1000.0f;
	physicalAnimationData.AngularVelocityStrength = 50.0f;
	physicalAnimationData.PositionStrength = 20.0f;
	physicalAnimationData.VelocityStrength = 15.0f;
	physicalAnimationData.MaxLinearForce = 5.0f;
	physicalAnimationData.MaxAngularForce = 5.0f;

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -73), FRotator(0, 270, 0));

	physicalAnimation->SetSkeletalMeshComponent(GetMesh());
	physicalAnimation->ApplyPhysicalAnimationSettingsBelow(bodyName, physicalAnimationData);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(bodyName, true, false);
}

void ASY_BasePlayer::ServerSetDieRagdoll_Implementation()
{
	FPhysicalAnimationData physicalAnimationData;
	physicalAnimationData.bIsLocalSimulation = true;
	physicalAnimationData.OrientationStrength = 0;
	physicalAnimationData.AngularVelocityStrength = 0;
	physicalAnimationData.PositionStrength = 0;
	physicalAnimationData.VelocityStrength = 0;
	physicalAnimationData.MaxLinearForce = 0;
	physicalAnimationData.MaxAngularForce = 0;

	FPhysicalAnimationData phyanimdata = physicalAnimationData;
	MultiSetDieRagdoll(phyanimdata);
}

void ASY_BasePlayer::MultiSetDieRagdoll_Implementation(FPhysicalAnimationData physicalAnimationData)
{
	physicalAnimation->SetSkeletalMeshComponent(GetMesh());
	physicalAnimation->ApplyPhysicalAnimationSettingsBelow(bodyName, physicalAnimationData);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(bodyName, true);
	GetMesh()->SetSimulatePhysics(true);
	this->cameraBoom->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "None");
}

void ASY_BasePlayer::Move(const FInputActionValue& value)
{
	FVector2D moveValue = value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FRotator yawRot(0, Controller->GetControlRotation().Yaw, 0);
		FVector forwardDir = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);
		FVector rightDir = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(rightDir, moveValue.X);
		AddMovementInput(forwardDir, moveValue.Y);
	}
}

void ASY_BasePlayer::Jump()
{
	// character.h 에 있는 jump 함수 사용
	Super::Jump();
}

void ASY_BasePlayer::Sprint()
{
	if (!bisHolding)
	{
		GetCharacterMovement()->MaxWalkSpeed = 800;
	}
	else
	{
		// shift 가 눌렸을 때 Max walk speed 를 900으로 바꿔준다.
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	
	stamina -= 0.3f;

	if (stamina <= 0)
	{
		SprintStop();
		canIncreaseStamina = true;
	}
}

void ASY_BasePlayer::SprintStop()
{
	if (!bisHolding)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
	else
	{
		// shift 를 떼면 Max Walk Speed 를 기본 400으로 바꿔준다.
		GetCharacterMovement()->MaxWalkSpeed = 320;
	}
	canIncreaseStamina = true;
}

void ASY_BasePlayer::ServerDamaged_Implementation(int32 stuntime)
{
	if (HasAuthority())
	{
		MultiDamaged_Implementation(stuntime);
	}
	else
	{
		MultiDamaged(stuntime);
	}
}

void ASY_BasePlayer::MultiDamaged_Implementation(int32 stuntime)
{	
	if (bGunDamaged)
	{
		GetMesh()->SetSimulatePhysics(true);
	}
	ServerSetDieRagdoll();
	bisRagDoll = true;
	UE_LOG(LogTemp, Warning, TEXT("setdieragdoll %d "), stuntime);
	GetCharacterMovement()->Deactivate();

	FTimerHandle stunHandle;

	GetWorldTimerManager().SetTimer(stunHandle, this, &ASY_BasePlayer::ServerRecoverDamage, stuntime, false);


}

void ASY_BasePlayer::ServerRecoverDamage_Implementation()
{	
	UE_LOG(LogTemp, Warning, TEXT("ServerRecoverDamage_Implementation"));
	MultiRecoverDamage();
}

void ASY_BasePlayer::MultiRecoverDamage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiRecoverDamage_Implementation"));
	if (bGunDamaged)
	{
		UE_LOG(LogTemp, Warning, TEXT("GUN!!!!!!!!!"));
		nia_comp->Activate(true);
		//GetMesh()->SetSkeletalMesh(selectedMesh);
		GetMesh()->SetOverlayMaterial(nullptr);
	}

	//GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetComponentLocation());
	UE_LOG(LogTemp, Warning, TEXT("Recovering"));
	SetRagdoll();
	GetCharacterMovement()->Activate();
	this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->cameraBoom->AttachToComponent(this->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform, "None");


	bGunDamaged = false;
	bisRagDoll = false;
}

void ASY_BasePlayer::ServerStartRoll_Implementation()
{
	rollTimeline.PlayFromStart();
	MultiStartRoll();
}

void ASY_BasePlayer::MultiStartRoll_Implementation()
{
	PlayAnimMontage(roll_montage);
	//GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

//void ASY_BasePlayer::ServerEndRoll_Implementation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("server END roll"));
//	MultiEndRoll();
//}
//
//void ASY_BasePlayer::MultiEndRoll_Implementation()
//{
//	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
//}

// 타임라인이 진행 중일 때 매 프레임마다 실행되는 함수
void ASY_BasePlayer::OnRoll(float Output)
{
	if (!bisHolding)
	{
		stamina -= 15;

		FVector rollDistance = Output * 1000.0f * GetActorForwardVector();

		GetCharacterMovement()->Velocity = FVector(rollDistance.X, rollDistance.Y, GetCharacterMovement()->Velocity.Z);

		UE_LOG(LogTemp, Warning, TEXT("onroll"));

		canIncreaseStamina = true;
	}
}

// 타임라인이 종료될 때 1회 실행되는 함수
//void ASY_BasePlayer::FinishRoll()
//{
//	UE_LOG(LogTemp, Warning, TEXT("finishroll"));
//	if (HasAuthority())
//	{
//		ServerEndRoll_Implementation();
//	}
//	else
//	{
//		ServerEndRoll();
//	}
//}

void ASY_BasePlayer::Attack()
{
	if(bisHolding)
	{
		if (bisHammer)
		{
			HammerAttack();
		}
		else if (bisGun)
		{
			GunAttack();
		}
		else if (bisMagicStick)
		{
			MagicStickAttack();
		}
		else if (bisBow)
		{
			BowAttack();
		}
	}
	else 
	{
		PunchAttack();
	}
}


void ASY_BasePlayer::ServerGrabOthers_Implementation()
{
	FVector startPos = GetMesh()->GetSocketLocation(FName("hand_l_socket"));
	FVector endPos = startPos + GetMesh()->GetRightVector() * 50;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
	};
	TArray<AActor*> actorsToIgnore;
	FHitResult hitResult;

	/*bool bhit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startPos, endPos, 40.0f,  TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);*/
	bool bhit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos, endPos, 40.0f, objectTypes, false, actorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);
	if (bhit && hitResult.GetActor() != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.GetActor()->GetName());
		ASY_BasePlayer* player = Cast<ASY_BasePlayer>(hitResult.GetActor());
		if (player && player->bisRagDoll)
		{
			MultiGrabOthers(hitResult);
			bisGrabing = true;
			bisPunching = false;
		}

	}
	else
	{
		FVector startPos1 = GetMesh()->GetSocketLocation(FName("hand_r_socket"));
		FVector endPos1 = startPos;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes1 = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
		};
		TArray<AActor*> actorsToIgnore1;
		FHitResult hitResult1;


		bool bhit1 = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos1, endPos1, 40.0f, objectTypes1, false, actorsToIgnore1, EDrawDebugTrace::ForDuration, hitResult1, true);

		if (bhit1 && hitResult1.GetActor() != this )
		{
			ASY_BasePlayer* player = Cast<ASY_BasePlayer>(hitResult1.GetActor());
			if (player && player->bisRagDoll)
			{
				MultiGrabOthers(hitResult1);
				bisGrabing = true;
			}
		}
		else
		{
			bisGrabing = false;
		}
	}
}

void ASY_BasePlayer::MultiGrabOthers_Implementation(const FHitResult& hitResult)
{
	
	bisGrabing = true;
	bisPunching = false;
	PlayAnimMontage(grab_montage);
	physicsConstraint->SetConstrainedComponents(GetCapsuleComponent(), FName("spine_03"), hitResult.GetComponent(), FName("None"));
	

	/*RagdollTarget = hitResult.GetComponent();
	RagdollLoc = this->GetMesh()->GetSocketLocation("hand_l_socket");
	physicsHandle->GrabComponentAtLocationWithRotation(RagdollTarget, hitResult.BoneName, RagdollLoc, GetMesh()->GetRelativeRotation());

	RagdollLoc = UKismetMathLibrary::InverseTransformLocation(GetMesh()->GetSocketTransform("hand_l_socket"), RagdollLoc);

	ServerUpdateRagdollLoc();*/
	/*FVector startPos1 = GetMesh()->GetSocketLocation(FName("hand_r_socket"));
	FVector endPos1 = startPos;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes1 = {
   UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
	};
	TArray<AActor*> actorsToIgnore1;
	FHitResult hitResult1;


	bool bhit1 = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos1, endPos1, 100.0f, objectTypes1, false, actorsToIgnore1, EDrawDebugTrace::None, hitResult1, true);

	if (bhit1)
	{
		physicsConstraint->SetConstrainedComponents(GetCapsuleComponent(), FName("spine_03"), hitResult1.GetComponent(), FName("None"));

		bisGrabing = true;
		bisPunching = false;
	}
	else
	{
		bisGrabing = false;
	}*/
	
}

//void ASY_BasePlayer::ServerUpdateRagdollLoc_Implementation()
//{
//	MultiUpdateRagollLoc();
//}
//
//void ASY_BasePlayer::MultiUpdateRagollLoc_Implementation()
//{
//	if(IsValid(RagdollTarget))
//	{
//		physicsHandle->SetTargetLocation(UKismetMathLibrary::TransformLocation(GetMesh()->GetSocketTransform("hand_l_socket"), RagdollLoc));
//
//		FTimerHandle TimerHandle;
//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
//			{
//
//			}, 0.001, false);
//		
//		MultiUpdateRagollLoc();
//	}
//}

void ASY_BasePlayer::ServerGrabOthersEnd_Implementation()
{
	if (bisGrabing)
	{
		MultiGrabOthersEnd();
	}
}

void ASY_BasePlayer::MultiGrabOthersEnd_Implementation()
{
	physicsConstraint->BreakConstraint();
	bisGrabing = false;
}

void ASY_BasePlayer::ServerIsRagdollUpdate_Implementation()
{
	MultiIsRagdollUpdate(this);
}

void ASY_BasePlayer::MultiIsRagdollUpdate_Implementation(const class ASY_BasePlayer* player)
{
	this->GetCapsuleComponent()->SetWorldLocation(player->GetMesh()->GetComponentLocation());
	//this->GetCapsuleComponent()->AddWorldTransformKeepScale();

}

bool ASY_BasePlayer::HoldingCheck()
{
	/*FVector startPos = GetMesh()->GetComponentLocation();
	FVector endPos = GetMesh()->GetRightVector() * 100 + startPos;
	FCollisionQueryParams param;
	FHitResult hitResult;

	bool bhit = GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECollisionChannel::ECC_Visibility, param);

	if (bhit)
	{
		FVector start = GetMesh()->GetBoneLocation(FName("hand_l"));
		FVector end = start;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {
		   UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
		};
		TArray<AActor*> actorsToIgnore;
		FHitResult hitinfo;

		bool hithit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos, endPos, 30.0f, objectTypes, false, actorsToIgnore, EDrawDebugTrace::None, hitinfo, true);

		if (hithit)
		{
			bisHolding = true;
		}
		else
		{
			bisHolding = false;
		}
	}*/
	return bisHolding;
}

void ASY_BasePlayer::ServerSetName_Implementation(const FString& name)
{
	myName = name;
}

// 초기 데이터 설정 RPC 함수
void ASY_BasePlayer::ServerSetInitInfo_Implementation(struct FPlayerInitInfo initInfo)
{
	myName = initInfo.userName;
	myMeshNumber = initInfo.meshNumber;
	//myMeshColor = initInfo.meshColor;
}

// 플레이어 초기화 함수
void ASY_BasePlayer::InitializePlayer()
{
	// 이름 설정
	if (info_UI != nullptr)
	{
		info_UI->text_userName->SetText(FText::FromString(myName));
	}

	// 메쉬 설정
	selectedMesh = LoadObject<USkeletalMesh>(NULL, *meshPathList[myMeshNumber], NULL, LOAD_None, NULL);
	if (selectedMesh != nullptr)
	{
		GetMesh()->SetSkeletalMesh(selectedMesh);
	}

	//// 컬러 설정
	//UMaterialInterface* mat1 = GetMesh()->GetMaterial(0);
	//UMaterialInterface* mat2 = GetMesh()->GetMaterial(1);

	//UMaterialInstanceDynamic* dynamicMat1 =  UMaterialInstanceDynamic::Create(mat1, this);
	//UMaterialInstanceDynamic* dynamicMat2 =  UMaterialInstanceDynamic::Create(mat2, this);

	//GetMesh()->SetMaterial(0, dynamicMat1);
	//GetMesh()->SetMaterial(1, dynamicMat2);

	//dynamicMat1->SetVectorParameterValue(FName("MyColor"), myMeshColor);
	//dynamicMat2->SetVectorParameterValue(FName("MyColor"), myMeshColor);
	
}

void ASY_BasePlayer::ServerReadytoStart_Implementation()
{
	MultiReadytoStart();
}

void ASY_BasePlayer::MultiReadytoStart_Implementation()
{
	info_UI->img_readyCheck->SetVisibility(ESlateVisibility::Visible);
}

void ASY_BasePlayer::ServerHeading_Implementation()
{
	if (!bisHolding) 
	{
		MultiHeading();

	}
}

void ASY_BasePlayer::MultiHeading_Implementation()
{

	PlayAnimMontage(heading_montage);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			bisHeading = false;
		}, 2.0, false);
	stamina -= 15.0f;
}

void ASY_BasePlayer::ServerDropKick_Implementation()
{
	if(!bisHolding) MultiDropKick();
}

void ASY_BasePlayer::MultiDropKick_Implementation()
{
	// bisdropkick 이 true일 때 (몽타주 노티파이 불릴 때)
	PlayAnimMontage(dropkick_montage);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			bisDropKick = false;
		}, 1.0, false);
	stamina -= 15.0f;
}

void ASY_BasePlayer::TurnOnCollisionWhileMontagePlay_Implementation(const bool isDropkick)
{
	FVector startPos = isDropkick ? GetMesh()->GetBoneLocation(FName("foot_r")) : GetMesh()->GetComponentLocation() + (GetActorUpVector() * 100);
	FVector endPos = startPos;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> hitResult;

	bool bhit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startPos, endPos, 30.0f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);
	if (bhit)
	{
		for (auto result : hitResult)
		{
			class ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(result.GetActor());

			if (hitActor && hitActor != this)
			{
				if (result.BoneName == FName("head") && !hitActor->bisRagDoll)
				{
				/*	hitActor->LaunchCharacter(GetCapsuleComponent()->GetForwardVector() * 200.0f, false, false);*/
					hitActor->ServerDamaged_Implementation(6.0f);
				}
				else if (!hitActor->bisRagDoll)
				{
					hitActor->LaunchCharacter(GetCapsuleComponent()->GetForwardVector() * 200.0f, false, false);
				}
				break;
			}
		}
	}
}

void ASY_BasePlayer::ServerGetWeapon_Implementation()
{
	if (!bisHolding)
	{
		FVector startPos = GetMesh()->GetBoneLocation(FName(TEXT("hand_r")));
		FVector endPos = startPos;
		TArray<AActor*> ActorsToIgnore;
		FCollisionQueryParams params;
		FHitResult hitResult;

		bool bhit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startPos, endPos, 50.0f, TraceTypeQuery3, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, hitResult, true);

		if (bhit)
		{
			MultiGetWeapon(hitResult);
		}
	}
}

void ASY_BasePlayer::MultiGetWeapon_Implementation(const FHitResult result)
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (grab_montage && AnimInstance)
	{
		AnimInstance->Montage_Play(grab_montage, 0.7f);
	}


	//for (auto HitResult : result)
	{
		AActor* hitActor = result.GetActor();
		GetCharacterMovement()->MaxWalkSpeed = 320.0f;
		if (hitActor != nullptr && hitActor->ActorHasTag("Bow"))
		{
			heldItem = hitActor;
			UE_LOG(LogTemp,Warning,TEXT("%s"), *hitActor->GetName());
			bow = Cast<AWea_BowActor>(hitActor);
			bow->boxComp->SetSimulatePhysics(false);
			heldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Bow"));
			bisBow = true;
			bisHolding = true;
			bisPunching = false;
			//break;
		}
		else if (hitActor != nullptr && hitActor->ActorHasTag("IceGun"))
		{
			heldItem = hitActor;
			gun = Cast<AWea_Bow>(hitActor);
			gun->boxComp->SetSimulatePhysics(false);
			heldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Gun"));
			bisGun = true;
			bisHolding = true;
			bisPunching = false;
			//break;
		}
		else if (hitActor != nullptr && hitActor->ActorHasTag("Hammer"))
		{	
			UE_LOG(LogTemp,Warning,TEXT("%s"), *hitActor->GetName());
			heldItem = hitActor;
			hammer = Cast<AWea_Hammer>(hitActor);
			hammer->bodyMesh->SetSimulatePhysics(false);
			heldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hammer"));
			hammer->SetOwner(this);
			bisHammer = true;
			bisHolding = true;
			bisPunching = false;
		
			//break;
		}
		else if (hitActor != nullptr && hitActor->ActorHasTag("MagicStick"))
		{
			
			heldItem = hitActor;
			magicStick = Cast<AWea_Hammer>(hitActor);
			magicStick->bodyMesh->SetSimulatePhysics(false);
			heldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagicStick"));
			magicStick->SetOwner(this);
			bisMagicStick = true;
			bisHolding = true;
			bisPunching = false;
			
			//break;
		}
	}
}

void ASY_BasePlayer::ServerDropWeapon_Implementation()
{
	if (bisHolding)
	{
		FVector startPos = GetActorLocation();
		FVector endPos = GetActorLocation() + FVector(0, 0, -300.0f);
		FHitResult hitResult;
		FCollisionQueryParams params;

		// 바닥쪽으로 linetrace 발사
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECC_Visibility, params);

		if (bhit)
		{
			MultiDropWeapon(hitResult);
		}
	}
}

void ASY_BasePlayer::MultiDropWeapon_Implementation(const FHitResult hitResult)
{
	
	heldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// holding 상태 false
	bisHolding = false;
	FailToAttach = true;

	if (heldItem == bow)
	{
		bisBow = false;
		bow->boxComp->SetSimulatePhysics(true);
		bow->SetOwner(nullptr);
		heldItem = nullptr;
	}
	else if (heldItem == gun)
	{
		bisGun = false;
		gun->boxComp->SetSimulatePhysics(true);
		gun->SetOwner(nullptr);
		heldItem = nullptr;
	}
	else if (heldItem == hammer)
	{
		bisHammer = false;
		hammer->bodyMesh->SetSimulatePhysics(true);
		hammer->SetOwner(nullptr);
		heldItem = nullptr;
	}
	else if (heldItem == magicStick)
	{
		bisMagicStick = false;
		magicStick->bodyMesh->SetSimulatePhysics(true);
		magicStick->SetOwner(nullptr);
		heldItem = nullptr;
	}
	// 속도 정상화
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

		
	
}

void ASY_BasePlayer::LTurnCamera()
{
	FRotator camRot = cameraBoom->GetSocketRotation(FName("None"));
	float lRot = camRot.Yaw;
	cameraBoom->SetRelativeRotation(FRotator(camRot.Pitch, FMath::Lerp(lRot, lRot - 10.0f, 0.1f), camRot.Roll));
}

void ASY_BasePlayer::RTurnCamera()
{
	FRotator camRot = cameraBoom->GetSocketRotation(FName("None"));
	float lRot = camRot.Yaw;
	cameraBoom->SetRelativeRotation(FRotator(camRot.Pitch, FMath::Lerp(lRot, lRot + 10.0f, 0.1f), camRot.Roll));
}

void ASY_BasePlayer::BackTurnCamera()
{
	FRotator camRot = cameraBoom->GetSocketRotation(FName("None"));
	float lRot = camRot.Yaw;
	cameraBoom->SetRelativeRotation(FRotator(camRot.Pitch, FMath::Lerp(lRot, 0, 0.07f), camRot.Roll));
}


void ASY_BasePlayer::PunchAttack()
{

	if (HasAuthority())
	{
		ServerPunchAttack_Implementation();
	}
	else
	{
		ServerPunchAttack();
	}
}


void ASY_BasePlayer::ServerPunchAttack_Implementation()
{
	FVector startPos = GetMesh()->GetBoneLocation(FName("hand_r"));
	FVector endPos = startPos;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> hitResult;

	bool bhit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startPos, endPos, 30.0f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);
	if(bhit) 
	{
		for(auto result : hitResult)
		{
			class ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(result.GetActor());
		
			if (hitActor && hitActor != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("punch %s"), *result.GetActor()->GetName());
				//hitActor->bisRagDoll = true;
				hitActor->bisStunned = true;
				MultiPunchAttack(result);
				break;
			}else{
				FHitResult null;
				UE_LOG(LogTemp, Warning, TEXT("Multi Punching Started2"));
				MultiPunchAttack(null);
			}
		}
	}
	else
	{
		FHitResult null; 
		UE_LOG(LogTemp, Warning, TEXT("Multi Punching Started"));
		MultiPunchAttack(null);
	}

}

void ASY_BasePlayer::MultiPunchAttack_Implementation(const FHitResult& hitResult)
{
	bisPunching = true;
	if(IsValid(hitResult.GetActor()))
	{
		class ASY_BasePlayer* hitActor = Cast<ASY_BasePlayer>(hitResult.GetActor());

		if(hitActor)
		{
			if (hitResult.BoneName == FName("head") && !hitActor->bisRagDoll)
			{
				/*hitActor->LaunchCharacter(GetCapsuleComponent()->GetForwardVector() * 2000.0f, false, false);*/
				hitActor->ServerDamaged_Implementation(6.0f);
			}
			else if (!hitActor->bisRagDoll)
			{
				hitActor->LaunchCharacter(GetCapsuleComponent()->GetForwardVector() * 300.0f, false, false);
			}
		}
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			bisPunching = false;
		}, 1.0, false);
	stamina -= 15.0f;
}

void ASY_BasePlayer::GunAttack()
{
	if (bisGun)
	{
		if (HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("GUNATTACK_SERVER"));
			ServerGunAttack_Implementation();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GUNATTACK_Client"));
			ServerGunAttack();
		}
	}
}

void ASY_BasePlayer::ServerGunAttack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerGunAttack_Implementation"));
	gun->Shoot();
	MultiGunAttack();
}

void ASY_BasePlayer::MultiGunAttack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiGunAttack_Implementation"));
}

void ASY_BasePlayer::BowAttack()
{
	if (HasAuthority())
	{
		ServerBowAttack_Implementation();
	}
	else
	{
		ServerBowAttack();
	}
}

void ASY_BasePlayer::ServerBowAttack_Implementation()
{
	MultiBowAttack();
}

void ASY_BasePlayer::MultiBowAttack_Implementation()
{
	bow->Fire();
}

void ASY_BasePlayer::HammerAttack()
{
	if (HasAuthority())
	{
		ServerHammerAttack_Implementation();
	}
	else
	{
		ServerHammerAttack();
	}
}

void ASY_BasePlayer::ServerHammerAttack_Implementation()
{
	MultiHammeraAttack();
}

void ASY_BasePlayer::MultiHammeraAttack_Implementation()
{
	PlayAnimMontage(hammer_montage, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("multihammer"));
}

void ASY_BasePlayer::MagicStickAttack()
{
	if (HasAuthority())
	{
		ServerHammerAttack_Implementation();
	}
	else
	{
		ServerHammerAttack();
	}
}

void ASY_BasePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASY_BasePlayer, heldItem);
	DOREPLIFETIME(ASY_BasePlayer, stamina);
	DOREPLIFETIME(ASY_BasePlayer, myName);
	DOREPLIFETIME(ASY_BasePlayer, myMeshNumber);
	//DOREPLIFETIME(ASY_BasePlayer, myMeshColor);
	DOREPLIFETIME(ASY_BasePlayer, bisHeading);
	DOREPLIFETIME(ASY_BasePlayer, bisHolding);
	DOREPLIFETIME(ASY_BasePlayer, bisPunching);
	DOREPLIFETIME(ASY_BasePlayer, bisGrabing);
	DOREPLIFETIME(ASY_BasePlayer, bGunDamaged);
	DOREPLIFETIME(ASY_BasePlayer, bisBow);
	DOREPLIFETIME(ASY_BasePlayer, bisGun);
	DOREPLIFETIME(ASY_BasePlayer, bisHammer);
	DOREPLIFETIME(ASY_BasePlayer, bisMagicStick);
	DOREPLIFETIME(ASY_BasePlayer, gun);
	DOREPLIFETIME(ASY_BasePlayer, bullet);
	DOREPLIFETIME(ASY_BasePlayer, bow);
	DOREPLIFETIME(ASY_BasePlayer, arrow);
	DOREPLIFETIME(ASY_BasePlayer, hammer);
	DOREPLIFETIME(ASY_BasePlayer, magicStick);
	DOREPLIFETIME(ASY_BasePlayer, fogTime);
	DOREPLIFETIME(ASY_BasePlayer, RagdollLoc);
	DOREPLIFETIME(ASY_BasePlayer, RagdollTarget);
	DOREPLIFETIME(ASY_BasePlayer, newMat);
}