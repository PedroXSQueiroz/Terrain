// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DensitiesMapFactory/DensitiesMapFactoryNoise.h"

#pragma optimize( "", off )
TMap<FXYZIndex, float> UDensitiesMapFactoryNoise::Build(UMarshingCubesConfig* config)
{
	
	TMap<FXYZIndex, float> densities = TMap<FXYZIndex, float>();

	float randFactor = FMath::RandRange(0.00001, 0.99999);

	for (int currentX = config->StartXCubes; currentX <= config->EndXCubes; currentX++)
	{
		for (int currentY = config->StartYCubes; currentY <= config->EndYCubes; currentY++)
		{
			float height = FMath::PerlinNoise2D(FVector2D((float)currentX + randFactor, (float)currentY + randFactor) * this->NoiseScale);

			float normalizedHeight = (config->EndZCubes - config->StartZCubes) * ( ( height + 1 ) / 2 );
			normalizedHeight = normalizedHeight > this->MinHeight ? normalizedHeight : this->MinHeight;
			normalizedHeight += 1;
			normalizedHeight *= this->Amplitude;

			int currentZ = FMath::FloorToInt(normalizedHeight);
			float currentDensity = normalizedHeight - currentZ;

			densities.Add(FXYZIndex(currentX, currentY, currentZ), currentDensity);

			for (int densitiesBellow = currentZ - 1; densitiesBellow >= 0; densitiesBellow--) 
			{
				densities.Add(FXYZIndex(currentX, currentY, densitiesBellow), 1);
			}
			

			int cubeXIndex = currentX % 2 == 0 ? currentX - 1 : FMath::FloorToInt((float)currentX / (float)2);
			int cubeYIndex = currentX % 2 == 0 ? currentX - 1 : FMath::FloorToInt((float)currentY / (float)2);
			int cubeZIndex = currentX % 2 == 0 ? currentX - 1 : FMath::FloorToInt((float)currentZ / (float)2);
			this->SetDefaultIfNotPresent(FXYZIndex(0 + cubeXIndex, 0 + cubeYIndex, 0 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(1 + cubeXIndex, 0 + cubeYIndex, 0 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(0 + cubeXIndex, 1 + cubeYIndex, 0 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(1 + cubeXIndex, 1 + cubeYIndex, 0 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(0 + cubeXIndex, 0 + cubeYIndex, 1 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(1 + cubeXIndex, 0 + cubeYIndex, 1 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(0 + cubeXIndex, 1 + cubeYIndex, 1 + cubeZIndex), 0, densities);
			this->SetDefaultIfNotPresent(FXYZIndex(1 + cubeXIndex, 1 + cubeYIndex, 1 + cubeZIndex), 0, densities);
		}
	}
	
	return densities;
}
#pragma optimize( "", on )

void UDensitiesMapFactoryNoise::SetDefaultIfNotPresent(FXYZIndex index, float defaultValue, TMap<FXYZIndex, float>& destiny)
{
	if (!destiny.Contains(index))
	{
		destiny.Add(index);
	}
}
