#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "FactoryLine.h"
#include "EmergencyPanelWidget.generated.h"

/// Delegates 

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLineReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLineFailed);

UCLASS()
class FINALTASKRAMONZABARA_API UEmergencyPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /// Initialize the emergency panel with the specified factory line
    void Init(AFactoryLine* inLine);

protected:
    /// Default constructor
    virtual void NativeConstruct() override;

	/// Button reference
    UPROPERTY(meta = (BindWidget))
    UButton* saveButton;

    /// Line reference
    UPROPERTY()
    AFactoryLine* targetLine;

	/// Timer for emergency failure
    FTimerHandle emergencyTimer;

    /// Main functions

    UFUNCTION()
    void OnSavePressed();

    UFUNCTION()
    void OnEmergencyFailed();

    /// Delegates

    UPROPERTY(BlueprintAssignable)
    FOnLineReset OnLineReset;

    UPROPERTY(BlueprintAssignable)
    FOnLineFailed OnLineFailed;
};