#pragma once

#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "LchCustomSignalProcessor.generated.h"

namespace UE::Mass::Signals
{
	const FName LchCustomSignalTest = FName(TEXT("LchCustomSignalTest"));
};

UCLASS()
class ULchCustomSignalProcessor : public UMassSignalProcessorBase
{
	GENERATED_BODY()

public:
	ULchCustomSignalProcessor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager) override;
	virtual void SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals) override;
};