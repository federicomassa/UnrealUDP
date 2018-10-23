// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UDPSender.h"
#include "AnyCustomData.h"
#include <GameFramework/Actor.h>

AUDPSender::AUDPSender(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SenderSocket = NULL;
	ShowOnScreenDebugMessages = true;
}

bool AUDPSender::SendString(FString ToSend)
{
	if (!SenderSocket)
	{
		ScreenMsg("No sender socket"); 
		return false; 
	} 
	
	int32 BytesSent = 0;

	FAnyCustomData NewData;
	NewData.Scale = FMath::FRandRange(0, 1000);
	NewData.Count = FMath::RandRange(0, 100);
	NewData.Color = FLinearColor(FMath::FRandRange(0, 1), FMath::FRandRange(0, 1), FMath::FRandRange(0, 1), 1);

	FArrayWriter Writer;

	Writer << NewData; //Serializing our custom data, thank you UE4!

	//SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddr);
	float x = 5.67;
	SenderSocket->SendTo(reinterpret_cast<uint8*>(&x), sizeof(x), BytesSent, *RemoteAddr);

	if (BytesSent <= 0) { const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"; UE_LOG(LogTemp, Error, TEXT("%s"), *Str); ScreenMsg(Str); return false; }

	ScreenMsg("UDP~ Send Succcess! Bytes Sent = ", BytesSent);

	return true;
}

bool AUDPSender::Start(const FString & YourChosenSocketName, const FString & TheIP, const int32 ThePort, bool UDP)
{
	//Create Remote Address. 
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid; 
	RemoteAddr->SetIp(*TheIP, bIsValid);
	RemoteAddr->SetPort(ThePort);

	if (!bIsValid) 
	{ 
		ScreenMsg("Rama UDP Sender>> IP address was not valid!", TheIP); 
		return false;
	}

	SenderSocket = FUdpSocketBuilder(*YourChosenSocketName).AsReusable().WithBroadcast();

	check(SenderSocket->GetSocketType() == SOCKTYPE_Datagram);

	//Set Send Buffer Size
	int32 SendSize = 2*1024*1024; 
	SenderSocket->SetSendBufferSize(SendSize,SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);

	UE_LOG(LogTemp, Log, TEXT("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	UE_LOG(LogTemp, Log, TEXT("Rama ****UDP**** Sender Initialized Successfully!!!"));
	UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n"));

	return true;
}

void AUDPSender::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SenderSocket) 
	{ 
		SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket); 
	}


}
