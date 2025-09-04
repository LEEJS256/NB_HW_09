// Fill out your copyright notice in the Description page of Project Settings.


#include "JSLeePlayerController.h"

#include "ContentBrowserItemData.h"
#include "JSLeeChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"

#include "Kismet/GameplayStatics.h"
#include "JSLeeGameMode.h"
#include "JSLeePlayerState.h"
#include "JSLeeTalk.h"
#include "JSLeePlayerController.h"

#include "JSLeeGameState.h"
#include "Net/UnrealNetwork.h"

AJSLeePlayerController::AJSLeePlayerController()
{
	bReplicates = true;
}

void AJSLeePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UJSLeeChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AJSLeePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,NotificationText);
}

void AJSLeePlayerController::SetChatMessageString(const FString& InChatMessage)
{
	ChatMessage = InChatMessage;
	
	if (IsLocalController())
	{
		AJSLeePlayerState* JSLeePS = GetPlayerState<AJSLeePlayerState>();
		if (IsValid(JSLeePS))
		{
			FString CombineMessage = JSLeePS->PlayerNameString + TEXT(": ") + ChatMessage;

			ServerRPC_PrintChatMessageString(CombineMessage);
		}
	}
}

void AJSLeePlayerController::PrintChatMessageString(const FString& InChatMessage)
{
	JSLeeTalkFunctionLibrary::MyPrintString(this, InChatMessage, 10.f);
}

void AJSLeePlayerController::Client_ReceiveChatMessageString_Implementation(const FString& InChatMessageString)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *InChatMessageString);
}

void AJSLeePlayerController::ClientRPC_PrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AJSLeePlayerController::ServerRPC_PrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AJSLEEGameMode* JSLeeGM = Cast<AJSLEEGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(JSLeeGM) == true)
	{
		JSLeeGM->Print_ChatMessageString(this, InChatMessageString);
	}
	
}
