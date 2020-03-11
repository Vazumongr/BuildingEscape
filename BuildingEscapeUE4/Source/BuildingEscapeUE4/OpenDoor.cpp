// Copyright Troy Records Jr. 2020


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if(!PressurePlate)
		UE_LOG(LogTemp,Error, TEXT("%s is missing PressurePlate assignment!!!"), *GetOwner()->GetName());

	FindAudioComponent();
	SetupYawValues();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TotalMassOfActors() > MassThatOpens)
	{
		RotateDoorOpen(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			RotateDoorShut(DeltaTime);
		}
		
	}
		
}

void UOpenDoor::RotateDoorOpen(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DoorOpenSpeed * DeltaTime);
	CurrentRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(CurrentRotation);

	if(!AudioComponent){return;}
	if(!DoorOpen && CurrentYaw != InitialYaw)
	{
		AudioComponent->Play();
		DoorOpen = true;
	}

}

void UOpenDoor::RotateDoorShut(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DoorCloseSpeed * DeltaTime);
	CurrentRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(CurrentRotation);

	if(CurrentYaw - InitialYaw < 1)
		CurrentYaw = InitialYaw;

	if(!AudioComponent){return;}
	if(DoorOpen && CurrentYaw == InitialYaw)
	{
		AudioComponent->Play();
		DoorOpen = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	//Find overlapping actors

	TArray<AActor*> OverlappingActors;

	if(!PressurePlate){return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning,TEXT("%f"), TotalMass);
	}

	//Add masses
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	
	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing its AudioComponent!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::SetupYawValues()
{
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
}