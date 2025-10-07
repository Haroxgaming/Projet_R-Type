// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

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

AProjectile::AProjectile(float ProjectileSpeed, AActor* launcher)
{
	PrimaryActorTick.bCanEverTick = true;

	Self = launcher;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);


	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	SphereCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->InitCapsuleSize(50.f, 50.f);
	SphereCollision->SetGenerateOverlapEvents(true);
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor != Self && !OtherActor->IsA(AProjectile::StaticClass()) )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		//IDamage::Execute_Hit(OtherActor, this);
		Destroy();
	}
}

