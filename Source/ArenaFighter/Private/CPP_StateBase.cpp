// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StateBase.h"

void UCPP_StateBase::Init(UObject* Context)
{
	StateName = this->GetClass()->GetDisplayNameText();
	this->OwnerContext = Context;
	OnInitEvent();
}

void UCPP_StateBase::OnEnter()
{
	FString outputMessage = TEXT("State.OnEnter - " + StateName.ToString());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, outputMessage);
	UE_LOG(LogTemp, Log, TEXT("%s"), *outputMessage);
	OnEnterEvent();
}

void UCPP_StateBase::OnTick(float deltaTime)
{
	OnTickEvent(deltaTime);
}

void UCPP_StateBase::OnExit()
{
	FString outputMessage = TEXT("State.OnExit - " + StateName.ToString());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, outputMessage);
	UE_LOG(LogTemp, Log, TEXT("%s"), *outputMessage);
	OnExitEvent();
}
