// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KDYGameModeBase.h"
#include "Game/KDYGameStateBase.h"

void AKDYGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer); //게임에 접속(로그인) 할때마다.

	AKDYGameStateBase* KDYGameStateBase = GetGameState<AKDYGameStateBase>(); //GSB를 가져오고
	if (IsValid(KDYGameStateBase)) //유효하다면
	{
		KDYGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX")); //로그인메세지 브로드캐스트
	}
}
