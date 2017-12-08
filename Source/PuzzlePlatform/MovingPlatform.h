// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	float PlatformMovingSpeed = 100;
	
	UPROPERTY(EditAnywhere, Category = "Platform Settings", Meta = (MakeEditWidget = true))
	FVector TargetLocation;
private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere)
	int32 ActiveTriggers = 1;

};
