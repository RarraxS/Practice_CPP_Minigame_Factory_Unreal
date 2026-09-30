#include "GameOverWidget.h"
#include "FactoryLine.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

/// Sets default values
void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (resetButton)
        resetButton->OnClicked.AddDynamic(this, &UGameOverWidget::ResetButtonPressed);
}

/// Initialize the game over widget with the final score
void UGameOverWidget::Init(float score)
{
    scoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %.0f"), score)));
}

/// Handle pressing the reset button
void UGameOverWidget::ResetButtonPressed()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor(255, 0, 128), // Pink
            FString::Printf(
                TEXT("Game reset")
            )
        );

    OnResetPressed.Broadcast();

    RemoveFromParent();
}