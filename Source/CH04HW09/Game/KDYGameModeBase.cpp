// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KDYGameModeBase.h"
#include "Game/KDYGameStateBase.h"
#include "Player/KDYPlayerController.h"
#include "EngineUtils.h"

void AKDYGameModeBase::OnPostLogin(AController* NewPlayer) //로그인 후 호출 -> 컨트롤러를 넣어줌
{
	Super::OnPostLogin(NewPlayer); //게임에 접속(로그인) 할때마다.

	AKDYGameStateBase* KDYGameStateBase = GetGameState<AKDYGameStateBase>(); //GSB를 가져오고
	if (IsValid(KDYGameStateBase)) //유효하다면
	{
		KDYGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX")); //로그인메세지 브로드캐스트
	}

	AKDYPlayerController* KDYPlayerController = Cast<AKDYPlayerController>(NewPlayer);
	if (IsValid(KDYPlayerController))
	{
		AllPlayerControllers.Add(KDYPlayerController);//플레이어 컨트롤러 형변환 하여 컨테이너에 저장
	}
}

void AKDYGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning,TEXT("%s"), *SecretNumberString)
}

//아래로직은 서버에서 처리해야한다.

void AKDYGameModeBase::PrintChatMessageString(AKDYPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMeddageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;// 뒤에서 3개만 봄
	FString GuessNumberString = InChatMessageString.RightChop(Index);//오른쪽부터 Index만큼 컷하고 나머지 오른쪽 값
	if (IsGuessNumberString(GuessNumberString) == true) //제출한 숫자가 GuessNumber인지
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString); //JudgeReulst() 함수 호출하여 판단.
		for (TActorIterator<AKDYPlayerController> It(GetWorld()); It; ++It)
		{
			AKDYPlayerController* KDYPlayerControllers = *It; //컨트롤러 전체를 순회돌면서 그 값을
			if (IsValid(KDYPlayerControllers) == true)
			{
					//String 컴바인 "입력숫자 -> ?S?B"
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				KDYPlayerControllers->ClientRPCPrintChatMessageString(CombinedMessageString); //클라이언트 RPC에 전송
			}
		}
	}
	else //게임을 위한 숫자입력아 아니라면(그냥 저챗)
	{
		for (TActorIterator<AKDYPlayerController> It(GetWorld()); It; ++It)
		{
			AKDYPlayerController* KDYPlayerControllers = *It;
			if (IsValid(KDYPlayerControllers) == true)
			{
				KDYPlayerControllers->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

FString AKDYGameModeBase::GenerateSecretNumber()
{
	TArray<int32> SecretNumber; // 1~9 배열 생성
	for (int32 i = 1; i < 10; ++i)
	{
		SecretNumber.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks()); //실행할 때마다 다른 수가 나옴
	//SecretNumber = SecretNumber.FilterByPredicate([](int32 Num) {return Num > 0;  });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 index = FMath::RandRange(0, SecretNumber.Num() - 1); //인덱스 0 ~ 8까지
		Result.Append(FString::FromInt(SecretNumber[index])); //문자열에 결과추가(정수를 텍스트로 변환)
		SecretNumber.RemoveAt(index); //배열에서 5 제거, 중복 방지
	}

	return Result;
}

bool AKDYGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false; // 기본값은 false -> 아래 방어코드 통과시 true

	do { //딱 한번만 실행
		
		if (InNumberString.Len() != 3) //3자리 숫자가 아니면 break
		{
			break;
		}

		bool bIsUnique = true; 
		TSet<TCHAR> UniqueDigits; //TCHAR 문자 1개표현, TSet에 추가됨
		for (TCHAR C : InNumberString) 
			// == (int32 i = 0; i < InNumberString.Len(); ++i) -> 인덱스 필요함 But,TSet은 인덱스로 못찾음
		{
			if (FChar::IsDigit(C) == false || C == '0') //해당문자가 숫자인지 or 숫자가 0인지
			{
				bIsUnique = false; //유효한 문자가 아니다. 
				break;
			}

			UniqueDigits.Add(C); //TSet에 문자 추가, TSet ->중복 무시
		}

		if (bIsUnique == false)//중복된 숫자가 없는가?
		{
			break;
		}
		bCanPlay = true;

	} 
	while (false);

	return bCanPlay;; //위 조건 모두 만족시 실행가능
}

FString AKDYGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i]) //정답숫자와 입력숫자의 자리, 숫자 모두 일치
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
				//GuessNumber에서 특정 숫자를 봤더니
			if (InSecretNumberString.Contains(PlayerGuessChar)) //그 숫자가 SecretNumber에 포함이 되어있다.
			{
				BallCount++; //BallCount올림
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0) //숫자도 자리도 틀리면
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount); //출력
}

