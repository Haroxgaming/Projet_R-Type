// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damage.h"
#include "GameFramework/Actor.h"
#include "RType_Player.h"
#include "EnnemyParent.generated.h"

UCLASS()
class PROJECT_RTYPE_API AEnnemyParent : public AActor, public IDamage
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
	int Health = 1;

	UPROPERTY(EditAnywhere)
	float FireRate = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* ProjectileSpawnPoint; 

	UPROPERTY(EditAnywhere)
	bool Aiming = false;

	UPROPERTY(EditAnywhere)
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere)
	ARType_Player* PlayerPawn;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 100.0f;
	
	FTimerHandle FireRateTimer;

	void Hit_Implementation(AActor* Caller) override;
	void Fire();

};
