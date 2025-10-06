#pragma once

#include "MassEntityTypes.h"
#include "MassComponentHitTypes.h"
#include "MassSubsystemBase.h"
#include "LchMassTestSubsystem.generated.h"

class UMassSignalSubsystem;
class UMassAgentSubsystem;

UCLASS(MinimalAPI)
class ULchMassTestSubsystem : public UMassTickableSubsystemBase
{
    GENERATED_BODY()

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    UPROPERTY()
    TObjectPtr<UMassSignalSubsystem> SignalSubsystem;

    UPROPERTY()
    TObjectPtr<UMassAgentSubsystem> AgentSubsystem;
};