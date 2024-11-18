// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Weapon.generated.h"

/**
 * ACPP_Weapon is a class representing a weapon in the game.
 * It includes attributes like AttackRange, AttackSpeed, and Damage.
 * This class is derived from AActor.
 */
UCLASS(Blueprintable, BlueprintType)
class ARENAFIGHTER_API ACPP_Weapon : public AActor
{
	GENERATED_BODY()

protected:
	/**
	 * AttackRange represents the range within which the weapon can effectively attack an opponent.
	 * Configurable in the editor and accessible within Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float AttackRange = 1;

	/**
	 * AttackSpeed defines how fast the weapon can perform consecutive attacks.
	 * Configurable in the editor and accessible within Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float AttackSpeed = 1;

	/**
	 * Damage represents the amount of damage this weapon deals to opponents.
	 * Configurable in the editor and accessible within Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float AttackRangeMargin = 20.0f;


public:	
	// Sets default values for this actor's properties
	ACPP_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
