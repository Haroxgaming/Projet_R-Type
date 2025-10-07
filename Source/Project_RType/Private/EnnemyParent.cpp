// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyParent.h"

// Sets default values
AEnnemyParent::AEnnemyParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
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
		Projectile->Initialize(ProjectileSpeed, this);

		Projectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
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