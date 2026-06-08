#include "UnrealDoWork_06/Public/PooledObject/PoolingMonster.h"



APoolingMonster::APoolingMonster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APoolingMonster::BeginPlay()
{
	Super::BeginPlay();
}

void APoolingMonster::OnAcquiredFromPool()
{
	UE_LOG(LogTemp, Log, TEXT("[Pool] %s Acquired (Tag: %s)"), *GetName(), *PoolTag.ToString());
}

void APoolingMonster::OnReturnedToPool()
{
	UE_LOG(LogTemp, Log, TEXT("[Pool] %s Returned (Tag: %s)"), *GetName(), *PoolTag.ToString());
}




