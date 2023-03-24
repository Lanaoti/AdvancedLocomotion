// Copyright 2020 CQUnreal. All Rights Reserved.

#include "AnimGraphNode_FootIK.h"
#include "Animation/AnimInstance.h"
#include "AnimNodeEditModes.h"
#include "AnimationCustomVersion.h"
#include "Kismet2/CompilerResultsLog.h"

/////////////////////////////////////////////////////
// UAnimGraphNode_FootIK 

#define LOCTEXT_NAMESPACE "AnimGraphNode_FootIK"

UAnimGraphNode_FootIK::UAnimGraphNode_FootIK(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_FootIK::GetControllerDescription() const
{
	return LOCTEXT("FootIK", "FootIK");
}

void UAnimGraphNode_FootIK::Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
	if (PreviewSkelMeshComp)
	{
		if (FAnimNode_FootIK* ActiveNode = GetActiveInstanceNode<FAnimNode_FootIK>(PreviewSkelMeshComp->GetAnimInstance()))
		{
			ActiveNode->ConditionalDebugDraw(PDI, PreviewSkelMeshComp);
		}
	}
}

FText UAnimGraphNode_FootIK::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetControllerDescription();
}

#if UE_EDITOR
void UAnimGraphNode_FootIK::ValidateBone(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog, const FName& BoneName, const FName& TipName)
{
	if (ForSkeleton->GetReferenceSkeleton().FindBoneIndex(BoneName) == INDEX_NONE)
	{
		if (BoneName == NAME_None)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("TipName"), FText::FromName(TipName));

			FText Msg = FText::Format(LOCTEXT("NoBoneSelected", "@@ - You must pick a '{TipName}' bone"), Args);

			MessageLog.Warning(*Msg.ToString(), this);
		}
		else
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("BoneName"), FText::FromName(BoneName));

			FText Msg = FText::Format(LOCTEXT("NoBoneFound", "@@ - Bone {BoneName} not found in Skeleton"), Args);

			MessageLog.Warning(*Msg.ToString(), this);
		}
	}
}

void UAnimGraphNode_FootIK::ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog)
{
	// Temporary fix where skeleton is not fully loaded during AnimBP compilation and thus virtual bone name check is invalid UE-39499 (NEED FIX) 
	if (ForSkeleton && !ForSkeleton->HasAnyFlags(RF_NeedPostLoad))
	{
		ValidateBone(ForSkeleton, MessageLog, Node.PelvisBone.BoneName, TEXT("Pelvis Bone"));
		ValidateBone(ForSkeleton, MessageLog, Node.LeftFootBone.BoneName, TEXT("Left Foot Bone"));
		ValidateBone(ForSkeleton, MessageLog, Node.RightFootBone.BoneName, TEXT("Right Foot Bone"));
		ValidateBone(ForSkeleton, MessageLog, Node.LeftIKFootBone.BoneName, TEXT("Left IK Foot Bone"));
		ValidateBone(ForSkeleton, MessageLog, Node.RightIKFootBone.BoneName, TEXT("Right IK Foot Bone"));
		ValidateBone(ForSkeleton, MessageLog, Node.LeftThightCalfVBone.BoneName, TEXT("Left Thight Calf VBone"));
		ValidateBone(ForSkeleton, MessageLog, Node.RightThightCalfVBone.BoneName, TEXT("Right Thight Calf VBone"));
	}

	Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
}

void UAnimGraphNode_FootIK::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode)
{
	FAnimNode_FootIK* FootIK = static_cast<FAnimNode_FootIK*>(InPreviewNode);

}

FEditorModeID UAnimGraphNode_FootIK::GetEditorMode() const
{
	return TEXT("AnimGraph.SkeletalControl.FootIK");
}
#endif

void UAnimGraphNode_FootIK::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FAnimationCustomVersion::GUID);

	const int32 CustomAnimVersion = Ar.CustomVer(FAnimationCustomVersion::GUID);

	if (CustomAnimVersion < FAnimationCustomVersion::ConvertIKToSupportBoneSocketTarget)
	{

	}
}

#undef LOCTEXT_NAMESPACE
