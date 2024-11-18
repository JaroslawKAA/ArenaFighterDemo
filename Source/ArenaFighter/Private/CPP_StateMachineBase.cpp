// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StateMachineBase.h"


UCPP_StateMachineBase::UCPP_StateMachineBase()
{
}

void UCPP_StateMachineBase::Init(UObject* Context)
{
	this->OwnerContext = Context;
	OnInitEvent();
}

void UCPP_StateMachineBase::OnTick(float deltaTime)
{
	if (CurrentState && CurrentState->IsValidLowLevel())
		CurrentState->OnTick(deltaTime);
	OnTickEvent(deltaTime);
}

void UCPP_StateMachineBase::SetState(UCPP_StateBase* NewState)
{
	if (CurrentState == NewState) return;

	if (CurrentState && CurrentState->IsValidLowLevel())
		CurrentState->OnExit();

	CurrentState = NewState;
	OnStateChangedEvent();

	if (CurrentState && CurrentState->IsValidLowLevel())
		CurrentState->OnEnter();
}
