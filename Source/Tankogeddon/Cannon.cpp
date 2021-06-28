// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/ArrowComponent.h>

// Sets default values
ACannon::ACannon()
{
 
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("Spawn point");
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if (!bIsReadyToFire)
	{
		return;
	}
	bIsReadyToFire = false;
	if (Ammo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, TEXT("Out of ammo"));
		return;
	}
	bIsReadyToFire = false;


	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Green, TEXT("Fire - projectile"));
		Ammo--;
	}
	if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Green, TEXT("Fire - trace"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.0f / FireRate, false);
}

void ACannon::AltFire()
{
	if (!bIsReadyToFire)
	{
		return;
	}
	bIsReadyToFire = false;

	if (Ammo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, TEXT("Out of ammo"));
		return;
	}
	bIsReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{		
		
		GetWorld()->GetTimerManager().SetTimer(RapidFireTimerHandle, FTimerDelegate::CreateLambda([=]() 
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("AltFire - rapidshot - projectile"), true);
			}), AltFireShotsRate, true);
		GetWorld()->GetTimerManager().SetTimer(RapidFireTimerHandleDelegator, FTimerDelegate::CreateLambda([=]()
			{
				GetWorldTimerManager().ClearTimer(RapidFireTimerHandle);
			}
		), AltFireShotsRate + (AltFireShotsRate * AltFireShots), false);
	
		Ammo--;
	}
	if (Type == ECannonType::FireTrace)
	{
		GetWorld()->GetTimerManager().SetTimer(RapidFireTimerHandle, FTimerDelegate::CreateLambda([=]()
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("AltFire - rapidshot - trace"), true);
			}), AltFireShotsRate, true);
		GetWorld()->GetTimerManager().SetTimer(RapidFireTimerHandleDelegator, FTimerDelegate::CreateLambda([=]()
			{
				GetWorldTimerManager().ClearTimer(RapidFireTimerHandle);
			}
		), AltFireShotsRate + (AltFireShotsRate * AltFireShots), false);

		Ammo--;
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.0f / AltFireRate, false);
}

bool ACannon::IsReadyToFire() const
{
	return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::EndPlay(EEndPlayReason::Type Reason)
{
	ReloadTimerHandle.Invalidate();
	RapidFireTimerHandle.Invalidate();
	Super::EndPlay(Reason);
	
}


void ACannon::Reload()
{
	bIsReadyToFire = true;
}

