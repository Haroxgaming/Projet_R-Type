// Fill out your copyright notice in the Description page of Project Settings.


#include "RType_Player.h"

// Sets default values
ARType_Player::ARType_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARType_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARType_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARType_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

