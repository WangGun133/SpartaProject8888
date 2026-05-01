// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapBase.h"
#include "MyCharacter.h"

// Sets default values
ATrapBase::ATrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrapBase::ActivateTrap()
{
	bActive = true;
	SetActorTickEnabled(true);
}

void ATrapBase::DeactivateTrap()
{
	bActive = false;
	SetActorTickEnabled(false);
}

bool ATrapBase::ApplyTrapDamage(AActor* Target)
{
	if (!bActive || !IsValid(Target))
	{
		return false;
	}

	if (AMyCharacter* TargetCharacter = Cast<AMyCharacter>(Target))
	{
		return TargetCharacter->DieFromTrap(this);
	}

	return false;
}

// Called every frame
void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

