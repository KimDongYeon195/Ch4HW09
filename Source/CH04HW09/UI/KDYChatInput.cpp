// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KDYChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/KDYPlayerController.h"

void UKDYChatInput::NativeConstruct() //텍스트가 생성될때
{
	Super::NativeConstruct();
	//텍스트 커밋에 바인드 되어있지 않다면
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		//델리게이트 호출(바인드)
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UKDYChatInput::NativeDestruct() //텍스트가 파괴(사라질)때
{
	Super::NativeDestruct();
	//바인드되어있으면
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		//바인드 해제
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}


//작성한 텍스트 제출(보내기) like 엔터를 누른다 or 버튼을 누른다.
void UKDYChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		//입력을한 컴퓨터의 플레이어 컨트롤러를 가져옴
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AKDYPlayerController* OwningCXPlayerController = Cast<AKDYPlayerController>(OwningPlayerController);
			if (IsValid(OwningCXPlayerController) == true)
			{
				OwningCXPlayerController->SetChatMessageString(Text.ToString()); //입력한 텍스트 세팅

				EditableTextBox_ChatInput->SetText(FText()); //빈 텍스트로 한번 클리어
			}
		}
	}
}
