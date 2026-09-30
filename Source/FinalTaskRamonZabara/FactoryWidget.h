#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "FactoryLine.h"
#include "FactoryWidget.generated.h"

UCLASS()
class FINALTASKRAMONZABARA_API UFactoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/// Handler -------------------------------------------
    UFUNCTION()
    void HandleFactoryLineUpdated(
        int                  lineIndex,
        float                efficiency,
        float                resources,
        EProductionLineState state
    );

    /// Efficiency Lines ----------------------------------

    UPROPERTY(meta = (BindWidget))
    UProgressBar* efficiencyLine1;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* efficiencyLine2;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* efficiencyLine3;

    /// Resources Lines -----------------------------------

    UPROPERTY(meta = (BindWidget))
    UProgressBar* resourcesLine1;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* resourcesLine2;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* resourcesLine3;

	/// State Images --------------------------------------

    UPROPERTY(meta = (BindWidget))
    UImage* stateLine1;

    UPROPERTY(meta = (BindWidget))
    UImage* stateLine2;

    UPROPERTY(meta = (BindWidget))
    UImage* stateLine3;

	/// Setters -------------------------------------------

    UFUNCTION(BlueprintCallable)
    void SetEfficiencyValue(int index, float value);

    UFUNCTION(BlueprintCallable)
    void SetResourceValue(int index, float value);

    UFUNCTION(BlueprintCallable)
    void SetStateColor(const int index, const EProductionLineState state);
};
