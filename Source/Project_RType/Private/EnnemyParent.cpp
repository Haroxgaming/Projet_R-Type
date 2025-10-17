// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyParent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnnemyParent::AEnnemyParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
}

// Called when the game starts or when spawned
void AEnnemyParent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AEnnemyParent::Fire, FireRate, true);
}

// Called every frame
void AEnnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnnemyParent::Fire()
{
	if (!ProjectileClass) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		FTransform(SpawnRotation, SpawnLocation),
		this,
		GetInstigator(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Projectile)
	{
		Projectile->Initialize(ProjectileSpeed, false);

		if (Aiming && PlayerPawn)
		{
			FVector Direction = (PlayerPawn->GetActorLocation() - SpawnLocation);
			SpawnRotation = Direction.Rotation();
		}
		Projectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
	if (ShootSound)
	{
		UGameplayStatics::PlaySound2D(this, ShootSound);
	}
}

void AEnnemyParent::Hit_Implementation(AActor* Caller)
{
	Health--;
	if (Health <= 0)
	{
		K2_DestroyActor();
	}
}