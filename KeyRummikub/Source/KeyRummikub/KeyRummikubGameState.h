// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RummiTile.h"
#include "KeyRummikubGameState.generated.h"

/**
 * 
 */
UCLASS()
class KEYRUMMIKUB_API AKeyRummikubGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void InitializeTable();
	
	FRummiRuleset Ruleset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FColor> Colors;

	UPROPERTY(BlueprintReadOnly)
	FRummiTable Table;
};
