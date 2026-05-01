// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapBase.h"
#include "MovingSpikeTrap.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT8888_API AMovingSpikeTrap : public ATrapBase
{
	GENERATED_BODY()

public:
	AMovingSpikeTrap();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap")
	UBoxComponent* DamageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap")
	FVector DamageCollisionExtent = FVector(75.0f, 75.0f, 30.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap", meta = (ClampMin = "0.0", Units = "cm/s"))
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap", meta = (ClampMin = "0.0", Units = "cm"))
	float MoveDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Spike Trap")
	FVector LocalMoveDirection = FVector::RightVector;

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

	FVector StartLocation = FVector::ZeroVector;
	float CurrentMoveOffset = 0.0f;
	float MoveDirection = 1.0f;

public:
	virtual void Tick(float DeltaTime) override;
};
