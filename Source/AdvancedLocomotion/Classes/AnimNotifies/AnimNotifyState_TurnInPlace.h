// Copyright 2018 CQUnreal, Inc. All Rights Reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_TurnInPlace.generated.h"

UCLASS(Blueprintable, meta = (ShowWorldContextPin, DisplayName = "TurnInPlace"))
class ADVANCEDLOCOMOTION_API UAnimNotifyState_TurnInPlace : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

#if ENGINE_MAJOR_VERSION >= 5
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
#else
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = TurnInPlace)
	class UCurveFloat* RotationCurve;

	UPROPERTY(EditAnywhere, Category = TurnInPlace)
	class UAnimMontage* Montage;
};
