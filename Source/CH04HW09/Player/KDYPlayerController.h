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

	UFUNCTION(Client, Reliable) //클라이언트로 보내기위함
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable) // Server metadata -> 서버로 보내기위함, Reliable-> 챗메세지가 날아가지 않게끔
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	//채팅 위젯 연결
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKDYChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UKDYChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
	
};
