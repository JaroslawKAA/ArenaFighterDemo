// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_StateBase.h"
#include "CPP_PlayerAttackState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ARENAFIGHTER_API UCPP_PlayerAttackState : public UCPP_StateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attack State")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attack State")
	USkeletalMeshComponent* SkeletalMesh;

private:
	UPROPERTY()
	UAnimInstance* AnimInstance;

	// ThirdPersone* BP_ThirdPersonCharacter;

public:
	virtual void Init(UObject* Context) override;
	virtual void OnEnter() override;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);	
};
