// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass is not set on %s."), *GetName());
		return;
	}

	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PlayerController)
	{
		return;
	}

	HUDWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("HUD widget %s added to viewport."), *HUDWidget->GetName());
	}
}

UUserWidget* AMyGameMode::GetHUDWidget() const
{
	return HUDWidget;
}

FName AMyGameMode::GetCurrentStageName() const
{
	return FName(*UGameplayStatics::GetCurrentLevelName(this, true));
}

int32 AMyGameMode::GetCurrentStageNumber() const
{
	const FString StageName = GetCurrentStageName().ToString();
	FString StageNumberString;

	for (int32 Index = StageName.Len() - 1; Index >= 0; --Index)
	{
		const TCHAR Character = StageName[Index];
		if (!FChar::IsDigit(Character))
		{
			break;
		}

		StageNumberString.InsertAt(0, Character);
	}

	return StageNumberString.IsEmpty() ? 0 : FCString::Atoi(*StageNumberString);
}

FText AMyGameMode::GetCurrentStageText() const
{
	return FText::FromString(FString::Printf(TEXT("Stage: %d"), GetCurrentStageNumber()));
}
