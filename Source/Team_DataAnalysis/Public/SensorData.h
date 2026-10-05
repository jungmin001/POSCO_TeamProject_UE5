#pragma once

#include "CoreMinimal.h"
#include "SensorData.generated.h"

USTRUCT(BlueprintType)
struct FCSVRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "CSV")
	TMap<FString, FString> Values;

	FString Get(const FString& Column) const
	{
		const FString* Found = Values.Find(Column);
		return Found ? *Found : FString();
	}

	float GetFloat(const FString& Column) const
	{
		return FCString::Atof(*Get(Column));
	}
};