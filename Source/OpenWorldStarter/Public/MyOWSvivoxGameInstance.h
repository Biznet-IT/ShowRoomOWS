// Copyright 2022 Sabre Dart Studios

#pragma once

#include "VivoxConfig.h"
#include "CoreMinimal.h"
#include "OWSGameInstance.h"
#include "MyOWSvivoxGameInstance.generated.h"


UCLASS()
class OPENWORLDSTARTER_API UMyOWSvivoxGameInstance : public UOWSGameInstance
{
	GENERATED_BODY()
	
	virtual void Shutdown() override;
	virtual void Init() override;

	void InitVivox();
	void Login();
	void JoinChannel();

	IClient* VivoxVoiceClient;
	AccountId LoggedInUserId;
};
