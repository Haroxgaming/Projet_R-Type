
#include "RType_Player.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ARType_Player::ARType_Player()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. CREATE AND SET THE BOX AS ROOT
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetBoxExtent(FVector(50.f, 20.f, 20.f)); // Adapte la taille
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Optionnel: Block si tu veux bloquer d'autres pawns

    // 2. Attache le mesh et les autres composants
    ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    ShipMesh->SetupAttachment(RootComponent);

    Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
    Shield->SetupAttachment(RootComponent);

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    ArrowComponent->SetupAttachment(RootComponent);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(RootComponent);
    ProjectileSpawnPoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
    
}

void ARType_Player::Move(const FInputActionValue& Value)
{
    // Expecting a 2D axis input (X = Right/Left, Y = Forward/Backward)
    CurrentMovementInput = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        bIsMoving = true;

        
        FVector MoveDirection = (GetActorForwardVector() * CurrentMovementInput.X) + (GetActorUpVector() * CurrentMovementInput.Y);
        if (!MoveDirection.IsNearlyZero())
        {
            FVector MoveDelta = MoveDirection.GetSafeNormal() * MoveSpeed * GetWorld()->GetDeltaSeconds();
            FHitResult Hit;
            AddActorWorldOffset(MoveDelta, true, &Hit);
            if (Hit.bBlockingHit)
            {
                if (Hit.GetActor())
                {
                    UE_LOG(LogTemp, Warning, TEXT("BLOQUÉ PAR: %s"), *GetNameSafe(Hit.GetActor()));
                }

                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("BLOQUÉ PAR: None. BlockingComponent: %s"), *GetNameSafe(Hit.Component.Get()));
                }
                
            }
        }

        const float MaxYaw  = 35.0f; // tilt forward/backward
        const float MaxPitch = -15.0f; // tilt left/right only when moving backward
    
        // Roll depends on Y input
        float TargetYaw = -CurrentMovementInput.Y * MaxYaw;

        // Pitch only when moving backward
        float TargetPitch = 0.0f;
        if (CurrentMovementInput.X < 0.f) // moving backward
        {
            TargetPitch = CurrentMovementInput.X * MaxPitch;
        }

        FRotator CurrentRotation = ShipMesh->GetRelativeRotation();
        FRotator TargetRotation = BaseShipRotation + FRotator(TargetYaw, 0.f, TargetPitch);

        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

        ShipMesh->SetRelativeRotation(NewRotation);
    }
}

void ARType_Player::Shoot(const FInputActionValue& Value)
{
    if (CanShoot)
    {
        if (!ProjectileClass) return;

        FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
        FRotator SpawnRotation = GetActorRotation();

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
            Projectile->Initialize(ProjectileSpeed, true);

            Projectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
        }
        GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ARType_Player::Reload, FireRate, false);
        CanShoot = false;
    }

}

void ARType_Player::Reload()
{
    CanShoot= true;
}

void ARType_Player::InvincibleEnd()
{
    Invincible = false;
    Shield->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARType_Player::BeginPlay()
{
    Super::BeginPlay();
    
    BaseShipRotation = ShipMesh->GetRelativeRotation();

    // Add Input Mapping Context for Enhanced Input
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

// Called every frame
void ARType_Player::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving)
    {
        FRotator CurrentRotation = ShipMesh->GetRelativeRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, BaseShipRotation, DeltaTime, 5.0f);
        ShipMesh->SetRelativeRotation(NewRotation);
    }
}

// Called to bind functionality to input
void ARType_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARType_Player::Move);
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARType_Player::OnMoveReleased);
        }
        if (ShootAction)
        {
            EnhancedInput->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ARType_Player::Shoot);
        }
    }
}

void ARType_Player::OnMoveReleased(const FInputActionValue& Value)
{
    bIsMoving = false;
    CurrentMovementInput = FVector2D::ZeroVector;
}

void ARType_Player::Hit_Implementation(AActor* Caller)
{
    if (!Invincible)
    {
        Health--;
        if (Health <= 0)
        {
        }
        else
        {
            Shield->SetVisibility(true);
            Invincible = true;
            GetWorld()->GetTimerManager().SetTimer(InvinciblityTimer, this, &ARType_Player::InvincibleEnd, InvincibilityTime, false);
        }
    }
}
