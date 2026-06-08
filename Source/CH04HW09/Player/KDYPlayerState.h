
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KDYPlayerState.generated.h"

UCLASS()
class CH04HW09_API AKDYPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AKDYPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
