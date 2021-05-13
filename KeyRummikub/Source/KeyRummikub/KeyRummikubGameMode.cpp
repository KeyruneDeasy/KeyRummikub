// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyRummikubGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "KeyRummikubGameState.h"

AKeyRummikubGameMode::AKeyRummikubGameMode()
{
	GameStateClass = AKeyRummikubGameState::StaticClass();
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

