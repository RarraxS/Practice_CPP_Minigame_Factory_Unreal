#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/RandomStream.h"
#include "FactoryLine.generated.h"

///  Enum for states
UENUM(BlueprintType)
enum class EProductionLineState : uint8
{
	Operative UMETA(DisplayName = "Operative"),
	Warning   UMETA(DisplayName = "Warning"),
	Critical  UMETA(DisplayName = "Critical")
};

class AFactoryLine;

/// Delegates -------------------------------------------------------------------------------------

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnProductionChanged,
	AFactoryLine*, FactoryLine,
	float, Efficiency,
	float, Resources,
	EProductionLineState, NewState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProductionSave,
	AFactoryLine*, FactoryLine
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProductionFail,
	AFactoryLine*, FactoryLine
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProductionReset,
	AFactoryLine*, FactoryLine
);

// ------------------------------------------------------------------------------------------------

UCLASS()
class FINALTASKRAMONZABARA_API AFactoryLine : public APawn
{
	GENERATED_BODY()

public:
	/// Sets default values for this pawn's properties
	AFactoryLine();

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:	
	/// Called every frame
	virtual void Tick(float deltaTime) override;

	/// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Main values ------------------------------------------------------------------------------------- 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Factory")
	EProductionLineState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float efficiency = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float resources = 100.f;

	/// State Switching thressholds ---------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float maxValue = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float warningThresshold = 74.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float criticalThresshold = 50.f;

	/// Reducing value ----------------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	float maxDecreasingValue = 5.f;
	
	/// Delegates ---------------------------------------------------------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "Factory|Delegates")
	FOnProductionChanged OnProductionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Factory|Delegates")
	FOnProductionSave OnProductionSave;
	
	UPROPERTY(BlueprintAssignable, Category = "Factory|Delegates")
	FOnProductionFail OnProductionFail;

	UPROPERTY(BlueprintAssignable, Category = "Factory|Delegates")
	FOnProductionReset OnProductionReset;

	/// Getters -----------------------------------------------------------------------------------------

	float GetEfficiency() const { return efficiency; }
	float GetResources()  const { return resources; }
	EProductionLineState GetState() const { return state; }
	
	/// Main functions -----------------------------------------------------------------------------------------

	UFUNCTION()
	void SaveLine();

	UFUNCTION()
	void FailLine();

	UFUNCTION()
	void ResetLine();

private:
	/// Internal state
	bool running;
	bool active;

	/// Random seed for getting different values with each instance
	FRandomStream randomStream;

	/// Thread simulation
	void StartSimulation();
	void StopSimulation();

	/// Simulation logic
	void EvaluateState();
};