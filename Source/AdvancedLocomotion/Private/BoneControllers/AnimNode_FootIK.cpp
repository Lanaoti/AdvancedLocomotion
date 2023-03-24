// Copyright 2020 CQUnreal. All Rights Reserved.

#include "BoneControllers/AnimNode_FootIK.h"
#include "AnimationRuntime.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstanceProxy.h"

/////////////////////////////////////////////////////
// AnimNode_FootIK

FAnimNode_FootIK::FAnimNode_FootIK()
	: PelvisZOffset(0.f)
	, LeftFootZOffset(0.f)
	, RightFootZOffset(0.f)
	, LeftFootRotationOffset(FRotator::ZeroRotator)
	, RightFootRotationOffset(FRotator::ZeroRotator)
	, LeftFootVBOffset(25.f, 25.f, 0.f)
	, RightFootVBOffset(-25.f, -25.f, 0.f)
	, ZOffsetMaxLimit(30.f)
	, ZOffsetMinLimit(-50.f)
	, RollOffsetMaxLimit(30.f)
	, RollOffsetMinLimit(-30.f)
	, PitchOffsetMaxLimit(30.f)
	, PitchOffsetMinLimit(-30.f)
	, ZOffsetFastInterpSpeed(25.f)
	, ZOffsetSlowInterpSpeed(10.f)
	, RotationOffsetInterpSpeed(15.f)
	, TraceStartHeight(50.f)
	, TraceLengthBelowCapsule(75.f)
	, LeftFootTarget(0.f)
	, RightFootTarget(0.f)
#if WITH_EDITORONLY_DATA
	,bEnableDebugDraw(false)
#endif
{

}

void FAnimNode_FootIK::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread)

	USkeletalMeshComponent* SkeletalMeshComponent = Output.AnimInstanceProxy->GetSkelMeshComponent();
	if (SkeletalMeshComponent == nullptr || SkeletalMeshComponent->bBlendPhysics)
	{
		return;
	}

	UWorld* World = SkeletalMeshComponent->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const float DeltaSeconds = World->GetDeltaSeconds();

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();

	// modify pelvis bone
	float LeftTraceZOffset = 0.f;
	float RightTraceZOffset = 0.f;

	TraceFootIK(DeltaSeconds, SkeletalMeshComponent, LeftFootBone.BoneName, RightFootTarget, LeftTraceZOffset, LeftFootZOffset, LeftFootTarget, LeftFootRotationOffset);
	TraceFootIK(DeltaSeconds, SkeletalMeshComponent, RightFootBone.BoneName, LeftFootTarget, RightTraceZOffset, RightFootZOffset, RightFootTarget, RightFootRotationOffset);

	const float TargetPelvisZOffset = LeftTraceZOffset <= RightTraceZOffset ? LeftFootTarget : RightFootTarget;

	PelvisZOffset = FMath::FInterpTo(PelvisZOffset, TargetPelvisZOffset, DeltaSeconds, ZOffsetSlowInterpSpeed);

	FCompactPoseBoneIndex PelvisBoneToModify = PelvisBone.GetCompactPoseIndex(BoneContainer);
	FTransform PelvisBoneTM = Output.Pose.GetComponentSpaceTransform(PelvisBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, PelvisBoneTM, PelvisBoneToModify, EBoneControlSpace::BCS_ComponentSpace);
	PelvisBoneTM.AddToTranslation(FVector(0.f, 0.f, PelvisZOffset));
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, PelvisBoneTM, PelvisBoneToModify, EBoneControlSpace::BCS_ComponentSpace);

	OutBoneTransforms.Add(FBoneTransform(PelvisBone.GetCompactPoseIndex(BoneContainer), PelvisBoneTM));

	// modify left foot ik
	FCompactPoseBoneIndex LeftIKFootBoneToModify = LeftIKFootBone.GetCompactPoseIndex(BoneContainer);
	FTransform LeftIKFootTM = Output.Pose.GetComponentSpaceTransform(LeftIKFootBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, LeftIKFootTM, LeftIKFootBoneToModify, EBoneControlSpace::BCS_ComponentSpace);
	LeftIKFootTM.AddToTranslation(FVector(0.f, 0.f, LeftFootZOffset));
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, LeftIKFootTM, LeftIKFootBoneToModify, EBoneControlSpace::BCS_ComponentSpace);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, LeftIKFootTM, LeftIKFootBoneToModify, EBoneControlSpace::BCS_WorldSpace);
	LeftIKFootTM.SetRotation(LeftFootRotationOffset.Quaternion() * LeftIKFootTM.GetRotation());
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, LeftIKFootTM, LeftIKFootBoneToModify, EBoneControlSpace::BCS_WorldSpace);

	// modify right foot ik
	FCompactPoseBoneIndex RightIKFootBoneToModify = RightIKFootBone.GetCompactPoseIndex(BoneContainer);
	FTransform RightIKFootTM = Output.Pose.GetComponentSpaceTransform(RightIKFootBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, RightIKFootTM, RightIKFootBoneToModify, EBoneControlSpace::BCS_ComponentSpace);
	RightIKFootTM.AddToTranslation(FVector(0.f, 0.f, RightFootZOffset));
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, RightIKFootTM, RightIKFootBoneToModify, EBoneControlSpace::BCS_ComponentSpace);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, RightIKFootTM, RightIKFootBoneToModify, EBoneControlSpace::BCS_WorldSpace);
	RightIKFootTM.SetRotation(RightFootRotationOffset.Quaternion() * RightIKFootTM.GetRotation());
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, RightIKFootTM, RightIKFootBoneToModify, EBoneControlSpace::BCS_WorldSpace);

	OutBoneTransforms.Add(FBoneTransform(LeftIKFootBone.GetCompactPoseIndex(BoneContainer), LeftIKFootTM));
	OutBoneTransforms.Add(FBoneTransform(RightIKFootBone.GetCompactPoseIndex(BoneContainer), RightIKFootTM));

	// modify left foot ik vb
	FCompactPoseBoneIndex LeftThightCalfVBoneToModify = LeftThightCalfVBone.GetCompactPoseIndex(BoneContainer);
	FTransform LeftThightCalfTM = Output.Pose.GetComponentSpaceTransform(LeftThightCalfVBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, LeftThightCalfTM, LeftThightCalfVBoneToModify, EBoneControlSpace::BCS_BoneSpace);
	LeftThightCalfTM.AddToTranslation(LeftFootVBOffset);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, LeftThightCalfTM, LeftThightCalfVBoneToModify, EBoneControlSpace::BCS_BoneSpace);

	// modify right foot ik vb
	FCompactPoseBoneIndex RightThightCalfVBoneToModify = RightThightCalfVBone.GetCompactPoseIndex(BoneContainer);
	FTransform RightThightCalfTM = Output.Pose.GetComponentSpaceTransform(RightThightCalfVBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, RightThightCalfTM, RightThightCalfVBoneToModify, EBoneControlSpace::BCS_BoneSpace);
	RightThightCalfTM.AddToTranslation(RightFootVBOffset);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, RightThightCalfTM, RightThightCalfVBoneToModify, EBoneControlSpace::BCS_BoneSpace);

	OutBoneTransforms.Add(FBoneTransform(LeftThightCalfVBone.GetCompactPoseIndex(BoneContainer), LeftThightCalfTM));
	OutBoneTransforms.Add(FBoneTransform(RightThightCalfVBone.GetCompactPoseIndex(BoneContainer), RightThightCalfTM));
	OutBoneTransforms.Sort([](const FBoneTransform& A, const FBoneTransform& B) {
		return A.BoneIndex < B.BoneIndex;
	});
}

bool FAnimNode_FootIK::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// Allow evaluation if all parameters are initialized and TipBone is child of RootBone
	return
		(
			PelvisBone.IsValidToEvaluate(RequiredBones)
			&& LeftFootBone.IsValidToEvaluate(RequiredBones)
			&& RightFootBone.IsValidToEvaluate(RequiredBones)
			&& LeftIKFootBone.IsValidToEvaluate(RequiredBones)
			&& RightIKFootBone.IsValidToEvaluate(RequiredBones)
			&& LeftThightCalfVBone.IsValidToEvaluate(RequiredBones)
			&& RightThightCalfVBone.IsValidToEvaluate(RequiredBones)
			);
}

void FAnimNode_FootIK::ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
#if WITH_EDITORONLY_DATA
	if (bEnableDebugDraw && PreviewSkelMeshComp && PreviewSkelMeshComp->GetWorld())
	{
		// to do something
	}
#endif
}

void FAnimNode_FootIK::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)
	PelvisBone.Initialize(RequiredBones);
	LeftFootBone.Initialize(RequiredBones);
	RightFootBone.Initialize(RequiredBones);
	LeftIKFootBone.Initialize(RequiredBones);
	RightIKFootBone.Initialize(RequiredBones);
	LeftThightCalfVBone.Initialize(RequiredBones);
	RightThightCalfVBone.Initialize(RequiredBones);
}

void FAnimNode_FootIK::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);

	DebugData.AddDebugItem(DebugLine);
	ComponentPose.GatherDebugData(DebugData);
}

void FAnimNode_FootIK::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	Super::Initialize_AnyThread(Context);
}

bool FAnimNode_FootIK::TraceFootIK(float DeltaSeconds, USkeletalMeshComponent* SkeletalMeshComponent, const FName& FootName, const float OtherFootTarget, float& OutTraceZOffset, float& OutFootZOffset, float& OutFootTarget, FRotator& OutFootRotationOffset)
{
	if (SkeletalMeshComponent == nullptr)
	{
		return false;
	}

	UWorld* World = SkeletalMeshComponent->GetWorld();
	if (World == nullptr)
	{
		return false;
	}

	const FVector& ComponentLocation = SkeletalMeshComponent->GetComponentLocation();
	const FVector& FootLocation = SkeletalMeshComponent->GetSocketLocation(FootName);

	const FVector StartTrace(FootLocation.X, FootLocation.Y, ComponentLocation.Z + TraceStartHeight);
	const FVector EndTrace(FootLocation.X, FootLocation.Y, ComponentLocation.Z - TraceLengthBelowCapsule);

	FHitResult HitResult;
	const bool bFootTraceHit = World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

	const float Roll = FMath::Clamp(180.f / PI * FMath::Atan2(HitResult.ImpactNormal.Y, HitResult.ImpactNormal.Z), RollOffsetMinLimit, RollOffsetMaxLimit);
	const float Pitch = FMath::Clamp(180.f / PI * FMath::Atan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Z), PitchOffsetMinLimit, PitchOffsetMaxLimit) * -1.f;

	const FRotator TargetRotation(Pitch, 0.f, Roll);

	OutFootRotationOffset = FMath::RInterpTo(OutFootRotationOffset, TargetRotation, DeltaSeconds, RotationOffsetInterpSpeed);
	OutTraceZOffset = FMath::Clamp(HitResult.Location.Z - ComponentLocation.Z, ZOffsetMinLimit, ZOffsetMaxLimit);
	OutFootTarget = bFootTraceHit ? OutTraceZOffset : 0.f;

	const float ZOffsetInterpSpeed = OutFootTarget > OtherFootTarget ? ZOffsetFastInterpSpeed : (OutFootZOffset < OutFootTarget ? ZOffsetFastInterpSpeed : ZOffsetSlowInterpSpeed);

	OutFootZOffset = FMath::FInterpTo(OutFootZOffset, OutFootTarget, DeltaSeconds, ZOffsetInterpSpeed);

	return bFootTraceHit;
}