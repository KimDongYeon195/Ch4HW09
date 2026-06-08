// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KDYGameModeBase.generated.h"

class AKDYPlayerController;

UCLASS()
class CH04HW09_API AKDYGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void BeginPlay() override;

	void PrintChatMessageString(AKDYPlayerController* InChattingPlayerController, const FString& InChatMessageString);

		//정답번호 3자리
	FString GenerateSecretNumber();

		//정답 유추번호
	bool IsGuessNumberString(const FString& InNumberString);

		//정답과 유추번호 대조
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

		//유추횟수 증가
	void IncreaseGuessCount(AKDYPlayerController* InChattingPlayerController);

		//조건 만족시 게임리셋
	void ResetGame();

		//게임의 승패 판단(숫자를 기회안에 맞췄는지 아닌지)
	void JudgeGame(AKDYPlayerController* InChatingPlayerController, int InStrikeCount);
protected:
		//정답숫자 캐싱
	FString SecretNumberString; 
		//플레이어들 정보 저장
	TArray <TObjectPtr<AKDYPlayerController>> AllPlayerControllers;
};
