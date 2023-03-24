// Copyright 2018 CQUnreal, Inc. All Rights Reserved.

#pragma once

#include "TurnInPlaceInterface.generated.h"

UINTERFACE()
class ADVANCEDLOCOMOTION_API UTurnInPlaceInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDLOCOMOTION_API ITurnInPlaceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TurnInPlaceInterface")
	void TurnBegin();	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TurnInPlaceInterface")
	void TurnTick(const FRotator& DeltaRotation, float FrameDeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TurnInPlaceInterface")
	void TurnEnd();
};