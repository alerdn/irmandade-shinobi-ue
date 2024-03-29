// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NinjaCharacter.generated.h"

class UInputAction;

UCLASS()
class IRMANDADESHINOBI_API ANinjaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANinjaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext *InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction *MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction *LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction *RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float CameraSensibility = 15.f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float RotationSpeed = 50.f;

	float WalkSpeed;

	class APlayerController *PlayerController;
	class UCharacterMovementComponent *MovementComp;
	class UMeshComponent *MeshComp;

	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void Run(const FInputActionValue &Value);
};
