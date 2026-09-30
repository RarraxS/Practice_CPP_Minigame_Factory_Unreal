#include "FactoryLine.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Math/RandomStream.h"
#include "Misc/ScopeLock.h"

/// Sets default values
AFactoryLine::AFactoryLine()
{
 	// Set this pawn to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	efficiency = 100.f;
	resources = 100.f;
	state = EProductionLineState::Operative;

	running = true;
    active  = true;

	// Seed for the random values
	randomStream.Initialize(static_cast<int32>(FPlatformTime::Cycles64() ^ reinterpret_cast<uintptr_t>(this)));
}

/// Called when the game starts or when spawned
void AFactoryLine::BeginPlay()
{
	Super::BeginPlay();

    StartSimulation();
}

/// Called when the game ends or when despawned
void AFactoryLine::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    running = false;
    Super::EndPlay(endPlayReason);
}

// Called every frame
void AFactoryLine::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

/// Called to bind functionality to input
void AFactoryLine::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
}

/// Called to start the multithreading simulation
void AFactoryLine::StartSimulation()
{
    // Save weak pointer to avoid issues if the actor is destroyed while the async task is running
    TWeakObjectPtr<AFactoryLine> safeThis(this);

    // Thread async task
    Async(EAsyncExecution::Thread, [safeThis]()
    {
        while (safeThis.IsValid() && safeThis->running)
        {
            FPlatformProcess::Sleep(1.f);

            float deltaEfficiency = 0.f;
            float deltaResources  = 0.f;

            if (safeThis.IsValid() && safeThis.Get()->active)
            {
                // Get the random values for the line
                const float minRange = -0.5f * safeThis->maxDecreasingValue;
                const float maxRange = safeThis->maxDecreasingValue;

                // Use the randomStream so different instances produce different outputs
                deltaEfficiency = safeThis->randomStream.FRandRange(minRange, maxRange);
                deltaResources  = safeThis->randomStream.FRandRange(minRange, maxRange);
            }

			// Back to main thread
            AsyncTask(ENamedThreads::GameThread, [safeThis, deltaEfficiency, deltaResources]()
            {
                if (!safeThis.IsValid()) return;

                AFactoryLine* self = safeThis.Get();
                        
                self->efficiency = FMath::Clamp(self->efficiency - deltaEfficiency, 0.f, 100.f);
                self->resources  = FMath::Clamp(self->resources  - deltaResources,  0.f, 100.f);

                self->EvaluateState();

                if (GEngine)
                    GEngine->AddOnScreenDebugMessage(
                        -1,
                        1.f,
                        FColor::Cyan,
                        FString::Printf(
                            TEXT("%s | Efficiency: %.1f | Resources: %.1f | State: %s"),
                            *self->GetName(),
                            self->efficiency,
                            self->resources,
                            *UEnum::GetValueAsString(self->state)
                        )
                    );
            });
        }
    });
}

/// Evaluates the state of the line based on efficiency and resources and broadcasts the change
void AFactoryLine::EvaluateState()
{
    // Evaluate the state of the factory line based on efficiency and resources

    EProductionLineState previousState = state;

    if (efficiency <= criticalThresshold || resources <= criticalThresshold)
        state = EProductionLineState::Critical;

    else if (efficiency < warningThresshold || resources < warningThresshold)
        state = EProductionLineState::Warning;

    else
        state = EProductionLineState::Operative;

    OnProductionChanged.Broadcast(this, efficiency/maxValue, resources/maxValue, state);
}

/// Saving the line
void AFactoryLine::SaveLine()
{
    efficiency = maxValue * 0.8f;
    resources  = maxValue * 0.8f;
    state = EProductionLineState::Operative;

    // Notify change
    OnProductionChanged.Broadcast(this, efficiency/maxValue, resources/maxValue, state);
    OnProductionSave.Broadcast(this);
}

/// Fail to save the line
void AFactoryLine::FailLine()
{
    active = false;
    OnProductionFail.Broadcast(this);
}

/// Resets the line after a Game Over
void AFactoryLine::ResetLine()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor::Green,
            FString::Printf(
                TEXT("Line restarted")
            )
        );

    efficiency = maxValue;
    resources = maxValue;
    state = EProductionLineState::Operative;

    active = true;

    // Notify change
    OnProductionChanged.Broadcast(this, efficiency / maxValue, resources / maxValue, state);
    OnProductionReset.Broadcast(this);
}