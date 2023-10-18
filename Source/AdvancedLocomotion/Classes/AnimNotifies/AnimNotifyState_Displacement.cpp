// Copyright 2020-2022 CQUnreal. All Rights Reserved.

#include "AnimNotifyState_Displacement.h"
#include "Animation/AnimCurveTypes.h"
#include "Animation/AnimSequenceBase.h"
#include "Interfaces/DisplacementInterface.h"
#include "GameFramework/Character.h"


UAnimNotifyState_Displacement::UAnimNotifyState_Displacement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, DistCurveValueMultiple(1.f)
{

}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_Displacement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
#else
void UAnimNotifyState_Displacement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character && Character->GetClass()->ImplementsInterface(UDisplacementInterface::StaticClass()))
	{
		IDisplacementInterface::Execute_DisplacementBegin(Character);
	}

	PlayPostion = 0.f;
}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_Displacement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
#else
void UAnimNotifyState_Displacement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
#endif

	PlayPostion += FrameDeltaTime;

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character && Character->GetClass()->ImplementsInterface(UDisplacementInterface::StaticClass()) && Animation)
	{
		const FRawCurveTracks& CurveTracks = Animation->GetCurveData();
		for (const FFloatCurve& Curve : CurveTracks.FloatCurves)
		{
			const float PlayRate = Animation->RateScale;
			const float Value = Curve.Evaluate(PlayPostion) * DistCurveValueMultiple;
			const float FrameDeltaValue = Curve.Evaluate(PlayPostion - PlayRate * FrameDeltaTime);

			#if ENGINE_MAJOR_VERSION >=5 && ENGINE_MINOR_VERSION >= 3
			if (Curve.GetName().IsEqual(DistCurveName))
			#else
			if (Curve.Name.DisplayName.IsEqual(DistCurveName))
			#endif
			{
				FVector DeltaLocation = FVector(Value - FrameDeltaValue, 0.f, 0.f);
				Character->AddActorLocalOffset(DeltaLocation, true);
			}

			#if ENGINE_MAJOR_VERSION >=5 && ENGINE_MINOR_VERSION >= 3
			if (Curve.GetName().IsEqual(TurnCurveName))
			#else
			if (Curve.Name.DisplayName.IsEqual(TurnCurveName))
			#endif
			{
				FRotator DeltaRotation = FRotator(0.f, FrameDeltaValue - Value, 0.f);
				Character->AddActorLocalRotation(DeltaRotation);
				IDisplacementInterface::Execute_DisplacementTick(Character, DeltaRotation, FrameDeltaTime);
			}
		}
	}
}

#if ENGINE_MAJOR_VERSION >= 5
void UAnimNotifyState_Displacement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
#else
void UAnimNotifyState_Displacement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
#endif

	ACharacter* Character = MeshComp ? Cast<ACharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character && Character->GetClass()->ImplementsInterface(UDisplacementInterface::StaticClass()))
	{
		IDisplacementInterface::Execute_DisplacementEnd(Character);
	}
}
