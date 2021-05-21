// Copyright Ciaran Deasy 2021

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RummiTile.h"
#include "RummiAi.generated.h"

class AKeyRummikubGameState;

UCLASS()
class KEYRUMMIKUB_API URummiAi : public UObject
{
	GENERATED_BODY()
	
public:
	// Returns true if it is ready to end turn.
	virtual bool Update(float DeltaTime, AKeyRummikubGameState* GameState);

	void DetermineTargetBoardState(AKeyRummikubGameState* GameState);
	void SnapToTargetBoardState(AKeyRummikubGameState* GameState);

	FRummiBoard TargetBoardState;
	FRummiTileArray TilesToPlayFromHand;
};
