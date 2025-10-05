#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "LchBlendTransformProcessor.generated.h"

UCLASS()
class LOCHMASSAI_API ULchBlendTransformProcessor : public UMassProcessor
{
    GENERATED_BODY()

public:
    ULchBlendTransformProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery EntityQuery;
};
