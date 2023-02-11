// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RummiTile.generated.h"

#define HARD_MAX_COLORS 32

UENUM()
enum class ETileArrayType : uint8
{
	AscendingNumbers,
	MatchingNumbers,
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiRuleset
{
	GENERATED_BODY()

public:
	FRummiRuleset() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumColors = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumNumbers = 13;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumRepetitions = 2; // The number of copies of each unique tile
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LowestNumber = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumPlayers = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartingHandSize = 14;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FirstTilesRequiredValue = 30;
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

	FString GetAsLongString() const;
	FString GetAsShortString() const;

	bool operator==(const FRummiTile& RHS) const;
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

	void AddTileToStart(FRummiTile Tile);
	void AddTileToEnd(FRummiTile Tile);
	int NumTiles() const { return Tiles.Num(); }
	const FRummiTile& GetTileByIndex(int Index) const { return Tiles[Index]; }
	void Shuffle();
	FRummiTile PopLastTile() { return PopTileAtIndex(Tiles.Num() - 1); }
	FRummiTile PopTileAtIndex(int Index);
	void RemoveTile(const FRummiTile& Tile);
	void Reset() { Tiles.Empty(); }
	void SortByAscendingNumber();
	void SortByColor();
	void FindValidSubsets(TArray<FRummiTileArray>& OutSubsets);
	bool EvaluateIsValidSet();
	ETileArrayType GetTileArrayType();
	FRummiTile& GetFirstTile();
	FRummiTile& GetLastTile();
	int GetTotalValue() const;
	bool ContainsColor(int Color) const;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRummiTile> Tiles;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiTileBoardSet : public FRummiTileArray
{
	GENERATED_BODY()

public:
	FRummiTileBoardSet() {}
	FRummiTileBoardSet(const FRummiTileArray& InSet);

	bool bIsValidSet;
};

USTRUCT(BlueprintType)
struct KEYRUMMIKUB_API FRummiBoard
{
	GENERATED_BODY()

public:
	FRummiBoard() {}

	void Reset() { TileSets.Empty(); }
	void EvaluateIsValidBoard();
	bool IsTileInValidSet(const FRummiTile& Tile) const;
	void AddTileSet(const FRummiTileArray& NewTileSet);
	void RemoveTileSet(int Index);

	UPROPERTY(BlueprintReadOnly)
	TArray<FRummiTileBoardSet> TileSets;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValidBoard;
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
