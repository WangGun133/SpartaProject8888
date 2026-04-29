#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SPARTAPROJECT8888_API ACollectibleActor : public AActor
{
	GENERATED_BODY()

public:
	ACollectibleActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Collectible")
	bool IsCollected() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	UStaticMeshComponent* CollectibleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	UBoxComponent* CollectCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectible")
	FVector CollectCollisionExtent = FVector(80.0f, 80.0f, 80.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectible", meta = (Units = "deg/s"))
	FRotator MeshRotationSpeed = FRotator(0.0f, 90.0f, 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Collectible")
	void Collect(AActor* CollectingActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Collectible")
	void OnCollected(AActor* CollectingActor);

	UFUNCTION()
	void OnCollectCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	bool bCollected = false;
};
