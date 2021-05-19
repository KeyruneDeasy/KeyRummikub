// Copyright Ciaran Deasy 2021


#include "RummiAi.h"
#include "KeyRummikubGameState.h"

bool URummiAi::Update(float DeltaTime, AKeyRummikubGameState* GameState)
{
	DetermineTargetBoardState(GameState);
	return true;
}

void URummiAi::DetermineTargetBoardState(AKeyRummikubGameState* GameState)
{
	TargetBoardState = GameState->Table.Board;

	TArray<FRummiTileArray> ValidSubsets;
	GameState->Table.Hands[GameState->CurrentTurnPlayerIndex].FindValidSubsets(ValidSubsets);
}
