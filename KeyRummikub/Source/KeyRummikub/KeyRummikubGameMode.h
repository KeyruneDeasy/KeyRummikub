// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RummiTile.h"
#include "KeyRummikubGameMode.generated.h"


UCLASS()
class KEYRUMMIKUB_API AKeyRummikubGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AKeyRummikubGameMode();
	
	virtual void BeginPlay() override;
	virtual void InitGameState();

	UPROPERTY(BlueprintReadOnly)
	class AKeyRummikubGameState* RummiGameState;

private:
	void DealOpeningHand();
	void DealOpeningHand_TimerStep();
	void DealOpeningHand_SingleTilePerPlayer();

	FTimerHandle Timer;
	int NumTilesRemainingToDeal = 0;
};
