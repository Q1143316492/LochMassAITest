#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassSignalSubsystem.h"
#include "LchFindNewTargetProcessor.generated.h"

UCLASS()
class ULchFindNewTargetProcessor : public UMassProcessor
{
    GENERATED_BODY()

public:
    ULchFindNewTargetProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery EntityQuery;
};