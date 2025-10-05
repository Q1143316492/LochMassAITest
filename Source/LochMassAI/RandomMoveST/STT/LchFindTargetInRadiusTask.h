#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassStateTreeTypes.h"
#include "StateTreeLinker.h"
#include "MassNavigationTypes.h"
#include "MassMovementTypes.h"
#include "LchFindTargetInRadiusTask.generated.h"

USTRUCT()
struct FLchFindTargetInRadiusInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Output")
    FVector MoveTargetLocation = FVector::ZeroVector;
};

USTRUCT(meta = (DisplayName = "Lch Find Target In Radius"))
struct FLchFindTargetInRadiusTask : public FMassStateTreeTaskBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float Radius = 1000.f;

protected:
    virtual bool Link(FStateTreeLinker& Linker) override
    {
        Linker.LinkExternalData(TransformHandle);
        return true;
    }

    virtual const UStruct* GetInstanceDataType() const override { return FLchFindTargetInRadiusInstanceData::StaticStruct(); }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

    TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
};