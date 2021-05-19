// Copyright Ciaran Deasy 2021

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RummiAi.generated.h"

UCLASS()
class KEYRUMMIKUB_API URummiAi : public UObject
{
	GENERATED_BODY()
	
public:
	// Returns true if it is ready to end turn.
	virtual bool Update(float DeltaTime);
};
