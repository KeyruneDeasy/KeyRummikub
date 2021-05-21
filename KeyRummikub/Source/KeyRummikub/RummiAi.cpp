// Copyright Ciaran Deasy 2021


#include "RummiAi.h"
#include "KeyRummikubGameState.h"

bool URummiAi::Update(float DeltaTime, AKeyRummikubGameState* GameState)
{
	DetermineTargetBoardState(GameState);
	SnapToTargetBoardState(GameState);
	return true;
}

void URummiAi::DetermineTargetBoardState(AKeyRummikubGameState* GameState)
{
	TargetBoardState = GameState->Table.Board;

	TArray<FRummiTileArray> ValidSubsets;
	GameState->Table.Hands[GameState->CurrentTurnPlayerIndex].FindValidSubsets(ValidSubsets);

	for (const FRummiTileArray& Subset : ValidSubsets)
	{
		TargetBoardState.AddTileSet(Subset);
		for (const FRummiTile& Tile : Subset.Tiles)
		{
			TilesToPlayFromHand.Tiles.Add(Tile);
		}
	}
}

void URummiAi::SnapToTargetBoardState(AKeyRummikubGameState* GameState)
{
	GameState->Table.Board = TargetBoardState;
	for (const FRummiTile& Tile : TilesToPlayFromHand.Tiles)
	{
		GameState->Table.Hands[GameState->CurrentTurnPlayerIndex].RemoveTile(Tile);
	}
	GameState->UpdateGridsFromLogicalRepresentation();
}
