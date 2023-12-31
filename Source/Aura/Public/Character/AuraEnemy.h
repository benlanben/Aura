// Copyright Benlan.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	FORCEINLINE void SetLevel(const int32 InLevel) { Level = InLevel; }
	
	//~ Begin Highlight Interface 
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	FORCEINLINE virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override { /* Do not change OutDestination */ }
	//~ End Highlight Interface 

	//~ Begin Combat Interface
	FORCEINLINE virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual void Die(const FVector& DeathImpulse) override;
	//~ End Combat Interface

	//~ Begin Enemy Interface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override { CombatTarget = InCombatTarget; }
	FORCEINLINE virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	//~ End Enemy Interface
	
	UPROPERTY(BlueprintReadWrite, Category = Combat)
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	bool bHitReacting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float LifeSpan = 10.f;	

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
