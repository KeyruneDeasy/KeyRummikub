// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RummiTile.h"
#include "RummiTileActor.h"
#include "RummiGrid.h"
#include "KeyRummikubGameState.generated.h"

class URummiAi;

USTRUCT()
struct KEYRUMMIKUB_API FRummiPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	URummiAi* Ai = nullptr;
};

UCLASS()
class KEYRUMMIKUB_API AKeyRummikubGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AKeyRummikubGameState();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	void InitializeTable();
	UFUNCTION(BlueprintPure)
	ARummiTileActor* GetActorFromTileInfo(const FRummiTile& TileInfo);
	UFUNCTION(BlueprintPure)
	ARummiTileActor* GetActorFromTileId(int TileId);


	UFUNCTION(BlueprintCallable)
	void DrawCardFromDeckToHand(int HandIndex);

	UFUNCTION(BlueprintCallable)
	bool TryMoveTileToWorldLocation(ARummiTileActor* Tile, const FVector& WorldLocation);

	UFUNCTION(BlueprintPure)
	int GetNumRummiPlayers() { return PlayerInfos.Num(); }

	UFUNCTION(BlueprintCallable)
	void UpdateGridsFromLogicalRepresentation();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
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

	int DisplayedHandIndex = 0;

	int CurrentTurnPlayerIndex = 0;

	UPROPERTY(Transient)
	TArray<FRummiPlayerInfo> PlayerInfos;

private:
	void CreateTileActors(const FRummiTileArray& Tiles);
	UFUNCTION(BlueprintCallable)
	void DealRandomCardsFromDeckOntoBoard();
	UFUNCTION(BlueprintCallable)
	void FindTileActorInGrids(ARummiTileActor* Tile, ARummiGrid*& OutGrid, int& OutX, int& OutY);
	bool TryMoveTile(ARummiTileActor* Tile, ARummiGrid* OldGrid, int OldX, int OldY, ARummiGrid* NewGrid, int NewX, int NewY);

	void UpdateLogicalRepresentationFromGrids();

	UFUNCTION(BlueprintCallable)
	void EndPlayerTurn();
	void EndTurn();
	void RunAiTurns();
};
