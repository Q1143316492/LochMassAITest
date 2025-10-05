#include "LchFindNewTargetProcessor.h"
#include "RandomMoveSTType.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassExecutionContext.h"
#include "MassMovementFragments.h"
#include "MassSignalSubsystem.h"
#include "MassStateTreeTypes.h"
#include "LchCustomSignalProcessor.h"


ULchFindNewTargetProcessor::ULchFindNewTargetProcessor()
    : EntityQuery(*this)
{
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
}

void ULchFindNewTargetProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    EntityQuery.AddRequirement<FLchRandomMoveSTFragment>(EMassFragmentAccess::ReadOnly);
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
    EntityQuery.AddSubsystemRequirement<UMassSignalSubsystem>(EMassFragmentAccess::ReadWrite);
    EntityQuery.RegisterWithProcessor(*this);
}

void ULchFindNewTargetProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
    {
        const TArrayView<const FLchRandomMoveSTFragment> RandomMoveFragments = Context.GetFragmentView<FLchRandomMoveSTFragment>();
        const TArrayView<const FTransformFragment> TransformFragments = Context.GetFragmentView<FTransformFragment>();

        UMassSignalSubsystem& SignalSubsystem = Context.GetMutableSubsystemChecked<UMassSignalSubsystem>();
        TArray<FMassEntityHandle> EntitiesToSignal;

        const int32 NumEntities = Context.GetNumEntities();
        for (int32 i = 0; i < NumEntities; ++i)
        {
            const FLchRandomMoveSTFragment& RandomMoveFragment = RandomMoveFragments[i];
            const FTransformFragment& TransformFragment = TransformFragments[i];

            const FVector CurrentLocation = TransformFragment.GetTransform().GetLocation();

            const float DistanceToTarget = FVector::Dist(CurrentLocation, RandomMoveFragment.TargetLocation);
            if (DistanceToTarget < 100.0f)
            {
                EntitiesToSignal.Add(Context.GetEntity(i));
            }
        }

        if (!EntitiesToSignal.IsEmpty())
        {
            SignalSubsystem.SignalEntities(UE::Mass::Signals::NewStateTreeTaskRequired, EntitiesToSignal);
            SignalSubsystem.SignalEntities(UE::Mass::Signals::LchCustomSignalTest, EntitiesToSignal);
        }
    });
}