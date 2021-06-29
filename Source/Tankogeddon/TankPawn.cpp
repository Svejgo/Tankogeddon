// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"

DECLARE_LOG_CATEGORY_EXTERN(TankLog, Log, All);
DEFINE_LOG_CATEGORY(TankLog);

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body mesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon component"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(FirstCannonClass);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	
	auto ForwardLocation = GetActorForwardVector() * MoveSpeed * CurrentMoveForwardAxis * DeltaTime;
	auto RightLocation = GetActorRightVector() * MoveSpeed * CurrentMoveRightAxis * DeltaTime;

	FVector NewLocation = CurrentLocation + ForwardLocation + RightLocation;		
	
	SetActorLocation(NewLocation);

	CurrentRotateRightAxis = FMath::Lerp(CurrentRotateRightAxis, TargetRotateRightAxis, RotationSensitivity);

	UE_LOG(TankLog, VeryVerbose, TEXT("CurrentRotateRightAxis=%f"), CurrentRotateRightAxis);

	FRotator CurrentRotation = GetActorRotation();
	float YawRotation = CurrentRotation.Yaw + RotateSpeed * CurrentRotateRightAxis * DeltaTime;
	FRotator NewRotation = FRotator(0.f, YawRotation, 0.f);
	SetActorRotation(NewRotation);

	if (TankController)
	{
		FVector TurretLookAtPoint = TankController->GetMousePos();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TurretLookAtPoint);
		FRotator CurTurrentRotation = TurretMesh->GetComponentRotation();
		TargetRotation.Pitch = CurTurrentRotation.Pitch;
		TargetRotation.Roll = CurTurrentRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(CurTurrentRotation, TargetRotation, TurretRotationSensitivity));

	}
}

void ATankPawn::MoveForward(float InAxisValue)
{
	CurrentMoveForwardAxis = InAxisValue;
}
void ATankPawn::MoveRight(float InAxisValue)
{
	CurrentMoveRightAxis = InAxisValue;
}
void ATankPawn::RotateRight(float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}void ATankPawn::AltFire()
{
	if (Cannon)
	{
		Cannon->AltFire();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> NewCannon)
{
	if (Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}

	if (NewCannon)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(NewCannon, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ATankPawn::AddAmmo(int32 Ammo)
{
	Cannon->AddAmmo(Ammo);
}

void ATankPawn::SwapWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon swapped"));
	//??????
	TSubclassOf<ACannon> TempCannon = this->FirstCannonClass;
	this->SetupCannon(SecondCannonClass);
	SecondCannonClass = TempCannon;
}