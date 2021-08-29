// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Net/UnrealNetwork.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebAuth, Log, All);

#define LOG(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define LOG_EXTENDED(text, color, time_to_show) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, time_to_show, color,text)
#define LogFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))
#define LogFStringWithColor(text, fstring, color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), fstring))

#define DBG


namespace UeWebAuth
{
	namespace HUD
	{
		const TMap<FString, FString> LOC_STRINGS =
		{
			{ TEXT("SOME_STRING"), TEXT("SOME STRING") }
		};

		namespace Menu
		{
			const TArray<FString> QUALITY_SETINGS = {
				{ "Low"		},
				{ "Medium"	},
				{ "High"	},
				{ "Epic"	},
				{ "Ultra"	}
			};

			const TArray<FString> VIEW_DISTANCE = {
				{ "Near"	},
				{ "Medium"	},
				{ "Far"		},
				{ "Very Far" }
			};

			const TArray<FString> TEXTURE_FILTERING = {
				{ "NONE" },
				{ "X2"	 },
				{ "X4"	 },
				{ "X8"   },
				{ "X16"  }
			};

			const TArray<FString> ANTIALIASING = {//???
				{ "NONE" },
				{ "X2"	 },
				{ "X4"	 },
				{ "X8"   },
				{ "X16"  }
			};
		}
	} // End Of HUD

	namespace Net
	{
#include "Containers/UnrealString.h"

		const FString  URL_AUTHSRV_LOGIN = "http://127.0.0.1:7979/auth?";
		const FString  URL_BUY			 = "http://127.0.0.1:7979/buy?";
		const FString  URL_ECHO			 = "http://127.0.0.1:7979/echo";
		const FString  URL_FAVICO		 = "http://127.0.0.1:7979/favicon.ico";
		const FString  URL_PSAVE		 = "http://127.0.0.1:7979/psave?";
		const FString  URL_PLOAD		 = "http://127.0.0.1:7979/pload?";
		const FString  URL_PRICELIST	 = "http://127.0.0.1:7979/pricelist?";
		const FString  URL_REGUSR		 = "http://127.0.0.1:7979/regusr?";
		const FString  URL_RESETPWD		 = "http://127.0.0.1:7979/resetpwd?";//http://localhost:7979/echo?reinit
	} // End Of Net
} // End Of UeWebAuth