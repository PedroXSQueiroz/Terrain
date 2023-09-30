// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "MeshFactory.h"

#include "MeshFactoryMarshingCubes.generated.h"

USTRUCT(BlueprintType)
struct FDensityIndex
{
	GENERATED_BODY()

public:

	FDensityIndex() {}

	FDensityIndex(int x, int y, int z) :X(x), Y(y), Z(z) {}

	FDensityIndex(FVector param) : X(param.X),Y(param.Y),Z(param.Z) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int X;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Y;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Z;

	bool operator==(const FDensityIndex& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FDensityIndex& Other) const
	{
		return	this->X == Other.X
			&&	this->Y == Other.Y
			&&	this->Z == Other.Z;
	}

	FString ToString() 
	{
		return FString::Printf( TEXT( "%i,%i,%i"), this->X, this->Y, this->Z);
	}
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FHashMeIfYouCan& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FDensityIndex& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDensityIndex));
	return Hash;
}
#endif

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
	bool Debug;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	FString DebugDensitiesFilePath;

	virtual TArray<FMeshTriangleData> Build(AActor* owner) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL", meta = (MustImplement = "DensitiesMapFactory"))
	TSubclassOf<UObject> DensitiesMapFactoryType;



private:

	TArray<FMeshTriangleData> GetTrianglesOfCube(
			const int  triTable[256][16]
		,	int  edgeConnections[12][2]
		,	FVector  cornerOffsets[8]
		,	int xIndex
		,	int yIndex
		,	int zIndex
		,	TMap<FDensityIndex, float> densities
		,	AActor* owner);
	
	FVector InteroplateEdge(
			FVector firstVertex
		,	float	firstVertexValue
		,	FVector secondVertex
		,	float	secondVertexValue
	);

	float GetDensity(FDensityIndex index, TMap<FDensityIndex, float> densities);

};
