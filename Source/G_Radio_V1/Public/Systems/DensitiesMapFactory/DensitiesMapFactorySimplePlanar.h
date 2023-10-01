// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DensitiesMapFactory.h"

#include "DensitiesMapFactorySimplePlanar.generated.h"

/**
 * 
 */
UCLASS()
class G_RADIO_V1_API UDensitiesMapFactorySimplePlanar : public UObject, public IDensitiesMapFactory
{
	GENERATED_BODY()

public:

	virtual TMap<FXYZIndex, float> Build(UMarshingCubesConfig* config);
	
};
