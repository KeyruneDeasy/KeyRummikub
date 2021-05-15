// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyUtil.h"

//template<class T>
/*void KeyUtil::ShuffleArray(TArray<FRummiTile>& Array)
{
    if (Array.Num() > 0)
    {
        int32 LastIndex = Array.Num() - 1;
        for (int32 i = 0; i <= LastIndex; ++i)
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i != Index)
            {
                Array.Swap(i, Index);
            }
        }
    }
}*/

FVector UKeyUtil::TraceToPlane(
    const FVector& StartPoint, 
    const FVector& CastDir, 
    const FVector& PlanePoint, 
    const FVector& PlaneNormal)
{
    FVector StartToPlane = PlanePoint - StartPoint;
    float DistanceToPlane = FVector::DotProduct(StartToPlane, PlaneNormal);
    float ParallelCastSpeed = FVector::DotProduct(CastDir, PlaneNormal);
    if (ParallelCastSpeed == 0.0f)
    {
        return FVector::ZeroVector; // Cast is parallel to plane.
    }
    float CastScale = DistanceToPlane / ParallelCastSpeed;
    return StartPoint + (CastScale * CastDir);
}
