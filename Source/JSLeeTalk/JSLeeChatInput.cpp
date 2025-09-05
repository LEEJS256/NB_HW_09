// Fill out your copyright notice in the Description page of Project Settings.


#include "JSLeeChatInput.h"

#include "JSLEEGameMode.h"
#include "Components/EditableTextBox.h"
#include "JSLeePlayerController.h"
#include "JSLeePlayerState.h"
#include "Kismet/GameplayStatics.h"

void UJSLeeChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UJSLeeChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UJSLeeChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController))
		{
			AJSLeePlayerController* OwningJSLeePlayerController = Cast<AJSLeePlayerController>(OwningPlayerController);
			if (IsValid(OwningJSLeePlayerController))
			{
				AJSLeePlayerState* JSLeePlayerState = OwningJSLeePlayerController->GetPlayerState<AJSLeePlayerState>();
				bool bFinalTurn = JSLeePlayerState->AddCurrentTurn(1);

				if (bFinalTurn)
				{
					AJSLEEGameMode* JSLeeGM = Cast<AJSLEEGameMode>(UGameplayStatics::GetGameMode(this));
					if (IsValid(JSLeeGM) == true)
					{
						JSLeeGM->ResetGame();
					}
				}

				FString FormattedString = FString::Printf(
					TEXT("Recent turn/Maxturn : %d / %d"),
					JSLeePlayerState->GetCurrentTurn(),
					JSLeePlayerState->GetMaxTurns()
				);
				OwningJSLeePlayerController->Set_SituationText(FText::FromString(FormattedString));

				OwningJSLeePlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
