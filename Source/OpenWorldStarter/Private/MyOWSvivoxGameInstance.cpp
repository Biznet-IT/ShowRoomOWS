// Copyright 2022 Sabre Dart Studios

#include "Kismet/KismetMathLibrary.h"
#include "MyOWSvivoxGameInstance.h"
#include <VivoxCore.h>

#define VIVOX_VOICE_SERVER TEXT("https://unity.vivox.com/appconfig/46736-showr-11582-udash")
#define VIVOX_VOICE_DOMAIN TEXT("mtu1xp.vivox.com")
#define VIVOX_VOICE_ISSUER TEXT("46736-showr-11582-udash")
#define VIVOX_VOICE_KEY TEXT("mYF4nfoUJxva8VFIgb7gEFOiGX4RTeK8")

void UMyOWSvivoxGameInstance::Init()
{
	Super::Init();
	InitVivox();
}

void UMyOWSvivoxGameInstance::Shutdown()
{
	Super::Shutdown();
	VivoxVoiceClient->Uninitialize();
}

void UMyOWSvivoxGameInstance::InitVivox()
{
	VivoxVoiceClient = &static_cast<FVivoxCoreModule*>(&FModuleManager::Get().LoadModuleChecked(TEXT("VivoxCore")))->VoiceClient();
	if (VivoxVoiceClient)
	{
		VivoxVoiceClient->Initialize();
	}
	Login();
}

void UMyOWSvivoxGameInstance::Login()
{
	LoggedInUserId = AccountId(VIVOX_VOICE_ISSUER, FString::FromInt(UKismetMathLibrary::RandomIntegerInRange(0, 1000)), VIVOX_VOICE_DOMAIN);
	ILoginSession& MyLoginSession(VivoxVoiceClient->GetLoginSession(LoggedInUserId));
	bool IsLoggedIn = false;
	// Setup the delegate to execute when login completes

	FTimespan TokenExpiration = FTimespan::FromSeconds(90);
	FString LoginToken = MyLoginSession.GetLoginToken(VIVOX_VOICE_KEY, TokenExpiration);

	ILoginSession::FOnBeginLoginCompletedDelegate OnBeginLoginCompleted;
	OnBeginLoginCompleted.BindLambda([this, &IsLoggedIn](VivoxCoreError Error)
	{
		if (VxErrorSuccess == Error)
		{
			IsLoggedIn = true;
		}
		UE_LOG(LogTemp, Log, TEXT("Logged into Vivox! :D"));
		JoinChannel();
	});

	MyLoginSession.BeginLogin(VIVOX_VOICE_SERVER, LoginToken, OnBeginLoginCompleted);
}

void UMyOWSvivoxGameInstance::JoinChannel()
{
	ILoginSession& MyLoginSession = VivoxVoiceClient->GetLoginSession(LoggedInUserId);
	ChannelId Channel = ChannelId(VIVOX_VOICE_ISSUER, "ChannelId", VIVOX_VOICE_DOMAIN, ChannelType::NonPositional);
	IChannelSession& ChannelSession = MyLoginSession.GetChannelSession(Channel);

	FTimespan TokenExpiration = FTimespan::FromSeconds(90);
	FString JoinToken = ChannelSession.GetConnectToken(VIVOX_VOICE_KEY, TokenExpiration);

	ChannelSession.BeginConnect(true, false, true, JoinToken, NULL);

}

/*void UMyOWSvivoxGameInstance::OnLoginSessionStateChanged(LoginState State)
{
	if (LoginState::LoggedOut == State)
	{
		UE_LOG(MyLog, Error, TEXT("LoginSession Logged Out Unexpectedly\n"));
		// Optionally handle other cases
	}
}*/
