#include "LchBlendTransformProcessor.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassExecutionContext.h"
#include "MassMovementFragments.h"
#include "MassSignalSubsystem.h"
#include "RandomMoveType.h"


ULchBlendTransformProcessor::ULchBlendTransformProcessor()
    : EntityQuery(*this)
{
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
}

void ULchBlendTransformProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    EntityQuery.AddRequirement<FLchBlendTransformFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FLchTransformBaseFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.RegisterWithProcessor(*this);
}


void ULchBlendTransformProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) 
{
    EntityQuery.ForEachEntityChunk(Context, [&](FMassExecutionContext& Context)
    {
        const int32 NumEntities = Context.GetNumEntities();

        TArrayView<FLchBlendTransformFragment> BlendTransformList = Context.GetMutableFragmentView<FLchBlendTransformFragment>();
        TArrayView<FLchTransformBaseFragment> TransformBaseList = Context.GetMutableFragmentView<FLchTransformBaseFragment>();
        TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();

        for (int32 i = 0; i < NumEntities; ++i)
        {
            FLchBlendTransformFragment& BlendTransform = BlendTransformList[i];
            FLchTransformBaseFragment& TransformBase = TransformBaseList[i];
            FTransformFragment& Transform = TransformList[i];

            if (BlendTransform.CurrentTime < BlendTransform.MaxTime)
            {
                // 正常的混合插值
                BlendTransform.CurrentTime += Context.GetDeltaTimeSeconds();
                float Alpha = FMath::Clamp(BlendTransform.CurrentTime / BlendTransform.MaxTime, 0.f, 1.f);
                FTransform NewTransform = FTransform::Identity;
                NewTransform.Blend(BlendTransform.StartTransform, BlendTransform.EndTransform, Alpha);
                TransformBase.BaseTransform = NewTransform;
                Transform.SetTransform(NewTransform);
            }
            else
            {
                // 到达终点后，交换起点和终点，生成新的随机目标位置，实现循环往复运动
                FTransform OldStart = BlendTransform.StartTransform;
                FTransform OldEnd = BlendTransform.EndTransform;
                
                // 当前位置作为新的起点
                BlendTransform.StartTransform = OldEnd;
                
                // 生成随机的新终点（在当前位置附近的随机偏移）
                FVector RandomOffset = FVector(
                    FMath::RandRange(-500.f, 500.f),  // X 轴随机偏移
                    FMath::RandRange(-500.f, 500.f),  // Y 轴随机偏移
                    FMath::RandRange(0.f, 100.f)   // Z 轴随机偏移（较小的垂直变化）
                );
                
                FTransform NewEndTransform = OldEnd;
                NewEndTransform.AddToTranslation(RandomOffset);
                BlendTransform.EndTransform = NewEndTransform;
                
                // 重置时间，开始新的循环
                BlendTransform.CurrentTime = 0.f;
                
                // 可选：随机化每次循环的持续时间
                BlendTransform.MaxTime = FMath::RandRange(2.f, 5.f);
            }
        }
    });
    
}