#include "LchMeshPathFollowTask.h"
#include "StateTreeLinker.h"
#include "MassStateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "../RandomMoveSTType.h"


bool FLchMeshPathFollowTask::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(TransformHandle);
    Linker.LinkExternalData(MoveTargetHandle);
    Linker.LinkExternalData(AgentRadiusHandle);
    Linker.LinkExternalData(MovementParamsHandle);
    Linker.LinkExternalData(RandomMoveSTHandle);
    return true;
}

EStateTreeRunStatus FLchMeshPathFollowTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FMassStateTreeExecutionContext& MassContext = static_cast<FMassStateTreeExecutionContext&>(Context);

    const FTransformFragment& TransformFragment = Context.GetExternalData(TransformHandle);
    FMassMoveTargetFragment& MoveTargetFragment = Context.GetExternalData(MoveTargetHandle);
    const FAgentRadiusFragment& AgentRadiusFragment = Context.GetExternalData(AgentRadiusHandle);
    const FMassMovementParameters& MovementParams = Context.GetExternalData(MovementParamsHandle);
    FLchRandomMoveSTFragment& RandomMoveSTFragment = Context.GetExternalData(RandomMoveSTHandle);

    UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(Context.GetWorld()->GetNavigationSystem());
    if (NavSys == nullptr)
    {
        return EStateTreeRunStatus::Failed;
    }

    const FNavAgentProperties NavAgentProperties(AgentRadiusFragment.Radius);
    const FVector AgentLocation = Context.GetExternalData(TransformHandle).GetTransform().GetLocation();

    const ANavigationData* NavData = NavSys->GetNavDataForProps(NavAgentProperties, AgentLocation);
    if (!NavData)
    {
        return EStateTreeRunStatus::Failed;
    }

    FLchMeshPathFollowTaskInstanceData& InstanceData = Context.GetInstanceData<FLchMeshPathFollowTaskInstanceData>(*this);
    FPathFindingQuery Query(nullptr, *NavData, AgentLocation, InstanceData.TargetLocation);
    if (!Query.NavData.IsValid())
    {
        Query.NavData = NavSys->GetNavDataForProps(NavAgentProperties, Query.StartLocation);
    }

    FPathFindingResult Result(ENavigationQueryResult::Error);
    if (Query.NavData.IsValid())
    {
        Result = NavSys->FindPathSync(NavAgentProperties, Query);
    }

    if (!Result.IsSuccessful() || Result.Path->GetPathPoints().Num() <= 1)
    {
        return EStateTreeRunStatus::Failed;
    }

    const TArray<FNavPathPoint>& PathPoints = Result.Path->GetPathPoints();
    const FVector PathEndLocation = Result.Path->GetEndLocation();

    const float BaseSpeed = MovementParams.GenerateDesiredSpeed(InstanceData.MovementStyle, MassContext.GetEntity().Index);
    const float DesiredSpeed = FMath::Min(BaseSpeed * InstanceData.SpeedScale, MovementParams.MaxSpeed);
    const float DistanceToGoal = FVector::Distance(PathEndLocation, AgentLocation);

    MoveTargetFragment.DesiredSpeed.Set(DesiredSpeed);
    MoveTargetFragment.DistanceToGoal = DistanceToGoal;
    MoveTargetFragment.Center = PathEndLocation;
    MoveTargetFragment.Forward = (PathEndLocation - AgentLocation).GetSafeNormal2D();
    MoveTargetFragment.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());

    if (PathPoints.Num() > 1)
    {
        RandomMoveSTFragment.TargetLocation = PathPoints[1].Location;
    }

    if (DistanceToGoal <= AgentRadiusFragment.Radius)
    {
        return EStateTreeRunStatus::Succeeded;
    }

    return EStateTreeRunStatus::Running;
}