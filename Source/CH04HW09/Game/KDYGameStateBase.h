
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KDYGameStateBase.generated.h"

UCLASS()
class CH04HW09_API AKDYGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(NetMulticast, Reliable) //서버, 클라 모두에게 전달
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
		//옵션인자 : 임시로 접속클라이언트 이름 XXXXXXX로 지정

		// 타이머 시작 (GameMode의 BeginPlay에서 호출)
	void StartTurnTimer();

public:
		// 남은 시간	
	UPROPERTY(ReplicatedUsing = OnRep_TurnTimeRemaining, BlueprintReadOnly)
	float TurnTimeRemaining;

protected:
		//턴 시간 제한
	UPROPERTY(EditDefaultsOnly)
	float TurnDuration = 30.f;

	FTimerHandle TurnTimerHandle;


protected:
		//매 초마다 타이머 실행
	void OnTurnTimerTick();

		//타이머 변경시 클라이언트에서 호출
	UFUNCTION()
	void OnRep_TurnTimeRemaining();

		//레플리케이션 등록
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
