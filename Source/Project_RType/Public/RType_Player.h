// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "RType_Player.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PROJECT_RTYPE_API ARType_Player : public APawn
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

	bool bIsMoving = false;

public:
	// Sets default values for this pawn's properties
	ARType_Player();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Shooting")
	TSubclassOf<AProjectile> ProjectileClass;
	
protected:

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnMoveReleased(const FInputActionValue& Value);
};
