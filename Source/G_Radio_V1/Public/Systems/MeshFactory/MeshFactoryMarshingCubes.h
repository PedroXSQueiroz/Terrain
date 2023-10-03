// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "MeshFactory.h"

#include "MeshFactoryMarshingCubes.generated.h"

USTRUCT(BlueprintType)
struct FXYZIndex
{
	GENERATED_BODY()

public:

	FXYZIndex() {}

	FXYZIndex(int x, int y, int z) :X(x), Y(y), Z(z) {}

	FXYZIndex(FVector param) : X(param.X),Y(param.Y),Z(param.Z) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int X;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Y;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Z;

	bool operator==(const FXYZIndex& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FXYZIndex& Other) const
	{
		return	this->X == Other.X
			&&	this->Y == Other.Y
			&&	this->Z == Other.Z;
	}

	FVector ToVector(float scale = 1) 
	{
		return FVector(this->X, this->Y, this->Z) * scale;
	}

	FString ToString() 
	{
		return FString::Printf( TEXT( "%i,%i,%i"), this->X, this->Y, this->Z);
	}
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FHashMeIfYouCan& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FXYZIndex& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FXYZIndex));
	return Hash;
}
#endif

USTRUCT(BlueprintType)
struct FCubeMeshData 
{
	
	GENERATED_BODY()

public:
	
	FCubeMeshData() {}
	
	FCubeMeshData(TArray<FMeshTriangleData> triangles) : Triangles(triangles) {}

	TArray<FMeshTriangleData> GetTriangles() 
	{
		return this->Triangles;
	}

	int TotalVertex() 
	{
		int total = 0;
		for (FMeshTriangleData triangle : this->Triangles) 
		{
			total += triangle.Vertex.Num();
		}

		return total;
	}

protected:
	
	TArray<FMeshTriangleData> Triangles;
};

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class UMarshingCubesConfig : public UObject 
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float IsoLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartXCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartYCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartZCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EndXCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EndYCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EndZCubes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugVertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugVerticesData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	FVector DebugVerticesDataOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugEdges;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugEdgesData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugDensities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool DebugData;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class G_RADIO_V1_API UMeshFactoryMarshingCubes : public UObject, public IMeshFactory
{
	GENERATED_BODY()

public:

	UMeshFactoryMarshingCubes();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	TSubclassOf<UMarshingCubesConfig> ConfigType;

	UPROPERTY()
	UMarshingCubesConfig* Config;

	virtual FMeshData Build(AActor* owner) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL", meta = (MustImplement = "DensitiesMapFactory"))
	TSubclassOf<UObject> DensitiesMapFactoryType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL", meta = (MustImplement = "DensitiesMapFactory"))
	bool RemoveOverlapedVertex;

private:

	TArray<FMeshTriangleData> GetTrianglesOfCube(
			const int  triTable[256][16]
		,	int  edgeConnections[12][2]
		,	FVector  cornerOffsets[8]
		,	int xIndex
		,	int yIndex
		,	int zIndex
		,	TMap<FXYZIndex, float> densities
		,	AActor* owner);
	
	FVector InteroplateEdge(
			FVector firstVertex
		,	float	firstVertexValue
		,	FVector secondVertex
		,	float	secondVertexValue
	);
	

	float GetDensity(FXYZIndex index, TMap<FXYZIndex, float> densities);

	int BuildTriangleSequentially(int indexesOffset, FMeshTriangleData& triangleData);
	
	int BuildTriangleWithUniqueVertex(
				FXYZIndex index
			,	FMeshTriangleData& trianglesData
			,	int vertexIndexOffset
			,	TMap<FXYZIndex,	FCubeMeshData> cubes
			,	int adjascentsEdges[12][3]
			,	FVector adjascentsCubesOffsets[12][3]
	);

};
