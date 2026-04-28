#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SPARTAPROJECT8888_API ASavePoint : public AActor
{
	GENERATED_BODY()

public:
	ASavePoint();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Point")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Point")
	UStaticMeshComponent* SavePointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Point")
	UBoxComponent* SaveCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save Point")
	FVector SaveCollisionExtent = FVector(100.0f, 100.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save Point")
	FTransform RespawnOffset = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save Point", meta = (Units = "deg/s"))
	FRotator MeshRotationSpeed = FRotator(0.0f, 90.0f, 0.0f);

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSaveCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	bool bCollected = false;
};
