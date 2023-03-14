// Copyright 2022 Sabre Dart Studios


#include "VivoxGameInstance.h"
#include <VivoxCore.h>

#define VIVOX_VOICE_SERVER TEXT("https://unity.vivox.com/appconfig/46736-showr-11582-udash")
#define VIVOX_VOICE_DOMAIN TEXT("mtu1xp.vivox.com")
#define VIVOX_VOICE_ISSUER TEXT("46736-showr-11582-udash")
#define VIVOX_VOICE_KEY TEXT("mYF4nfoUJxva8VFIgb7gEFOiGX4RTeK8")


void UVivoxGameInstance::Init()
{
	Super::Init();
	InitVivox();
}

void UVivoxGameInstance::Shutdown()
{
	Super::Shutdown();
	VivoxVoiceClient->Uninitialize();
}

void UVivoxGameInstance::InitVivox()
{
	VivoxVoiceClient = &static_cast<FVivoxCoreModule*>(&FModuleManager::Get().LoadModuleChecked(TEXT("VivoxCore")))->VoiceClient();
	VivoxVoiceClient->Initialize();
	Login();
}

void UVivoxGameInstance::Login()
{
	LoggedInUserId = AccountId(VIVOX_VOICE_ISSUER, "PlayerId", VIVOX_VOICE_DOMAIN);
	ILoginSession& MyLoginSession(VivoxVoiceClient->GetLoginSession(LoggedInUserId));

	FTimespan TokenExpiration = FTimespan::FromSeconds(90);
	FString LoginToken = MyLoginSession.GetLoginToken(VIVOX_VOICE_KEY, TokenExpiration);

	ILoginSession::FOnBeginLoginCompletedDelegate OnBeginLoginCompleted;
	OnBeginLoginCompleted.BindLambda([this, &MyLoginSession](VivoxCoreError Error)
	{
		UE_LOG(LogTemp, Log, TEXT("Logged into Vivox! :D"));
		JoinChannel();
	});

	MyLoginSession.BeginLogin(VIVOX_VOICE_SERVER, LoginToken, OnBeginLoginCompleted);
}

void UVivoxGameInstance::JoinChannel()
{
	ILoginSession& MyLoginSession = VivoxVoiceClient->GetLoginSession(LoggedInUserId);
	ChannelId Channel = ChannelId(VIVOX_VOICE_ISSUER, "ChannelId", VIVOX_VOICE_DOMAIN, ChannelType::Echo);
	IChannelSession& ChannelSession = MyLoginSession.GetChannelSession(Channel);

	FTimespan TokenExpiration = FTimespan::FromSeconds(90);
	FString JoinToken = ChannelSession.GetConnectToken(VIVOX_VOICE_KEY, TokenExpiration);

	ChannelSession.BeginConnect(true, false, true, JoinToken, NULL);

}