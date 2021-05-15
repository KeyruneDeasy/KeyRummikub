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

void FRummiTable::InitializeDeck(const FRummiRuleset& Ruleset)
{
	FRummiTileArray::CreateDeck(Deck, Ruleset.NumColors, Ruleset.NumNumbers, Ruleset.NumRepetitions, Ruleset.LowestNumber);
}

void FRummiTable::ShuffleDeck()
{
	Deck.Shuffle();
}
