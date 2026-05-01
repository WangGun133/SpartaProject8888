// MyCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;

UCLASS()
class SPARTAPROJECT8888_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter();
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintPure, Category = "Status")
    int32 GetLife() const;

    UFUNCTION(BlueprintPure, Category = "Status")
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, Category = "Status")
    bool DieFromTrap(AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Respawn")
    void SetRespawnTransform(const FTransform& NewRespawnTransform);

    UFUNCTION(BlueprintCallable, Category = "Respawn")
    void Respawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Rotation")
    float RotationInterpSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Plane")
    bool bLockToInitialY = true;

    UPROPERTY(Transient)
    USpringArmComponent* CameraBoom;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.0", Units = "cm"))
    float MapViewCameraDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.0"))
    float CameraDistanceInterpSpeed = 6.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    bool bDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn", meta = (ClampMin = "0.0", Units = "s"))
    float RespawnDelay = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn", meta = (ClampMin = "0.0", Units = "s"))
    float RespawnInvincibilityTime = 0.75f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (ClampMin = "0"))
    int32 Life = 3;

    UFUNCTION(BlueprintNativeEvent, Category = "Status")
    void OnDeath(AController* EventInstigator, AActor* DamageCauser);
    virtual void OnDeath_Implementation(AController* EventInstigator, AActor* DamageCauser);

    void Move(const FInputActionValue& Value);
    void StopMove(const FInputActionValue& Value);
    void ToggleMapCamera();
    void UpdateFacingDirection(float DeltaSeconds);
    void UpdateCameraDistance(float DeltaSeconds);
    void ApplyYConstraint();
    void EndTrapDamageInvulnerability();
 

    float MoveInputValue = 0.0f;
    float LockedYLocation = 0.0f;
    float BaseActorYaw = 0.0f;
    FRotator InitialMeshRelativeRotation = FRotator::ZeroRotator;
    FVector InitialActorScale = FVector::OneVector;
    float NormalCameraDistance = 0.0f;
    bool bMapCameraActive = false;
    bool bTrapDamageInvulnerable = false;
    FTransform RespawnTransform = FTransform::Identity;
    FTimerHandle RespawnTimerHandle;
    FTimerHandle TrapDamageInvulnerabilityTimerHandle;
    
};
