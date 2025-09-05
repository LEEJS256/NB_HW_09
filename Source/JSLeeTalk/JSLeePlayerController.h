// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JSLeePlayerController.generated.h"


class UJSLeeChatInput;
class UUserWidget;
/**
 * 
 */
UCLASS()
class JSLEETALK_API AJSLeePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJSLeePlayerController();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessageString(const FString& InChatMessage);

	void PrintChatMessageString(const FString& InChatMessage);

	UFUNCTION(Client,Reliable)
	void ClientRPC_PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server,Reliable)
	void ServerRPC_PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client,Reliable)
	void Client_ReceiveChatMessageString(const FString& InChatMessageString);


	
	void Set_NotificationText(FText InText);
	void Set_SituationText(FText InText);
	void Set_TimerText(FText InText);
protected:
	FString ChatMessage;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UJSLeeChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UJSLeeChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameSituationWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> GameSituationWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TimerWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> TimerWidgetInstance;
	

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText SituationText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText TimerText;
	
	
};
