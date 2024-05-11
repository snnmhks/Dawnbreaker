#pragma once

#include "GameplayTagContainer.h"

#define CDTAG_Character_Test FGameplayTag::RequestGameplayTag(FName("Character.Test"))
#define CDTAG_Character_Tests FGameplayTag::RequestGameplayTag(FName("Character.Tests"))
#define CDTAG_Character_Weapon_Pistol FGameplayTag::RequestGameplayTag(FName("Character.Weapon.Pistol"))
#define CDTAG_Character_State_Dead FGameplayTag::RequestGameplayTag(FName("Character.State.Dead"))
#define CDTAG_Character_Debuf_Stun FGameplayTag::RequestGameplayTag(FName("Character.Debuf.Stun"))
#define CDTAG_Character_State_Pistol FGameplayTag::RequestGameplayTag(FName("Character.State.IsHavePistol")