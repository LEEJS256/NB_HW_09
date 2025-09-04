// Fill out your copyright notice in the Description page of Project Settings.


#include "JSLeeGameState.h"

#include "Kismet/GameplayStatics.h"
#include "JSLeePlayerController.h"
#include "Net/UnrealNetwork.h"


void AJSLeeGameState::MulticastRPC_BroadCastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AJSLeePlayerController* JSLeePC = Cast<AJSLeePlayerController>(PC);
			if (IsValid(JSLeePC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				JSLeePC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void AJSLeeGameState::BroadCastChatMessage(const FString& InChatMessage)
{
	if (GetNetMode() == NM_ListenServer)
	{
		ChatMessage = InChatMessage;
		
	}
}

void AJSLeeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,ChatMessage);
}
