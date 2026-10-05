#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SensorData.h"
#include "SensorLoaderWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UComboBoxString;

UCLASS()
class TEAM_DATAANALYSIS_API USensorLoaderWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* FilePathInput;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BrowseButton;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* EquipmentColumnCombo;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* TimestampColumnCombo;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* StatusColumnCombo;

	UPROPERTY(meta = (BindWidget))
	UButton* ApplyMappingButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusText;

	UFUNCTION()
	void OnBrowseClicked();

	UFUNCTION()
	void OnLoadClicked();

	UFUNCTION()
	void OnApplyMappingClicked();

private:
	TArray<FString> Headers;
	TArray<FCSVRow> Rows;
	
};
