// Fill out your copyright notice in the Description page of Project Settings.

#include "NinjaCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"

// Sets default values
ANinjaCharacter::ANinjaCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANinjaCharacter::BeginPlay()
{
	Super::BeginPlay();

	MovementComp = FindComponentByClass<UCharacterMovementComponent>();
	WalkSpeed = MovementComp->MaxWalkSpeed;

	MeshComp = FindComponentByClass<UMeshComponent>();

	// Pega referência do APlayerController
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// Pega InputLocalPlayerController
		UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			// Adiciona o InputMapping no sistema, com prioridade 0
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

// Called every frame
void ANinjaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANinjaCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Primeiro pega referência do UEnhancedInputComponent
	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// BindActions...
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANinjaCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANinjaCharacter::Look);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ANinjaCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ANinjaCharacter::Run);
	}
}

void ANinjaCharacter::Move(const FInputActionValue &Value)
{
	// É necessário adicionar movimento ao Forward e Right Vector
	// Porque o personagem rotaciona e o seu forward e right mudam
	FVector CurrentValue = Value.Get<FVector>();
	FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	FVector RightVector = PlayerController->PlayerCameraManager->GetActorRightVector();

	// Não precisa multiplicar por DeltaTime
	// porque o controller faz isso por baixao dos panos
	AddMovementInput(ForwardVector, CurrentValue.X);
	AddMovementInput(RightVector, CurrentValue.Y);

	FRotator Rotation(0.f, MovementComp->Velocity.Rotation().Yaw - 90.f, 0.f);
	MeshComp->SetWorldRotation(
		FMath::RInterpTo(MeshComp->GetComponentRotation(),
						 Rotation,
						 GetWorld()->GetDeltaSeconds(),
						 RotationSpeed));
}

void ANinjaCharacter::Look(const FInputActionValue &Value)
{
	FVector CurrentValue = Value.Get<FVector>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Precisa multiplicar por DeltaTime
	// e pelo rate de rotação
	AddControllerPitchInput(CurrentValue.Y * CameraSensibility * DeltaTime);
	AddControllerYawInput(CurrentValue.X * CameraSensibility * DeltaTime);
}

void ANinjaCharacter::Run(const FInputActionValue &Value)
{
	bool bRunning = Value.Get<bool>();
	float WalkSpeedModifier = 1.f;

	if (bRunning)
	{
		WalkSpeedModifier = 2.f;
	}
	else
	{
		WalkSpeedModifier = 1.f;
	}

	MovementComp->MaxWalkSpeed = WalkSpeed * WalkSpeedModifier;
}
