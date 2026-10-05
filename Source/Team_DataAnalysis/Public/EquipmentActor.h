#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SensorData.h"
#include "EquipmentActor.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EEquipmentStatus : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Warning		UMETA(DisplayName = "Warning"),
	Danger		UMETA(DisplayName = "Danger")
};

UCLASS()
class TEAM_DATAANALYSIS_API AEquipmentActor : public AActor
{
	GENERATED_BODY()

public:
	AEquipmentActor();

	// CSV의 equipment 컬럼 값과 매칭되는 ID (ex: "Motor_01")
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FString EquipmentId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UStaticMeshComponent* Mesh;

	// 이 설비에 마지막으로 적용된 CSV 행 (컬럼이 바뀌어도 그대로 보관)
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	FCSVRow CurrentRow;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	EEquipmentStatus Status = EEquipmentStatus::Normal;

	// 매핑된 컬럼명을 받아 상태를 갱신. StatusColumn 값이 "Warning"/"Danger"/그 외(Normal)로 매핑됨
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ApplyRow(const FCSVRow& Row, const FString& StatusColumn);

protected:
	// Blueprint에서 오버라이드해서 머티리얼/하이라이트/Niagara 연출 구현
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnStatusChanged(EEquipmentStatus NewStatus);

private:
	// 블루프린트로 머티리얼을 따로 안 만든 경우를 위한 최소한의 색상 피드백 (best-effort)
	UPROPERTY()
	class UMaterialInstanceDynamic* DynMaterial;

	void ApplyDefaultColorFeedback();
};
