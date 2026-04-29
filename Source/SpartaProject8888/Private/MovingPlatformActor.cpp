#include "MovingPlatformActor.h"
#include "Components/StaticMeshComponent.h"

AMovingPlatformActor::AMovingPlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(SceneRoot);
	PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlatformMesh->SetCollisionObjectType(ECC_WorldStatic);
	PlatformMesh->SetCollisionResponseToAllChannels(ECR_Block);
	PlatformMesh->SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	MoveDirectionSign = bStartMovingForward ? 1.0f : -1.0f;
}

void AMovingPlatformActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovePlatform(DeltaSeconds);
}

void AMovingPlatformActor::ActivatePlatform()
{
	bActive = true;
}

void AMovingPlatformActor::DeactivatePlatform()
{
	bActive = false;
}

void AMovingPlatformActor::MovePlatform(float DeltaSeconds)
{
	if (!bActive || MoveSpeed <= 0.0f || MoveDistance <= 0.0f || LocalMoveDirection.IsNearlyZero())
	{
		return;
	}

	if (WaitTimeRemaining > 0.0f)
	{
		WaitTimeRemaining = FMath::Max(0.0f, WaitTimeRemaining - DeltaSeconds);
		return;
	}

	CurrentMoveOffset += MoveDirectionSign * MoveSpeed * DeltaSeconds;

	if (CurrentMoveOffset >= MoveDistance)
	{
		CurrentMoveOffset = MoveDistance;
		MoveDirectionSign = -1.0f;
		WaitTimeRemaining = WaitTimeAtEnds;
	}
	else if (CurrentMoveOffset <= 0.0f)
	{
		CurrentMoveOffset = 0.0f;
		MoveDirectionSign = 1.0f;
		WaitTimeRemaining = WaitTimeAtEnds;
	}

	const FVector TargetLocation = StartLocation + GetWorldMoveDirection() * CurrentMoveOffset;
	SetActorLocation(TargetLocation, bSweepMovement);
}

FVector AMovingPlatformActor::GetWorldMoveDirection() const
{
	return GetActorTransform().TransformVectorNoScale(LocalMoveDirection.GetSafeNormal());
}
