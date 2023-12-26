// Copyright 2020-2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_DisableOrientRotationToMovement.generated.h"

/**
 * UAnimNotifyState_DisableOrientRotationToMovement
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin, DisplayName = "DisableOrientRotationToMovement"))
class ADVANCEDLOCOMOTION_API UAnimNotifyState_DisableOrientRotationToMovement : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

#if ENGINE_MAJOR_VERSION >= 5
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
#else
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#endif
};
