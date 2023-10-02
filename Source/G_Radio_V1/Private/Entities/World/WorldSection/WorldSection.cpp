// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/World/WorldSection/WorldSection.h"

#include "Async/AsyncWork.h"
#include "KismetProceduralMeshLibrary.h"

#include "Systems/MeshFactory/MeshFactory.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AWorldSection::AWorldSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Ground = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GROUND"));
	this->Ground->SetupAttachment(this->RootComponent);
	this->Ground->bUseComplexAsSimpleCollision = false;
	this->Ground->SetSimulatePhysics(true);
	this->Ground->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	this->Ground->SetCollisionProfileName(TEXT("PhysicsActor"));
	this->Ground->SetCollisionResponseToAllChannels(ECR_Block);
	this->Ground->SetEnableGravity(true);
	this->Ground->WakeRigidBody();
	this->Ground->SetMobility(EComponentMobility::Movable);

	this->SetActorEnableCollision(true);

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
			this->Ground->ClearCollisionConvexMeshes();
			this->Ground->AddCollisionConvexMesh(vertex);
			this->Ground->UpdatePhysicsVolume(true);
			
			UE_LOG(LogTemp, Log, TEXT("FINISEHD MESH"))
		});

		
		UObject* factoryInstance = this->MeshFactoryType.GetDefaultObject();
		IMeshFactory* factory = Cast<IMeshFactory>(factoryInstance);
		bool debugVertex	= this->DebugVertex;
		bool debugNormals	= this->DebugNormals;
		bool debugTangents	= this->DebugTangents;

		AActor* owner = this;

		FMeshCompletedEvent finishEvent = this->OnMeshCompleted();

		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, 
		[	owner
		,	debugVertex
		,	debugNormals
		,	factory
		,	finishEvent
		,	debugTangents]() {
			UE_LOG(LogTemp, Log, TEXT("BUILDING MESH"))
			
			FMeshData meshData = factory->Build(owner);

			TArray<FVector> normals				= TArray<FVector>();
			TArray<FProcMeshTangent> tangents	= TArray<FProcMeshTangent>();

			UKismetProceduralMeshLibrary::CalculateTangentsForMesh(meshData.Vertex, meshData.Triangles, TArray<FVector2D>(), normals, tangents );

			finishEvent.Broadcast(meshData.Vertex, meshData.Triangles, normals, tangents);
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

