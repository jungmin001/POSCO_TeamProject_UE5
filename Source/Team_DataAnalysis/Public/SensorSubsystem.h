#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SensorData.h"
#include "SensorSubsystem.generated.h"

class AEquipmentActor;

UCLASS()
class TEAM_DATAANALYSIS_API USensorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Rows: LoadCSVGeneric로 읽은 전체 CSV 행
	// EquipmentColumn/StatusColumn: 위젯에서 사용자가 매핑한 컬럼명
	UFUNCTION(BlueprintCallable, Category = "SensorData")
	void ApplyRows(const TArray<FCSVRow>& Rows, const FString& EquipmentColumn, const FString& TimestampColumn, const FString& StatusColumn);

private:
	// EquipmentId -> Actor 캐시. 레벨 전체를 매번 훑지 않고 O(1)로 찾기 위함.
	UPROPERTY()
	TMap<FString, TWeakObjectPtr<AEquipmentActor>> EquipmentRegistry;

	bool bRegistryInitialized = false;

	// 레벨에 이미 배치된 AEquipmentActor들을 최초 1회만 스캔해서 레지스트리에 채움
	void EnsureRegistryInitialized();

	AEquipmentActor* FindOrSpawnEquipmentActor(const FString& EquipmentId);
};
