// MyCharacter.cpp

#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;
    JumpMaxHoldTime = 0.2f;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    BaseActorYaw = GetActorRotation().Yaw;

    if (USkeletalMeshComponent* CharacterMesh = GetMesh())
    {
        InitialMeshRelativeRotation = CharacterMesh->GetRelativeRotation();
    }

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacter::StopMove);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AMyCharacter::StopMove);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);
    }
}

void AMyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateFacingDirection(DeltaSeconds);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    MoveInputValue = Value.Get<float>();

    if (!FMath::IsNearlyZero(MoveInputValue))
    {
        AddMovementInput(FVector::ForwardVector, MoveInputValue);
    }
}

void AMyCharacter::StopMove(const FInputActionValue& Value)
{
    (void)Value;
    MoveInputValue = 0.0f;
}

void AMyCharacter::UpdateFacingDirection(float DeltaSeconds)
{
    FVector FacingDirection = FVector::ZeroVector;

    if (!FMath::IsNearlyZero(MoveInputValue))
    {
        FacingDirection = FVector::ForwardVector * FMath::Sign(MoveInputValue);
    }
    else
    {
        const FVector PlanarVelocity = FVector(GetVelocity().X, GetVelocity().Y, 0.0f);

        if (!PlanarVelocity.IsNearlyZero())
        {
            FacingDirection = PlanarVelocity.GetSafeNormal();
        }
        else if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            FVector CameraLocation;
            FRotator CameraRotation;
            PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

            FVector ToCamera = CameraLocation - GetActorLocation();
            ToCamera.Z = 0.0f;

            if (!ToCamera.IsNearlyZero())
            {
                FacingDirection = ToCamera.GetSafeNormal();
            }
        }
    }

    if (FacingDirection.IsNearlyZero())
    {
        return;
    }

    const float TargetMeshYaw =
        InitialMeshRelativeRotation.Yaw + FMath::FindDeltaAngleDegrees(BaseActorYaw, FacingDirection.Rotation().Yaw);
    const FRotator TargetMeshRotation(
        InitialMeshRelativeRotation.Pitch,
        TargetMeshYaw,
        InitialMeshRelativeRotation.Roll);

    if (USkeletalMeshComponent* CharacterMesh = GetMesh())
    {
        const FRotator NewMeshRotation = FMath::RInterpTo(
            CharacterMesh->GetRelativeRotation(),
            TargetMeshRotation,
            DeltaSeconds,
            RotationInterpSpeed);
        CharacterMesh->SetRelativeRotation(NewMeshRotation);
    }
}
