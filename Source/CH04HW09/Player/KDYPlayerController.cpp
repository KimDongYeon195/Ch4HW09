// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KDYPlayerController.h"
#include "UI/KDYChatInput.h"
#include <Kismet/KismetSystemLibrary.h>
#include "CH04HW09/CH04HW09.h"
#include "EngineUtils.h"

void AKDYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false) //내 위젯(Like 게임오버 위젯)은 내 클라에만 나온다
	{
		return;
	}

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

	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController())
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void AKDYPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Green, 5.0f);
		//디버깅 포인트 1 -> 각 클라에 메시지가 다르게 출력된것
	
	
	//FString NetModeString = HW09FunctionLibrary::GetNetModeString(this);
	//FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	//HW09FunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
}

void AKDYPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString); //입력메세지 출력
}

void AKDYPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	for (TActorIterator<AKDYPlayerController> It(GetWorld()); It; ++It) 
			//It -> Iterator의 약자
			//TActorIterator -> 언리얼 월드에 존재하는특정 타입의 액터를 순회하는 반복자 클래스
			//GetWorld() -> 어느 월드에서 찾을건가요?
	{
		AKDYPlayerController* KDYPlayerController = *It;
		if (IsValid(KDYPlayerController))
		{
			KDYPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
				//해당 텍스트를 클라이언트에 뿌려줌
		}
	}
}


