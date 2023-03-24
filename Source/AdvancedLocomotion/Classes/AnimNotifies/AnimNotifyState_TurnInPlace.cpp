// Copyright 2018 CQUnreal, Inc. All Rights Reserved.

#include "AnimNotifyState_TurnInPlace.h"
#include "Interfaces/TurnInPlaceInterface.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"


UAnimNotifyState_TurnInPlace::UAnimNotifyState_TurnInPlace(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_TurnInPlace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
#else
void UAnimNotifyState_TurnInPlace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character && Character->GetClass()->ImplementsInterface(UTurnInPlaceInterface::StaticClass()))
	{
		ITurnInPlaceInterface::Execute_TurnBegin(Character);
	}
}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_TurnInPlace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
#else
void UAnimNotifyState_TurnInPlace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
#endif

	if (Montage != nullptr)
	{
		ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
		UAnimInstance* AnimInstance = MeshComp ? Cast<UAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;
		if (Character && Character->GetClass()->ImplementsInterface(UTurnInPlaceInterface::StaticClass()) && AnimInstance)
		{
			const float PlayPosition = AnimInstance->Montage_GetPosition(Montage);
			const float PlayRate = AnimInstance->Montage_GetPlayRate(Montage);

			if (RotationCurve != nullptr)
			{
				const float TurnYaw = RotationCurve->GetFloatValue(PlayPosition);
				const float FrameDeltaYaw = RotationCurve->GetFloatValue(PlayPosition - PlayRate * FrameDeltaTime);

				const FRotator DeltaRotation(0.f, TurnYaw - FrameDeltaYaw, 0.f);

				ITurnInPlaceInterface::Execute_TurnTick(Character, DeltaRotation, FrameDeltaTime);
			}

			if (Character->GetVelocity().SizeSquared() > 10.f)
			{
				AnimInstance->Montage_Stop(Montage->BlendOut.GetBlendTime(), Montage);
			}
		}
	}
}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_TurnInPlace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
#else
void UAnimNotifyState_TurnInPlace::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character && Character->GetClass()->ImplementsInterface(UTurnInPlaceInterface::StaticClass()))
	{
		ITurnInPlaceInterface::Execute_TurnEnd(Character);
	}
}


