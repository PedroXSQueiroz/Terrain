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
		
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]() {
			UE_LOG(LogTemp, Log, TEXT("BUILDING MESH"))
			
			UObject* factoryInstance = this->MeshFactoryType.GetDefaultObject();
			IMeshFactory* factory = Cast<IMeshFactory>(factoryInstance);
			TArray<FMeshTriangleData> triangles = factory->Build(this);

			TArray<FVector> allVertex = TArray<FVector>();
			TArray<int> allTriangles = TArray<int>();
			TArray<FVector> normals = TArray<FVector>();
			for (FMeshTriangleData triangleData : triangles)
			{
				for (int vertexIndex = 0; vertexIndex < triangleData.Vertex.Num(); vertexIndex++)
				{
					FVector currentVertex = triangleData.Vertex[vertexIndex];
					allVertex.Add(currentVertex);
					allTriangles.Add(allTriangles.Num());

					if (allVertex.Num() % 3 == 0)
					{
						FVector first = allVertex[vertexIndex - 2];
						FVector second = allVertex[vertexIndex - 1];
						FVector third = allVertex[vertexIndex];

						FVector a = second - first;
						FVector b = third - first;

						normals.Add(
							FVector(
								(a.Y * b.Z) - (a.Z * b.Y),
								(a.Z * b.X) - (a.X * b.Z),
								(a.X * b.Y) - (a.Y * b.Z)
							)
						);
					}

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
				, allVertex
				, allTriangles
				, normals
				, TArray<FVector2D>()
				, TArray<FColor>()
				, TArray<FProcMeshTangent>()
				, false
			);

			this->Ground->SetMaterial(0, this->MainGroundMaterial);

			UE_LOG(LogTemp, Log, TEXT("FINISEHD MESH"))
		});
		
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

