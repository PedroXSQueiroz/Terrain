// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DensitiesMapFactory/DensitiesMapFactorySimplePlanar.h"

TMap<FXYZIndex, float> UDensitiesMapFactorySimplePlanar::Build(UMarshingCubesConfig* config)
{
	TMap<FXYZIndex, float> densitties = TMap<FXYZIndex, float>();
	
	for (int currentCubeX = config->StartXCubes; currentCubeX <= config->EndXCubes; currentCubeX++)
	{
		for (int currentCubeY = config->StartYCubes; currentCubeY <= config->EndYCubes; currentCubeY++)
		{
			FXYZIndex lower = FXYZIndex(currentCubeX, currentCubeY, 0);
			FXYZIndex upper = FXYZIndex(currentCubeX, currentCubeY, 1);
			densitties.Add(lower, 1);
			densitties.Add(upper, 0);	
		}
	}
	
	return densitties;
}
