// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_StateBase.h"
#include "UObject/NoExportTypes.h"
#include "CPP_StateMachineBase.generated.h"

/**
 * @class UCPP_StateMachineBase
 *
 * @brief State pattern. A base class for state machine implementations to manage states of objects in Unreal Engine.
 *
 * This class provides the foundational elements required to implement state machines.
 * Derive from this class to create specific state machine behaviors.
 *
 * @details
 * UCPP_StateMachineBase serves as a base for all state machines, offering basic
 * functionalities like state transition handling, initialization, and update loops.
 * It encapsulates state management logic, ensuring derived classes only need to
 * implement state-specific behaviors.
 *
 * Users are expected to extend this class and override virtual methods to provide custom
 * state transitions and behaviors. The state machine maintains and transitions between
 * different states based on the logic defined in the derived classes.
 */
UCLASS(Blueprintable, BlueprintType)
class ARENAFIGHTER_API UCPP_StateMachineBase : public UObject
{
	GENERATED_BODY()

	protected:
	/**
	 * @brief A pointer to the current state of the state machine.
	 *
	 * This variable holds the reference to an instance of UCPP_StateBase,
	 * representing the active state of the state machine. Transitioning
	 * to a new state involves updating this pointer.
	 *
	 * @details
	 * The CurrentState variable is crucial for state machine operations
	 * as it determines which state's logic should be executed at any given
	 * moment. When a state change occurs, the state machine updates this
	 * pointer to reflect the new active state, ensuring that subsequent
	 * operations are delegated to the appropriate state instance.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	UCPP_StateBase* CurrentState;

	/**
	 * @brief The context or owner of the state machine.
	 *
	 * This variable holds a reference to the UObject that owns or is associated with
	 * the state machine. It is used to provide context or additional information
	 * that particular states within the state machine might need to operate.
	 *
	 * @details
	 * The OwnerContext is particularly useful when states require access to
	 * properties or functions of the owning object. During the initialization
	 * of the state machine, it can be set to encapsulate relevant contextual
	 * details that are necessary for various state transitions and operations.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	UObject* OwnerContext;
	
	public:
	UCPP_StateMachineBase();

	/**
	 * @brief Event trigger for initialization of the state machine.
	 *
	 * This method is designed to be overridden in Blueprints to provide custom
	 * initialization logic for the state machine. When the `Init` function of
	 * `UCPP_StateMachineBase` is called, this event gets triggered, allowing
	 * Blueprint scripts to execute any necessary actions during the initialization phase.
	 *
	 * @details
	 * The `OnInitEvent` provides a hook in the state machine's initialization
	 * process where Blueprint-specific initialization logic can be placed.
	 * Derived Blueprint classes can implement this event to execute code that
	 * should run when the state machine is being initialized.
	 *
	 * Usage:
	 * - Override this method in a Blueprint derived from `UCPP_StateMachineBase` to
	 *   set up custom initialization steps.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnInitEvent();

	/**
	 * @brief Event trigger for updating the state machine each tick.
	 *
	 * This method is intended to be overridden in Blueprints to provide custom logic
	 * that should run every tick/frame. The `OnTickEvent` method gets triggered with
	 * the `deltaTime` parameter, allowing Blueprint scripts to execute time-based actions.
	 *
	 * @param deltaTime The time elapsed since the last tick/frame update.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnTickEvent(float deltaTime);

	/**
	 * @brief Event triggered when the state of the state machine changes.
	 *
	 * This BlueprintImplementableEvent is intended to allow Blueprints to respond to state
	 * changes in the state machine. When the `SetState` method is called, this event gets
	 * triggered, enabling custom logic to execute during a state transition.
	 *
	 * @details
	 * Override this event in a Blueprint derived from `UCPP_StateMachineBase` to define
	 * specific actions or effects that should occur whenever the state of the state machine
	 * changes. This event can be used to update UI elements, trigger animations, or perform
	 * any other necessary operations in response to a state change.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnStateChangedEvent();

	UFUNCTION(Blueprintable, BlueprintCallable, Category = "State")
	virtual void Init(UObject* Context);
	
	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void OnTick(float deltaTime);

	protected:
	/**
	 * @brief Sets the current state of the state machine.
	 *
	 * This method changes the active state to the provided new state. It first exits
	 * the current state (if valid), updates the current state to the new state, and
	 * then enters the new state (if valid). Additionally, it triggers the
	 * `OnStateChangedEvent` to notify about the state change.
	 *
	 * @param NewState A pointer to the new state that the state machine should transition to.
	 */
	UFUNCTION(Blueprintable, BlueprintCallable)
	void SetState(UPARAM() UCPP_StateBase* NewState);
};
