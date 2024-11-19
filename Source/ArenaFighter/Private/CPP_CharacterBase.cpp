// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterBase.h"

#include "Kismet/GameplayStatics.h"

const FString ACPP_CharacterBase::HandSockedName = TEXT("ik_hand_rSocket");

bool ACPP_CharacterBase::IsDead()
{
	if(Health <= 0) return true;

	return false;
}

// Sets default values
ACPP_CharacterBase::ACPP_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectedPawn = nullptr;
}

// Called when the game starts or when spawned
void ACPP_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing = FindComponentByClass<UPawnSensingComponent>();
	if (PawnSensing)
		PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_CharacterBase::OnSeePawn);
	else
		UE_LOG(LogTemp, Error, TEXT("No PawnSensingComponent found!"))

	EquipSelectedWeapon();

	GetWorldTimerManager().SetTimer(CheckSightTimerHandle, this, &ACPP_CharacterBase::CheckForLostSight, 0.5f, true);

	OnTakeAnyDamage.AddDynamic(this, &ACPP_CharacterBase::HandleAnyDamage);
}

// Called every frame
void ACPP_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SelectedPawn && !IsDead())
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation() + SelectedPawnArrowOffset,
		                          SelectedPawn->GetActorLocation() + SelectedPawnArrowOffset,
		                          5, SelectedItemArrowColor, false, 0, 2.0f);
}

void ACPP_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(EquippedWeapon->IsValidLowLevel())
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	if (PawnSensing)
		PawnSensing->OnSeePawn.RemoveDynamic(this, &ACPP_CharacterBase::OnSeePawn);

	GetWorldTimerManager().ClearTimer(CheckSightTimerHandle);
}

void ACPP_CharacterBase::ChangeWeapon(float actionValue)
{
	if (actionValue > 0)
		NextWeapon();

	if (actionValue < 0)
		PrevWeapon();
}

// Called to bind functionality to input
void ACPP_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPP_CharacterBase::NextWeapon()
{
	CurrentWeaponIndex++;
	if (CurrentWeaponIndex > Weapons.Num() - 1)
		CurrentWeaponIndex = 0;

	EquipSelectedWeapon();
}

void ACPP_CharacterBase::PrevWeapon()
{
	CurrentWeaponIndex--;
	if (CurrentWeaponIndex < 0)
		CurrentWeaponIndex = Weapons.Num() - 1;

	EquipSelectedWeapon();
}

void ACPP_CharacterBase::OnSeePawn(APawn* DetectedPawn)
{
	if (DetectedPawn)
	{
		DetectedPawns.Add(DetectedPawn);
		UE_LOG(LogTemp, Log, TEXT("Pawn added: %s"), *DetectedPawn->GetName());
		TrySelectPawn();
		OnDetectedPawnsChanged();
	}
}

void ACPP_CharacterBase::CheckForLostSight()
{
	if(IsDead()) return;
	
	bool bWasAnyPawnRemoved = false;

	for (TSet<APawn*>::TIterator it = DetectedPawns; it; ++it)
	// for (APawn* Pawn : DetectedPawns)
	{
		APawn* Pawn = *it;
		if (!Pawn || !PawnSensing->CouldSeePawn(Pawn))
		{
			UE_LOG(LogTemp, Log, TEXT("Stopped seeing Pawn: %s"), *Pawn->GetName());
			DetectedPawns.Remove(Pawn);
			bWasAnyPawnRemoved = true;
		}
	}

	if (bWasAnyPawnRemoved || DetectedPawns.IsEmpty())
	{
		TrySelectPawn();
		OnDetectedPawnsChanged();
	}
}

void ACPP_CharacterBase::TakeAttack(ACharacter* attacker, float damage)
{
	if(!IsDead())
		UGameplayStatics::ApplyDamage(this, damage, GetController(), attacker, UDamageType::StaticClass());
}

void ACPP_CharacterBase::TrySelectPawn()
{
	APawn* ClosestPawn = nullptr;
	float ClosestDistance = FLT_MAX;
	float MaxDotProduct = -FLT_MAX;

	FVector CharacterLocation = GetActorLocation();
	FVector CharacterForward = GetActorForwardVector();

	for (APawn* DetectedPawn : DetectedPawns)
		if (DetectedPawn)
		{
			ACPP_CharacterBase* CharacterBase = Cast<ACPP_CharacterBase>(DetectedPawn);
			if(CharacterBase->IsDead())
				continue;

			// Calculate the vector from the character to the detected pawn
			FVector DirectionToPawn = DetectedPawn->GetActorLocation() - CharacterLocation;

			// Calculate distance to the pawn
			float DistanceToPawn = DirectionToPawn.Size();

			// Calculate the dot product to determine how "in front" the pawn is
			float DotProduct = FVector::DotProduct(CharacterForward, DirectionToPawn.GetSafeNormal());

			// Check if this pawn is closer and more in front than the previous one
			if (DotProduct > 0.0f // Pawn must be in front
				&& DotProduct > MaxDotProduct
				&& DistanceToPawn < ClosestDistance)
			{
				MaxDotProduct = DotProduct;
				ClosestPawn = DetectedPawn;
				ClosestDistance = DistanceToPawn;
			}
		}

	// Set the SelectedPawn to the closest and most in front pawn
	if (ClosestPawn->IsValidLowLevel())
	{
		if (ClosestPawn != SelectedPawn)
		{
			SelectedPawn = ClosestPawn;
			UE_LOG(LogTemp, Log, TEXT("Selected Pawn: %s"), *SelectedPawn->GetName());
			OnSelectedPawnChanged();
		}
	}
	else
	{
		if (SelectedPawn != nullptr)
		{
			SelectedPawn = nullptr; // No valid pawn found
			OnSelectedPawnChanged();
		}
	}
}

void ACPP_CharacterBase::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                         AController* InstigatedBy, AActor* DamageCauser)
{
	AddHealth(-Damage);
	UE_LOG(LogTemp, Log, TEXT("%s - Applied damage: %f - Caster: %s"),
	       *DamagedActor->GetName(), Damage, *DamageCauser->GetName());

	if (Health <= 0) Die();
}

void ACPP_CharacterBase::Die()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Die"));
	OnDie();
	OnDieDispatcher.Broadcast();
}

void ACPP_CharacterBase::AddHealth(float add)
{
	Health += add;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	OnHealthChanged(Health);
}


void ACPP_CharacterBase::EquipSelectedWeapon()
{
	// Destroy previous weapon
	if (EquippedWeapon && EquippedWeapon->IsValidLowLevel())
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	// Spawn current weapon indicated by CurrentWeaponIndex
	if (Weapons.Num() > 0 && Weapons[CurrentWeaponIndex])
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACPP_Weapon* equippedWeapon = GetWorld()->SpawnActor<ACPP_Weapon>(
			Weapons[CurrentWeaponIndex], spawnParameters);

		if (equippedWeapon)
		{
			USkeletalMeshComponent* StaticMeshComponent = GetMesh();
			equippedWeapon->AttachToComponent(
				StaticMeshComponent,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName(HandSockedName)
			);
			EquippedWeapon = equippedWeapon;
		}
	}
}
