// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_EnemyCharacterBase.h"
#include "CPP_RoundsConfig.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCPP_RoundsConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LevelsSpan = FIntPoint(1, 2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ACPP_EnemyCharacterBase>> Enemies;
};

UCLASS(BlueprintType)
class ARENAFIGHTER_API U_CPP_RoundsConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCPP_RoundsConfig Data;
};
