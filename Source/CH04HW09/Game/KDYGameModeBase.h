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

protected:
		//정답숫자 캐싱
	FString SecretNumberString; 
		//플레이어들 정보 저장
	TArray <TObjectPtr<AKDYPlayerController>> AllPlayerControllers;
};
