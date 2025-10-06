#pragma once

#include "MassComponentHitSubsystem.h"
#include "MassStateTreeTypes.h"
#include "RandomMoveSTType.h"
#include "LchMassTestSubsystem.h"
#include "LchMassTestRvaluator.generated.h"

namespace UE::MassBehavior
{
    struct FStateTreeDependencyBuilder;
}

USTRUCT(BlueprintType)
struct FLchMassTestEvaluatorInstanceData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, Category = "Output")
    bool bGotHit = false;

    UPROPERTY(VisibleAnywhere, Category = "Output")
    FMassEntityHandle HitEntity;
};

USTRUCT(meta = (DisplayName = "Lch Mass Test Evaluator"))
struct FLchMassTestEvaluator : public FMassStateTreeEvaluatorBase
{
    GENERATED_BODY()

protected:
    virtual bool Link(FStateTreeLinker& Linker) override;

    virtual const UStruct* GetInstanceDataType() const override { return FLchMassTestEvaluatorInstanceData::StaticStruct(); }

    virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

    virtual void GetDependencies(UE::MassBehavior::FStateTreeDependencyBuilder& Builder) const override;

    TStateTreeExternalDataHandle<ULchMassTestSubsystem> TestSubsystemHandle;

    TStateTreeExternalDataHandle<FLchRandomMoveSTFragment> RandomMoveFragmentHandle;
};
