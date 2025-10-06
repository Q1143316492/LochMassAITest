#include "LchMassTestSubsystem.h"
#include "MassAgentComponent.h"
#include "MassAgentSubsystem.h"
#include "MassSignalSubsystem.h"
#include "MassSimulationSubsystem.h"

UE_DISABLE_OPTIMIZATION
void ULchMassTestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    SignalSubsystem = Collection.InitializeDependency<UMassSignalSubsystem>();
    AgentSubsystem = Collection.InitializeDependency<UMassAgentSubsystem>();
}

void ULchMassTestSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void ULchMassTestSubsystem::Tick(float DeltaTime)
{
}

TStatId ULchMassTestSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ULchMassTestSubsystem, STATGROUP_Tickables);
}

UE_ENABLE_OPTIMIZATION
