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
    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

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

    UPROPERTY(Transient)
    USpringArmComponent* CameraBoom;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.0", Units = "cm"))
    float MapViewCameraDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.0"))
    float CameraDistanceInterpSpeed = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bDead = false;

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void OnDeath(AController* EventInstigator, AActor* DamageCauser);
    virtual void OnDeath_Implementation(AController* EventInstigator, AActor* DamageCauser);

    void Move(const FInputActionValue& Value);
    void StopMove(const FInputActionValue& Value);
    void ToggleMapCamera();
    void UpdateFacingDirection(float DeltaSeconds);
    void UpdateCameraDistance(float DeltaSeconds);

    float MoveInputValue = 0.0f;
    float BaseActorYaw = 0.0f;
    FRotator InitialMeshRelativeRotation = FRotator::ZeroRotator;
    float NormalCameraDistance = 0.0f;
    bool bMapCameraActive = false;
};
