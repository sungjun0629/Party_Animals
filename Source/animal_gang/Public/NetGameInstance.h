// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include <../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include "NetGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FString roomName = TEXT("");
	FString hostName = TEXT("");
	FString playerCount = TEXT("");
	int32 pingSpeed = 0;
	int32 sessionIndex = 0;

	void Set(FString rName, FString hName, FString pCount, int32 ping, int32 index)
	{
		roomName = rName; hostName = hName; playerCount = pCount; pingSpeed = ping; sessionIndex = index;
	}
};

USTRUCT(BlueprintType)
struct FPlayerInitInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	FString userName = TEXT("");
	UPROPERTY()
	int32 meshNumber = 0;
	/*UPROPERTY()
	FLinearColor meshColor = FLinearColor::White;*/

	FORCEINLINE void Set(FString name, int32 num/*, FLinearColor color*/)
	{
		userName = name;
		meshNumber = num;
		/*meshColor = color;*/
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, FSessionSlotInfo, sessionInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonSignature, bool, bisActivation);


UCLASS()
class ANIMAL_GANG_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UNetGameInstance();

	virtual void Init() override;

	UPROPERTY()
	FString mySessionName;

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FSearchSignature onSearchCompleted;
	FFindButtonSignature onFindButtonActivation;
	FPlayerInitInfo playerInitInfo;


	void CreateMySession(FText roomName, int32 playerCount);

	void OnCreatedMySession(FName sessionName, bool bWasSuccessful);

	void OnTravelToBattleMap(bool bWasSuccessful);

	void FindOtherSession();

	void OnFindOtherSessions(bool bWasSuccessful);

	void JoinSelectedSession(int32 index);

	void OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);



};
