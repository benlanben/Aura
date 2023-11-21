// Copyright Benlan.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilityTypes.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	FORCEINLINE virtual FGameplayEffectContext* AllocGameplayEffectContext() const override
	{ return new FAuraGameplayEffectContext(); }
};
