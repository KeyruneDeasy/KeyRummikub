// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "RummiTile.h"
#include "KeyUtil.generated.h"

UCLASS(BlueprintType)
class KEYRUMMIKUB_API UKeyUtil : public UObject
{
	GENERATED_BODY()

public:
	//template<class T>
	//static void ShuffleArray(TArray<FRummiTile>& Array);

	UFUNCTION(BlueprintCallable)
	static FVector TraceToPlane(
		const FVector& StartLocation,
		const FVector& CastDir,
		const FVector& PlanePoint,
		const FVector& PlaneNormal);
};
