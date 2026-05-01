// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapBase.h"
#include "ASpikeTrap.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT8888_API ASpikeTrap : public ATrapBase
{
	GENERATED_BODY()

public:
	ASpikeTrap();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike Trap")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike Trap")
	UBoxComponent* DamageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike Trap")
	FVector DamageCollisionExtent = FVector(75.0f, 75.0f, 30.0f);

	virtual void BeginPlay() override;
	virtual void ActivateTrap() override;
	virtual void DeactivateTrap() override;
	virtual bool ApplyTrapDamage(AActor* Target) override;

	UFUNCTION()
	void OnDamageCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
