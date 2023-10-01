// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DensitiesMapFactory/DensitiesMapFactoryFileReader.h"

#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"

TMap<FXYZIndex, float> UDensitiesMapFactoryFileReader::Build(UMarshingCubesConfig* config)
{
	TMap<FXYZIndex, float> densities = TMap<FXYZIndex, float>();
	
	FString Content;

	if (FFileHelper::LoadFileToString(Content, *this->SourcePath)) 
	{
		
		TSharedPtr<FJsonObject> rawData = MakeShareable(new FJsonObject());
		TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(Content);

		if (FJsonSerializer::Deserialize(JsonReader, rawData) &&
			rawData.IsValid())
		{
			for (auto currentDensityKey : rawData->Values)
			{
				TArray<FString> indexes;
				currentDensityKey.Key.ParseIntoArray(indexes, TEXT(","), false);
		
				int x = FCString::Atoi(*(indexes[0]));
				int y = FCString::Atoi(*(indexes[1]));
				int z = FCString::Atoi(*(indexes[2]));

				densities.Add(FXYZIndex(x, y, z), rawData->GetNumberField(currentDensityKey.Key));
			}
	
		}
	
	}

	return densities;
}
