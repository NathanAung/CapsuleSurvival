#include "PlayerPawnCapsule.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"

APlayerPawnCapsule::APlayerPawnCapsule()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root capsule
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    RootComponent = CapsuleComponent;

    // Movement component
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

    // Camera boom (spring arm)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 800.f; // Distance from top
    SpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // Look straight down
    SpringArm->bDoCollisionTest = false;

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void APlayerPawnCapsule::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerPawnCapsule::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerPawnCapsule::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawnCapsule::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawnCapsule::MoveRight);
}

void APlayerPawnCapsule::MoveForward(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void APlayerPawnCapsule::MoveRight(float Value)
{
    if (Value != 0.f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}
