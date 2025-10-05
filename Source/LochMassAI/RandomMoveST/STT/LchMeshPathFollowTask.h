#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassStateTreeTypes.h"
#include "StateTreeLinker.h"
#include "MassMovementFragments.h"
#include "../RandomMoveSTType.h"
#include "MassNavigationFragments.h"
#include "LchMeshPathFollowTask.generated.h"

USTRUCT(meta = (DisplayName = "Lch Mesh Path Follow"))
struct FLchMeshPathFollowTask : public FMassStateTreeTaskBase
{
    GENERATED_BODY()

protected:
    virtual const UStruct* GetInstanceDataType() const override { return FLchMeshPathFollowTaskInstanceData::StaticStruct(); }

    virtual bool Link(FStateTreeLinker& Linker) override;

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
protected:
    TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
    TStateTreeExternalDataHandle<FMassMoveTargetFragment> MoveTargetHandle;
    TStateTreeExternalDataHandle<FAgentRadiusFragment> AgentRadiusHandle;
    TStateTreeExternalDataHandle<FMassMovementParameters> MovementParamsHandle;
    TStateTreeExternalDataHandle<FLchRandomMoveSTFragment> RandomMoveSTHandle;
};