// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DensitiesMapFactory.h"

#include "DensitiesMapFactoryNoise.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class G_RADIO_V1_API UDensitiesMapFactoryNoise : public UObject, public IDensitiesMapFactory
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Amplitude = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NoiseScale;

	virtual TMap<FDensityIndex, float> Build(UMarshingCubesConfig* config) override;

private:

	void SetDefaultIfNotPresent(FDensityIndex index, float defaultValue, TMap<FDensityIndex, float>& destiny);
	
};
