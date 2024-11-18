// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerAttackState.h"

void UCPP_PlayerAttackState::Init(UObject* Context)
{
	Super::Init(Context);
	AnimInstance = SkeletalMesh->GetAnimInstance();
}

void UCPP_PlayerAttackState::OnEnter()
{
	Super::OnEnter();
	float animMontageLenght = AnimInstance->Montage_Play(AnimMontage);

	if (animMontageLenght <= 0.0f)
	{
		FString outputMessage = TEXT("Failed to play attack anim montage");
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, outputMessage);
		UE_LOG(LogTemp, Error, TEXT("%s"), *outputMessage);
		return;
	}

	AnimInstance->OnMontageEnded.AddDynamic(this, &UCPP_PlayerAttackState::OnMontageEnded);
}

void UCPP_PlayerAttackState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == AnimMontage)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &UCPP_PlayerAttackState::OnMontageEnded);
		// TODO to locomotion state
	}
}
