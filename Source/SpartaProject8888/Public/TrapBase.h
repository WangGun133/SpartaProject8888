// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

UCLASS()
class SPARTAPROJECT8888_API ATrapBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATrapBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float Damage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	bool bInstantKill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	bool bActive = true;

	UFUNCTION(BlueprintCallable, Category = "Trap")
	virtual void ActivateTrap();

	UFUNCTION(BlueprintCallable, Category = "Trap")
	virtual void DeactivateTrap();

	UFUNCTION(BlueprintCallable, Category = "Trap")
	virtual void ApplyTrapDamage(AActor* Target);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
