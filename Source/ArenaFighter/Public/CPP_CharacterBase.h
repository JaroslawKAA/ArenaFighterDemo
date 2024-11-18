// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_AttackTarget.h"
#include "CPP_Weapon.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "CPP_CharacterBase.generated.h"

/**
 * ACPP_CharacterBase defines the base character class in the Arena Fighter game.
 * This class manages character attributes like health, weapon handling, and related events.
 */
UCLASS()
class ARENAFIGHTER_API ACPP_CharacterBase : public ACharacter, public ICPP_AttackTarget
{
	GENERATED_BODY()


protected:
	/**
	 * HandSockedName defines the socket name used for attaching weapons to the character's hand.
	 * It is used during the weapon equip process to correctly position the weapon.
	 */
	static const FString HandSockedName;

	/**
	 * Health represents the current health value of the character.
	 * Configurable in the editor and accessible within Blueprints.
	 * Default value is set to 100.0f.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Health = 100.0f;

	/**
	 * MaxHealth defines the maximum health value a character can have.
	 * Configurable in the editor and accessible within Blueprints.
	 * Default value is set to 100.0f.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.0f;

	/**
	 * EquippedWeapon is a pointer to the weapon currently equipped by the character.
	 * This variable is configurable in the editor and accessible within Blueprints under the "Weapon" category.
	 * It is used to manage the character's currently equipped weapon, including attaching, detaching, and updating weapon-specific behavior.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ACPP_Weapon* EquippedWeapon;

	/**
	 * Weapons is an array that holds different types of weapon classes available to the character.
	 * Configurable in the editor and accessible within Blueprints, this array is used to manage and switch between various weapons the character can equip.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ACPP_Weapon>> Weapons;

	/**
	 * CurrentWeaponIndex keeps track of the index of the weapon currently equipped by the character.
	 * This variable is configurable in the editor and accessible within Blueprints under the "Weapon" category.
	 * Default value is set to 0.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
	UPawnSensingComponent* PawnSensing = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
	TSet<APawn*> DetectedPawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
	APawn* SelectedPawn;

	UPROPERTY(EditAnywhere, Category = "Sensing")
	FColor SelectedItemArrowColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "Sensing")
	FVector SelectedPawnArrowOffset = FVector::ZeroVector;
	
	FTimerHandle CheckSightTimerHandle;

public:
	// EVENTS
	
	/**
	 * OnApplyAttackDamage is a Blueprint event that gets called when an attack, received from anim notice.
	 * This function can be implemented in Blueprints to define custom behavior when character applying attack damage to opponent.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notifies")
	void OnApplyAttackDamage();

	/**
	 * OnBlockAttackCanceling is a Blueprint event that gets triggered when character can't cancel an attack, received from anim notice.
	 * This function can be implemented in Blueprints to define custom behavior when the canceling attack is not possible.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notifies")
	void OnBlockAttackCanceling();

	/**
	 * OnUnblockAttackCanceling is a Blueprint event for handling the scenario when an attack can be canceled.
	 * This function can be implemented in Blueprints to define custom behavior when unblocked canceling attack before animation completed.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notifies")
	void OnUnblockAttackCanceling();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sensing")
	void OnDetectedPawnsChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sensing")
	void OnSelectedPawnChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character State")
	void OnDie();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void OnHealthChanged(float InHealth);
	
	// METHODS

	// Sets default values for this character's properties
	ACPP_CharacterBase();

	/**
	 * ChangeWeapon is used to switch the currently equipped weapon based on the provided action value.
	 * If the action value is positive, it switches to the next weapon in the inventory.
	 * If the action value is negative, it switches to the previous weapon.
	 *
	 * @param actionValue The value used to determine which direction to switch weapons. Positive values move to the next weapon, negative values move to the previous weapon.
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(float actionValue);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Character State")
	bool IsDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Tries to select the most appropriate pawn for interaction based on its proximity and position relative to the character.
	 *
	 * This function iterates over a list of detected pawns, evaluates each pawn's distance and angle relative to the character,
	 * and selects the one that is both closest and most in front of the character. If no suitable pawn is found, it resets the selected pawn to null.
	 *
	 * Criteria for selection:
	 * - The pawn must be located in front of the character.
	 * - The pawn must be closest to the character.
	 * - The pawn must have the highest dot product value, indicating it is more directly in front.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sensing")
	void TrySelectPawn();
	
	UFUNCTION()
	virtual void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	void Die();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddHealth(float add);

private:
	/**
	 * EquipSelectedWeapon handles the process of equipping a new weapon for the character.
	 * It first destroys the currently equipped weapon if it exists, and then spawns and attaches
	 * the new weapon from the character's weapon inventory based on the CurrentWeaponIndex.
	 */
	void EquipSelectedWeapon();

	/**
	 * NextWeapon is used to cycle to the next weapon in the character's weapons inventory.
	 * When the end of the weapon list is reached, it loops back to the first weapon.
	 * This method updates the CurrentWeaponIndex and equips the selected weapon.
	 */
	void NextWeapon();


	/**
	 * PrevWeapon is used to cycle to the previous weapon in the character's weapons inventory.
	 * When the beginning of the weapon list is reached, it loops back to the last weapon.
	 * This method updates the CurrentWeaponIndex and equips the selected weapon.
	 */
	void PrevWeapon();

	/**
	 * OnSeePawn is called when the character detects another pawn.
	 * This function can be implemented to define custom behavior upon detecting other pawns in the game world.
	 *
	 * @param DetectedPawn The pawn that has been detected by the character.
	 */
	UFUNCTION()
	void OnSeePawn(APawn* DetectedPawn);

	/**
	 * Checks for any pawns that the character has lost sight of and updates the list of detected pawns accordingly.
	 * If any pawns are removed from the detection list, it attempts to select a new target.
	 */
	void CheckForLostSight();

public:
	virtual void TakeAttack(ACharacter* attacker, float damage) override;
};
