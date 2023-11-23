// Copyright Benlan.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAuraAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;
	/*
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
	*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	
public:
	AAuraCharacterBase();
	virtual void Tick(const float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE void SetCharacterClass(const ECharacterClass InClass) { CharacterClass = InClass; }
	
	//~ Combat Interface
	FORCEINLINE virtual UAnimMontage* GetHitReactMontage_Implementation() override { return HitReactMontage; }
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	FORCEINLINE virtual bool IsDead_Implementation() const override { return bDead; }
	FORCEINLINE virtual AActor* GetAvatar_Implementation() override { return this; }
	FORCEINLINE virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override { return AttackMontages; }
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	FORCEINLINE virtual UNiagaraSystem* GetBloodEffect_Implementation() override { return BloodEffect; }
	FORCEINLINE virtual int32 GetMinionCount_Implementation() override { return MinionCount; }
	FORCEINLINE virtual void IncrementMinionCount_Implementation(const int32 Amount) override { MinionCount += Amount; }
	FORCEINLINE virtual ECharacterClass GetCharacterClass_Implementation() override { return CharacterClass; }
	FORCEINLINE virtual USkeletalMeshComponent* GetWeapon_Implementation() override { return Weapon; }
	FORCEINLINE virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override { return OnAscRegistered; }
	FORCEINLINE virtual FOnDeathSignature& GetOnDeathDelegate() override { return OnDeathDelegate; }
	FORCEINLINE virtual FOnDamageSignature& GetOnDamageSignature() override { return OnDamageDelegate; }
	//~ End Combat Interface

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	UFUNCTION()
	virtual void OnRep_Stunned() {}
	UFUNCTION()
	virtual void OnRep_Burned() {}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = Combat)
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = Combat)
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere, Category = Combat)
	FName TailSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo() {}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float BaseWalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UNiagaraSystem* BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	USoundBase* DeathSound;
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	/* Dissolve effects */	
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartMeshDissolveTimeline(UMaterialInstanceDynamic* DynamicMeshMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicWeaponMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	/* Minions */
	int32 MinionCount = 0;
};
