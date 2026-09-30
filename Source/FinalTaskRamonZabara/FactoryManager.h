#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FactoryLine.h"
#include "FactoryWidget.h"
#include "GameOverWidget.h"
#include "EmergencyPanelWidget.h"
#include "FactoryManager.generated.h"

/// Delegates -------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnFactoryLineUpdated,
	int,   lineIndex,
	float, efficiency,
	float, resources,
	EProductionLineState, state
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetGame);

UCLASS()
class FINALTASKRAMONZABARA_API AFactoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/// Sets default values for this actor's properties
	AFactoryManager();

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Delegates ---------------------------------------------------------

	UFUNCTION()
	void HandleProductionStateChanged(
		AFactoryLine* factoryLine,
		float efficiency,
		float resources,
		EProductionLineState state
	);

	UFUNCTION()
	void SaveEmergency(AFactoryLine* factoryLine);

	UFUNCTION()
	void FailEmergency(AFactoryLine* factoryLine);

	UFUNCTION()
	void ResetEmergency();

	/// UI References -----------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UFactoryWidget> factoryWidgetClass;	

	/// Widget references -------------------------------------------------
	UPROPERTY()
	UFactoryWidget* factoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UEmergencyPanelWidget> emergencyPanelWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGameOverWidget> gameOverWidgetClass;

public:	
	/// Called every frame
	virtual void Tick(float deltaTime) override;

	/// Delegates ---------------------------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "Factory")
	FOnFactoryLineUpdated OnFactoryLineUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Factory")
	FOnResetGame OnResetGame;

private:
	/// Info storage arrays

	TArray<AActor*> foundLines;
	TArray<bool> emergencyActive;
	TArray<bool> failedLines;
	
	float score;
};
