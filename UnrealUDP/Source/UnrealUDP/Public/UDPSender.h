// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Networking.h"
#include "GameFramework/EngineMessage.h"
#include "UDPSender.generated.h"

class AActor;

UCLASS()
class AUDPSender : public AActor
{
	GENERATED_BODY()
	
public:
	AUDPSender(const FObjectInitializer& ObjectInitializer);

private:
	bool isUDP;
	

public:
	TSharedPtr<FInternetAddr> RemoteAddr; 
	FSocket* SenderSocket;

	UFUNCTION(BlueprintCallable, Category=UDPSender)
		bool Start(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP = false);

	UFUNCTION(BlueprintCallable, Category = UDPSender)
		bool SendString(FString ToSend);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UDPSender) 
		bool ShowOnScreenDebugMessages;

	FORCEINLINE void ScreenMsg(const FString& Msg) { 
		if(!ShowOnScreenDebugMessages) return; 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg); 
	} 
	
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value) 
	{ 
		if(!ShowOnScreenDebugMessages) return; 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value)); 
	} 
	
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2) 
	{ 
		if(!ShowOnScreenDebugMessages) return; 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2)); 
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};