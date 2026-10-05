#include "SensorSubsystem.h"
#include "EquipmentActor.h"
#include "Kismet/GameplayStatics.h"

void USensorSubsystem::ApplyRows(const TArray<FCSVRow>& Rows, const FString& EquipmentColumn, const FString& TimestampColumn, const FString& StatusColumn)
{
	(void)TimestampColumn;

	if (EquipmentColumn.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SensorSubsystem: EquipmentColumn이 매핑되지 않았습니다."));
		return;
	}

	TMap<FString, const FCSVRow*> LatestByEquipment;
	LatestByEquipment.Reserve(Rows.Num());
	for (const FCSVRow& Row : Rows)
	{
		const FString EqId = Row.Get(EquipmentColumn);
		if (!EqId.IsEmpty())
		{
			LatestByEquipment.Add(EqId, &Row);
		}
	}

	for (const auto& Pair : LatestByEquipment)
	{
		if (AEquipmentActor* Actor = FindOrSpawnEquipmentActor(Pair.Key))
		{
			Actor->ApplyRow(*Pair.Value, StatusColumn);
		}
	}
}

void USensorSubsystem::EnsureRegistryInitialized()
{
	if (bRegistryInitialized)
		return;
	bRegistryInitialized = true;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEquipmentActor::StaticClass(), Found);

	for (AActor* A : Found)
	{
		if (AEquipmentActor* Eq = Cast<AEquipmentActor>(A))
		{
			if (!Eq->EquipmentId.IsEmpty())
			{
				EquipmentRegistry.Add(Eq->EquipmentId, Eq);
			}
		}
	}
}

AEquipmentActor* USensorSubsystem::FindOrSpawnEquipmentActor(const FString& EquipmentId)
{
	EnsureRegistryInitialized();

	if (TWeakObjectPtr<AEquipmentActor>* Existing = EquipmentRegistry.Find(EquipmentId))
	{
		if (Existing->IsValid())
		{
			return Existing->Get();
		}
		EquipmentRegistry.Remove(EquipmentId);
	}

	UWorld* World = GetWorld();
	if (!World)
		return nullptr;

	const float Spacing = 300.f;
	const FVector SpawnLocation(EquipmentRegistry.Num() * Spacing, 0.f, 50.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEquipmentActor* NewActor = World->SpawnActor<AEquipmentActor>(AEquipmentActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, Params);
	if (NewActor)
	{
		NewActor->EquipmentId = EquipmentId;
		EquipmentRegistry.Add(EquipmentId, NewActor);
	}

	return NewActor;
}
