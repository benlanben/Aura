// Copyright Benlan.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void InitAbilityActorInfo() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Player Interface 
	FORCEINLINE virtual void AddToXP_Implementation(const int32 InXP) override { GetAuraPS()->AddToXP(InXP); }
	FORCEINLINE virtual void LevelUp_Implementation() override { MulticastLevelUpParticles(); }
	FORCEINLINE virtual int32 GetXP_Implementation() const override { return GetAuraPS()->GetXP(); }
	FORCEINLINE virtual int32 FindLevelForXP_Implementation(const int32 InXP) const override
	{ return GetAuraPS()->LevelUpInfo->FindLevelForXP(InXP); }
	FORCEINLINE virtual int32 GetAttributePointsReward_Implementation(const int32 Level) const override
	{ return GetAuraPS()->LevelUpInfo->LevelUpInformation[Level].AttributePointAward; }
	FORCEINLINE virtual int32 GetSpellPointsReward_Implementation(const int32 Level) const override
	{ return GetAuraPS()->LevelUpInfo->LevelUpInformation[Level].SpellPointAward; }
	virtual void AddToPlayerLevel_Implementation(const int32 InPlayerLevel) override;
	FORCEINLINE virtual void AddToAttributePoints_Implementation(const int32 InAttributePoints) override
	{ GetAuraPS()->AddToAttributePoints(InAttributePoints); }
	FORCEINLINE virtual void AddToSpellPoints_Implementation(const int32 InSpellPoints) override
	{ GetAuraPS()->AddToSpellPoints(InSpellPoints); }
	FORCEINLINE virtual int32 GetAttributePoints_Implementation() const override { return GetAuraPS()->GetAttributePoints(); }
	FORCEINLINE virtual int32 GetSpellPoints_Implementation() const override { return GetAuraPS()->GetSpellPoints(); }
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	//~ End Player Interface 
	
	//~ Begin Combat Interface
	FORCEINLINE virtual int32 GetCharacterLevel_Implementation() override { return GetAuraPS()->GetCharacterLevel(); }
	//~ End Combat Interface

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;

	FTimerHandle DeathTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	//virtual void OnRep_Stunned() override;
	//virtual void OnRep_Burned() override;

	//void LoadProgress();

protected:
	AAuraPlayerState* GetAuraPS() const;
};
