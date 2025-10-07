
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
    // Set this pawn to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(RootComponent);
    ProjectileSpawnPoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    }
    
    FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

    ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    ShipMesh->SetupAttachment(RootComponent);

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    ArrowComponent->SetupAttachment(RootComponent);
}

void ARType_Player::Move(const FInputActionValue& Value)
{
    // Expecting a 2D axis input (X = Right/Left, Y = Forward/Backward)
    CurrentMovementInput = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        bIsMoving = true;

        // Move right/left (X)
        AddMovementInput(GetActorForwardVector(), CurrentMovementInput.X);

        // Move forward/backward (Y)
        AddMovementInput(GetActorUpVector(), CurrentMovementInput.Y);

        const float MaxRoll  = 25.0f; // tilt forward/backward
        const float MaxPitch = -15.0f; // tilt left/right only when moving backward

        // Roll depends on Y input
        float TargetRoll = -CurrentMovementInput.Y * MaxRoll;

        // Pitch only when moving backward
        float TargetPitch = 0.0f;
        if (CurrentMovementInput.X < 0.f) // moving backward
        {
            TargetPitch = CurrentMovementInput.X * MaxPitch;
        }

        FRotator CurrentRotation = ShipMesh->GetRelativeRotation();
        FRotator TargetRotation  = FRotator(TargetPitch, 0.f, TargetRoll);

        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

        ShipMesh->SetRelativeRotation(NewRotation);
    }
}

void ARType_Player::Shoot(const FInputActionValue& Value)
{
    if (!ProjectileClass) return;

    FVector SpawnLocation = GetActorLocation();
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
        Projectile->Initialize(ProjectileSpeed, this);

        Projectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

// Called when the game starts or when spawned
void ARType_Player::BeginPlay()
{
    Super::BeginPlay();

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
        FRotator TargetRotation = FRotator::ZeroRotator;
        FRotator CurrentRotation = ShipMesh->GetRelativeRotation();

        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
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

void ARType_Player::Hit(const FHitResult& Hit)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit"));
}
