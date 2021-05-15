// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyRummikubGameState.h"
#include "EngineUtils.h"

AKeyRummikubGameState::AKeyRummikubGameState()
	: Super()
	, TileActorClass(ARummiTileActor::StaticClass())
{
}

void AKeyRummikubGameState::BeginPlay()
{
	for (TActorIterator<ARummiGrid> It(GetWorld()); It; ++It)
	{
		ARummiGrid* Grid = *It;
		if (Grid->ActorHasTag(FName(TEXT("BoardGrid"))))
		{
			BoardGrid = Grid;
		}
	}
}

void AKeyRummikubGameState::InitializeTable()
{
	Table.InitializeDeck(Ruleset);
	CreateTileActors(Table.Deck);
	Table.ShuffleDeck();
}

void AKeyRummikubGameState::CreateTileActors(const FRummiTileArray& Tiles)
{
	FActorSpawnParameters SpawnInfo;
	for (int i = 0; i < Tiles.NumTiles(); ++i)
	{
		ARummiTileActor* TileActor = GetWorld()->SpawnActor<ARummiTileActor>(TileActorClass);
		TileActor->TileInfo = Tiles.GetTileByIndex(i);

		TileActors.Add(TileActor);
	}
}

ARummiTileActor* AKeyRummikubGameState::GetActorFromTileInfo(const FRummiTile& TileInfo)
{
	return GetActorFromTileId(TileInfo.Id);
}

ARummiTileActor* AKeyRummikubGameState::GetActorFromTileId(int TileId)
{
	if (TileId < 0 || TileId >= TileActors.Num())
	{
		FString ErrorStr = FString::Printf(TEXT("Looked up tile with invalid id %d."), TileId);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
		return nullptr;
	}

	return TileActors[TileId];
}

void AKeyRummikubGameState::DealRandomCardsFromDeckOntoBoard()
{
	FRummiTileArray& Deck = Table.Deck;
	FRummiBoard& Board = Table.Board;
	for (int Y = 0; Y < BoardGrid->GetGridSizeY(); ++Y)
	{
		for (int X = 0; X < BoardGrid->GetGridSizeX(); ++X)
		{
			if (Deck.NumTiles() > 0)
			{
				bool Draw = FMath::RandBool();
				if (Draw)
				{
					FRummiTile Tile = Deck.PopLastTile();
					// Should add the Tile to the logical board, but skip for now.
					ARummiTileActor* TileActor = GetActorFromTileInfo(Tile);
					//FString ErrorStr = FString::Printf(TEXT("Dealing card into grid square (%d,%d)"), X, Y);
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
					BoardGrid->PlaceTileAtGridLocation(TileActor, X, Y);
				}
			}
		}
	}
}
