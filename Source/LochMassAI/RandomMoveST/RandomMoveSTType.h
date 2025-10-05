#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassMovementTypes.h"
#include "MassCommonFragments.h"
#include "MassNavigationTypes.h"
#include "RandomMoveSTType.generated.h"


USTRUCT(BlueprintType)
struct FLchRandomMoveSTFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    FVector TargetLocation;
};

USTRUCT(BlueprintType)
struct FLchMeshPathFollowTaskInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    FMassMovementStyleRef MovementStyle;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    float SpeedScale = 1.0f;
};
