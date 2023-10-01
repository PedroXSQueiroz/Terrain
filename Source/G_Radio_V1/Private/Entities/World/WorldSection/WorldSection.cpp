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
	//this->RootComponent = this->Ground;
	this->Ground->SetupAttachment(this->RootComponent);
	//this->Ground->bUseAsyncCooking = true;

}

#pragma optimize( "", off )
// Called when the game starts or when spawned
void AWorldSection::BeginPlay()
{
	Super::BeginPlay();

	if (this->MeshFactoryType && this->IsValidFactoryType(this->MeshFactoryType.Get()))
	{
		
		this->OnMeshCompleted().AddLambda([&](TArray<FVector> vertex, TArray<int> triangles, TArray<FVector> normals, TArray<FProcMeshTangent> tangents) {
			this->Ground->CreateMeshSection(
				0
				, vertex
				, triangles
				, normals
				, TArray<FVector2D>()
				, TArray<FColor>()
				, tangents
				, false
			);

			this->Ground->UpdateBounds();
			this->Ground->SetMaterial(0, this->MainGroundMaterial);

			UE_LOG(LogTemp, Log, TEXT("FINISEHD MESH"))
		});

		
		UObject* factoryInstance = this->MeshFactoryType.GetDefaultObject();
		bool debugVertex = this->DebugVertex;
		bool debugNormals = this->DebugNormals;

		AActor* owner = this;

		FMeshCompletedEvent finishEvent = this->OnMeshCompleted();

		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [
					owner
				,	debugVertex
				,	debugNormals
				,	factoryInstance
				,	finishEvent]() {
			UE_LOG(LogTemp, Log, TEXT("BUILDING MESH"))
			
			IMeshFactory* factory = Cast<IMeshFactory>(factoryInstance);
			TArray<FMeshTriangleData> triangles = factory->Build(owner);

			TArray<FVector> allVertex			= TArray<FVector>();
			TArray<int> allTriangles			= TArray<int>();
			TArray<FVector> normals				= TArray<FVector>();
			TArray<FProcMeshTangent> tangents	= TArray<FProcMeshTangent>();

			for (FMeshTriangleData triangleData : triangles)
			{
				for (int vertexIndex = 0; vertexIndex < triangleData.Vertex.Num(); vertexIndex++)
				{
					FVector currentVertex = triangleData.Vertex[vertexIndex];
					allVertex.Add(currentVertex);
					allTriangles.Add(allTriangles.Num());

					//----------------------------
					// AT EACH TRIANGLE 
					//----------------------------
					if (allVertex.Num() % 3 == 0)
					{
						//----------------------------
						// NORMALS 
						//----------------------------
						
						int lastVertexIndex = allVertex.Num() - 1;
						
						FVector first =		allVertex[lastVertexIndex - 2] * owner->GetActorScale();
						FVector second =	allVertex[lastVertexIndex - 1] * owner->GetActorScale();
						FVector third =		allVertex[lastVertexIndex] * owner->GetActorScale();

						FVector a = second - first;
						FVector b = third - first;

						FVector currentNormal = FVector(
							(a.Y * b.Z) - (a.Z * b.Y),
							(a.Z * b.X) - (a.X * b.Z),
							(a.X * b.Y) - (a.Y * b.Z)
						) * -1;

						normals.Add(currentNormal);
						
						if (debugNormals) 
						{
							FVector triangleCenter = FVector(
								(first.X + second.X + third.X) / 3,
								(first.Y + second.Y + third.Y) / 3,
								(first.Z + second.Z + third.Z) / 3
							) * owner->GetActorScale();
							
							DrawDebugLine(
								owner->GetWorld(),
								triangleCenter,
								triangleCenter + ( currentNormal * owner->GetActorScale()),
								FColor::Red,
								true
							);

							DrawDebugSphere(
								owner->GetWorld(),
								triangleCenter,
								5,
								6,
								FColor::Red,
								true
							);
						}
						
						//----------------------------
						// NORMALS 
						//----------------------------
						
						//----------------------------
						// TANGENTS
						//----------------------------
						
						FVector triangleProduct = FVector::CrossProduct(second - first, third - first) * -1;
						
						triangleProduct.Normalize();

						tangents.Add(FProcMeshTangent(triangleProduct, false));
						tangents.Add(FProcMeshTangent(triangleProduct, false));
						tangents.Add(FProcMeshTangent(triangleProduct, false));

						DrawDebugLine(
							owner->GetWorld(),
							first,
							first + (triangleProduct * owner->GetActorScale()),
							FColor::Red,
							true
						);

						DrawDebugLine(
							owner->GetWorld(),
							second,
							second + (triangleProduct * owner->GetActorScale()),
							FColor::Red,
							true
						);

						DrawDebugLine(
							owner->GetWorld(),
							third,
							third + (triangleProduct * owner->GetActorScale()),
							FColor::Red,
							true
						);

						//----------------------------
						// TANGENTS
						//----------------------------
					}


					if (debugVertex)
					{
						DrawDebugSphere(
							owner->GetWorld(),
							currentVertex * owner->GetActorScale(),
							5,
							6,
							FColor::Red,
							true
						);
					}


				}
			}

			finishEvent.Broadcast(allVertex, allTriangles, normals, tangents);
		});
		
	}
}
#pragma optimize( "", on )

bool AWorldSection::IsValidFactoryType(UClass* factoryType)
{
	return factoryType->ImplementsInterface(UMeshFactory::StaticClass());
}

// Called every frame
void AWorldSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

