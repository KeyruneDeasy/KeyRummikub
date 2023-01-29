// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyRummikubGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "KeyRummikubGameState.h"

AKeyRummikubGameMode::AKeyRummikubGameMode()
	: Super()
{
	GameStateClass = AKeyRummikubGameState::StaticClass();
}

void AKeyRummikubGameMode::BeginPlay()
{
	static const float InitialWaitBeforeDraw = 0.1f;
	GetWorldTimerManager().SetTimer(Timer, this, &AKeyRummikubGameMode::DealOpeningHand, InitialWaitBeforeDraw, false, InitialWaitBeforeDraw);
}

void AKeyRummikubGameMode::InitGameState()
{
	Super::InitGameState();

	RummiGameState = Cast<AKeyRummikubGameState>(GameState);
	if (IsValid(RummiGameState))
	{
		RummiGameState->InitializeTable();
	}
}

void AKeyRummikubGameMode::DealOpeningHand()
{
	const FKeyRummikubSettings& GameSettings = RummiGameState->GameSettings;
	const FRummiRuleset& Ruleset = RummiGameState->Ruleset;

	if (GameSettings.bFastDeal)
	{
		for (int i = 0; i < Ruleset.StartingHandSize; ++i)
		{
			DealOpeningHand_SingleTilePerPlayer();
		}
		RummiGameState->StartGame();
	}
	else
	{
		NumTilesRemainingToDeal = Ruleset.StartingHandSize;
		GetWorldTimerManager().SetTimer(Timer, this, &AKeyRummikubGameMode::DealOpeningHand_TimerStep, GameSettings.DealInterval, true, 0.0f);
	}
}

void AKeyRummikubGameMode::DealOpeningHand_TimerStep()
{
	DealOpeningHand_SingleTilePerPlayer();
	NumTilesRemainingToDeal -= 1;
	if (NumTilesRemainingToDeal <= 0)
	{
		GetWorldTimerManager().ClearTimer(Timer);
		RummiGameState->StartGame();
	}
}

void AKeyRummikubGameMode::DealOpeningHand_SingleTilePerPlayer()
{
	const FRummiRuleset& Ruleset = RummiGameState->Ruleset;

	for (int i = 0; i < Ruleset.NumPlayers; ++i)
	{
		RummiGameState->DrawCardFromDeckToHand(i);
	}
}

