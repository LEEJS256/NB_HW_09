// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Engine/TimerHandle.h"
#include "JSLEEGameMode.generated.h"

class AJSLeePlayerController;
/**
 * 
 */
UCLASS()
class JSLEETALK_API AJSLEEGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	
	FString GenerateRandomNumber();
	bool IsGuessNumberString(const FString& Number);
	FString JudgeResult(AJSLeePlayerController* InChattingPlayerController , const FString& InSecretNumber, const FString& InGuessNumber);
	void Print_ChatMessageString(AJSLeePlayerController* InChattingPlayerController , const FString& InMessage);

	void ResetGame();

	void JudgeGame(AJSLeePlayerController* InChattingPlayerController, int InStrikeCount);


	void Reset_RunTime();
	void Take_RunTime();
	
protected:
	FString  SecretNumber;
	TArray<TObjectPtr<AJSLeePlayerController>> AllPlayerControllers;
	
	FTimerHandle RuntimeTimerHandle;

	float RuntimeSeconds  = 15.f;
	float MaxRuntimeSeconds = 15.f;
};
