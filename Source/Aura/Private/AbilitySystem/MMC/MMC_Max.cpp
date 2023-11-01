// Copyright Benlan.


#include "AbilitySystem/MMC/MMC_Max.h"

#include "Interaction/CombatInterface.h"

UMMC_Max::UMMC_Max()
{
}

float UMMC_Max::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// empty for now just to pass	
	FAggregatorEvaluateParameters EvalParams;
	
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
 
	float AttributeContribution = 0.f;
 
	int32 Index = 0;
	for (FGameplayEffectAttributeCaptureDefinition Attribute : RelevantAttributesToCapture)
	{
		float AttValue = 0.f;
		GetCapturedAttributeMagnitude(Attribute, Spec, EvalParams, AttValue);
		AttValue = FMath::Max<float>(AttValue, 0.f);
		AttributeContribution += (AttributeCoefficients[Index] * AttValue);
		Index++;
	}
 
	// BaseValue + Coeff1*Att1 ... + LevelMult * PlayerLevel
	return BaseValue + AttributeContribution + LevelMultiplier * PlayerLevel;
}
