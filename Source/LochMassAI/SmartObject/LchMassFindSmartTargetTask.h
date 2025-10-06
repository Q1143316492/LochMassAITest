#pragma once

#include "MassCommonFragments.h"
#include "MassNavigationFragments.h"
#include "MassStateTreeTypes.h"
#include "SmartObjectRuntime.h"
#include "LchMassFindSmartTargetTask.generated.h"

class USmartObjectSubsystem;
struct FSmartObjectClaimHandle;


USTRUCT(BlueprintType)
struct FLchMassFindSmartTargetTaskInstanceData
{
    GENERATED_BODY()

    /** Smart object location to navigate to */
    UPROPERTY(EditAnywhere, Category = Output)
    FMassTargetLocation SmartObjectLocation;

    /** Handle to the claimed smart object slot */
    UPROPERTY(EditAnywhere, Category = Output)
    FSmartObjectClaimHandle ObjectClaimHandle;

    /** Whether we successfully found and claimed a target */
    UPROPERTY(EditAnywhere, Category = Output)
    bool bFoundTarget = false;

    /** Search radius for finding smart objects */
    UPROPERTY(EditAnywhere, Category = Parameter)
    float SearchRadius = 2000.f;
};

USTRUCT(meta = (DisplayName = "Lch Find Smart Target"))
struct FLchMassFindSmartTargetTask : public FMassStateTreeTaskBase
{
    GENERATED_BODY()

    using FInstanceDataType = FLchMassFindSmartTargetTaskInstanceData;

    // Begin FMassStateTreeTaskBase interface
    virtual bool Link(FStateTreeLinker& Linker) override;

    virtual const UStruct* GetInstanceDataType() const override { return FLchMassFindSmartTargetTaskInstanceData::StaticStruct(); }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

    TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;

    TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;
};