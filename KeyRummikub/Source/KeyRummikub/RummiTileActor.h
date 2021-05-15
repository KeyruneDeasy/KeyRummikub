// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RummiTile.h"
#include "RummiTileActor.generated.h"

UCLASS()
class KEYRUMMIKUB_API ARummiTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARummiTileActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FRummiTile TileInfo;

	UFUNCTION(BlueprintNativeEvent)
	void OnPlacedIntoGrid(const FVector& InGridCellLocation);
	void OnPlacedIntoGrid_Implementation(const FVector& InGridCellLocation);

	UFUNCTION(BlueprintNativeEvent)
	void OnRemovedFromGrid();
	void OnRemovedFromGrid_Implementation() {}

	UFUNCTION(BlueprintCallable)
	void RevertToGridCellLocation();

protected:
	virtual void BeginPlay() override;

	FVector GridCellLocation;
};
