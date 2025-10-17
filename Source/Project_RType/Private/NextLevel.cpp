// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevel.h"



// Sets default values
ANextLevel::ANextLevel()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox); 
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetBoxExtent(FVector(1000.f, 100.f, 1400.f)); 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnOverlapBegin);
}

void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANextLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARType_Player* PlayerCharacter = Cast<ARType_Player>(OtherActor);
	if (PlayerCharacter)
	{
		if (LevelToLoad.IsPending() || LevelToLoad.IsValid())
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToLoad);
		}
	}
}
