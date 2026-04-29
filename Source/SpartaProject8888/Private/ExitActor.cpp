#include "ExitActor.h"
#include "CollectibleActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AExitActor::AExitActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	ExitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitMesh"));
	ExitMesh->SetupAttachment(SceneRoot);
	ExitMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ExitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitCollision"));
	ExitCollision->SetupAttachment(SceneRoot);
	ExitCollision->SetBoxExtent(ExitCollisionExtent);
	ExitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExitCollision->SetCollisionObjectType(ECC_WorldDynamic);
	ExitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExitCollision->SetGenerateOverlapEvents(true);
}

void AExitActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ExitCollision)
	{
		ExitCollision->SetBoxExtent(ExitCollisionExtent);
	}
}

int32 AExitActor::GetCollectedCount() const
{
	int32 CollectedCount = 0;
	const UWorld* World = GetWorld();
	if (!World)
	{
		return CollectedCount;
	}

	for (TActorIterator<ACollectibleActor> It(World); It; ++It)
	{
		if (It->IsCollected())
		{
			++CollectedCount;
		}
	}

	return CollectedCount;
}

bool AExitActor::CanExit() const
{
	return GetCollectedCount() >= RequiredCollectibleCount;
}

void AExitActor::BeginPlay()
{
	Super::BeginPlay();

	if (ExitCollision)
	{
		ExitCollision->OnComponentBeginOverlap.AddDynamic(this, &AExitActor::OnExitCollisionBeginOverlap);
	}
}

void AExitActor::TryExit(AActor* ExitingActor)
{
	if (!CanExit())
	{
		return;
	}

	if (NextLevelName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExitActor %s cannot open next level because NextLevelName is not set."), *GetName());
		return;
	}

	UGameplayStatics::OpenLevel(this, NextLevelName);
}

void AExitActor::OnExitCollisionBeginOverlap(
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

	TryExit(OtherCharacter);
}
