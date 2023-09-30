// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DensitiesMapFactory.h"

#include "DensitiesMapFactoryFileReader.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class G_RADIO_V1_API UDensitiesMapFactoryFileReader : public UObject, public IDensitiesMapFactory
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SourcePath;

	virtual TMap<FDensityIndex, float> Build(UMarshingCubesConfig* config) override;
	
};
