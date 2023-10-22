// Copyright Benlan.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Max.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Max : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	// Multiplication Coefficients matching captured definitions 1 for 1.
	UPROPERTY(EditAnywhere)
	TArray<float> AttributeCoefficients;
 
	UPROPERTY(EditAnywhere)
	float BaseValue = 1.f;
 
	UPROPERTY(EditAnywhere)
	float LevelMultiplier = 10.f;

public:
	UMMC_Max();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
