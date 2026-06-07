#include "UnrealDoWork_06/Public/PooledObject/PoolingMonster.h"



APoolingMonster::APoolingMonster()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolingMonster::BeginPlay()
{
	Super::BeginPlay();
}

void APoolingMonster::OnAcquiredFromPool()
{
	
}

void APoolingMonster::OnReturnedToPool()
{
	
}




