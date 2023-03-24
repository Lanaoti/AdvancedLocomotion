// Copyright 2020 CQUnreal. All Rights Reserved.

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FAdvancedLocomotionModule"

class FAdvancedLocomotionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{

	}

	virtual void ShutdownModule() override
	{

	}
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAdvancedLocomotionModule, AdvancedLocomotion)