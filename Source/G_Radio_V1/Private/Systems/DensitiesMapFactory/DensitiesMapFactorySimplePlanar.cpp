// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DensitiesMapFactory/DensitiesMapFactorySimplePlanar.h"

TMap<FDensityIndex, float> UDensitiesMapFactorySimplePlanar::Build(UMarshingCubesConfig* config)
{
	TMap<FDensityIndex, float> densitties = TMap<FDensityIndex, float>();
	
	for (int currentCubeX = 0; config->StartXCubes <= config->EndXCubes; currentCubeX++)
	{
		for (int currentCubeY = config->StartYCubes; currentCubeY <= config->EndYCubes; currentCubeY++)
		{
			FDensityIndex lower = FDensityIndex(currentCubeX, currentCubeY, 0);
			FDensityIndex upper = FDensityIndex(currentCubeX, currentCubeY, 1);
			densitties.Add(lower, 1);
			densitties.Add(upper, 0);	
		}
	}
	
	return densitties;
}
