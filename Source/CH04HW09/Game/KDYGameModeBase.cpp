// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KDYGameModeBase.h"
#include "Game/KDYGameStateBase.h"
#include "Player/KDYPlayerController.h"
#include "EngineUtils.h"
#include "Player/KDYPlayerState.h"

void AKDYGameModeBase::OnPostLogin(AController* NewPlayer) //로그인 후 호출 -> 컨트롤러를 넣어줌
{
	Super::OnPostLogin(NewPlayer); //게임에 접속(로그인) 할때마다.

	AKDYPlayerController* KDYPlayerController = Cast<AKDYPlayerController>(NewPlayer);
	if (IsValid(KDYPlayerController))
	{
			//게임모드(서버)에서 플레이어컨트롤러의 속성을 변경했으므로 플레이어컨트롤러는 레플리케이티드 됨
		KDYPlayerController->NotificationText = FText::FromString(TEXT("Connected to the Game server."));
		AllPlayerControllers.Add(KDYPlayerController);

		AKDYPlayerState* KDYPlayerState = KDYPlayerController->GetPlayerState<AKDYPlayerState>(); //플레이어 스테이트 가져오기
		if (IsValid(KDYPlayerState))
		{
			//플레이어네임String출력(Player 1,2,3,...)
			KDYPlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num()); 
			//PlayerState의 특정 속성을 변경하고있음 -> 레플리케이션
		}

		AKDYGameStateBase* KDYGameStateBase = GetGameState<AKDYGameStateBase>(); //GSB를 가져오고
		if (IsValid(KDYGameStateBase)) //유효하다면
		{
			KDYGameStateBase->MulticastRPCBroadcastLoginMessage(KDYPlayerState->PlayerNameString); //로그인메세지 브로드캐스트
		}
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
	if (IsGuessNumberString(GuessNumberString)) //제출한 숫자가 GuessNumber인지
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString); //JudgeReulst() 함수 호출하여 판단.
		IncreaseGuessCount(InChattingPlayerController); //게임 중에만 카운트가 올라감
		for (TActorIterator<AKDYPlayerController> It(GetWorld()); It; ++It)
		{
			AKDYPlayerController* KDYPlayerControllers = *It; //컨트롤러 전체를 순회돌면서 그 값을
			if (IsValid(KDYPlayerControllers))
			{
					//String 컴바인 "입력숫자 -> ?S?B"
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				KDYPlayerControllers->ClientRPCPrintChatMessageString(CombinedMessageString); //클라이언트 RPC에 전송

					//FCString::Atoi(원본값의 왼쪽 1글자) -> 문자열을 정수로 변환(2S1B에서 왼쪽 2만 정수로 변환)
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
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

void AKDYGameModeBase::IncreaseGuessCount(AKDYPlayerController* InChattingPlayerController)
{
	AKDYPlayerState* KDYPlayerState = InChattingPlayerController->GetPlayerState<AKDYPlayerState>();
	if (IsValid(KDYPlayerState))
	{
		KDYPlayerState->CurrentGuessCount++;
	}
}

void AKDYGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& KDYPlayerController : AllPlayerControllers)
	{
		AKDYPlayerState* KDYPlayerState = KDYPlayerController->GetPlayerState<AKDYPlayerState>();
		if (IsValid(KDYPlayerState))
		{
			KDYPlayerState->CurrentGuessCount = 0;
		}
	}
}

void AKDYGameModeBase::JudgeGame(AKDYPlayerController* InChatingPlayerController, int InStrikeCount)
		//채팅 플레이어와 스트라이크 카운트를 넣어준다.
{
	if (InStrikeCount == 3)
	{
			//플레이어스테이트에서 채팅플레이어컨트롤러 가져오기
		AKDYPlayerState* KDYPlayerState = InChatingPlayerController->GetPlayerState<AKDYPlayerState>();
		for (const auto& KDYPlayerController : AllPlayerControllers) //한번씩 다 순회하라.
		{
				//"Player N has won the game"
			FString CombinedMessageString = KDYPlayerState->PlayerNameString + TEXT("has won the game.");
				//NotificationText에 출력
			KDYPlayerController->NotificationText = FText::FromString(CombinedMessageString);

			ResetGame(); //리셋게임
		}
	}
	else
	{
		bool bIsDraw = true; //일단 비겼다고 가정
		for (const auto& KDYPlayerController : AllPlayerControllers) //모든플레이어들을 순회
		{
			AKDYPlayerState* KDYPlayerState = KDYPlayerController->GetPlayerState<AKDYPlayerState> ();
			if (IsValid(KDYPlayerState))
			{
					//아무도 정답을 맞추지 못하고 기회를 모두 소진했을때
				if (KDYPlayerState->CurrentGuessCount < KDYPlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		if (true == bIsDraw)
		{
			for (const auto& KDYPlayerController : AllPlayerControllers)
			{
				KDYPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}

