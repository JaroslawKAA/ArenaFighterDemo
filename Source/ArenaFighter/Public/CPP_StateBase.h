// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPP_StateBase.generated.h"

/**
 * @class UCPP_StateBase
 * @brief State pattern. Represents the base class for all state objects in the state machine.
 *
 * This class serves as the foundation for creating different states within a
 * state machine. It provides the basic interface that all state classes must
 * implement to ensure they can transition and handle events correctly.
 *
 * Derived classes should override the `OnEnter` and `OnExit` methods to define behavior
 * that occurs upon entering and exiting the state, respectively. Additionally,
 * the `OnTick` method should be overridden to define how the state behaviour
 * during each frame.
 */
UCLASS(Blueprintable, BlueprintType)
class ARENAFIGHTER_API UCPP_StateBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	FText StateName;

public:
	/**
	 * @brief The context owner for the state instance.
	 *
	 * This UObject contains contextual information that the state may need to access or modify.
	 * It is provided during the initialization phase and is intended to be used throughout the
	 * lifecycle of the state to reference or interact with external data and systems that are
	 * relevant to the current state.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	UObject* OwnerContext;

	/**
	 * @brief Event triggered during the initialization phase of the state.
	 *
	 * This method is intended to be overridden in Blueprint to provide custom initialization
	 * logic specific to the state. It is automatically called when the state is initialized
	 * via the `Init` method, allowing for any necessary setup or preparation to be done.
	 *
	 * Custom behavior defined in Blueprints can involve setting up default values, binding
	 * events, or any other preparatory steps that are essential for the state.
	 *
	 * It is critical to ensure that any required context provided to the state is utilized
	 * during this phase.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnInitEvent();

	/**
	 * @brief Event triggered when entering a state.
	 *
	 * This method is designed to be overridden in Blueprint to provide custom logic
	 * that should execute upon entering a new state. When the state transition occurs,
	 * this event method will be called, allowing for initialization, setup, or any other
	 * necessary actions to be performed.
	 *
	 * Implement this event in Blueprint to define behaviors specific to the entry of
	 * the state, such as updating UI, playing animations, initializing resources, etc.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnEnterEvent();

	/**
	 * @brief Event triggered on each tick update.
	 *
	 * This method is designed to be overridden in Blueprint to provide custom logic
	 * that should execute on each tick, allowing for continuous state-specific processing.
	 * The `deltaTime` parameter represents the time elapsed since the last tick, enabling
	 * time-sensitive calculations and operations.
	 *
	 * @param deltaTime The elapsed time since the last tick.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnTickEvent(float deltaTime);

	/**
	 * @brief Event triggered when exiting a state.
	 *
	 * This method is designed to be overridden in Blueprint to provide custom logic
	 * that should execute upon exiting a state. When the state transition occurs,
	 * this event method will be called, allowing for cleanup, resource deallocation,
	 * or any other necessary actions to be performed.
	 *
	 * Implement this event in Blueprint to define behaviors specific to the exit of
	 * the state, such as removing UI elements, stopping animations, or resetting
	 * variables to their default states.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnExitEvent();

	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void Init(UObject* Context);

	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void OnEnter();

	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void OnTick(float deltaTime);

	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void OnExit();
};
