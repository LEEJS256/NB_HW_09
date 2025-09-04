// Fill out your copyright notice in the Description page of Project Settings.


#include "JSLeePlayerState.h"
#include "Net/UnrealNetwork.h"
AJSLeePlayerState::AJSLeePlayerState()
	:CurrentGameResult(EGameResult::ENone),
	CurrentPlayerTurn(0),
	MaxPlayerTurn(3)
{
	bReplicates = true;
}

void AJSLeePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass,PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentPlayerTurn);
	DOREPLIFETIME(ThisClass, MaxPlayerTurn);
}

void AJSLeePlayerState::AddCurrentTurn(int32 PlusTurn)
{
	CurrentPlayerTurn += PlusTurn;
	if (CurrentPlayerTurn > MaxPlayerTurn)
		CurrentPlayerTurn = MaxPlayerTurn;
}
