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
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetCharacterLevel();
 
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
