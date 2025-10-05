#pragma once

#include "CoreMinimal.h"
#include "MassVisualizationLODProcessor.h"
#include "MassRepresentationProcessor.h"
#include "LchMassAIVisualizationProcessor.generated.h"

UCLASS()
class ULchMassAIVisualizationProcessor : public UMassVisualizationProcessor
{
    GENERATED_BODY()

public:
    ULchMassAIVisualizationProcessor()
    {
        bAutoRegisterWithProcessingPhases = true;
    }
};