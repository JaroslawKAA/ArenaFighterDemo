// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterBase.h"
#include "CPP_EnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class ARENAFIGHTER_API ACPP_EnemyCharacterBase : public ACPP_CharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackTargetRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float SecondsToLostTarget = 5.0f;
};
