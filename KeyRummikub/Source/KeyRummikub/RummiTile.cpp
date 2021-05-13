// Fill out your copyright notice in the Description page of Project Settings.


#include "RummiTile.h"

void FRummiTileArray::CreateDeck(FRummiTileArray& InDeck, int NumColors, int NumNumbers, int NumRepetitions, int LowestNumber)
{
	for (int Repetition = 0; Repetition < NumRepetitions; ++Repetition)
	{
		for (int Color = 0; Color < NumColors; ++Color)
		{
			for (int Number = LowestNumber; Number < NumNumbers + LowestNumber; ++Number)
			{
				InDeck.AddTileToEnd(FRummiTile(Color, Number));
			}
		}
	}
}

void FRummiTileArray::AddTileToEnd(FRummiTile Tile)
{
	Tiles.Add(Tile);
}

void FRummiTable::Initialize(const FRummiRuleset& Ruleset)
{
	FRummiTileArray::CreateDeck(Deck, Ruleset.NumColors, Ruleset.NumNumbers, Ruleset.NumRepetitions, Ruleset.LowestNumber);
}
