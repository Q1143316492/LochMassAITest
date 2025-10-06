# UE 5.6 智能对象（Smart Object）使用指南

## 概述
这个示例展示了如何在 Unreal Engine 5.6 中使用 Mass AI 框架查找和声明智能对象。

## 完整流程

### 1. 包含必要的头文件
```cpp
#include "SmartObjectSubsystem.h"         // 智能对象子系统
#include "SmartObjectRequestTypes.h"      // 请求类型定义
#include "SmartObjectRuntime.h"           // 运行时类型
#include "MassCommonFragments.h"          // Mass 基础片段
#include "MassNavigationFragments.h"      // Mass 导航片段
```

### 2. 创建查找请求（Request）
```cpp
// 获取当前位置
const FVector AgentLocation = GetCurrentLocation();

// 创建请求
FSmartObjectRequest Request;

// 设置搜索范围（一个包围盒）
Request.QueryBox = FBox(
    AgentLocation - FVector(SearchRadius),  // 最小点
    AgentLocation + FVector(SearchRadius)   // 最大点
);
```

### 3. 配置过滤器（Filter）
```cpp
// 基本设置
Request.Filter.bShouldEvaluateConditions = true;      // 是否评估选择条件
Request.Filter.bShouldIncludeClaimedSlots = false;   // 是否包含已占用的槽位
Request.Filter.bShouldIncludeDisabledSlots = false;  // 是否包含被禁用的槽位
Request.Filter.ClaimPriority = ESmartObjectClaimPriority::Normal;

// 可选：按用户标签过滤
Request.Filter.UserTags.AddTag(FGameplayTag::RequestGameplayTag(FName("MyTag")));

// 可选：按活动要求过滤
Request.Filter.ActivityRequirements = FGameplayTagQuery::MakeQuery_MatchTag(
    FGameplayTag::RequestGameplayTag(FName("Activity.Sit"))
);

// 可选：限制行为定义类型
Request.Filter.BehaviorDefinitionClasses = { 
    UMyBehaviorDefinition::StaticClass() 
};

// 可选：自定义谓词过滤
Request.Filter.Predicate = [](FSmartObjectHandle Handle) -> bool {
    // 返回 true 表示接受这个智能对象
    return true;
};
```

### 4. 执行查找
```cpp
TArray<FSmartObjectRequestResult> Results;

// 方法1：不带用户数据
bool bFound = SmartObjectSubsystem.FindSmartObjects(
    Request, 
    Results,
    FConstStructView()
);

// 方法2：带Actor用户数据
bool bFound = SmartObjectSubsystem.FindSmartObjects(
    Request, 
    Results,
    FConstStructView::Make(FSmartObjectActorUserData(MyActor))
);

// 方法3：只查找第一个
FSmartObjectRequestResult FirstResult = SmartObjectSubsystem.FindSmartObject(
    Request,
    FConstStructView()
);
```

### 5. 选择结果
```cpp
if (bFound && Results.Num() > 0)
{
    // 随机选择
    const int32 Index = FMath::RandRange(0, Results.Num() - 1);
    const FSmartObjectRequestResult& Selected = Results[Index];
    
    // 或者按距离选择最近的
    FSmartObjectRequestResult* Closest = nullptr;
    float MinDistance = MAX_FLT;
    for (auto& Result : Results)
    {
        TOptional<FTransform> Transform = SmartObjectSubsystem.GetSlotTransform(Result.SlotHandle);
        if (Transform.IsSet())
        {
            float Dist = FVector::Distance(AgentLocation, Transform.GetValue().GetLocation());
            if (Dist < MinDistance)
            {
                MinDistance = Dist;
                Closest = &Result;
            }
        }
    }
}
```

### 6. 声明槽位（Claim）
```cpp
// 声明槽位
const FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem.MarkSlotAsClaimed(
    SelectedResult.SlotHandle,
    ESmartObjectClaimPriority::Normal  // 声明优先级
);

// 检查是否成功
if (ClaimHandle.IsValid())
{
    // 成功声明
}

// 带用户数据的声明
const FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem.Claim(
    SelectedResult.SlotHandle,
    FConstStructView::Make(FSmartObjectActorUserData(MyActor))
);
```

### 7. 获取槽位信息
```cpp
// 获取槽位位置
TOptional<FTransform> Transform = SmartObjectSubsystem.GetSlotTransform(ClaimHandle);
if (Transform.IsSet())
{
    FVector Location = Transform.GetValue().GetLocation();
    FRotator Rotation = Transform.GetValue().Rotator();
}

// 获取槽位标签
const FGameplayTagContainer& Tags = SmartObjectSubsystem.GetSlotTags(ClaimHandle.SlotHandle);

// 检查槽位状态
bool bIsValid = SmartObjectSubsystem.IsClaimedSmartObjectValid(ClaimHandle);
```

### 8. 使用槽位
```cpp
// 标记为正在使用
const USmartObjectBehaviorDefinition* Behavior = SmartObjectSubsystem.MarkSlotAsOccupied(
    ClaimHandle,
    UMyBehaviorDefinition::StaticClass()
);

if (Behavior)
{
    // 开始使用智能对象的行为
}
```

### 9. 释放槽位
```cpp
// 使用完毕后释放
bool bReleased = SmartObjectSubsystem.MarkSlotAsFree(ClaimHandle);

// 或者使用旧API（已弃用但仍可用）
SmartObjectSubsystem.Release(ClaimHandle);
```

## 完整的 Mass StateTree Task 示例

参见 `LchMassFindSmartTargetTask.cpp` 中的完整实现。

### 关键点：
1. **Link**: 链接外部数据句柄（Transform 和 SmartObjectSubsystem）
2. **EnterState**: 在进入状态时执行查找和声明逻辑
3. **ExitState** (可选): 在退出时释放声明

### 实例数据结构：
```cpp
USTRUCT()
struct FMyTaskInstanceData
{
    GENERATED_BODY()
    
    // 导航目标
    UPROPERTY()
    FMassTargetLocation TargetLocation;
    
    // 声明句柄
    UPROPERTY()
    FSmartObjectClaimHandle ClaimHandle;
    
    // 是否找到
    UPROPERTY()
    bool bFoundTarget = false;
    
    // 搜索半径（参数）
    UPROPERTY(EditAnywhere)
    float SearchRadius = 2000.f;
};
```

## 声明优先级

```cpp
enum class ESmartObjectClaimPriority : uint8
{
    None,         // 无效
    Low,          // 低优先级
    BelowNormal,  // 略低
    Normal,       // 正常（默认）
    AboveNormal,  // 略高
    High          // 高优先级
};
```

高优先级可以抢占低优先级的声明（如果槽位还没被占用）。

## 常见问题

### Q: 如何找到特定类型的智能对象？
A: 使用 `Filter.BehaviorDefinitionClasses` 来限制：
```cpp
Request.Filter.BehaviorDefinitionClasses = { 
    USitBehaviorDefinition::StaticClass(),
    UStandBehaviorDefinition::StaticClass()
};
```

### Q: 如何按标签过滤？
A: 使用 `ActivityRequirements`:
```cpp
Request.Filter.ActivityRequirements = FGameplayTagQuery::MakeQuery_MatchAllTags(
    FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Action.Sit")))
);
```

### Q: 如何在 Mass 中集成？
A: 在 StateTree Task 中：
1. 链接 `TransformFragment` 获取位置
2. 链接 `SmartObjectSubsystem` 执行查找
3. 将结果存储在 `FMassTargetLocation` 中
4. 返回 `Succeeded` 让导航系统处理

### Q: 如何调试？
A: 使用控制台命令：
```
ai.debug.smartobject 1      // 显示智能对象调试信息
ai.debug.smartobject.draw 1 // 绘制智能对象
```

## 性能注意事项

1. **搜索范围**: 不要使用过大的搜索半径（建议 < 5000 单位）
2. **查找频率**: 不要每帧都查找，使用冷却时间
3. **结果缓存**: 考虑缓存查找结果
4. **过滤器**: 添加合适的过滤条件减少无效结果

## 参考资源

- UE 文档：Smart Objects
- 源码：`Engine/Plugins/Runtime/SmartObjects/`
- 示例：Content Examples 项目

## 更新日志

- **2025-10-07**: 创建初始版本，适配 UE 5.6
