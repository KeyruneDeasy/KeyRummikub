// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RummiTile.h"
#include "RummiTileActor.h"
#include "RummiGrid.h"
#include "KeyRummikubGameState.generated.h"

UCLASS()
class KEYRUMMIKUB_API AKeyRummikubGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AKeyRummikubGameState();

	virtual void BeginPlay();

	void InitializeTable();
	UFUNCTION(BlueprintPure)
	ARummiTileActor* GetActorFromTileInfo(const FRummiTile& TileInfo);
	UFUNCTION(BlueprintPure)
	ARummiTileActor* GetActorFromTileId(int TileId);
	
	FRummiRuleset Ruleset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FColor> Colors;

	UPROPERTY(BlueprintReadOnly)
	FRummiTable Table;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARummiTileActor> TileActorClass;

	TArray<ARummiTileActor*> TileActors;

	UPROPERTY(BlueprintReadOnly)
	ARummiGrid* BoardGrid;
	UPROPERTY(BlueprintReadOnly)
	ARummiGrid* HandGrid;

private:
	void CreateTileActors(const FRummiTileArray& Tiles);
	UFUNCTION(BlueprintCallable)
	void DealRandomCardsFromDeckOntoBoard();
};
