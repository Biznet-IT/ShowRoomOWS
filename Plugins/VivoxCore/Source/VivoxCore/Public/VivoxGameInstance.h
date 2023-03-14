// Copyright 2022 Sabre Dart Studios

#pragma once

#include "VivoxConfig.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VivoxGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIVOXCORE_API UVivoxGameInstance : public UGameInstance
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
