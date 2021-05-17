// Fill out your copyright notice in the Description page of Project Settings.


#include "RummiTile.h"
#include "KeyUtil.h"

void FRummiTileArray::CreateDeck(FRummiTileArray& InDeck, int NumColors, int NumNumbers, int NumRepetitions, int LowestNumber)
{
	int TileId = 0;
	for (int Repetition = 0; Repetition < NumRepetitions; ++Repetition)
	{
		for (int Color = 0; Color < NumColors; ++Color)
		{
			for (int Number = LowestNumber; Number < NumNumbers + LowestNumber; ++Number)
			{
				InDeck.AddTileToEnd(FRummiTile(Color, Number, TileId));
				++TileId;
			}
		}
	}
}

void FRummiTileArray::AddTileToEnd(FRummiTile Tile)
{
	Tiles.Add(Tile);
}

void FRummiTileArray::Shuffle()
{
	if (Tiles.Num() > 0)
	{
		int32 LastIndex = Tiles.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Tiles.Swap(i, Index);
			}
		}
	}
}

FRummiTile FRummiTileArray::PopTileAtIndex(int Index)
{
	FRummiTile Out = Tiles[Index];
	Tiles.RemoveAt(Index);
	return Out;
}

void FRummiTileArray::RemoveTile(const FRummiTile& Tile)
{
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		const FRummiTile& ThisTile = Tiles[i];
		if (ThisTile.Id == Tile.Id)
		{
			Tiles.RemoveAt(i);
		}
	}
}

void FRummiTileBoardSet::EvaluateIsValidSet()
{
	if (Tiles.Num() < 3 || Tiles.Num() > 13)
	{
		bIsValidSet = false;
		return;
	}

	bIsValidSet = true;
	for (int i = 1; i < Tiles.Num() && bIsValidSet; ++i)
	{
		const FRummiTile& PrevTile = Tiles[i - 1];
		const FRummiTile& ThisTile = Tiles[i];

		if (ThisTile.Color != PrevTile.Color || ThisTile.Number != PrevTile.Number + 1)
		{
			bIsValidSet = false;
			break;
		}
	}

	if (!bIsValidSet)
	{
		int Number = Tiles[0].Number;
		TArray<bool, TInlineAllocator<32>> Colors;
		Colors.AddZeroed(32);

		bIsValidSet = true;
		for (int i = 0; i < Tiles.Num(); ++i)
		{
			if (Tiles[i].Number != Number || Colors[Tiles[i].Color])
			{
				bIsValidSet = false;
				break;
			}
			Colors[i] = true;
		}
	}
}

void FRummiBoard::EvaluateIsValidBoard()
{
	bIsValidBoard = true;
	for (FRummiTileBoardSet& TileSet : TileSets)
	{
		TileSet.EvaluateIsValidSet();
		if (!TileSet.bIsValidSet)
		{
			bIsValidBoard = false;
		}
	}
}

bool FRummiBoard::IsTileInValidSet(const FRummiTile& Tile) const
{
	for (const FRummiTileBoardSet& TileSet : TileSets)
	{
		for (const FRummiTile& ThisTile : TileSet.Tiles)
		{
			if (ThisTile.Id == Tile.Id)
			{
				return TileSet.bIsValidSet;
			}
		}
	}
	return false;
}

void FRummiTable::InitializeDeck(const FRummiRuleset& Ruleset)
{
	FRummiTileArray::CreateDeck(Deck, Ruleset.NumColors, Ruleset.NumNumbers, Ruleset.NumRepetitions, Ruleset.LowestNumber);
}

void FRummiTable::ShuffleDeck()
{
	Deck.Shuffle();
}
