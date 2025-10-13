// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCamera.h"


// Sets default values
AGameCamera::AGameCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameCamera::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(PlayerREF->GetController()))
	{
		PC->SetViewTarget(this);
	}
	FVector StartLocation (-2531.f,2694.f,1253.f);
	SetActorLocation(StartLocation);
}

// Called every frame
void AGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Offset(0.f, MoveSpeed, 0.f);
	FVector ActualLocation = GetActorLocation();
	FVector NewLocation = ActualLocation + Offset;
	
	NewLocation.Y = FMath::Clamp(NewLocation.Y, 2694.f, 39944.f);

	
	SetActorLocation(NewLocation);

}

