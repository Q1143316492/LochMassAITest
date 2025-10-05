#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassMovementTypes.h"
#include "MassCommonFragments.h"
#include "MassNavigationTypes.h"
#include "RandomMoveType.generated.h"


USTRUCT(BlueprintType)
struct FLchRandomMoveFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    FMassTargetLocation TargetPos;
};

USTRUCT(BlueprintType)
struct FLchTransformBaseFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    FTransform BaseTransform;
};

USTRUCT(BlueprintType)
struct FLchBlendTransformFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FTransform StartTransform;

    UPROPERTY(EditAnywhere)
    FTransform EndTransform;

    UPROPERTY(EditAnywhere)
    float MaxTime = 1.f;

    UPROPERTY(EditAnywhere)
    float CurrentTime = 0.f;
};
