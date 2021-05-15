// Fill out your copyright notice in the Description page of Project Settings.


#include "RummiTileActor.h"

ARummiTileActor::ARummiTileActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARummiTileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARummiTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARummiTileActor::OnPlacedIntoGrid_Implementation(const FVector& InGridCellLocation)
{
	GridCellLocation = InGridCellLocation;
}

void ARummiTileActor::RevertToGridCellLocation()
{
	SetActorLocation(GridCellLocation);
}
