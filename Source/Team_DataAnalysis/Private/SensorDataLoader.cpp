#include "SensorDataLoader.h"
#include "Misc/FileHelper.h"
#include "Async/ParallelFor.h"

bool USensorDataLoader::LoadCSVGeneric(const FString& FilePath, TArray<FString>& OutHeaders, TArray<FCSVRow>& OutRows)
{
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("SensorDataLoader: failed to read %s"), *FilePath);
		return false;
	}

	if (Lines.Num() < 2)
		return false;

	OutHeaders.Empty();
	Lines[0].ParseIntoArray(OutHeaders, TEXT(","), true);
	for (FString& H : OutHeaders)
		H = H.TrimStartAndEnd();

	const int32 NumDataLines = Lines.Num() - 1;
	const int32 NumColumns = OutHeaders.Num();

	TArray<FCSVRow> ParsedRows;
	ParsedRows.SetNum(NumDataLines);

	ParallelFor(NumDataLines, [&](int32 Index)
	{
		const FString& Line = Lines[Index + 1];
		if (Line.TrimStartAndEnd().IsEmpty())
			return;

		TArray<FString> Cols;
		Line.ParseIntoArray(Cols, TEXT(","), true);

		FCSVRow& Row = ParsedRows[Index];
		Row.Values.Reserve(NumColumns);
		for (int32 c = 0; c < NumColumns; ++c)
		{
			FString Value = Cols.IsValidIndex(c) ? Cols[c].TrimStartAndEnd() : FString();
			Row.Values.Add(OutHeaders[c], MoveTemp(Value));
		}
	});

	OutRows.Reset(NumDataLines);
	for (FCSVRow& Row : ParsedRows)
	{
		if (Row.Values.Num() > 0)
		{
			OutRows.Add(MoveTemp(Row));
		}
	}

	return true;
}
