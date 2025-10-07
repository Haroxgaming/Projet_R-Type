// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Damage.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()

class PROJECT_RTYPE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	AProjectile(float ProjectileSpeed, AActor* launcher);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
	UCapsuleComponent* SphereCollision;

	UPROPERTY()
	AActor* Self;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
