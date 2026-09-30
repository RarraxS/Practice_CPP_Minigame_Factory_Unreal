#include "FactoryWidget.h"

/// Update the line's params
void UFactoryWidget::HandleFactoryLineUpdated(
    int                  lineIndex,
    float                efficiency,
    float                resources,
    EProductionLineState state
)
{
    SetEfficiencyValue(lineIndex + 1, efficiency);
    SetResourceValue(lineIndex + 1, resources);
    SetStateColor(lineIndex + 1, state);
}

/// Set efficiency value for the specified line index (1-based)
void UFactoryWidget::SetEfficiencyValue(int index, float value)
{
    value = FMath::Clamp(value, 0.f, 1.f);

    switch (index)
    {
    case 1: if (efficiencyLine1) efficiencyLine1->SetPercent(value); break;
    case 2: if (efficiencyLine2) efficiencyLine2->SetPercent(value); break;
    case 3: if (efficiencyLine3) efficiencyLine3->SetPercent(value); break;
    }
}

/// Set resource value for the specified line index (1-based)
void UFactoryWidget::SetResourceValue(int index, float value)
{
    value = FMath::Clamp(value, 0.f, 1.f);

    switch (index)
    {
    case 1: if (resourcesLine1) resourcesLine1->SetPercent(value); break;
    case 2: if (resourcesLine2) resourcesLine2->SetPercent(value); break;
    case 3: if (resourcesLine3) resourcesLine3->SetPercent(value); break;
    }
}

/// Set state image color for the specified line index (-1)
void UFactoryWidget::SetStateColor(const int index, const EProductionLineState state)
{
    FLinearColor color;

    switch (state)
    {
    case EProductionLineState::Operative: color = FLinearColor::Green;  break;
    case EProductionLineState::Warning:   color = FLinearColor::Yellow; break;
    case EProductionLineState::Critical:  color = FLinearColor::Red;    break;
    }

    switch (index)
    {
    case 1: if (stateLine1) stateLine1->SetColorAndOpacity(color); break;
    case 2: if (stateLine2) stateLine2->SetColorAndOpacity(color); break;
    case 3: if (stateLine3) stateLine3->SetColorAndOpacity(color); break;
    }
}

