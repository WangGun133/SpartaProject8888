#include "SavePoint.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	SavePointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SavePointMesh"));
	SavePointMesh->SetupAttachment(SceneRoot);
	SavePointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SaveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SaveCollision"));
	SaveCollision->SetupAttachment(SceneRoot);
	SaveCollision->SetBoxExtent(SaveCollisionExtent);
	SaveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SaveCollision->SetCollisionObjectType(ECC_WorldDynamic);
	SaveCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SaveCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SaveCollision->SetGenerateOverlapEvents(true);
}

void ASavePoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SaveCollision)
	{
		SaveCollision->SetBoxExtent(SaveCollisionExtent);
	}
}

void ASavePoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bCollected && SavePointMesh)
	{
		SavePointMesh->AddLocalRotation(MeshRotationSpeed * DeltaSeconds);
	}
}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();

	if (SaveCollision)
	{
		SaveCollision->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnSaveCollisionBeginOverlap);
	}
}

void ASavePoint::OnSaveCollisionBeginOverlap(
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

	if (bCollected)
	{
		return;
	}

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!OtherCharacter || OtherComp != OtherCharacter->GetCapsuleComponent())
	{
		return;
	}

	if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherCharacter))
	{
		FTransform SaveTransform = RespawnOffset * GetActorTransform();
		SaveTransform.SetScale3D(FVector::OneVector);
		PlayerCharacter->SetRespawnTransform(SaveTransform);

		bCollected = true;

		if (SavePointMesh)
		{
			SavePointMesh->SetVisibility(false, true);
			SavePointMesh->SetHiddenInGame(true, true);
		}

		if (SaveCollision)
		{
			SaveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SaveCollision->SetGenerateOverlapEvents(false);
		}
	}
}
