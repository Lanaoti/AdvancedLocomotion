// Copyright 2020 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneIndices.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimNode_FootIK.generated.h"

class FPrimitiveDrawInterface;
class USkeletalMeshComponent;

/**
 *	Controller which implements the FootIK
 */
USTRUCT(BlueprintInternalUseOnly)
struct ADVANCEDLOCOMOTION_API FAnimNode_FootIK : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	/** Name of pelvis bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference PelvisBone;

	/** Name of foot_l bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference LeftFootBone;

	/** Name of foot_r bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference RightFootBone;

	/** Name of ik_foot_l bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference LeftIKFootBone;

	/** Name of ik_foot_r bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference RightIKFootBone;

	/** Name of VB_thight_l_calf_l bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference LeftThightCalfVBone;

	/** Name of VB_thight_r_calf_r bone */
	UPROPERTY(EditAnywhere, Category = Solver)
	FBoneReference RightThightCalfVBone;

	/** Pelvis Z offset */
	UPROPERTY(VisibleAnywhere, Category = Offsets)
	float PelvisZOffset;

	/** Left foot Z offset */
	UPROPERTY(VisibleAnywhere, Category = Offsets)
	float LeftFootZOffset;

	/** Right foot Z offset */
	UPROPERTY(VisibleAnywhere, Category = Offsets)
	float RightFootZOffset;

	/** Left foot rotation offset */
	UPROPERTY(VisibleAnywhere, Category = Offsets)
	FRotator LeftFootRotationOffset;

	/** Right foot rotation offset */
	UPROPERTY(VisibleAnywhere, Category = Offsets)
	FRotator RightFootRotationOffset;

	/** Left foot VB offset */
	UPROPERTY(EditAnywhere, Category = Offsets)
	FVector LeftFootVBOffset;

	/** Right foot VB offset */
	UPROPERTY(EditAnywhere, Category = Offsets)
	FVector RightFootVBOffset;

	/** Z offset max limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float ZOffsetMaxLimit;

	/** Z offset min limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float ZOffsetMinLimit;

	/** Roll offset max limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float RollOffsetMaxLimit;

	/** Roll offset min limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float RollOffsetMinLimit;

	/** Pitch offset max limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float PitchOffsetMaxLimit;

	/** Pitch ffset min limit */
	UPROPERTY(EditAnywhere, Category = OffsetLimits)
	float PitchOffsetMinLimit;

	/** Z offset fast interp speed */
	UPROPERTY(EditAnywhere, Category = InterpSpeeds)
	float ZOffsetFastInterpSpeed;

	/** Z offset slow interp speed */
	UPROPERTY(EditAnywhere, Category = InterpSpeeds)
	float ZOffsetSlowInterpSpeed;

	/** Rotation offset interp speed */
	UPROPERTY(EditAnywhere, Category = InterpSpeeds)
	float RotationOffsetInterpSpeed;

	/** Trace start height */
	UPROPERTY(EditAnywhere, Category = TraceSettings)
	float TraceStartHeight;

	/** Trace length below capsule */
	UPROPERTY(EditAnywhere, Category = TraceSettings)
	float TraceLengthBelowCapsule;

	/** Left foot target */
	UPROPERTY(VisibleAnywhere, Category = TraceSettings)
	float LeftFootTarget;

	/** Right foot target */
	UPROPERTY(VisibleAnywhere, Category = TraceSettings)
	float RightFootTarget;

#if WITH_EDITORONLY_DATA
	/** Toggle drawing of axes to debug foot ik*/
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bEnableDebugDraw;
#endif

public:
	FAnimNode_FootIK();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

	virtual void ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const;

private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

protected:
	virtual bool TraceFootIK(float DeltaSeconds, class USkeletalMeshComponent* SkeletalMeshComponent, const FName& FootName, const float OtherFootTarget, float& OutTraceZOffset, float& OutFootZOffset, float& OutFootTarget, FRotator& OutFootRotationOffset);
};
