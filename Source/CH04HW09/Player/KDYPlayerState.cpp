

#include "Player/KDYPlayerState.h"
#include "Net/UnrealNetwork.h"

AKDYPlayerState::AKDYPlayerState(): 
	PlayerNameString(TEXT("None"))
	, CurrentGuessCount(1)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void AKDYPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount); //심화 : 시간초과시 Count 차감
	DOREPLIFETIME(ThisClass, MaxGuessCount); 
}

FString AKDYPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
