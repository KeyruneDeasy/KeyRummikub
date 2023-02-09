// Fill out your copyright notice in the Description page of Project Settings.


#include "RummiTile.h"
#include "KeyUtil.h"
#include "Algo/Reverse.h"

FString FRummiTile::GetAsLongString() const
{
	return FString::Printf(TEXT("Color: %d, Number: %d, Id: %d"), Color, Number, Id);
}

FString FRummiTile::GetAsShortString() const
{
	return FString::Printf(TEXT("%d,%d,%d"), Color, Number, Id);
}

bool FRummiTile::operator==(const FRummiTile& RHS) const
{
	return
		this->Color == RHS.Color &&
		this->Number == RHS.Number &&
		this->Id == RHS.Id;
}

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

void FRummiTileArray::AddTileToStart(FRummiTile Tile)
{
	Tiles.Insert(Tile, 0);
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

void FRummiTileArray::SortByAscendingNumber()
{
	Tiles.Sort([](const FRummiTile& A, const FRummiTile& B)
		{
			return A.Number < B.Number || (A.Number == B.Number && A.Color < B.Color);
		});
}

void FRummiTileArray::SortByColor()
{
	Tiles.Sort([](const FRummiTile& A, const FRummiTile& B)
		{
			return A.Color < B.Color || (A.Color == B.Color && A.Number < B.Number);
		});
}

void FRummiTileArray::FindValidSubsets(TArray<FRummiTileArray>& OutSubsets)
{
	FRummiTileArray Temp = *this;
	Temp.SortByAscendingNumber();
	int BaseIndex = 0;
	int NumDistinctColors = 1;
	for (int i = 1; i < Temp.Tiles.Num(); ++i)
	{
		if (Temp.Tiles[i].Number == Temp.Tiles[i - 1].Number)
		{
			if (Temp.Tiles[i].Color != Temp.Tiles[i - 1].Color)
			{
				++NumDistinctColors;
			}
		}
		else
		{
			int EndIndex = i - 1;
			// BaseIndex to EndIndex, inclusive, is a set of tiles with the same number.
			if (NumDistinctColors >= 3)
			{
				OutSubsets.AddDefaulted(1);
				FRummiTileArray& NewSubset = OutSubsets.Last();
				// Take the first occurrence of each color and make a set out of them.
				for (int j = EndIndex; j >= BaseIndex; --j)
				{
					if (j == BaseIndex || Temp.Tiles[j].Color != Temp.Tiles[j - 1].Color)
					{
						NewSubset.AddTileToEnd(Temp.Tiles[j]);
						Temp.Tiles.RemoveAt(j);
					}
				}
				i = BaseIndex; // Recheck the remaining tiles to see if there's another set that can be made from them.
			}
			BaseIndex = i;
			NumDistinctColors = 1;
		}
	}

	Temp.SortByColor();
	BaseIndex = 0;
	int NumDistinctTiles = 1;
	for (int i = 1; i < Temp.Tiles.Num(); ++i)
	{
		bool bMatchingColor = Temp.Tiles[i].Color == Temp.Tiles[i - 1].Color;
		if (bMatchingColor && Temp.Tiles[i].Number == Temp.Tiles[i - 1].Number + 1)
		{
			++NumDistinctTiles;
		}
		else if (bMatchingColor && Temp.Tiles[i].Number == Temp.Tiles[i - 1].Number)
		{
			// Not a distinct tile, but keep counting.
		}
		else
		{
			int EndIndex = i - 1;
			// BaseIndex to EndIndex, inclusive, is a set of tiles of matching colour and ascending number.
			if (NumDistinctTiles >= 3)
			{
				OutSubsets.AddDefaulted(1);
				FRummiTileArray& NewSubset = OutSubsets.Last();
				// Take the first occurrence of each number and make a set out of them.
				for (int j = EndIndex; j >= BaseIndex; --j)
				{
					if (j == BaseIndex || Temp.Tiles[j].Number != Temp.Tiles[j - 1].Number)
					{
						NewSubset.AddTileToEnd(Temp.Tiles[j]);
						Temp.Tiles.RemoveAt(j);
					}
				}
				Algo::Reverse(NewSubset.Tiles);
				i = BaseIndex; // Recheck the remaining tiles to see if there's another set that can be made from them.
			}
			BaseIndex = i;
			NumDistinctTiles = 1;
		}
	}
}

bool FRummiTileArray::EvaluateIsValidSet()
{
	if (Tiles.Num() < 3 || Tiles.Num() > 13)
	{
		return false;
	}

	bool bIsValidSetLocal = true;
	for (int i = 1; i < Tiles.Num() && bIsValidSetLocal; ++i)
	{
		const FRummiTile& PrevTile = Tiles[i - 1];
		const FRummiTile& ThisTile = Tiles[i];

		if (ThisTile.Color != PrevTile.Color || ThisTile.Number != PrevTile.Number + 1)
		{
			bIsValidSetLocal = false;
			break;
		}
	}

	if (!bIsValidSetLocal)
	{
		int Number = Tiles[0].Number;
		TArray<bool, TInlineAllocator<32>> Colors;
		Colors.AddZeroed(32);

		bIsValidSetLocal = true;
		for (int i = 0; i < Tiles.Num(); ++i)
		{
			if (Tiles[i].Number != Number || Colors[Tiles[i].Color])
			{
				bIsValidSetLocal = false;
				break;
			}
			Colors[Tiles[i].Color] = true;
		}
	}
	return bIsValidSetLocal;
}

ETileArrayType FRummiTileArray::GetTileArrayType()
{
	if (ensureMsgf(Tiles.Num() >= 3, TEXT("Called GetTileArrayType on too small TileArray.")))
	{
		if (Tiles[0].Color == Tiles[1].Color)
		{
			return ETileArrayType::AscendingNumbers;
		}
		else if (Tiles[0].Number == Tiles[1].Number)
		{
			return ETileArrayType::MatchingNumbers;
		}
		else
		{
			ensureMsgf(false, TEXT("Called GetTileArrayType on invalid TileArray."));
		}
	}

	return ETileArrayType::AscendingNumbers;
}

FRummiTile& FRummiTileArray::GetFirstTile()
{
	return Tiles[0];
}

FRummiTile& FRummiTileArray::GetLastTile()
{
	return Tiles[Tiles.Num() - 1];
}

int FRummiTileArray::GetTotalValue() const
{
	int TotalValue = 0;
	for (const FRummiTile& Tile : Tiles)
	{
		TotalValue += Tile.Number;
	}
	return TotalValue;
}

FRummiTileBoardSet::FRummiTileBoardSet(const FRummiTileArray& InSet)
{
	this->Tiles = InSet.Tiles;
}

void FRummiBoard::EvaluateIsValidBoard()
{
	bIsValidBoard = true;
	for (FRummiTileBoardSet& TileSet : TileSets)
	{
		TileSet.bIsValidSet = TileSet.EvaluateIsValidSet();
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

void FRummiBoard::AddTileSet(const FRummiTileArray& NewTileSet)
{
	TileSets.Add(FRummiTileBoardSet(NewTileSet));
	bool bIsValidSet = TileSets.Last().EvaluateIsValidSet();
	bIsValidBoard &= bIsValidSet;
}

void FRummiBoard::RemoveTileSet(int Index)
{
	TileSets.RemoveAtSwap(Index);
}

void FRummiTable::InitializeDeck(const FRummiRuleset& Ruleset)
{
	FRummiTileArray::CreateDeck(Deck, Ruleset.NumColors, Ruleset.NumNumbers, Ruleset.NumRepetitions, Ruleset.LowestNumber);
}

void FRummiTable::ShuffleDeck()
{
	Deck.Shuffle();
}
