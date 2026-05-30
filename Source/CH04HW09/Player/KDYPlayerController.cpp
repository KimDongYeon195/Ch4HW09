// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KDYPlayerController.h"
#include "UI/KDYChatInput.h"
#include <Kismet/KismetSystemLibrary.h>

void AKDYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//입력모드를 UI로 흘러가게함
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UKDYChatInput>(this, ChatInputWidgetClass); //위젯 클래스를 플레이어 컨트롤러에 생성
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport(); //위젯 인스턴스를 화면에 보이기
		}
	}
}

void AKDYPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	PrintChatMessageString(ChatMessageString);
}

void AKDYPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Green, 5.0f);
}
