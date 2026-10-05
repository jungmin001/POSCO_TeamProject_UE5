#include "EquipmentActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

static EEquipmentStatus ParseStatus(const FString& InStatus)
{
	if (InStatus.Equals(TEXT("Danger"), ESearchCase::IgnoreCase))
		return EEquipmentStatus::Danger;
	if (InStatus.Equals(TEXT("Warning"), ESearchCase::IgnoreCase))
		return EEquipmentStatus::Warning;
	return EEquipmentStatus::Normal;
}

AEquipmentActor::AEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMeshAsset.Object);
		Mesh->SetWorldScale3D(FVector(1.5f));
	}
}

void AEquipmentActor::ApplyRow(const FCSVRow& Row, const FString& StatusColumn)
{
	CurrentRow = Row;

	const EEquipmentStatus NewStatus = ParseStatus(Row.Get(StatusColumn));
	const bool bStatusChanged = (Status != NewStatus);
	Status = NewStatus;

	if (bStatusChanged)
	{
		ApplyDefaultColorFeedback();
		OnStatusChanged(Status);
	}
}

void AEquipmentActor::ApplyDefaultColorFeedback()
{
	if (!Mesh)
		return;

	if (!DynMaterial)
	{
		UMaterialInterface* BaseMat = Mesh->GetMaterial(0);
		if (!BaseMat)
			return;
		DynMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
		Mesh->SetMaterial(0, DynMaterial);
	}

	if (!DynMaterial)
		return;

	FLinearColor Color = FLinearColor::Green;
	switch (Status)
	{
	case EEquipmentStatus::Warning: Color = FLinearColor::Yellow; break;
	case EEquipmentStatus::Danger:  Color = FLinearColor::Red;    break;
	default: break;
	}

	DynMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	DynMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
	DynMaterial->SetVectorParameterValue(TEXT("Tint"), Color);
}
