#pragma once

#define CLIENTCOMPLAINT(x) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x)));
#define CLIENTCOMPLAINT2(x,y) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x), y));

#define SERVERCOMPLAINT(x) if (Role == ROLE_Authority) UE_LOG(LogTemp, Warning, TEXT(x));
#define SERVERCOMPLAINT2(x,y) if (Role == ROLE_Authority) UE_LOG(LogTemp, Warning, TEXT(x), y);