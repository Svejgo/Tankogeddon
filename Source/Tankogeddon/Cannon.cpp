// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Projectile.h"
#include "DamageTaker.h"
#include "DrawDebugHelpers.h"
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
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->MoveRange = FireRange;
			Projectile->Start();
		}
		Ammo--;
		UE_LOG(LogTemp, Warning, TEXT("Ammo - %d"), this->Ammo);
	}
	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Green, TEXT("Fire - trace"));

		FHitResult HitResult;
		FVector StartPoint = ProjectileSpawnPoint->GetComponentLocation();
		FVector EndPoint = StartPoint + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("FireTrace"), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), StartPoint, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			AActor* MyOwner = GetOwner();
			AActor* OwnerByOwner = MyOwner != nullptr ? MyOwner->GetOwner() : nullptr;
			if (HitResult.Actor.IsValid())
			{
				if (HitResult.Actor != MyOwner && HitResult.Actor != OwnerByOwner)
				{
					IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(HitResult.Actor);
					if (DamageTakerActor)
					{
						FDamageData DamageData;
						DamageData.DamageValue = TraceDamage;
						DamageData.Instigator = MyOwner;
						DamageData.DamageMaker = this;

						DamageTakerActor->TakeDamage(DamageData);
					}
				}
				//HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.5f, 0, 5);
		}
		Ammo--;
		UE_LOG(LogTemp, Warning, TEXT("Ammo - %d"), this->Ammo);
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
				AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
				if (Projectile)
				{
					Projectile->MoveRange = FireRange;
					Projectile->Start();
				};
			}), AltFireShotsRate, true);
		GetWorld()->GetTimerManager().SetTimer(RapidFireTimerHandleDelegator, FTimerDelegate::CreateLambda([=]()
			{
				GetWorldTimerManager().ClearTimer(RapidFireTimerHandle);
			}
		), (AltFireShotsRate * AltFireShots), false);
	
		Ammo--;
		UE_LOG(LogTemp, Warning, TEXT("Ammo - %d"), this->Ammo);
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
		UE_LOG(LogTemp, Warning, TEXT("Ammo - %d"), this->Ammo);
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

void ACannon::AddAmmo(int32 AddedAmmo)
{
	Ammo += AddedAmmo;
}
