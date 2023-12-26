// Copyright 2020-2022 CQUnreal. All Rights Reserved.

#include "AnimNotifyState_DisableOrientRotationToMovement.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/EngineVersionComparison.h"


UAnimNotifyState_DisableOrientRotationToMovement::UAnimNotifyState_DisableOrientRotationToMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if UE_VERSION_NEWER_THAN(5, 0, 0)
void UAnimNotifyState_DisableOrientRotationToMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
#else
void UAnimNotifyState_DisableOrientRotationToMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	UCharacterMovementComponent* MovementComponent = Character ? Character->GetCharacterMovement() : nullptr;
	if (MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = false;
	}
}

#if UE_VERSION_NEWER_THAN(5, 0, 0)
void UAnimNotifyState_DisableOrientRotationToMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
#else
void UAnimNotifyState_DisableOrientRotationToMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	UCharacterMovementComponent* MovementComponent = Character ? Character->GetCharacterMovement() : nullptr;
	if (MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = true;
	}
}
