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
		else if (Grid->ActorHasTag(FName(TEXT("HandGrid"))))
		{
			HandGrid = Grid;
		}
	}
}

void AKeyRummikubGameState::InitializeTable()
{
	Table.InitializeDeck(Ruleset);
	CreateTileActors(Table.Deck);
	Table.ShuffleDeck();
	Table.Hands.SetNum(Ruleset.NumPlayers);
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

void AKeyRummikubGameState::DrawCardFromDeckToHand(int HandIndex)
{
	FRummiTile Tile = Table.Deck.PopLastTile();
	Table.Hands[HandIndex].AddTileToEnd(Tile);
	ARummiTileActor* TileActor = GetActorFromTileInfo(Tile);
	HandGrid->PlaceTileAtFirstOpenGridLocation(TileActor);
}

bool AKeyRummikubGameState::TryMoveTileToWorldLocation(ARummiTileActor* Tile, const FVector& WorldLocation)
{
	if (!IsValid(Tile)) { return false; }

	ARummiGrid* OldGrid;
	int OldX, OldY;
	FindTileActorInGrids(Tile, OldGrid, OldX, OldY);

	if (!OldGrid)
	{
		// ERROR MSG
		return false;
	}

	int NewX, NewY;
	bool bIsOnGrid;
	BoardGrid->GetGridIndexAtWorldLocation(WorldLocation, NewX, NewY, bIsOnGrid);

	if (bIsOnGrid)
	{
		return TryMoveTile(Tile, OldGrid, OldX, OldY, BoardGrid, NewX, NewY);
	}
	else
	{
		HandGrid->GetGridIndexAtWorldLocation(WorldLocation, NewX, NewY, bIsOnGrid);
		if (bIsOnGrid)
		{
			return TryMoveTile(Tile, OldGrid, OldX, OldY, HandGrid, NewX, NewY);
		}
	}
	return false;
}

bool AKeyRummikubGameState::TryMoveTile(ARummiTileActor* Tile, ARummiGrid* OldGrid, int OldX, int OldY, ARummiGrid* NewGrid, int NewX, int NewY)
{
	if (!IsValid(Tile) || !IsValid(OldGrid) || !IsValid(NewGrid))
	{
		return false;
	}

	if (NewGrid->IsGridSpaceOccupied(NewX, NewY))
	{
		return false;
	}

	OldGrid->RemoveTileFromGridLocation(OldX, OldY);
	NewGrid->PlaceTileAtGridLocation(Tile, NewX, NewY);

	return true;

	// TODO: Update the logical representation
}

void AKeyRummikubGameState::FindTileActorInGrids(ARummiTileActor* Tile, ARummiGrid*& OutGrid, int& OutX, int& OutY)
{
	bool Found = BoardGrid->FindTileActor(Tile, OutX, OutY);

	if (Found)
	{
		OutGrid = BoardGrid;
		return;
	}

	Found = HandGrid->FindTileActor(Tile, OutX, OutY);

	if (Found)
	{
		OutGrid = HandGrid;
		return;
	}

	OutGrid = nullptr;
}
