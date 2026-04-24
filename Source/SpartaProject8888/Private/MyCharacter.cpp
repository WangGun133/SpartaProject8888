// MyCharacter.cpp

#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"

AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;
    JumpMaxHoldTime = 0.2f;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = NormalCameraDistance;
    CameraBoom->SetRelativeRotation(SideViewCameraRotation);
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    UE_LOG(LogTemp, Warning, TEXT("Character %s starting health: %.1f / %.1f"), *GetName(), CurrentHealth, MaxHealth);

    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = NormalCameraDistance;
        ApplySideViewCameraRotation();
    }

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

    PlayerInputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &AMyCharacter::ToggleMapCamera);
}

void AMyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateFacingDirection(DeltaSeconds);
    UpdateCameraDistance(DeltaSeconds);
}

float AMyCharacter::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    const float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (bDead || AppliedDamage <= 0.0f)
    {
        return 0.0f;
    }

    const float PreviousHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - AppliedDamage, 0.0f, MaxHealth);

    if (CurrentHealth <= 0.0f)
    {
        bDead = true;
        OnDeath(EventInstigator, DamageCauser);
    }

    return PreviousHealth - CurrentHealth;
}

float AMyCharacter::GetCurrentHealth() const
{
    return CurrentHealth;
}

float AMyCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

bool AMyCharacter::IsDead() const
{
    return bDead;
}

void AMyCharacter::Heal(float HealAmount)
{
    if (bDead || HealAmount <= 0.0f)
    {
        return;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
}

void AMyCharacter::OnDeath_Implementation(AController* EventInstigator, AActor* DamageCauser)
{
    (void)EventInstigator;
    (void)DamageCauser;

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    if (bDead)
    {
        return;
    }

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

void AMyCharacter::ToggleMapCamera()
{
    bMapCameraActive = !bMapCameraActive;
}

void AMyCharacter::UpdateFacingDirection(float DeltaSeconds)
{
    if (bDead)
    {
        return;
    }

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

void AMyCharacter::UpdateCameraDistance(float DeltaSeconds)
{
    if (!CameraBoom)
    {
        return;
    }

    const float TargetDistance = bMapCameraActive ? MapViewCameraDistance : NormalCameraDistance;
    CameraBoom->TargetArmLength = FMath::FInterpTo(
        CameraBoom->TargetArmLength,
        TargetDistance,
        DeltaSeconds,
        CameraDistanceInterpSpeed);
}

void AMyCharacter::ApplySideViewCameraRotation()
{
    if (!CameraBoom)
    {
        return;
    }

    CameraBoom->SetRelativeRotation(SideViewCameraRotation);
}
