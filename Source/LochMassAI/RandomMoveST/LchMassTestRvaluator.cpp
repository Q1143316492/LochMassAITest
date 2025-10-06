#include "LchMassTestRvaluator.h"
#include "StateTreeLinker.h"
#include "MassStateTreeExecutionContext.h"
#include "MassStateTreeDependency.h"

UE_DISABLE_OPTIMIZATION

bool FLchMassTestEvaluator::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(TestSubsystemHandle);
    Linker.LinkExternalData(RandomMoveFragmentHandle);
    return true;
}

void FLchMassTestEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    const ULchMassTestSubsystem& TestSubsystem = Context.GetExternalData(TestSubsystemHandle);
    const FLchRandomMoveSTFragment& RandomMoveFragment = Context.GetExternalData(RandomMoveFragmentHandle);
}

void FLchMassTestEvaluator::GetDependencies(UE::MassBehavior::FStateTreeDependencyBuilder& Builder) const
{
    Builder.AddReadOnly(TestSubsystemHandle);
    Builder.AddReadOnly(RandomMoveFragmentHandle);
}

UE_ENABLE_OPTIMIZATION
