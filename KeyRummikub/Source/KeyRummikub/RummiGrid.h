// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RummiTile.h"
#include "RummiGrid.generated.h"

class ARummiTileActor;
struct FRummiTileArray;
struct FRummiBoard;

UCLASS(Blueprintable)
class KEYRUMMIKUB_API ARummiGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ARummiGrid();

public:	
	virtual void Tick(float DeltaTime) override;

	FVector GetWorldSpaceGridTileLocation(int X, int Y);

	UFUNCTION(BlueprintCallable)
	void PlaceTileAtGridLocation(ARummiTileActor* Tile, int X, int Y);
	UFUNCTION(BlueprintCallable)
	ARummiTileActor* RemoveTileFromGridLocation(int X, int Y);
	UFUNCTION(BlueprintCallable)
	void PlaceTileAtFirstOpenGridLocation(ARummiTileActor* Tile);

	UFUNCTION(BlueprintPure)
	int GetGridSizeX() { return GridSizeX; }
	UFUNCTION(BlueprintPure)
	int GetGridSizeY() { return GridSizeY; }
	UFUNCTION(BlueprintPure)
	float GetGridSpacingX() { return GridSpacingX; }
	UFUNCTION(BlueprintPure)
	float GetGridSpacingY() { return GridSpacingY; }

	UFUNCTION(BlueprintPure)
	ARummiTileActor* GetTileActorAtWorldLocation(const FVector& WorldLocation);
	UFUNCTION(BlueprintCallable)
	void GetGridIndexAtWorldLocation(const FVector& WorldLocation, int& OutX, int& OutY, bool& bIsOnGrid);

	UFUNCTION(BlueprintPure)
	bool IsValidGridIndex(int X, int Y);

	bool IsGridSpaceOccupied(int X, int Y);

	UFUNCTION(BlueprintCallable)
	bool FindTileActor(ARummiTileActor* TileActor, int& OutX, int& OutY);

	void PopulateHandLogicalRepresentation(FRummiTileArray& Hand);
	void PopulateBoardLogicalRepresentation(FRummiBoard& Board);

	void PopulateFromRummiBoard(const FRummiBoard& Board, const TArray<ARummiTileActor*>& TileActors);
	void PopulateFromRummiHand(const FRummiTileArray& Hand, const TArray<ARummiTileActor*>& TileActors);
	void Clear();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	int GridSizeX = 16;
	UPROPERTY(EditAnywhere)
	int GridSizeY = 8;

	UPROPERTY(EditAnywhere)
	float GridSpacingX = 200.0f;
	UPROPERTY(EditAnywhere)
	float GridSpacingY = 200.0f;

	TArray<ARummiTileActor*> GridSpaces;

	FVector ConvertLocalSpaceToWorldSpace(const FVector& LocalSpaceVector) const;
	FVector ConvertWorldSpaceToLocalSpace(const FVector& LocalSpaceVector) const;
	FVector GetLocalSpaceGridTileLocation(int X, int Y) const;
	FVector GetLocalSpaceBaseGridTileLocation() const;
	FVector GetLocalSpaceTopLeftCorner() const;
	float GetGridWidth();
	float GetGridHeight();
	UFUNCTION(BlueprintCallable)
	void DebugDrawGridCoords();
	UFUNCTION(BlueprintCallable)
	void DebugDrawGridLines() { DebugDrawGridLines_Internal(nullptr); }
	UFUNCTION(BlueprintCallable)
	void DebugDrawGridLinesWithColorCoding(const FRummiBoard& Board) { DebugDrawGridLines_Internal(&Board); }
	void DebugDrawGridLines_Internal(const FRummiBoard* Board);

	int GetGridIndicesAsArrayIndex(int X, int Y);
	ARummiTileActor* RemoveTileFromIndex(int Index);
};
