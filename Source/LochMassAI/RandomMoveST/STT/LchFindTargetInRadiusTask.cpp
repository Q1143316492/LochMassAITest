#include "LchFindTargetInRadiusTask.h"
#include "../RandomMoveSTType.h"
#include "MassStateTreeExecutionContext.h"
#include "NavigationSystem.h"


EStateTreeRunStatus FLchFindTargetInRadiusTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(Context.GetWorld()->GetNavigationSystem());
    if (NavSys == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation System not found"));
        return EStateTreeRunStatus::Failed;
    }

    const FTransformFragment& TransformFragment = Context.GetExternalData(TransformHandle);
    const FVector CurrentLocation = TransformFragment.GetTransform().GetLocation();

    FNavLocation RandomPoint;
    const bool bFound = NavSys->GetRandomPointInNavigableRadius(CurrentLocation, Radius, RandomPoint);
    if (!bFound)
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid random point found in radius"));
        return EStateTreeRunStatus::Failed;
    }

    FLchFindTargetInRadiusInstanceData& InstanceData = Context.GetInstanceData<FLchFindTargetInRadiusInstanceData>(*this);
    InstanceData.MoveTargetLocation = RandomPoint.Location;

    return EStateTreeRunStatus::Running;
}