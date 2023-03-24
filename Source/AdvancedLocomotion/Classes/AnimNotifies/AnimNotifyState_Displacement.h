// Copyright 2020-2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Displacement.generated.h"

/**
 * UAnimNotifyState_Displacement
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin, DisplayName = "Displacement"))
class ADVANCEDLOCOMOTION_API UAnimNotifyState_Displacement : public UAnimNotifyState
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
	UPROPERTY(EditAnywhere, Category = Translation)
	FName DistCurveName;

	UPROPERTY(EditAnywhere, Category = Translation)
	float DistCurveValueMultiple;

	UPROPERTY(EditAnywhere, Category = Translation)
	FName TurnCurveName;

	UPROPERTY(Transient)
	float PlayPostion;
};
