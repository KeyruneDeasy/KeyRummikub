// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RummiGrid.generated.h"

class ARummiTileActor;

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

	int GetGridSizeX() { return GridSizeX; }
	int GetGridSizeY() { return GridSizeY; }

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

	FVector GetLocalSpaceGridTileLocation(int X, int Y);
	FVector GetLocalSpaceBaseGridTileLocation();
	float GetGridWidth();
	float GetGridHeight();
	UFUNCTION(BlueprintCallable)
	void DebugDrawGrid();

	int GetGridIndicesAsArrayIndex(int X, int Y);
};
