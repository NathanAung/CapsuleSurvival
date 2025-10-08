// PlayerPawnCapsule.cpp
#include "PlayerPawnCapsule.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBullet.h"


APlayerPawnCapsule::APlayerPawnCapsule()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;

    // Root capsule
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    RootComponent = CapsuleComponent;

    // --- Collision setup ---
    CapsuleComponent->InitCapsuleSize(42.f, 96.f);
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);   // Only query overlaps (no physics blocking)
    CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
    CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Still blocks world geometry
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CapsuleComponent->SetGenerateOverlapEvents(true);

    // Movement component
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

    // Camera boom (spring arm)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);                      // Attach to root
	SpringArm->TargetArmLength = 800.f;                             // The camera follows at this distance behind the character
	SpringArm->bDoCollisionTest = false;                            // Disable collision test
	SpringArm->bUsePawnControlRotation = false;                     // Don't rotate arm based on controller
	SpringArm->bInheritPitch = false;                               // Don't inherit pitch from parent
	SpringArm->bInheritYaw = false;                                 // Don't inherit yaw from parent
	SpringArm->bInheritRoll = false;                                // Don't inherit roll from parent
	SpringArm->SetUsingAbsoluteRotation(true);                      // Use absolute rotation
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));      // Rotate the arm

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);    // Attach camera to end of boom
	Camera->bUsePawnControlRotation = false;                                // Camera does not rotate relative to arm
}


void APlayerPawnCapsule::BeginPlay()
{
    Super::BeginPlay();
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawnCapsule::OnOverlapBegin);
}


void APlayerPawnCapsule::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Velocity = GetVelocity();
    Velocity.Z = 0;

    if (!Velocity.IsNearlyZero())
    {
        FRotator NewRotation = Velocity.Rotation();
        SetActorRotation(NewRotation);
    }
}


// automatically called to bind functionality to input
void APlayerPawnCapsule::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawnCapsule::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawnCapsule::MoveRight);
    PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawnCapsule::Shoot);
}


// Move the pawn forward or backward
void APlayerPawnCapsule::MoveForward(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(FVector::ForwardVector, Value);
    }
}


// Move the pawn right or left
void APlayerPawnCapsule::MoveRight(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(FVector::RightVector, Value);
    }
}


// Shoot a projectile
void APlayerPawnCapsule::Shoot()
{
    if (!ProjectileClass) return;

	// Calculate spawn location and rotation
    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0, 0, 0);
    FRotator SpawnRotation = GetActorRotation();

	// Ensure no pitch or roll
    SpawnRotation.Pitch = 0.f;
    SpawnRotation.Roll = 0.f;

	// Spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

	// Spawn the projectile
    AProjectileBullet* Bullet = GetWorld()->SpawnActor<AProjectileBullet>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (Bullet)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bullet spawned!"));
        Bullet->SetLifeSpan(3.0f);

        // Force bullet to move forward instantly
        if (UProjectileMovementComponent* MoveComp = Bullet->FindComponentByClass<UProjectileMovementComponent>())
        {
            MoveComp->Velocity = GetActorForwardVector() * MoveComp->InitialSpeed;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn bullet!"));
    }
}


float APlayerPawnCapsule::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    HP -= static_cast<int32>(DamageAmount);

    UE_LOG(LogTemp, Warning, TEXT("Player took damage! HP: %d"), HP);

    if (HP <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Player is dead!"));
        Destroy(); // You can replace this with a "Game Over" later
    }

    return DamageAmount;
}

