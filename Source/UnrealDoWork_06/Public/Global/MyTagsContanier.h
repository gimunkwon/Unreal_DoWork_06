#pragma once

#include "NativeGameplayTags.h"

namespace ObjectPoolTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Object_Pooling)

	// 풀링 메커니즘 검증용 - 같은 BP_Monster를 서로 다른 풀(태그)로 분리해서 관리
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Monster_TypeA)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Monster_TypeB)
}

