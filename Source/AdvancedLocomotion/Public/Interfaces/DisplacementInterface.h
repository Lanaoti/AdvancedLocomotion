// Copyright 2018 CQUnreal, Inc. All Rights Reserved.

#pragma once

#include "DisplacementInterface.generated.h"

UINTERFACE()
class ADVANCEDLOCOMOTION_API UDisplacementInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDLOCOMOTION_API IDisplacementInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DisplacementInterface")
	void DisplacementBegin();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DisplacementInterface")
	void DisplacementTick(const FRotator& DeltaRotation, float FrameDeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DisplacementInterface")
	void DisplacementEnd();
};