// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Systems/MeshFactory/MeshFactoryMarshingCubes.h"

#include "DensitiesMapFactory.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDensitiesMapFactory : public UInterface
{
	GENERATED_BODY()
};

class G_RADIO_V1_API IDensitiesMapFactory 
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual TMap<FDensityIndex, float> Build(UMarshingCubesConfig* config) PURE_VIRTUAL(TEXT("NOT IMPLEMENTED YET"), return TMap<FDensityIndex, float>(); );
};
