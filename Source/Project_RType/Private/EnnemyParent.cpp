// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyParent.h"

// Sets default values
AEnnemyParent::AEnnemyParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
     GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
}

void AEnnemyParent::TakeHit()
{
	Health--;
	if (Health <= 0)
	{
		if (PlayerRef)
		{
			PlayerRef->Score += score;
		}
	
		K2_DestroyActor();
	}
}

