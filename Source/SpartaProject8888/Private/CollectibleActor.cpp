#include "CollectibleActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

ACollectibleActor::ACollectibleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	CollectibleMesh->SetupAttachment(SceneRoot);
	CollectibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollectCollision"));
	CollectCollision->SetupAttachment(SceneRoot);
	CollectCollision->SetBoxExtent(CollectCollisionExtent);
	CollectCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollectCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CollectCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollectCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollectCollision->SetGenerateOverlapEvents(true);
}

void ACollectibleActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (CollectCollision)
	{
		CollectCollision->SetBoxExtent(CollectCollisionExtent);
	}
}

void ACollectibleActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bCollected && CollectibleMesh)
	{
		CollectibleMesh->AddLocalRotation(MeshRotationSpeed * DeltaSeconds);
	}
}

bool ACollectibleActor::IsCollected() const
{
	return bCollected;
}

void ACollectibleActor::BeginPlay()
{
	Super::BeginPlay();

	if (CollectCollision)
	{
		CollectCollision->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleActor::OnCollectCollisionBeginOverlap);
	}
}

void ACollectibleActor::Collect(AActor* CollectingActor)
{
	if (bCollected)
	{
		return;
	}

	bCollected = true;

	if (CollectibleMesh)
	{
		CollectibleMesh->SetVisibility(false, true);
		CollectibleMesh->SetHiddenInGame(true, true);
	}

	if (CollectCollision)
	{
		CollectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollectCollision->SetGenerateOverlapEvents(false);
	}

	OnCollected(CollectingActor);
}

void ACollectibleActor::OnCollectCollisionBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	(void)OverlappedComponent;
	(void)OtherBodyIndex;
	(void)bFromSweep;
	(void)SweepResult;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!OtherCharacter || OtherComp != OtherCharacter->GetCapsuleComponent())
	{
		return;
	}

	Collect(OtherCharacter);
}
