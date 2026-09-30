#include "EmergencyPanelWidget.h"
#include "FactoryLine.h"
#include "Components/Button.h"
#include "Engine/World.h"

/// Sets default values
void UEmergencyPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (saveButton)
        saveButton->OnClicked.AddDynamic(this, &UEmergencyPanelWidget::OnSavePressed);
}

/// Initialize the emergency panel with specified factory line
void UEmergencyPanelWidget::Init(AFactoryLine* inLine)
{
    targetLine = inLine;

	// Initialize timer for emergency failure
    GetWorld()->GetTimerManager().SetTimer(
        emergencyTimer,
        this,
        &UEmergencyPanelWidget::OnEmergencyFailed,
        5.f,
        false
    );

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor(255, 0, 128), // Pink
            FString::Printf(
                TEXT("Emergency minigame started")
            )
        );
}

/// Handle pressing save button
void UEmergencyPanelWidget::OnSavePressed()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor(255, 0, 128), // Pink
            FString::Printf(
                TEXT("Pressed on time")
            )
        );

    if (targetLine)
        targetLine->SaveLine();

    GetWorld()->GetTimerManager().ClearTimer(emergencyTimer);

    RemoveFromParent();
}

/// Handle emergency failure when timer runs out
void UEmergencyPanelWidget::OnEmergencyFailed()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor(255, 0, 128), // Pink
            FString::Printf(
                TEXT("Failed to press")
            )
        );

    if (targetLine)
        targetLine->FailLine();

    RemoveFromParent();
}