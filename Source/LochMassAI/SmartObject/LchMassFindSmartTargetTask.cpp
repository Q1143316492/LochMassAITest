#include "LchMassFindSmartTargetTask.h"
#include "MassAIBehaviorTypes.h"
#include "MassCommonFragments.h"
#include "MassSmartObjectFragments.h"
#include "MassStateTreeExecutionContext.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectRequestTypes.h"
#include "StateTreeLinker.h"


bool FLchMassFindSmartTargetTask::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(TransformHandle);
    Linker.LinkExternalData(SmartObjectSubsystemHandle);
    return true;
}

EStateTreeRunStatus FLchMassFindSmartTargetTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    // 获取当前Agent的位置
    const FVector AgentLocation = Context.GetExternalData(TransformHandle).GetTransform().GetLocation();
    
    // 重置状态
    InstanceData.SmartObjectLocation.Reset();
    InstanceData.bFoundTarget = false;

    // 1. 设置查找请求 - 创建一个搜索范围
    FSmartObjectRequest Request;
    Request.QueryBox = FBox(
        AgentLocation - FVector(InstanceData.SearchRadius),
        AgentLocation + FVector(InstanceData.SearchRadius)
    );
    
    // 2. 设置过滤器（可选）
    // 这里可以添加更多过滤条件，比如特定的标签、行为定义类型等
    Request.Filter.bShouldEvaluateConditions = true;  // 是否评估选择条件
    Request.Filter.bShouldIncludeClaimedSlots = false; // 是否包含已被占用的槽位
    Request.Filter.ClaimPriority = ESmartObjectClaimPriority::Normal;  // 声明优先级
    
    // 如果你想只查找特定类型的SmartObject，可以这样设置：
    // Request.Filter.BehaviorDefinitionClasses = { UMyBehaviorDefinition::StaticClass() };
    
    // 3. 查找智能对象
    TArray<FSmartObjectRequestResult> Results;
    const bool bFoundObjects = SmartObjectSubsystem.FindSmartObjects(
        Request, 
        Results,
        FConstStructView()  // 可以传入用户数据，比如 FConstStructView::Make(FSmartObjectActorUserData(Actor))
    );

    // 4. 如果找到了智能对象
    if (bFoundObjects && Results.Num() > 0)
    {
        // 随机选择一个结果（你也可以根据距离等条件选择最优的）
        const int32 SelectedIndex = FMath::RandRange(0, Results.Num() - 1);
        const FSmartObjectRequestResult& SelectedResult = Results[SelectedIndex];

        // 5. 尝试声明（Claim）这个槽位
        const FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem.MarkSlotAsClaimed(
            SelectedResult.SlotHandle,
            ESmartObjectClaimPriority::Normal
        );

        // 6. 如果成功声明
        if (ClaimHandle.IsValid())
        {
            // 保存声明句柄
            InstanceData.ObjectClaimHandle = ClaimHandle;

            // 7. 获取槽位的位置
            const TOptional<FTransform> SlotTransform = SmartObjectSubsystem.GetSlotTransform(ClaimHandle);
            if (SlotTransform.IsSet())
            {
                // 8. 设置导航目标位置
                InstanceData.SmartObjectLocation.EndOfPathPosition = SlotTransform.GetValue().GetLocation();
                InstanceData.SmartObjectLocation.EndOfPathIntent = EMassMovementAction::Stand;  // 到达后站立
                InstanceData.bFoundTarget = true;

                // 成功！
                return EStateTreeRunStatus::Succeeded;
            }
            else
            {
                // 无法获取位置，释放声明
                SmartObjectSubsystem.MarkSlotAsFree(ClaimHandle);
            }
        }
    }

    // 没有找到合适的智能对象
    return EStateTreeRunStatus::Failed;
}