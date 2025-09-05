// Fill out your copyright notice in the Description page of Project Settings.


#include "JSLEEGameMode.h"
#include "JSLeeGameState.h"
#include "JSLeePlayerController.h"
#include "EngineUtils.h"
#include "JSLeePlayerState.h"

void AJSLEEGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);


	AJSLeePlayerController* JSleePlayerController = Cast<AJSLeePlayerController>(NewPlayer);
	if (IsValid(JSleePlayerController) == true)
	{
		// JSleePlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		JSleePlayerController->Set_NotificationText(FText::FromString(TEXT("Connected to the game server.")));
		AllPlayerControllers.Add(JSleePlayerController);
	}

	AJSLeePlayerState* JSLeePlayerState = JSleePlayerController->GetPlayerState<AJSLeePlayerState>();
	if (IsValid(JSLeePlayerState) == true)
	{
		JSLeePlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
	}

	AJSLeeGameState* JSLeeGameState = GetGameState<AJSLeeGameState>();
	if (IsValid(JSLeeGameState) == true)
	{
		JSLeeGameState->MulticastRPC_BroadCastLoginMessage(JSLeePlayerState->PlayerNameString);
	}
}

void AJSLEEGameMode::BeginPlay()
{
	Super::BeginPlay();

	SecretNumber = GenerateRandomNumber();
	//SecretNumber= TEXT("456");
	
	RuntimeSeconds  = 15.f;
	MaxRuntimeSeconds = 15.f;
	
	GetWorld()->GetTimerManager().SetTimer(
		RuntimeTimerHandle,
		this,
		&AJSLEEGameMode::Take_RunTime,
		1.0f,
		true);
}

FString AJSLEEGameMode::GenerateRandomNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AJSLEEGameMode::IsGuessNumberString(const FString& Number)
{
	bool bCanPlay = false;

	do
	{
		if (Number.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : Number)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
	}
	while (false);

	return bCanPlay;
}

FString AJSLEEGameMode::JudgeResult(AJSLeePlayerController* InChattingPlayerController, const FString& InSecretNumber,
                                    const FString& InGuessNumber)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumber[i] == InGuessNumber[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (InSecretNumber.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	// APlayerController를 통해 올바른 APlayerState를 가져옵니다.
	AJSLeePlayerState* JSLeePlayerState = InChattingPlayerController->GetPlayerState<AJSLeePlayerState>();
	// JSLeePlayerState->AddCurrentTurn(1);
	
	if (StrikeCount == 0 && BallCount == 0)
	{
	
		return TEXT("OUT");
	}


	// null 포인터 예외 처리는 좋은 습관입니다.
	if (JSLeePlayerState)
	{


		return FString::Printf(TEXT("%dS / %dB (Result)"),

		                       StrikeCount, BallCount);
	}

	return TEXT("Error: PlayerState not found.");
}

void AJSLEEGameMode::Print_ChatMessageString(AJSLeePlayerController* InChattingPlayerController,
                                             const FString& InMessage)
{
	int Index = InMessage.Len() - 3;
	FString GuessNumberString = InMessage.RightChop(Index);

	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(InChattingPlayerController, SecretNumber, GuessNumberString);

		for (TActorIterator<AJSLeePlayerController> It(GetWorld()); It; ++It)
		{
			AJSLeePlayerController* JSLeePlayerController = *It;
			if (IsValid(JSLeePlayerController) == true)
			{
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);

				FString CombinedMessageString = InMessage + TEXT(" -> ") + JudgeResultString;
				JSLeePlayerController->ClientRPC_PrintChatMessageString(CombinedMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<AJSLeePlayerController> It(GetWorld()); It; ++It)
		{
			AJSLeePlayerController* JSLeePlayerController = *It;
			if (IsValid(JSLeePlayerController) == true)
			{
				JSLeePlayerController->ClientRPC_PrintChatMessageString(InMessage);
			}
		}
	}
}

void AJSLEEGameMode::ResetGame()
{
	SecretNumber = GenerateRandomNumber();

// 	for (const auto& JSLeePlayerController : AllPlayerControllers)
// 	{
// 		AJSLeePlayerState* JSLeePS = JSLeePlayerController->GetPlayerState<AJSLeePlayerState>();
// 		if (IsValid(JSLeePS) == true)
// 		{
// 			JSLeePS->SetCurrentTurn(0);
// 			
// 			FString FormattedString = FString::Printf(
// 				TEXT("Recent turn/Maxturn : %d / %d"),
// 				JSLeePS->GetCurrentTurn(),
// 				JSLeePS->GetMaxTurns()
// 			);
// 			
// 			JSLeePlayerController->Set_SituationText(FText::FromString(FormattedString));
// 		}
// 	}
}

void AJSLEEGameMode::JudgeGame(AJSLeePlayerController* InChattingPlayerController, int InStrikeCount)
{
	Reset_RunTime();

	
	if (InStrikeCount == 3)
	{
		AJSLeePlayerState* JSLeePS = InChattingPlayerController->GetPlayerState<AJSLeePlayerState>();
		// FString CombinedMessageString = JSLeePS->PlayerNameString + TEXT(" has won the game.");
		// InChattingPlayerController->Set_NotificationText(FText::FromString(CombinedMessageString));
		// ResetGame();
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			if (IsValid(JSLeePS) == true)
			{
				FString CombinedMessageString = JSLeePS->PlayerNameString + TEXT(" has won the game. Reset the game");
				// CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				CXPlayerController->Set_NotificationText(FText::FromString(CombinedMessageString));
				ResetGame();
			}
		}

	}
	else if (InStrikeCount != 3)
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			AJSLeePlayerState* JSLeePS = CXPlayerController->GetPlayerState<AJSLeePlayerState>();
			if (IsValid(JSLeePS) == true)
			{
				if (JSLeePS->GetCurrentTurn() < JSLeePS->GetMaxTurns())
				{
					bIsDraw = false;
					InChattingPlayerController->Set_NotificationText(FText::FromString(TEXT("...")));
				
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& JSLeePlayerController : AllPlayerControllers)
			{
			//	JSLeePlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				JSLeePlayerController->Set_NotificationText(FText::FromString(TEXT("Draw...")));
				ResetGame();
			}
		}
	}
}

void AJSLEEGameMode::Reset_RunTime()
{
	RuntimeSeconds= MaxRuntimeSeconds;
}

void AJSLEEGameMode::Take_RunTime()
{
	RuntimeSeconds -= 1.f;

	if (RuntimeSeconds <= 0.f)
	{
		//RuntimeSeconds = 0.f;
		Reset_RunTime();

		for (const auto& JSLeePlayerController : AllPlayerControllers)
		{
			AJSLeePlayerState* JSLeePS = JSLeePlayerController->GetPlayerState<AJSLeePlayerState>();
			if (IsValid(JSLeePS) == true)
			{
				bool result = JSLeePS->AddCurrentTurn(1);
				JSLeePlayerController->Set_NotificationText(FText::FromString(TEXT("Time Out")));

				FString FormattedString = FString::Printf(
					TEXT("Recent turn/Maxturn : %d / %d"),
					JSLeePS->GetCurrentTurn(),
					JSLeePS->GetMaxTurns()
				);
				JSLeePlayerController->Set_SituationText(FText::FromString(FormattedString));
			}
		}
	}

	
	
	FString FormattedString = FString::Printf(
			TEXT("Time : %.0f / %.0f"),
			RuntimeSeconds,
			MaxRuntimeSeconds
		);
	
	for (const auto& JSLeePlayerController : AllPlayerControllers)
	{
		JSLeePlayerController->Set_TimerText(FText::FromString(FormattedString));
		
	}
}
