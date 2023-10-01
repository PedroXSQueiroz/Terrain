// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "MeshFactory.generated.h"

USTRUCT()
struct G_RADIO_V1_API FMeshTriangleData
{
	GENERATED_BODY()

public:

	TArray<int> Edges;

	TArray<int> VertexIndexes;

	TArray<int> TrianglesVertexIndexes;
	
	TArray<FVector> Vertex;

};

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UMeshFactory : public UInterface
{
	GENERATED_BODY()
};

class G_RADIO_V1_API IMeshFactory{
	
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual TArray<FMeshTriangleData> Build(AActor* owner) PURE_VIRTUAL(TEXT("NOT IMPLEMENTED YET"), return TArray<FMeshTriangleData>(););

};