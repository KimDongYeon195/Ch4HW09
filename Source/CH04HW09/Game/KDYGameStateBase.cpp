

#include "Game/KDYGameStateBase.h"
#include "Player/KDYPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AKDYGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false) //서버가 아니라면 ->클라이언트 라면
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0); 
			//0번 컨트롤러 -> 내 클라에는 1개의 플레이어컨트롤러만있다. 이게 0번, 로컬 컨트롤러
		if (IsValid(PC) == true)
		{
			AKDYPlayerController* KDYPC = Cast<AKDYPlayerController>(PC);
			if (IsValid(KDYPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game."); //NameString과 Text를 븉여 출력
				KDYPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void AKDYGameStateBase::StartTurnTimer()
{
	if (HasAuthority() == false) //타이머관리는 서버에서 해야한다.
	{
		return;
	}

	TurnTimeRemaining = TurnDuration;

	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ThisClass::OnTurnTimerTick, 1.f, true);
		//(타이머 핸들, 이 클래스에, 매초마다 타이머 실행, 1초간격, 계속해서)
}

void AKDYGameStateBase::OnTurnTimerTick() //이 또한 서버에서 실행되어야함
{
	TurnTimeRemaining -= 1.f;

	if (TurnTimeRemaining <= KINDA_SMALL_NUMBER)
	{
		TurnTimeRemaining = 0.f;
		GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	}
}

void AKDYGameStateBase::OnRep_TurnTimeRemaining()
{

}



	//레플리케이션 등록
void AKDYGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TurnTimeRemaining);
}
