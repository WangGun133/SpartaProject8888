#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SPARTAPROJECT8888_API AExitActor : public AActor
{
	GENERATED_BODY()

public:
	AExitActor();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintPure, Category = "Exit")
	int32 GetCollectedCount() const;

	UFUNCTION(BlueprintPure, Category = "Exit")
	bool CanExit() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
	UStaticMeshComponent* ExitMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
	UBoxComponent* ExitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
	FVector ExitCollisionExtent = FVector(120.0f, 120.0f, 180.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit", meta = (ClampMin = "0"))
	int32 RequiredCollectibleCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
	FName NextLevelName;

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void TryExit(AActor* ExitingActor);

	UFUNCTION()
	void OnExitCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
