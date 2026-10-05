#include "AnalysisGameMode.h"
#include "AnalysisPlayerController.h"

AAnalysisGameMode::AAnalysisGameMode()
{
	PlayerControllerClass = AAnalysisPlayerController::StaticClass();
	DefaultPawnClass = nullptr; 
}
