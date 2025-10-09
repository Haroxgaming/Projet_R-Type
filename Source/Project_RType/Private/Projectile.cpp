// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "EnnemyParent.h"
#include "RType_Player.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);


	ProjectileMovement->InitialSpeed = 200.0f;
	ProjectileMovement->MaxSpeed = 200.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	SphereCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->InitCapsuleSize(50.f, 50.f);
	
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	}
	GetWorld()->GetTimerManager().SetTimer(DestroyTime, this, &AProjectile::SelfDestroy, 8.0f, true);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Initialize(float ProjectileSpeed, bool IsPlayer)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->InitialSpeed = ProjectileSpeed;
        ProjectileMovement->MaxSpeed = ProjectileSpeed;
    }
	PlayerShoot = IsPlayer;
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerShoot)
	{
		if (!OtherActor->IsA(AProjectile::StaticClass()) && !OtherActor->IsA(ARType_Player::StaticClass()))
		{
			IDamage::Execute_Hit(OtherActor, this);
			Destroy();
		}
	}
	else
	{
		if (!OtherActor->IsA(AProjectile::StaticClass()) && !OtherActor->IsA(AEnnemyParent::StaticClass()))
		{
			IDamage::Execute_Hit(OtherActor, this);
			Destroy();
		}
	}
	
}

void AProjectile::SelfDestroy()
{
	Destroy();
}