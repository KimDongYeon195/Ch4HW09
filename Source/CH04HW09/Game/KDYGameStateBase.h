
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
};
