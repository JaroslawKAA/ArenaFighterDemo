// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_AttackTarget.generated.h"

/**
 * @brief Interface class for attack target functionality.
 *
 * This interface is used to define attack target related methods that can be implemented by any class.
 * It is marked as Blueprintable to allow usage in Unreal Engine's Blueprint visual scripting.
 *
 * @note MinimalAPI tag is used to reduce the compile time by excluding this class from the typical engine API export list.
 */
UINTERFACE(MinimalAPI, NotBlueprintable)
class UCPP_AttackTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface class for attack target functionality.
 *
 * This interface defines the necessary method for handling attacks received by a target.
 * Classes inheriting from this interface should implement the TakeAttack method to process
 * and apply the specified damage from an attack initiated by another character.
 */
class ARENAFIGHTER_API ICPP_AttackTarget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual  void TakeAttack(ACharacter* attacker, float damage) = 0;
};
