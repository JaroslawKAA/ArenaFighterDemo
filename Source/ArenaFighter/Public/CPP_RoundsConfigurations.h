// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_RoundsConfig.h"
#include "CPP_RoundsConfigurations.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCPP_RoundsConfigurations
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCPP_RoundsConfig> Configurations;
};

UCLASS(BlueprintType)
class ARENAFIGHTER_API U_CPP_RoundsConfigurations : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCPP_RoundsConfigurations Data;
};
