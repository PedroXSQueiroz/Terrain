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

	FMeshTriangleData(): 
		Edges(TArray<int>()),
		VertexIndexes(TArray<int>()),
		TrianglesVertexIndexes(TArray<int>()),
		Vertex(TArray<FVector>())
	{}

	UPROPERTY()
	TArray<int> Edges;

	UPROPERTY()
	TArray<int> VertexIndexes;

	UPROPERTY()
	TArray<int> TrianglesVertexIndexes;
	
	UPROPERTY()
	TArray<FVector> Vertex;
};

USTRUCT()
struct G_RADIO_V1_API FMeshData 
{

	GENERATED_BODY()	

public:

	FMeshData() :Vertex(TArray<FVector>()), Triangles(TArray<int>()) {}
	
	TArray<int> Triangles;
	
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
	virtual FMeshData Build(AActor* owner) PURE_VIRTUAL(TEXT("NOT IMPLEMENTED YET"), return FMeshData(););

};