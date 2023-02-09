// Copyright Ciaran Deasy 2021


#include "RummiAi.h"
#include "KeyRummikubGameState.h"

bool URummiAi::Update(float DeltaTime, AKeyRummikubGameState* GameState)
{
	DetermineTargetBoardState(GameState);
	if (GameState->CurrentPlayerHasEverPlayedTiles() || TilesToPlayFromHand.GetTotalValue() >= GameState->Ruleset.FirstTilesRequiredValue)
	{
		SnapToTargetBoardState(GameState);
	}
	return true;
}

void URummiAi::DetermineTargetBoardState(AKeyRummikubGameState* GameState)
{
	TargetBoardState = GameState->Table.Board;

	FRummiTileArray& MyInitialHand = GameState->Table.Hands[GameState->CurrentTurnPlayerIndex];
	FRummiTileArray MyCurrentHand = GameState->Table.Hands[GameState->CurrentTurnPlayerIndex];

	TArray<FRummiTileArray> ValidSubsets;
	MyCurrentHand.FindValidSubsets(ValidSubsets);

	auto PlayFromHand = [&](const FRummiTile& Tile)
	{
		TilesToPlayFromHand.AddTileToEnd(Tile);
		MyCurrentHand.RemoveTile(Tile);
	};

	for (const FRummiTileArray& Subset : ValidSubsets)
	{
		TargetBoardState.AddTileSet(Subset);
		for (const FRummiTile& Tile : Subset.Tiles)
		{
			PlayFromHand(Tile);
		}
	}

#define PLAY_TILE_AND_BREAK(T) PlayFromHand(T); bPlayedTile = true; break;

	bool bPlayedTile = true;
	while (bPlayedTile)
	{
		bPlayedTile = false;
		for (FRummiTile& Tile : MyCurrentHand.Tiles)
		{
			for (FRummiTileArray& Subset : TargetBoardState.TileSets)
			{
				ETileArrayType Type = Subset.GetTileArrayType();
				switch (Type)
				{
					case ETileArrayType::AscendingNumbers:
					{
						if (Tile.Color == Subset.GetFirstTile().Color)
						{
							if (Tile.Number == Subset.GetFirstTile().Number - 1)
							{
								Subset.AddTileToStart(Tile);
								PLAY_TILE_AND_BREAK(Tile);
							}
							else if (Tile.Number == Subset.GetLastTile().Number + 1)
							{
								Subset.AddTileToEnd(Tile);
								PLAY_TILE_AND_BREAK(Tile);
							}
						}
					}
				}
			}
			if (bPlayedTile)
			{
				break;
			}
		}
	}
}

void URummiAi::SnapToTargetBoardState(AKeyRummikubGameState* GameState)
{
	GameState->Table.Board = TargetBoardState;
	for (const FRummiTile& Tile : TilesToPlayFromHand.Tiles)
	{
		GameState->Table.Hands[GameState->CurrentTurnPlayerIndex].RemoveTile(Tile);
		GameState->TilesPlayedThisTurn.Add(GameState->GetActorFromTileInfo(Tile));
	}
	GameState->UpdateGridsFromLogicalRepresentation();
}

void URummiAi::NotifyTurnEnded()
{
	TilesToPlayFromHand.Tiles.Empty();
}
