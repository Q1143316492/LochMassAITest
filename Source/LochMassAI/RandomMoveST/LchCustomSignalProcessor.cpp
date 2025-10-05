#include "LchCustomSignalProcessor.h"
#include "MassSignalSubsystem.h"
#include "MassExecutionContext.h"
#include "RandomMoveSTType.h"



ULchCustomSignalProcessor::ULchCustomSignalProcessor(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	bRequiresGameThreadExecution = true;
}

void ULchCustomSignalProcessor::InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager)
{
	Super::InitializeInternal(Owner, InEntityManager);

	UMassSignalSubsystem* SignalSubsystem = UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
	SubscribeToSignal(*SignalSubsystem, UE::Mass::Signals::LchCustomSignalTest);
	
	EntityQuery.AddRequirement<FLchRandomMoveSTFragment>(EMassFragmentAccess::ReadOnly);
	ProcessorRequirements.AddSubsystemRequirement<UMassSignalSubsystem>(EMassFragmentAccess::ReadWrite);
}

void ULchCustomSignalProcessor::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals)
{
	EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		// UE_LOG(LogTemp, Warning, TEXT("LchCustomSignalProcessor received signal for %d entities"), NumEntities);
	});
}
