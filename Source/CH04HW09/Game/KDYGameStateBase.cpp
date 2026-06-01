

#include "Game/KDYGameStateBase.h"
#include "Player/KDYPlayerController.h"
#include "Kismet/GameplayStatics.h"

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