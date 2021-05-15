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

	//DebugDrawGrid();
}

FVector ARummiGrid::GetWorldSpaceGridTileLocation(int X, int Y)
{
	return GetActorTransform().TransformPosition(GetLocalSpaceGridTileLocation(X, Y));
}

FVector ARummiGrid::GetLocalSpaceGridTileLocation(int X, int Y)
{
	return GetLocalSpaceBaseGridTileLocation() + FVector((float)X * GridSpacingX, (float)Y * GridSpacingY, 0.0f);
}

FVector ARummiGrid::GetLocalSpaceBaseGridTileLocation()
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

void ARummiGrid::DebugDrawGrid()
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
	ARummiTileActor* RemovedTile = GridSpaces[Index];
	GridSpaces[Index] = nullptr;
	if (RemovedTile == nullptr)
	{
		FString ErrorStr = FString::Printf(TEXT("Tried to remove a tile from an unoccupied grid square (%d,%d)."), X, Y);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorStr);
		return nullptr;
	}
	RemovedTile->OnRemovedFromGrid();
	return RemovedTile;
}

int ARummiGrid::GetGridIndicesAsArrayIndex(int X, int Y)
{
	return Y * GridSizeX + X;
}
