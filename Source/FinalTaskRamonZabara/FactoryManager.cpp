#include "Kismet/GameplayStatics.h"
#include "FactoryManager.h"

/// Sets default values
AFactoryManager::AFactoryManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/// Called when the game starts or when spawned
void AFactoryManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Get all actors of class and bind delegates
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AFactoryLine::StaticClass(),
        foundLines
    );

    emergencyActive.Init(false, foundLines.Num());
    failedLines.Init(false, foundLines.Num());

    for (AActor* actor : foundLines)
    {
        AFactoryLine* line = Cast<AFactoryLine>(actor);
        if (line)
        {
            line->OnProductionChanged.AddDynamic(
                this,
                &AFactoryManager::HandleProductionStateChanged
            );

            line->OnProductionSave.AddDynamic(
                this,
                &AFactoryManager::SaveEmergency
            );

            line->OnProductionFail.AddDynamic(
                this,
                &AFactoryManager::FailEmergency
            );

            OnResetGame.AddDynamic(
                line,
                &AFactoryLine::ResetLine
			);
        }
    }

	// Create factory widget and bind delegates
    if (factoryWidgetClass)
    {
        factoryWidget = CreateWidget<UFactoryWidget>(
            GetWorld(),
            factoryWidgetClass
        );

        if (factoryWidget)
        {
            factoryWidget->AddToViewport();

            OnFactoryLineUpdated.AddDynamic(
                factoryWidget,
                &UFactoryWidget::HandleFactoryLineUpdated
            );
        }
    }
}

/// Called every frame
void AFactoryManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

/// Handle production changes for UI representation
void AFactoryManager::HandleProductionStateChanged(
    AFactoryLine* factoryLine,
    float efficiency,
    float resources,
    EProductionLineState state
)
{
    if (!factoryLine) return;

	//Checks which line has changed and updates the UI with the new values
    for (int i = 0; i < foundLines.Num(); i++)
    {
        if (foundLines[i] == factoryLine)
        {
            OnFactoryLineUpdated.Broadcast(
                i,
                efficiency,
                resources,
                state
            );

            //Checks if the emergency panel should be created
            if (emergencyActive[i] == false && state == EProductionLineState::Critical && emergencyPanelWidgetClass)
            {
                emergencyActive[i] = true;

                UEmergencyPanelWidget* panel =
                    CreateWidget<UEmergencyPanelWidget>(
                        GetWorld(),
                        emergencyPanelWidgetClass
                    );

                if (panel)
                {
                    panel->AddToViewport();
                    panel->Init(factoryLine);
                }
            }
            break;
        }
    }
}

/// Handle saving the line and updating the score
void AFactoryManager::SaveEmergency(AFactoryLine* factoryLine)
{
    if (!factoryLine) return;

    for (int i = 0; i < foundLines.Num(); i++)
    {
        if (foundLines[i] == factoryLine)
        {
            emergencyActive[i] = false;
			score++;
            break;
        }
    }
}

/// Handle failing the line and checking for Game Over condition
void AFactoryManager::FailEmergency(AFactoryLine* factoryLine)
{
    if (!factoryLine) return;

    bool gameOver = true;

    for (int i = 0; i < foundLines.Num(); i++)
    {
        if (foundLines[i] == factoryLine)
            failedLines[i] = true;

        // Check all stoped lines
        if (failedLines[i] == false)
            gameOver = false;
    }

	// If all lines are stopped create Game Over widget
    if (gameOver)
    {
        UGameOverWidget* panel =
            CreateWidget<UGameOverWidget>(
                GetWorld(),
                gameOverWidgetClass
            );

        if (panel)
        {
            panel->AddToViewport();
            panel->Init(score);

            panel->OnResetPressed.AddDynamic(this, &AFactoryManager::ResetEmergency);
        }
    }
}

/// Handle resetting the game after Game Over
void AFactoryManager::ResetEmergency()
{
    for (int i = 0; i < foundLines.Num(); i++)
    {
        emergencyActive[i] = false;
        failedLines[i] = false;
    }

    score = 0;

	OnResetGame.Broadcast();
}