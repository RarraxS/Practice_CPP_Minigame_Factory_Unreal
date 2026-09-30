#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameOverWidget.generated.h"

/// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetPressed);

UCLASS()
class FINALTASKRAMONZABARA_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
	/// Initialize the game over widget with the final score
    void Init(float score);

    /// Delegates
    UPROPERTY(BlueprintAssignable)
    FOnResetPressed OnResetPressed;

protected:
    /// Called when the widget is constructed
    virtual void NativeConstruct() override;

	/// UI References

    UPROPERTY(meta = (BindWidget))
    UTextBlock* scoreText;

    UPROPERTY(meta = (BindWidget))
    UButton* resetButton;

    /// Handle pressing the reset button
    UFUNCTION()
    void ResetButtonPressed();
};