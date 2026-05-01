// Fill out your copyright notice in the Description page of Project Settings.

#include "ASpikeTrap.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"

ASpikeTrap::ASpikeTrap()
{
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

void ASpikeTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (DamageCollision)
	{
		DamageCollision->SetBoxExtent(DamageCollisionExtent);
	}
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();

	if (DamageCollision)
	{
		DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnDamageCollisionBeginOverlap);
		DamageCollision->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ASpikeTrap::ActivateTrap()
{
	Super::ActivateTrap();

	if (DamageCollision)
	{
		DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ASpikeTrap::DeactivateTrap()
{
	Super::DeactivateTrap();

	if (DamageCollision)
	{
		DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool ASpikeTrap::ApplyTrapDamage(AActor* Target)
{
	return Super::ApplyTrapDamage(Target);
}

void ASpikeTrap::OnDamageCollisionBeginOverlap(
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
