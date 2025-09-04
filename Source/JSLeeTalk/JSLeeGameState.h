// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JSLeeGameState.generated.h"

/**
 * 
 */
UCLASS()
class JSLEETALK_API AJSLeeGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPC_BroadCastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

	UPROPERTY(Replicated)
	FString ChatMessage;

	void BroadCastChatMessage(const FString& InChatMessage);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
