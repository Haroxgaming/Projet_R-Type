// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "RType_Player.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PROJECT_RTYPE_API ARType_Player : public APawn, public IDamage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement;

	FVector2D CurrentMovementInput;

	UPROPERTY()
	class AGameCamera* CameraActorReference;

	bool bIsMoving = false;

public:
	// Sets default values for this pawn's properties
	ARType_Player();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Shield;

	UFUNCTION(BlueprintCallable, Category="Shield")
	void SetShieldMaterial(UMaterialInterface* NewMaterial)
	{
		if (Shield)
		{
			Shield->SetMaterial(0, NewMaterial);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shooting")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooting")
	class USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* CollisionBox;

	FTimerHandle FireRateTimer;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.4f;

	UPROPERTY(EditAnywhere)
	bool CanShoot = true;

	FTimerHandle InvinciblityTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float InvincibilityTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	bool Invincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	int Health = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditAnywhere)
	FRotator BaseShipRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MoveSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ShootSound;
	
protected:

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Reload();
	void InvincibleEnd();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnMoveReleased(const FInputActionValue& Value);

	void Hit_Implementation(AActor* Caller);
};
