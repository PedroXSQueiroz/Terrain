// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"
#include "Templates/SharedPointer.h"

#include "WorldSection.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FMeshCompletedEvent, TArray<FVector>, TArray<int>, TArray<FVector>, TArray<FProcMeshTangent>)

UCLASS()
class G_RADIO_V1_API AWorldSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldSection();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL", meta = (MustImplement = "MeshFactory"))
	TSubclassOf<UObject> MeshFactoryType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	UProceduralMeshComponent* Ground;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	class UMaterialInterface* MainGroundMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	bool DebugVertex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	bool DebugNormals;

	FMeshCompletedEvent& OnMeshCompleted()
	{
		return this->MeshCompletedEvent;
	}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsValidFactoryType(UClass* factoryType);

	FMeshCompletedEvent MeshCompletedEvent = FMeshCompletedEvent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
