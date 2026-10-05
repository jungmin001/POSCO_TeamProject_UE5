#include "SensorLoaderWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "SensorDataLoader.h"
#include "SensorSubsystem.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Kismet/GameplayStatics.h"

void USensorLoaderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BrowseButton)
		BrowseButton->OnClicked.AddDynamic(this, &USensorLoaderWidget::OnBrowseClicked);

	if (LoadButton)
		LoadButton->OnClicked.AddDynamic(this, &USensorLoaderWidget::OnLoadClicked);

	if (ApplyMappingButton)
		ApplyMappingButton->OnClicked.AddDynamic(this, &USensorLoaderWidget::OnApplyMappingClicked);
}

void USensorLoaderWidget::OnBrowseClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform || !FilePathInput)
		return;

	void* ParentWindowHandle = nullptr;
	if (GEngine && GEngine->GameViewport)
	{
		ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
	}

	TArray<FString> OutFiles;
	const bool bOpened = DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		TEXT("센서 데이터 CSV 선택"),
		TEXT(""),
		TEXT(""),
		TEXT("CSV Files (*.csv)|*.csv"),
		EFileDialogFlags::None,
		OutFiles
	);

	if (bOpened && OutFiles.Num() > 0)
	{
		FilePathInput->SetText(FText::FromString(OutFiles[0]));
	}
}

void USensorLoaderWidget::OnLoadClicked()
{
	if (!FilePathInput)
		return;

	const FString Path = FilePathInput->GetText().ToString();

	if (!USensorDataLoader::LoadCSVGeneric(Path, Headers, Rows))
	{
		if (StatusText)
			StatusText->SetText(FText::FromString(TEXT("CSV load failed: ") + Path));
		return;
	}

	for (UComboBoxString* Combo : { EquipmentColumnCombo, TimestampColumnCombo, StatusColumnCombo })
	{
		if (!Combo) continue;
		Combo->ClearOptions();
		for (const FString& H : Headers)
			Combo->AddOption(H);
	}

	auto AutoSelect = [this](UComboBoxString* Combo, const FString& Keyword)
	{
		if (!Combo) return;
		for (const FString& H : Headers)
		{
			if (H.Contains(Keyword, ESearchCase::IgnoreCase))
			{
				Combo->SetSelectedOption(H);
				return;
			}
		}
	};
	AutoSelect(EquipmentColumnCombo, TEXT("equipment"));
	AutoSelect(TimestampColumnCombo, TEXT("timestamp"));
	AutoSelect(StatusColumnCombo, TEXT("status"));

	if (StatusText)
	{
		StatusText->SetText(FText::FromString(
			FString::Printf(TEXT("%d개 행, %d개 컬럼 로드됨. 컬럼 매핑을 확인하세요."), Rows.Num(), Headers.Num())));
	}
}

void USensorLoaderWidget::OnApplyMappingClicked()
{
	if (Rows.Num() == 0)
	{
		if (StatusText)
			StatusText->SetText(FText::FromString(TEXT("먼저 CSV를 로드하세요.")));
		return;
	}

	const FString EquipmentCol = EquipmentColumnCombo ? EquipmentColumnCombo->GetSelectedOption() : FString();
	const FString TimestampCol = TimestampColumnCombo ? TimestampColumnCombo->GetSelectedOption() : FString();
	const FString StatusCol    = StatusColumnCombo    ? StatusColumnCombo->GetSelectedOption()    : FString();

	if (USensorSubsystem* Subsystem = GetWorld()->GetSubsystem<USensorSubsystem>())
	{
		Subsystem->ApplyRows(Rows, EquipmentCol, TimestampCol, StatusCol);
	}

	if (StatusText)
		StatusText->SetText(FText::FromString(TEXT("매핑 적용 완료")));
}
