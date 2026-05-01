// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingSpikeTrap.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"

AMovingSpikeTrap::AMovingSpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	bInstantKill = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
	DamageCollision->SetupAttachment(SceneRoot);
	DamageCollision->SetBoxExtent(DamageCollisionExtent);
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageCollision->SetCollisionObjectType(ECC_WorldDynamic);
	DamageCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DamageCollision->SetGenerateOverlapEvents(true);
}

void AMovingSpikeTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (DamageCollision)
	{
		DamageCollision->SetBoxExtent(DamageCollisionExtent);
	}
}

void AMovingSpikeTrap::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (DamageCollision)
	{
		DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMovingSpikeTrap::OnDamageCollisionBeginOverlap);
		DamageCollision->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void AMovingSpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActive || MoveSpeed <= 0.0f || MoveDistance <= 0.0f || LocalMoveDirection.IsNearlyZero())
	{
		return;
	}

	CurrentMoveOffset += MoveDirection * MoveSpeed * DeltaTime;

	if (CurrentMoveOffset > MoveDistance)
	{
		CurrentMoveOffset = MoveDistance;
		MoveDirection = -1.0f;
	}
	else if (CurrentMoveOffset < -MoveDistance)
	{
		CurrentMoveOffset = -MoveDistance;
		MoveDirection = 1.0f;
	}

	const FVector WorldMoveDirection = GetActorTransform().TransformVectorNoScale(LocalMoveDirection.GetSafeNormal());
	SetActorLocation(StartLocation + WorldMoveDirection * CurrentMoveOffset);
}

void AMovingSpikeTrap::ActivateTrap()
{
	Super::ActivateTrap();

	if (DamageCollision)
	{
		DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMovingSpikeTrap::DeactivateTrap()
{
	Super::DeactivateTrap();

	if (DamageCollision)
	{
		DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AMovingSpikeTrap::ApplyTrapDamage(AActor* Target)
{
	return Super::ApplyTrapDamage(Target);
}

void AMovingSpikeTrap::OnDamageCollisionBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	(void)OverlappedComponent;
	(void)OtherBodyIndex;
	(void)bFromSweep;
	(void)SweepResult;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!OtherCharacter || OtherComp != OtherCharacter->GetCapsuleComponent())
	{
		return;
	}

	(void)ApplyTrapDamage(OtherActor);
}

