// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SensorData.h"
#include "SensorDataLoader.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_DATAANALYSIS_API USensorDataLoader : public UObject
{
	GENERATED_BODY()
public:
	
	/* Read absolute path CSV file and parse into FSensorReading array */
	UFUNCTION(BlueprintCallable, Category = "SensorData")
	static bool LoadCSVGeneric(const FString& FilePath, TArray<FString>& OutHeaders, TArray<FCSVRow>& OutRows);
};
