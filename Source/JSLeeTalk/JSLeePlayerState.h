// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JSLeePlayerState.generated.h"


UENUM(BlueprintType)
enum class EGameResult : uint8
{
	ENone,
	ELose,
	EDraw,
	EWin,
	EEnd
};

/**
 * 
 */
UCLASS()
class JSLEETALK_API AJSLeePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJSLeePlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentTurn() const { return CurrentPlayerTurn; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentTurn(int32 ArgTurn)  {  CurrentPlayerTurn = ArgTurn; }

	UFUNCTION(BlueprintCallable)
	void AddCurrentTurn(int32 PlusTurn);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxTurns() const { return MaxPlayerTurn; }


	UFUNCTION(BlueprintCallable)
	EGameResult GetGameResut() const { return CurrentGameResult; }

	UFUNCTION(BlueprintCallable)
	void SetGameResut(EGameResult ArgGameResult) { CurrentGameResult = ArgGameResult; }

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

private:
	UPROPERTY(Replicated)
	int32 CurrentPlayerTurn;
	UPROPERTY(Replicated)
	int32 MaxPlayerTurn;
	EGameResult CurrentGameResult;
};
