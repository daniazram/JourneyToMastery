#pragma once

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary,
	Secondary                   
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	LightWeapon,
	HeavyWeapon             
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right,
	None
};

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	IA_Sprint,
	IA_Dash,
	IA_Attack_Light,
	IA_Attack_Heavy,
};