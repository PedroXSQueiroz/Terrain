// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldSection.generated.h"

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
	class UProceduralMeshComponent* Ground;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GENERAL")
	bool Debug;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsValidFactoryType(UClass* factoryType);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
