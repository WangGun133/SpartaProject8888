// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class SPARTAPROJECT8888_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintPure, Category = "Stage")
	FName GetCurrentStageName() const;

	UFUNCTION(BlueprintPure, Category = "Stage")
	int32 GetCurrentStageNumber() const;

	UFUNCTION(BlueprintPure, Category = "Stage")
	FText GetCurrentStageText() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidget;
	
};
