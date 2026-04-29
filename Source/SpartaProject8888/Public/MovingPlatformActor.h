#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatformActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SPARTAPROJECT8888_API AMovingPlatformActor : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatformActor();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Moving Platform")
	void ActivatePlatform();

	UFUNCTION(BlueprintCallable, Category = "Moving Platform")
	void DeactivatePlatform();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	bool bActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	bool bStartMovingForward = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform", meta = (ClampMin = "0.0", Units = "cm"))
	float MoveDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform", meta = (ClampMin = "0.0", Units = "cm/s"))
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	FVector LocalMoveDirection = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform", meta = (ClampMin = "0.0", Units = "s"))
	float WaitTimeAtEnds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moving Platform")
	bool bSweepMovement = false;

	FVector StartLocation = FVector::ZeroVector;
	float CurrentMoveOffset = 0.0f;
	float MoveDirectionSign = 1.0f;
	float WaitTimeRemaining = 0.0f;

	void MovePlatform(float DeltaSeconds);
	FVector GetWorldMoveDirection() const;
};
