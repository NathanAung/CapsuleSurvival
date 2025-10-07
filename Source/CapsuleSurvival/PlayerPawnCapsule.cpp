#include "PlayerPawnCapsule.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

APlayerPawnCapsule::APlayerPawnCapsule()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;


    // Root capsule
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    RootComponent = CapsuleComponent;

    // Movement component
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

    // Camera boom (spring arm)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 800.f; // distance from player
    SpringArm->bDoCollisionTest = false;
    SpringArm->bUsePawnControlRotation = false; // important!
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;
    SpringArm->SetUsingAbsoluteRotation(true);  // absolute rotation (doesn't follow pawn rotation)
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // top-down angle

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false; // camera won't rotate with pawn
}

void APlayerPawnCapsule::BeginPlay()
{
    Super::BeginPlay();
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

void APlayerPawnCapsule::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawnCapsule::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawnCapsule::MoveRight);
    PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawnCapsule::Shoot);
}

void APlayerPawnCapsule::MoveForward(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(FVector::ForwardVector, Value); // +X in world space
    }
}

void APlayerPawnCapsule::MoveRight(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(FVector::RightVector, Value);   // +Y in world space
    }
}


void APlayerPawnCapsule::Shoot()
{
    if (!ProjectileClass) return;

    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0, 0, 50);
    FRotator SpawnRotation = GetActorRotation();

    SpawnRotation.Pitch = 0.f; // make it flat in XY plane
    SpawnRotation.Roll = 0.f;

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
        Bullet->SetLifeSpan(3.0f); // destroys bullet after 3 seconds
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn bullet!"));
    }
}



