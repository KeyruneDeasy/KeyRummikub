// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyRummikubGameState.h"
#include "EngineUtils.h"
#include "RummiAi.h"

AKeyRummikubGameState::AKeyRummikubGameState()
	: Super()
	, TileActorClass(ARummiTileActor::StaticClass())
{
}

void AKeyRummikubGameState::BeginPlay()
{
	Super::BeginPlay();
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

void AKeyRummikubGameState::Tick(float DeltaTime)
{
	URummiAi* CurrentAi = PlayerInfos[CurrentTurnPlayerIndex].Ai;
	if (CurrentAi != nullptr)
	{
		bool bTurnReadyToEnd = CurrentAi->Update(DeltaTime, this);

		if (bTurnReadyToEnd)
		{
			EndTurn();
		}
	}
}

void AKeyRummikubGameState::InitializeTable()
{
	Table.InitializeDeck(Ruleset);
	CreateTileActors(Table.Deck);
	Table.ShuffleDeck();
	Table.Hands.Empty();
	Table.Hands.SetNum(Ruleset.NumPlayers);
	PlayerInfos.Empty();
	PlayerInfos.SetNum(Ruleset.NumPlayers);
	for (int i = 1; i < PlayerInfos.Num(); ++i)
	{
		PlayerInfos[i].Ai = NewObject<URummiAi>();
	}
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
	if (HandIndex == DisplayedHandIndex)
	{
		ARummiTileActor* TileActor = GetActorFromTileInfo(Tile);
		HandGrid->PlaceTileAtFirstOpenGridLocation(TileActor);
	}
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
			if (HandTilesAtStartOfTurn.Contains(Tile))
			{
				return TryMoveTile(Tile, OldGrid, OldX, OldY, HandGrid, NewX, NewY);
			}
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

	if (OldGrid == HandGrid && NewGrid == BoardGrid)
	{
		TilesPlayedThisTurn.Add(Tile);
	}
	else if (OldGrid == HandGrid && NewGrid == BoardGrid)
	{
		TilesPlayedThisTurn.Remove(Tile);
	}

	OldGrid->RemoveTileFromGridLocation(OldX, OldY);

	/*if (OldGrid == HandGrid)
	{
		if (NewGrid == HandGrid)
		{
			// No logical change.
			return;
		}
		else if(NewGrid == BoardGrid)
		{
			Table.Hands[DisplayedHandIndex].RemoveTile(Tile->TileInfo);
		}
	}*/


	NewGrid->PlaceTileAtGridLocation(Tile, NewX, NewY);

	//const FRummiTile& PlacedTile = Tile->TileInfo;

	UpdateLogicalRepresentationFromGrids();

	return true;
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

void AKeyRummikubGameState::UpdateLogicalRepresentationFromGrids()
{
	Table.Board.Reset();
	BoardGrid->PopulateBoardLogicalRepresentation(Table.Board);

	Table.Hands[DisplayedHandIndex].Reset();
	HandGrid->PopulateHandLogicalRepresentation(Table.Hands[DisplayedHandIndex]);
}

void AKeyRummikubGameState::UpdateGridsFromLogicalRepresentation()
{
	BoardGrid->PopulateFromRummiBoard(Table.Board, TileActors);
	HandGrid->PopulateFromRummiHand(Table.Hands[DisplayedHandIndex], TileActors);
}

void AKeyRummikubGameState::EndPlayerTurn()
{
	EndTurn();
}

void AKeyRummikubGameState::EndTurn()
{
	if (TilesPlayedThisTurn.Num() == 0 && !DeckIsEmpty())
	{
		DrawCardFromDeckToHand(CurrentTurnPlayerIndex);
	}

	if (TilesPlayedThisTurn.Num() > 0)
	{
		PlayerInfos[CurrentTurnPlayerIndex].bHasPlayedFirstTiles = true;
	}
	TilesPlayedThisTurn.Empty();

	if (!IsPlayerTurn())
	{
		PlayerInfos[CurrentTurnPlayerIndex].Ai->NotifyTurnEnded();
	}


	CurrentTurnPlayerIndex = (CurrentTurnPlayerIndex + 1) % PlayerInfos.Num();

	if (IsPlayerTurn())
	{
		SaveTableLayoutAtStartOfTurn();
	}
}

void AKeyRummikubGameState::SortHandByAscendingNumber()
{
	HandGrid->SortIndividualTilesByAscendingNumber();
	UpdateLogicalRepresentationFromGrids();
}

void AKeyRummikubGameState::SortHandByColor()
{
	HandGrid->SortIndividualTilesByColor();
	UpdateLogicalRepresentationFromGrids();
}

bool AKeyRummikubGameState::DeckIsEmpty() const
{
	return Table.Deck.Tiles.IsEmpty();
}

void AKeyRummikubGameState::RevertCurrentTurn()
{
	BoardGrid->RestoreLayout(BoardLayoutAtStartOfTurn);

	for (ARummiTileActor* Tile : HandTilesAtStartOfTurn)
	{
		if (!HandGrid->ContainsTileActor(Tile))
		{
			HandGrid->PlaceTileAtFirstOpenGridLocation(Tile);
		}
	}
	TilesPlayedThisTurn.Empty();

	UpdateLogicalRepresentationFromGrids();
}

bool AKeyRummikubGameState::IsPlayerTurn() const
{
	return PlayerInfos[CurrentTurnPlayerIndex].Ai == nullptr;
}

void AKeyRummikubGameState::SaveTableLayoutAtStartOfTurn()
{
	BoardLayoutAtStartOfTurn.Empty();
	BoardGrid->CopyLayout(BoardLayoutAtStartOfTurn);
	HandTilesAtStartOfTurn.Empty();
	HandGrid->CopyContainedTiles(HandTilesAtStartOfTurn);
}

void AKeyRummikubGameState::StartGame()
{
	bGameStarted = true;
	CurrentTurnPlayerIndex = 0;

	Table.Board.EvaluateIsValidBoard();

	if (IsPlayerTurn())
	{
		SaveTableLayoutAtStartOfTurn();
	}
}

bool AKeyRummikubGameState::IsBoardValid() const
{
	return Table.Board.bIsValidBoard;
}

int AKeyRummikubGameState::GetValuePlayedThisTurn() const
{
	int ValuePlayed = 0;
	for (ARummiTileActor* Tile : TilesPlayedThisTurn)
	{
		ValuePlayed += Tile->TileInfo.Number;
	}
	return ValuePlayed;
}

bool AKeyRummikubGameState::CanEndTurn() const
{
	return IsBoardValid() &&
		(TilesPlayedThisTurn.IsEmpty() || CurrentPlayerHasEverPlayedTiles() || GetValuePlayedThisTurn() >= Ruleset.FirstTilesRequiredValue);
}

bool AKeyRummikubGameState::CurrentPlayerHasEverPlayedTiles() const
{
	return PlayerInfos[CurrentTurnPlayerIndex].bHasPlayedFirstTiles;
}

int AKeyRummikubGameState::GetNumTilesInDeck() const
{
	return Table.Deck.NumTiles();
}

int AKeyRummikubGameState::GetNumTilesInPlayerHand(int PlayerIndex) const
{
	ensureMsgf(PlayerIndex > 0 && PlayerIndex < PlayerInfos.Num(), TEXT("Invalid player index %d."));

	return Table.Hands[PlayerIndex].NumTiles();
}

bool AKeyRummikubGameState::IsPlayerAiControlled(int PlayerIndex) const
{
	ensureMsgf(PlayerIndex >= 0 && PlayerIndex < PlayerInfos.Num(), TEXT("Invalid player index %d."));

	return PlayerInfos[PlayerIndex].Ai != nullptr;
}

FString AKeyRummikubGameState::GetAiPlayerTileCountsText() const
{
	FString OutText = FString::Printf(TEXT("Deck: %d\n\n"), GetNumTilesInDeck());
	for (int i = 0; i < PlayerInfos.Num(); ++i)
	{
		if (IsPlayerAiControlled(i))
		{
			OutText.Append(FString::Printf(TEXT("Player %d: %d\n"), i + 1, GetNumTilesInPlayerHand(i)));
		}
	}
	OutText.RemoveFromEnd("\n");
	return OutText;
}
