// MyCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTAPROJECT8888_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter();
    virtual void Tick(float DeltaSeconds) override;

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

    void Move(const FInputActionValue& Value);
    void StopMove(const FInputActionValue& Value);
    void UpdateFacingDirection(float DeltaSeconds);

    float MoveInputValue = 0.0f;
    float BaseActorYaw = 0.0f;
    FRotator InitialMeshRelativeRotation = FRotator::ZeroRotator;
};
