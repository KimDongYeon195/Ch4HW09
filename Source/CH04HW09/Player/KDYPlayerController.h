// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KDYPlayerController.generated.h"

class UKDYChatInput;

UCLASS()
class CH04HW09_API AKDYPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

protected:
	//채팅 위젯 연결
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKDYChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UKDYChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
	
};
