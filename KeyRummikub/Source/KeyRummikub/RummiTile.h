// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RummiTile.generated.h"


USTRUCT()
struct KEYRUMMIKUB_API FRummiRuleset
{
	GENERATED_BODY()

public:
	FRummiRuleset() {}

	int NumColors = 4;
	int NumNumbers = 13;
	int NumRepetitions = 2; // The number of copies of each unique tile
	int LowestNumber = 1;
	int NumPlayers = 2;
	int StartingHandSize = 14;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiTile
{
	GENERATED_BODY()

public:
	FRummiTile() {}
	FRummiTile(int InColor, int InNumber, int InId) 
		: Color(InColor), Number(InNumber), Id(InId) {}

	UPROPERTY(BlueprintReadOnly)
	int Color;
	UPROPERTY(BlueprintReadOnly)
	int Number;
	UPROPERTY(BlueprintReadOnly)
	int Id;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiTileArray
{
	GENERATED_BODY()

public:
	static void CreateDeck(FRummiTileArray& InDeck, int NumColors, int NumNumbers, int NumRepetitions, int LowestNumber);

	FRummiTileArray() {}
	FRummiTileArray(const TArray<FRummiTile>& InTiles)
		: Tiles(InTiles) {}

	void AddTileToEnd(FRummiTile Tile);
	int NumTiles() const { return Tiles.Num(); }
	const FRummiTile& GetTileByIndex(int Index) const { return Tiles[Index]; }
	void Shuffle();
	FRummiTile PopLastTile() { return PopTileAtIndex(Tiles.Num() - 1); }
	FRummiTile PopTileAtIndex(int Index);

	UPROPERTY(BlueprintReadOnly)
	TArray<FRummiTile> Tiles;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiBoard
{
	GENERATED_BODY()

public:
	FRummiBoard() {}

	UPROPERTY(BlueprintReadOnly)
	TArray<FRummiTileArray> TileSets;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiTable
{
	GENERATED_BODY()

public:
	FRummiTable() {}

	void InitializeDeck(const FRummiRuleset& Ruleset);
	void ShuffleDeck();

	UPROPERTY(BlueprintReadOnly)
	FRummiBoard Board;
	UPROPERTY(BlueprintReadOnly)
	TArray<FRummiTileArray> Hands;
	UPROPERTY(BlueprintReadOnly)
	FRummiTileArray Deck;
};
