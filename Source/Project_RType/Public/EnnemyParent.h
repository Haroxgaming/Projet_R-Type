// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RType_Player.h"
#include "EnnemyParent.generated.h"

UCLASS()
class PROJECT_RTYPE_API AEnnemyParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnnemyParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int score = 1;
	
	ARType_Player* PlayerRef;
	
	void death();

};
