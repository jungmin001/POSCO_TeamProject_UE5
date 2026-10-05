#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AnalysisPlayerController.generated.h"

class USensorLoaderWidget;

UCLASS(Blueprintable)
class TEAM_DATAANALYSIS_API AAnalysisPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LoaderWidgetClass;

private:
	UPROPERTY()
	UUserWidget* LoaderWidget;
};
