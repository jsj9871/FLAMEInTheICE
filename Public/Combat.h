// Copyright Epic Games, Inc. All Rights Reserved.
	
#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8	
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(Combat, Log, All);

#define CLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define CLOG_S(Verbosity) UE_LOG(Combat, Verbosity, TEXT("%s"), *CLOG_CALLINFO)
#define CLOG(Verbosity, Format, ...) UE_LOG(Combat, Verbosity, TEXT("%s %s"), *CLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define PCHECK(Expr, ...) { if (!(Expr)) { CLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;} }