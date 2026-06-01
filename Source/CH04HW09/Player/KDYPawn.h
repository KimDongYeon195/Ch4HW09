
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KDYPawn.generated.h"

UCLASS()
class CH04HW09_API AKDYPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NerController) override;


};
