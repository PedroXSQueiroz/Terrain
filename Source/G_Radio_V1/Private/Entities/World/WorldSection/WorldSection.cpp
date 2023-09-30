// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/World/WorldSection/WorldSection.h"

#include "Async/AsyncWork.h"

#include "Systems/MeshFactory/MeshFactory.h"

#include "ProceduralMeshComponent.h"

// Sets default values
AWorldSection::AWorldSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Ground = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GROUND"));
	this->RootComponent = this->Ground;
	this->Ground->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void AWorldSection::BeginPlay()
{
	Super::BeginPlay();

	if (this->MeshFactoryType && this->IsValidFactoryType(this->MeshFactoryType.Get()))
	{
		UObject* factoryInstance = this->MeshFactoryType.GetDefaultObject();
		IMeshFactory* factory = Cast<IMeshFactory>(factoryInstance);
		TArray<FMeshTriangleData> triangles = factory->Build(this);

		TArray<FVector> allVertex = TArray<FVector>();
		TArray<int> allTriangles = TArray<int>();
		TArray<FVector2D> uv = TArray<FVector2D>();
		for (FMeshTriangleData triangleData : triangles) 
		{
			for (int vertexIndex = 0; vertexIndex < triangleData.Vertex.Num(); vertexIndex++)
			{
				FVector currentVertex = triangleData.Vertex[vertexIndex];
				allVertex.Add(currentVertex);
				allTriangles.Add(allTriangles.Num());

				if (this->DebugVertex) 
				{
					DrawDebugSphere(
						this->GetWorld(),
						currentVertex * this->GetActorScale(),
						1,
						12,
						FColor::Red,
						true
					);
				}

			}
		}

		this->Ground->CreateMeshSection(
							0
						,	allVertex
						,	allTriangles
						,	TArray<FVector>()
						,	uv
						,	TArray<FColor>()
						,	TArray<FProcMeshTangent>()
						,	false
						);

		this->Ground->SetMaterial(0, this->MainGroundMaterial);

	}
}

bool AWorldSection::IsValidFactoryType(UClass* factoryType)
{
	return factoryType->ImplementsInterface(UMeshFactory::StaticClass());
}

// Called every frame
void AWorldSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

