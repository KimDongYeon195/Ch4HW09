

#include "Player/KDYPawn.h"
#include "CH04HW09.h"


void AKDYPawn::BeginPlay()
{
	Super::BeginPlay();
	

	FString NerRoleString = HW09FunctionLibrary::GetRoleString(this);
	FString ConbinedString = FString::Printf(TEXT("KDYPawn::BeginPlay() %s [%s]"), *HW09FunctionLibrary::GetNetModeString(this), *NerRoleString);
	HW09FunctionLibrary::MyPrintString(this, ConbinedString, 10.f);
}

void AKDYPawn::PossessedBy(AController* NetController)
{
	Super::PossessedBy(NetController);

	FString NetRoleString = HW09FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("KDYPawn::PossessedBy() %s [%s]"), *HW09FunctionLibrary::GetNetModeString(this), *NetRoleString);
	HW09FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


