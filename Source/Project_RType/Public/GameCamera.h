// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RType_Player.h"
#include "GameFramework/Actor.h"
#include "GameCamera.generated.h"

UCLASS()
class PROJECT_RTYPE_API AGameCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameCamera();

	UPROPERTY(EditAnywhere)
	ARType_Player* PlayerREF;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameC")
	float MoveSpeed = 100.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
