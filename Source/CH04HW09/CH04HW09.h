// ChatX.h

#pragma once

#include "CoreMinimal.h"

	//멀티플레이 디버깅용 매크로
class HW09FunctionLibrary
{
public:
		//정적 함수 
	static void MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
	{
			// InWorldContextActor == 월드객체를 얻을만한 액터
		if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer) //클라이언트 or 리슨서버
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString); //디버그 메시지
			}
			else //데디케이트 서버라면
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString); //데디케이트 서버에는 뷰포트가 필요없기때문에 ,Only Cmd창만 나옴
			}
		}
	}

	static FString GetNetModeString(const AActor* InWorldContextActor) //정적함수로 만든이유, 현재 클래스의 객체가 없어도 호출할수 있도록
	{
		FString NetModeString = TEXT("None"); //알맞는 넷모드의 스트링을 출력

		if (IsValid(InWorldContextActor) == true)
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("StandAlone");
				}
				else
				{
					NetModeString = TEXT("Server"); //현재는 데디케이티드 서버
				}
			}
		}

		return NetModeString;
	}

};