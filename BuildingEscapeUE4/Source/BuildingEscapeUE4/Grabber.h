// Copyright Troy Records Jr. 2020

#pragma once

#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEUE4_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float PlayerReach = 150.f;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	//Get the players reach
	FVector FindLineTraceEnd() const;

	//Return the first actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Get the playes location in world
	FVector GetPlayersWorldPos() const;
		
};
