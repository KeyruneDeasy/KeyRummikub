// Fill out your copyright notice in the Description page of Project Settings.


#include "RummiGrid.h"
#include "DrawDebugHelpers.h"
#include "RummiTileActor.h"

// Sets default values
ARummiGrid::ARummiGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARummiGrid::BeginPlay()
{
	Super::BeginPlay();

	GridSpaces.SetNum(GridSizeX * GridSizeY);
}

// Called every frame
void ARummiGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ARummiGrid::GetWorldSpaceGridTileLocation(int X, int Y)
{
	return ConvertLocalSpaceToWorldSpace(GetLocalSpaceGridTileLocation(X, Y));
}

FVector ARummiGrid::ConvertLocalSpaceToWorldSpace(const FVector& LocalSpaceVector) const
{
	return GetActorTransform().TransformPosition(LocalSpaceVector);
}

FVector ARummiGrid::ConvertWorldSpaceToLocalSpace(const FVector& LocalSpaceVector) const
{
	return GetActorTransform().InverseTransformPosition(LocalSpaceVector);
}

FVector ARummiGrid::GetLocalSpaceGridTileLocation(int X, int Y) const
{
	return GetLocalSpaceBaseGridTileLocation() + FVector((float)X * GridSpacingX, (float)Y * GridSpacingY, 0.0f);
}

FVector ARummiGrid::GetLocalSpaceBaseGridTileLocation() const
{
	return FVector((float)(GridSizeX - 1) * GridSpacingX * (-0.5f), (float)(GridSizeY - 1) * GridSpacingY * (-0.5f), 0.0f);
}

float ARummiGrid::GetGridWidth()
{
	return (float)GridSizeX * GridSpacingX;
}

float ARummiGrid::GetGridHeight()
{
	return (float)GridSizeY * GridSpacingY;
}

void ARummiGrid::DebugDrawGridCoords()
{
	for (int X = 0; X < GridSizeX; ++X)
	{
		for (int Y = 0; Y < GridSizeY; ++Y)
		{
			FString DebugStr = FString::Printf(TEXT("(%d,%d)"), X, Y);
			DrawDebugString(GetWorld(), GetWorldSpaceGridTileLocation(X, Y), DebugStr, nullptr, FColor::White, 0.0f);
		}
	}
}

void ARummiGrid::DebugDrawGridLines_Internal(const FRummiBoard* Board)
{
	UWorld* World = GetWorld();
	for (int X = -1; X < GridSizeX; ++X)
	{
		for (int Y = -1; Y < GridSizeY; ++Y)
		{
			FColor ColorVertical = FColor::Black;
			FColor ColorHorizontal = FColor::Black;
			if (Board != nullptr)
			{
				ARummiTileActor* ThisTile = nullptr;
				if (X != -1 && Y != -1)
				{
					int Index = GetGridIndicesAsArrayIndex(X, Y);
					ThisTile = GridSpaces[Index];
				}
				ARummiTileActor* TileBelow = nullptr;
				if (X != -1 && Y != GridSizeY - 1)
				{
					int Index = GetGridIndicesAsArrayIndex(X, Y + 1);
					TileBelow = GridSpaces[Index];
				}
				ARummiTileActor* TileRight = nullptr;
				if (X != GridSizeX - 1 && Y != -1)
				{
					int Index = GetGridIndicesAsArrayIndex(X + 1, Y);
					TileRight = GridSpaces[Index];
				}

				bool bIsThisTileInInvalidSet = ThisTile != nullptr && !Board->IsTileInValidSet(ThisTile->TileInfo);

				if (bIsThisTileInInvalidSet)
				{
					ColorVertical = FColor::Red;
					ColorHorizontal = FColor::Red;
				}
				else
				{
					bool bIsThisTileInValidSet = ThisTile != nullptr && !bIsThisTileInInvalidSet;

					if (ThisTile != nullptr || TileBelow != nullptr)
					{
						bool bIsTileBelowInInvalidSet = TileBelow != nullptr && !Board->IsTileInValidSet(TileBelow->TileInfo);
						ColorHorizontal = bIsThisTileInInvalidSet || bIsTileBelowInInvalidSet ? FColor::Red : FColor::Green; 
					}
					if (ThisTile != nullptr || TileRight != nullptr)
					{
						bool bIsTileRightInInvalidSet = TileRight != nullptr && !Board->IsTileInValidSet(TileRight->TileInfo);
						ColorVertical = bIsThisTileInInvalidSet || bIsTileRightInInvalidSet ? FColor::Red : FColor::Green;
					}
				}
			}
			FVector TileCentre = GetLocalSpaceGridTileLocation(X, Y);
			FVector TileBottomRight = TileCentre + FVector(GridSpacingX / 2.0f, GridSpacingY / 2.0f, 0.0f);
			if (X != -1)
			{
				FVector TileBottomLeft = TileBottomRight - FVector(GridSpacingX, 0.0f, 0.0f);
				DrawDebugLine(World, ConvertLocalSpaceToWorldSpace(TileBottomRight), ConvertLocalSpaceToWorldSpace(TileBottomLeft), ColorHorizontal, false, -1.0f, 0, 5.0f);
			}
			if (Y != -1)
			{
				FVector TileTopRight = TileBottomRight - FVector(0.0f, GridSpacingY, 0.0f);
				DrawDebugLine(World, ConvertLocalSpaceToWorldSpace(TileBottomRight), ConvertLocalSpaceToWorldSpace(TileTopRight), ColorVertical, false, -1.0f, 0, 5.0f);
			}
		}
	}
}

void ARummiGrid::PlaceTileAtGridLocation(ARummiTileActor* Tile, int X, int Y)
{
	if (Tile == nullptr)
	{
		FString ErrorStr = FString::Printf(TEXT("Tried to place a NULL tile into grid square (%d,%d)."), X, Y);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
		return;
	}
	int Index = GetGridIndicesAsArrayIndex(X, Y);
	if (GridSpaces[Index] != nullptr)
	{
		FString ErrorStr = FString::Printf(TEXT("Tried to place a tile in occupied grid square (%d,%d)."), X, Y);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
		return;
	}
	GridSpaces[Index] = Tile;

	Tile->OnPlacedIntoGrid(GetWorldSpaceGridTileLocation(X, Y));
}

ARummiTileActor* ARummiGrid::RemoveTileFromGridLocation(int X, int Y)
{
	int Index = GetGridIndicesAsArrayIndex(X, Y);
	return RemoveTileFromIndex(Index);
}

ARummiTileActor* ARummiGrid::RemoveTileFromIndex(int Index)
{
	ARummiTileActor* RemovedTile = GridSpaces[Index];
	GridSpaces[Index] = nullptr;
	if (RemovedTile == nullptr)
	{
		//FString ErrorStr = FString::Printf(TEXT("Tried to remove a tile from an unoccupied grid square (%d,%d)."), X, Y);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
		return nullptr;
	}
	RemovedTile->OnRemovedFromGrid();
	return RemovedTile;
}

int ARummiGrid::GetGridIndicesAsArrayIndex(int X, int Y)
{
	return Y * GridSizeX + X;
}

void ARummiGrid::PlaceTileAtFirstOpenGridLocation(ARummiTileActor* Tile)
{
	bool bTilePlaced = false;
	for (int Y = 0; Y < GridSizeY && !bTilePlaced; ++Y)
	{
		for (int X = 0; X < GridSizeX && !bTilePlaced; ++X)
		{
			int Index = GetGridIndicesAsArrayIndex(X, Y);
			if (GridSpaces[Index] == nullptr)
			{
				PlaceTileAtGridLocation(Tile, X, Y);
				bTilePlaced = true;
			}
		}
	}
}

ARummiTileActor* ARummiGrid::GetTileActorAtWorldLocation(const FVector& WorldLocation)
{
	int X, Y;
	bool bIsValidIndex;
	GetGridIndexAtWorldLocation(WorldLocation, X, Y, bIsValidIndex);

	if (bIsValidIndex)
	{
		int Index = GetGridIndicesAsArrayIndex(X, Y);
		return GridSpaces[Index];
	}
	return nullptr;
}

FVector ARummiGrid::GetLocalSpaceTopLeftCorner() const
{
	return GetLocalSpaceBaseGridTileLocation() - FVector(GridSpacingX / 2.0f, GridSpacingY / 2.0f, 0.0f);
}

bool ARummiGrid::IsValidGridIndex(int X, int Y)
{
	return X >= 0 && X < GridSizeX && Y >= 0 && Y < GridSizeY;
}

void ARummiGrid::GetGridIndexAtWorldLocation(const FVector& WorldLocation, int& OutX, int& OutY, bool& bIsOnGrid)
{
	FVector LocalLocation = ConvertWorldSpaceToLocalSpace(WorldLocation);

	FVector Corner = GetLocalSpaceTopLeftCorner();

	FVector RelativeLocation = LocalLocation - Corner;

	OutX = FMath::Floor(RelativeLocation.X / GridSpacingX);
	OutY = FMath::Floor(RelativeLocation.Y / GridSpacingY);

	bIsOnGrid = IsValidGridIndex(OutX, OutY);
}

bool ARummiGrid::IsGridSpaceOccupied(int X, int Y)
{
	if (IsValidGridIndex(X, Y))
	{
		int Index = GetGridIndicesAsArrayIndex(X, Y);
		return GridSpaces[Index] != nullptr;
	}
	return false;
}

bool ARummiGrid::FindTileActor(ARummiTileActor* TileActor, int& OutX, int& OutY)
{
	if (!IsValid(TileActor)) { return false; }

	for (int Y = 0; Y < GridSizeY; ++Y)
	{
		for (int X = 0; X < GridSizeX; ++X)
		{
			int Index = GetGridIndicesAsArrayIndex(X, Y);
			if (GridSpaces[Index] == TileActor)
			{
				OutX = X;
				OutY = Y;
				return true;
			}
		}
	}
	
	return false;
}

void ARummiGrid::PopulateHandLogicalRepresentation(FRummiTileArray& Hand)
{
	for (ARummiTileActor* TileActor : GridSpaces)
	{
		if (TileActor != nullptr)
		{
			Hand.AddTileToEnd(TileActor->TileInfo);
		}
	}
}

void ARummiGrid::PopulateBoardLogicalRepresentation(FRummiBoard& Board)
{
	FRummiTileArray* CurrentArray = nullptr;
	for (int Y = 0; Y < GridSizeY; ++Y)
	{
		for (int X = 0; X < GridSizeX; ++X)
		{
			int Index = GetGridIndicesAsArrayIndex(X, Y);
			ARummiTileActor* TileActor = GridSpaces[Index];
			if (TileActor == nullptr)
			{
				CurrentArray = nullptr;
			}
			else
			{
				if (CurrentArray == nullptr)
				{
					Board.TileSets.AddDefaulted(1);
					CurrentArray = &Board.TileSets.Last();
				}
				CurrentArray->AddTileToEnd(TileActor->TileInfo);
			}
		}
		CurrentArray = nullptr;
	}
	Board.EvaluateIsValidBoard();
}

void ARummiGrid::PopulateFromRummiBoard(const FRummiBoard& Board, const TArray<ARummiTileActor*>& TileActors)
{
	Clear();

	int NextGridX = 0;
	int NextGridY = 0;

	for (const FRummiTileArray& TileSet : Board.TileSets)
	{
		int TilesRemainingInRow = GridSizeX - NextGridX;
		if (TileSet.NumTiles() > TilesRemainingInRow)
		{
			NextGridX = 0;
			++NextGridY;
			if (NextGridY >= GridSizeY)
			{
				return;
			}
		}
		for (const FRummiTile& Tile : TileSet.Tiles)
		{
			ARummiTileActor* TileActor = TileActors[Tile.Id];
			PlaceTileAtGridLocation(TileActor, NextGridX, NextGridY);
			++NextGridX;
		}
		++NextGridX;
	}
}

void ARummiGrid::PopulateFromRummiHand(const FRummiTileArray& Hand, const TArray<ARummiTileActor*>& TileActors)
{
	Clear();

	for (int i = 0; i < Hand.NumTiles(); ++i)
	{
		ARummiTileActor* TileActor = TileActors[Hand.Tiles[i].Id];
		PlaceTileAtFirstOpenGridLocation(TileActor);
	}
}

void ARummiGrid::Clear()
{
	for (int i = 0; i < GridSpaces.Num(); ++i)
	{
		if (GridSpaces[i] != nullptr)
		{
			RemoveTileFromIndex(i);
		}
	}
}
