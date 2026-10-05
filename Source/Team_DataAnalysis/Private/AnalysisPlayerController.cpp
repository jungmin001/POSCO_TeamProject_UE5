#include "AnalysisPlayerController.h"
#include "SensorLoaderWidget.h"
#include "Blueprint/UserWidget.h"

void AAnalysisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget> ClassToUse = LoaderWidgetClass;
	if (!ClassToUse)
	{
		ClassToUse = UUserWidget::StaticClass();
	}

	LoaderWidget = CreateWidget<UUserWidget>(this, ClassToUse);
	if (LoaderWidget)
	{
		LoaderWidget->AddToViewport();
	}

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}
