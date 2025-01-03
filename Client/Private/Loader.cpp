
#include "..\Public\Loader.h"

#include "GameInstance.h"

#include "BackGround.h"
#include "LogoBackGround.h"


#include "UI_Include.h"

#include "Monster.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Camera_FPS.h"
#include "Camera_TPS.h"
#include "Player.h"
#include "Revolver.h"
#include "ImGui_Manager.h"
#include "Model_Tester.h"
#include "Portal.h"
#include "Boss_Portal.h"
#include "Weapon_Revolver.h"
#include "Weapon_Pistol.h"
#include "Weapon_Gatling.h"
#include "Weapon_RocketLauncher.h"
#include "Monster_Fly.h"
#include "Monster_Spider.h"
#include "Monster_Bat.h"
#include "Monster_Worm.h"
#include "Monster_Draugr.h"
#include "Monster_BigFly.h"
#include "Monster_ClockWork.h"
#include "Monster_Golem.h"
#include "Monster_Luminous.h"
#include "Monster_ScorpionQueen.h"
#include "Monster_Nidhog.h"
#include "Monster_Alvis.h"
#include "Monster_Surt.h"
#include "Monster_Gideon.h"
#include "Monster_Reaper.h"
#include "Monster_DarkKnight.h"
#include "Monster_Demon.h"
#include "Monster_DemonBrute.h"
#include "Monster_Gullveig.h"
#include "Monster_Mistcalf.h"

#include "Statue.h"
#include "RoomManager.h"
#include "Chest.h"
#include "Item_Consume.h"
#include "Item_Equip.h"
#include "ShopKeeper.h"
#include "Effect_MuzzleFlash.h"
#include "Effect_Projectile.h"
#include "Effect_Wall.h"
#include "Effect_Charge.h"
#include "Effect_Sweep.h"
#include "Effect_Hex_Shield.h"
#include "Effect_Teardrop.h"
#include "Effect_Spike.h"
#include "Effect_Fragment.h"
#include "Effect_RuneField.h"
#include "Effect_Space.h"
#include "Effect_SummonRune.h"
#include "Effect_Pheonix.h"
#include "Effect_RuneSpear.h"
#include "Effect_Scorch.h"
#include "Effect_DraugrRune.h"
#include "Effect_StatueFragment.h"
#include "Effect_NidhoggField.h"
#include "Effect_FireBall.h"
#include "Effect_Explosion.h"
#include "Effect_ThreeRune.h"
#include "Effect_DiscImpact.h"
#include "Effect_LightningArc.h"
#include "Effect_Emission.h"
#include "Effect_AlvisImpact.h"
#include "Effect_AlvisRock.h"
#include "Effect_ThunderClap.h"
#include "Effect_SurtLaser_Warning.h"
#include "Effect_SurtLaser.h"
#include "Effect_SurtRune.h"
#include "Effect_SurtRock.h"
#include "Effect_SurtRockFrag.h"
#include "Effect_VortexFire.h"
#include "Effect_GideonField.h"
#include "Effect_GideonSpear.h"
#include "Effect_GideonPortal.h"
#include "Effect_GideonBeam.h"
#include "Effect_FlowerBurst.h"
#include "Effect_GideonRune.h"
#include "Effect_GideonBlackHole.h"
#include "Effect_FireWall.h"
#include "Effect_Ally.h"
#include "Effect_AllyProjectile.h"
#include "Effect_PlayerDash.h"
#include "Effect_GideonSlowProjectile.h"

#include "Effect_Sharp_Projectile.h"
#include "Effect_Gullveig_Field.h"
#include "Effect_Gullveig_LightWall.h"
#include "Effect_ShockWave.h"
#include "Effect_Sonar.h"
#include "Effect_SharpSweep.h"
#include "Effect_BlockLight.h"
#include "Effect_Equipping.h"

#include "Effect_DashAura.h"
#include "Effect_DashIndicator.h"
#include "Effect_ShockImpact.h"

#include "Particle_Rect.h"
#include "Particle_Point.h"
//#include "Monster.h"
//#include "Player.h"
//#include "Camera.h"
//#include "Effect.h"
//#include "Sky.h"
#include "Trail.h"
#include "AfterImage.h"
#include "Wing.h"
#include "MagmaField.h"
#include "SkeletonAlter.h"
#include "SkeletonKey.h"

#include "Camera_Logo.h"
#include "PlayerSelectModel.h"

#include "TPS_Player.h"

void Load_Items(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
#pragma region Consume
	CItem_Consume::CONSUME_DESC ConsumeDesc;
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Heal25",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Heal_25.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_HEAL_25;
	ConsumeDesc.strName = L"25 HP";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Heal25";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal25"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal25_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Heal50",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Heal_50.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_HEAL_50;
	ConsumeDesc.strName = L"50 HP";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Heal50";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal50"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal50_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Converter",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Converter.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_CONVERTER;
	ConsumeDesc.strName = L"Converter";
	ConsumeDesc.strDesc = L"Convert Shield To HP";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Converter";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Converter"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	ConsumeDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Converter_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine

	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Key",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Key.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_KEY;
	ConsumeDesc.strName = L"Key";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Key";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Key"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Key_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine


	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Shield25",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Shield_25.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_SHIELD;
	ConsumeDesc.strName = L"25 Shield";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Shield25";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Shield25"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Shield25_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine

	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Coin",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Coin.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_END;
	ConsumeDesc.strName = L"";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Coin";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Coin"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
	//Routine
#pragma endregion
#pragma region EQUIP
	CItem_Equip::EQUIP_DESC EquipDesc;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Svalinn",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Svalinn.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_TALLSHIELD;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Generate Shield If You Stand Still";
	EquipDesc.strName = L"Svalinn";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Svalinn";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Svalinn"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Svalinn_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Sunspot",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Sunspot.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SUN_CHEST;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Release Fire On Shot";
	EquipDesc.strName = L"SunSpot";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Sunspot";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Sunspot"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Sunspot_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Masquerade",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Masquerade.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_BLANK;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Cast Ult";
	EquipDesc.strName = L"Masquerade";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Masquerade";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Masquerade"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Masquerade_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Gungnir",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Gungnir.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_RANGE;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Increase Range";
	EquipDesc.strName = L"Gungnir";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Gungnir";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Gungnir"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Gungnir_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Death",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Death.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SKULL_CHEST;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Doubled Damage";
	EquipDesc.strName = L"Death Mask";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Death";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Death"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Death_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_BestDefence",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_BestDefence.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_INFINITE;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Infinite Ammo";
	EquipDesc.strName = L"The Best Defence";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_BestDefence";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_BestDefence"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_BestDefence_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Rage",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Rage.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_RAGE_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Multiply Damage With Your Coolness";
	EquipDesc.strName = L"Helm of Rage";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Rage";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Rage"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Rage_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Ogre",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Ogre.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAD_ORC;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"For the top";
	EquipDesc.strName = L"Helm of Ogre";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Ogre";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Ogre"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Ogre_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Horned",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Horned.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SHOTS_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Your Bullet Will Explode";
	EquipDesc.strName = L"Horned Helm";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Horned";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Horned"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Horned_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Fluffy",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Fluffy.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAD_FLUFFY;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Reload And Shoot";
	EquipDesc.strName = L"Fluffy";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Fluffy";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fluffy"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fluffy_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Fedora",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Fedora.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SWAP_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Rotate Slots";
	EquipDesc.strName = L"Fedora";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Fedora";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fedora"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fedora_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Tumbler",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Tumbler.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_DOC_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Shot to Evade";
	EquipDesc.strName = L"Tumbler Boots";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Tumbler";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Tumbler"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Tumbler_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Rock",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Rock.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAVY_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Stomp Your Enemy";
	EquipDesc.strName = L"Rock Boots";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Rock";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Rock"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Rock_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Hermod",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Hermod.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_FOOT_SANDALS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Do a barrel roll";
	EquipDesc.strName = L"Grave of Hermod";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Hermod";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Hermod"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Hermod_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_GiantSlayer",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_GiantSlayer.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_KINGHT_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Damage Per Max Hp";
	EquipDesc.strName = L"Giant Slayer";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_GiantSlayer";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_GiantSlayer"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_GiantSlayer_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Ring_SixShoot",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Ring_SixShoot.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_GLOVE_SIXSHOOTER;
	EquipDesc.eSlotID = SLOT_ARM;
	EquipDesc.strDesc = L"Double Beat";
	EquipDesc.strName = L"Six Shooter";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Ring_SixShoot";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Ring_SixShoot"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Ring_SixShoot_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		throw;

#pragma endregion

#pragma region CHEST

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Mythic",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_06.txt", XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Epic",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_08.txt", XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Rare",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_18.txt", XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Uncommon",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_19.txt",  XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Common",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_13.txt",  XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
#pragma endregion
}
void Load_Gameplay_Extra(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DashIndicator",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DashIndicator/DashIndicator.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_MagmaField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/MagmaField/MagmaField.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonKey",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/SkeletonKey/SkeletonKey.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonAlter",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/SkelKeyAlter/SkelKeyAlter.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	//
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Null",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Door/Asgard_Door.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Ally",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Ally/Ally.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AllyProjectile",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Ally/AllyProjectile.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Revolver",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/revolver/revolver.txt",
			//XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.0f, XM_PI, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, -XM_PI * 0.0866f, 0.f), XMVectorSet(0.f, -0.03f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Pistol",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pistol/BA_pistol.txt",
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Gatling",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/gatling/gatling.txt",
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RocketLauncher",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Rocket_launcher/rocket_launcher.txt",
			//XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.0f, XM_PI, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;
	if(s_ePC == PC_FREY)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player_Frey/FreyHand.txt"))))
		{
			throw;
		}
	}
	else if (s_ePC == PC_HILDR)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player_Hildr/HildrHand.txt"))))
		{
			throw;
		}
	}
	else if(s_ePC == PC_GOLL)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player/FP_player.txt"))))
		{
			throw;
		}
	}
	else
	{
		//TPS_Player
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Gideon",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Gideon/Gideon.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
			throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Asgard_Door",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Door/Asgard_Door.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShopKeeper",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/NPC/ShopKeeper.txt", XMMatrixRotationRollPitchYaw(XM_PI, 0.f, XM_PIDIV2) * XMMatrixScaling(0.1f, 0.1f, 0.1f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SplineTail",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SplineTail/SplineTail.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PIDIV2 + XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Sweep",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Sweep/Sweep.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Hex_Shield",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Hex_Shield/Hex_Shield.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Teardrop",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Teardrop/Teardrop.txt", XMMatrixRotationRollPitchYaw(XM_PIDIV2, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Spike",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Spike/Spike.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.05f, 0.05f, 0.05f)))))
	{
		MSG_BOX(L"USUCK");
	}
	//TODO FRAGMENT
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Fragment",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Fragment/Fragment.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RuneField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/RuneField/RuneField.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Aura",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Aura/Aura.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Space",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Space/Space.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Pheonix",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Pheonix/Pheonix.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RuneSpear",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/RuneSpear/Spear.txt", XMMatrixRotationRollPitchYaw(XM_PIDIV2, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag0",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag0.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag1.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag2.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag3.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Boss_End",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Boss_End/Boss_End.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.015f, 0.015f, 0.015f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_FireBall",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/FireBall/FireBall.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Explosion",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Explosion/Explosion.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DiscImpact",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DiscImpact/DiscImpact.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_NidhoggField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/NidhoggField/NidhoggField.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_FireWall",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/FireWall/FireWall.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShockWave",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/ShockWave/ShockWave.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SharpSweep",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SharpSweep/SharpSweep.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	//
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DashAura",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DashAura/DashAura.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShockImpact",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/ShockImpact/ShockImpact.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
}

void Load_Trails(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_Trail_Ice_01.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_FireTrail.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_VenusTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_Venus_Trail.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpeedTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_ZinxSpeedTrails_alphafade1.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EnergyTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_EnergyPlasma_Tile_H_01.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokyTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_Smoke_Trail.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ElectricTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Trail/T_ElectricalBolt_3.png"), 1))))
		throw;
	//
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Explosion/T_FireBall_07C_ResdiualSmokey_%d.png"), 64))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PlayerDash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_shinningfloor_teal.png"), 1))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PerlinNoise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_PerlinPlasma_Noisey_Packed.png"), 1))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EngineFlameMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_EngineFlameMask.png"), 1))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ramp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_NoisyRamp.png"), 1))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterFlow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_WaterFlow_Tiling01.png"), 1))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LinearUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_LinearUVs.png"), 1))))
		throw;
	//
	//
	//
	//
	CAfterImage::AFTERIMAGE_DESC aiDesc;
	aiDesc.bAnimated = true;
	aiDesc.fLifeTime = 1.f;
	aiDesc.iMaxImage = 10;
	aiDesc.iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::MonoColor;
	aiDesc.vColor = { 0.2f,1.0f,0.8f, 1.f};
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AfterImage_Anim_1sec_10"),
		CAfterImage::Create(m_pDevice, m_pContext, aiDesc))))
		throw;
	aiDesc.fLifeTime = 0.03f;
	aiDesc.vColor = { 1.0f,0.7f,0.3f, 1.f };
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AfterImage_Anim_0.03sec_10"),
		CAfterImage::Create(m_pDevice, m_pContext, aiDesc))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_PlayerDash"),
		CEffect_PlayerDash::Create(m_pDevice, m_pContext))))
		throw;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_PlayerDash", nullptr);
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_PlayerDash", nullptr);

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Sonar"),
		CEffect_Sonar::Create(m_pDevice, m_pContext))))
		throw;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_Sonar", nullptr);
	//

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Equipping"),
		CEffect_Equipping::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 5; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_Equipping", nullptr);
	}

	//Prototype_GameObject_Effect_SharpSweep
}

void Load_Gameplay_Monster1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext) 
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Fly",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Fly/Fly.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Fly"),
		CMonster_Fly::Create(m_pDevice, m_pContext))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Spider",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Spider/Spider.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Spider"),
		CMonster_Spider::Create(m_pDevice, m_pContext))))
		throw;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Worm",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ClawWorm/Claw_Worm.txt", XMMatrixRotationY(XM_PIDIV2 * 3.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Worm"),
		CMonster_Worm::Create(m_pDevice, m_pContext))))
		throw;
}
void Load_Gameplay_Monster2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Bat",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Bat/Bat.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Bat"),
		CMonster_Bat::Create(m_pDevice, m_pContext))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Draugr",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Draugr/Draugr.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Draugr"),
		CMonster_Draugr::Create(m_pDevice, m_pContext))))
		throw;

}
void Load_Gameplay_Room()
{
	CRoomManager::Get_Instance()->Ready_Rooms_Asgard();
}

void Load_Helhime_Monster1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_BigFly",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/BigFly/BigFly.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ClockWork",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ClockWork/ClockWork.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	{
		MSG_BOX(L"USUCK");
	}

}
void Load_Helhime_Monster2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Luminous",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Luminous/Luminous.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ScorpionQueen",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ScorpionQueen/ScorpionQueen.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

}
void Load_Helhime_Monster3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Golem",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Golem/Golem.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.04f, 0.04f, 0.04f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Nidhog",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Nidhog/Nidhog.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
}
void Load_Hel_Room()
{
	CRoomManager::Get_Instance()->Ready_Rooms_Hel();
}

void Load_BossRush_EXT(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DashIndicator",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DashIndicator/DashIndicator.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Ally",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Ally/Ally.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AllyProjectile",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Ally/AllyProjectile.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	//
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Null",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Door/Asgard_Door.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Coin",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Coin.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (s_ePC == PC_FREY)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player_Frey/FreyHand.txt"))))
		{
			throw;
		}
	}
	else if (s_ePC == PC_HILDR)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player_Hildr/HildrHand.txt"))))
		{
			throw;
		}
	}
	else if(s_ePC == PC_GOLL)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Player",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Player/FP_player.txt"))))
		{
			throw;
		}
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Asgard_Door",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Door/Asgard_Door.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShopKeeper",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/NPC/ShopKeeper.txt", XMMatrixRotationRollPitchYaw(XM_PI, 0.f, XM_PIDIV2) * XMMatrixScaling(0.1f, 0.1f, 0.1f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SplineTail",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SplineTail/SplineTail.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PIDIV2 + XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Sweep",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Sweep/Sweep.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Hex_Shield",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Hex_Shield/Hex_Shield.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Teardrop",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Teardrop/Teardrop.txt", XMMatrixRotationRollPitchYaw(XM_PIDIV2, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Spike",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Spike/Spike.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.05f, 0.05f, 0.05f)))))
	{
		MSG_BOX(L"USUCK");
	}
	//TODO FRAGMENT
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Fragment",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Fragment/Fragment.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RuneField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/RuneField/RuneField.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Aura",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Aura/Aura.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, XM_PIDIV2) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Space",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Space/Space.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Pheonix",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Pheonix/Pheonix.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RuneSpear",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/RuneSpear/Spear.txt", XMMatrixRotationRollPitchYaw(XM_PIDIV2, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag0",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag0.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag1.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag2.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Statue_Frag3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Statue/Statue_Frag3.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Boss_End",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Boss_End/Boss_End.txt", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.015f, 0.015f, 0.015f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_FireBall",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/FireBall/FireBall.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Explosion",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Explosion/Explosion.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DiscImpact",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DiscImpact/DiscImpact.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_NidhoggField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/NidhoggField/NidhoggField.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_FireWall",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/FireWall/FireWall.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_DashAura",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/DashAura/DashAura.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShockImpact",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/ShockImpact/ShockImpact.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	CItem_Consume::CONSUME_DESC ConsumeDesc;
	ConsumeDesc.eID = ITEM_END;
	ConsumeDesc.strName = L"";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Coin";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Coin"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		throw;
}
void Load_BossRush_Boss_1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{

	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock_Frag0",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/RockFragment0.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock_Frag1",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/RockFragment1.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock_Frag2",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/RockFragment2.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock_Frag3",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/RockFragment3.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock_Frag4",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/RockFragment4.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_AlvisRock",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/Rock.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Shard_Upper",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/AlvisRock/Shard_Upper.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
		{
			MSG_BOX(L"USUCK");
		}
		
	}

	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SurtRock_Frag0",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SurtRock/Surt_Rock_Frag_0.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SurtRock_Frag1",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SurtRock/Surt_Rock_Frag_1.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SurtRock_Frag2",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SurtRock/Surt_Rock_Frag_2.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SurtRock",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/SurtRock/Surt_Rock.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
	}

	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_VortexFire",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/VortexFire/VortexFire.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_FlowerBurst",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/VortexFire/FlowerBurst.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonSpear",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonSpear/RuneSpear.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonField",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonField/GideonField.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonPortal",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonPortal/Portal.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		//
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam_Frag0",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonBeam/GideonBeam_Frag0.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam_Frag1",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonBeam/GideonBeam_Frag1.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam_Frag2",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonBeam/GideonBeam_Frag2.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam_Frag3",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonBeam/GideonBeam_Frag3.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBeam",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/GideonBeam/GideonBeam.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_GideonBlackHole",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/BlackHole/BlackHole.dat", XMMatrixRotationRollPitchYaw(0.f, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		//
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Gideon",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Gideon/Gideon.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		throw;
}
void Load_BossRush_Boss_2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Alvis",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Rampage/Rampage.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		throw;

}
void Load_BossRush_Boss_3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Surt",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Surt/Surt.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Pistol",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pistol/BA_pistol.txt",
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;
}
void Load_BossRush_Boss_4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Draugr",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Draugr/Draugr.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}
void Load_Boss_Room()
{
	CRoomManager::Get_Instance()->Ready_Rooms_BossRush();
}

void Load_Vana_Monster1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Reaper",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Reaper/Reaper.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_DemonBrute",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/DemonBrute/DemonBrute.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Sharp_Projectile",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Sharp_Projectile/Sharp_Projectile.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Gullveig_Field",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Gullveig_Field/Gullveig_Field.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Gullveig_LightWall",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Gullveig_LightWall/Gullveig_LightWall.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)));
	//
	//
}
void Load_Vana_Monster2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Demon",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Demon/Demon.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_DarkKnight",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/DarkKnight/DarkKnight.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
}
void Load_Vana_Monster3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Gullveig",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Gullveig/Gullveig.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Wing",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Wing/Wing.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)));

}
void Load_Vana_Room()
{
	CRoomManager::Get_Instance()->Ready_Rooms_Vana();
}

void Load_Svart_Monster(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Monster_Mistcalf",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Mistcalf/Riktor.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
}

void Load_Svart_Room()
{
	CRoomManager::Get_Instance()->Ready_Rooms_Svart();
}
void Load_Room_Tool(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext, CImGui_Manager* pImguiManager)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterFlow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/T_WaterFlow_Tiling01.png"), 1))))
		throw;
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Asgard_I_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	pImguiManager->m_MapModels.push_back("Prototype_Component_Model_Asgard_I_1");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Asgard_Door",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Door/Asgard_Door.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

// typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
unsigned int APIENTRY Loading_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;	

	return pLoader->Loading();	
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

unsigned int CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT		hr{};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL_TOOL:
		hr = Loading_For_Tool_Level();
		break;
	case LEVEL_HEL:
		hr = Loading_For_Hel_Level();
		break;
	case LEVEL_VANA:
		hr = Loading_For_Vana_Level();
		break;
	case LEVEL_SVART:
		hr = Loading_For_Svart_Level();
		break;
	case LEVEL_BOSSRUSH:
		hr = Loading_For_BossRush_Level();
		break;
	case LEVEL_ENDING:
		hr = Loading_For_Ending_Level();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;
}
void Load_Logo1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Model_PSM_Serath",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/PlayerSelect/Serath.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}
void Load_Logo2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Model_PSM_Frey",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/PlayerSelect/Frey.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}
void Load_Logo3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Model_PSM_Hildr",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/PlayerSelect/Hildr.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}
void Load_Logo4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Model_PSM_Run",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/PlayerSelect/Run.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
}
HRESULT CLoader::Loading_For_Logo_Level()
{	
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), 
		CTexture::Create(m_pDevice,m_pContext, TEXT("../Bin/Resource/Texture/UI/Loading/Loading%d.png"), 4));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Font_Numeric"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Font/Numeric/MotorblockFont_PageA_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Font_Literal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Font/Literal/MotorblockFont_PageA_L%d.png"), 28))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RoomLogo",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/MapLogo.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	//oad_Svart_Monster(m_pGameInstance, m_pDevice, m_pContext);
	thread t1(Load_Logo1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Logo2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Logo3, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Logo4, m_pGameInstance, m_pDevice, m_pContext);



	
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Font"),
		CUI_Font::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_TextBox"),
		CUI_TextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* for.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoBackGround"),
		CLogoBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Model_Tester", CModel_Tester::Create(m_pDevice, m_pContext));
	CModel_Tester::MODEL_TESTER_DESC mtDesc;
	mtDesc.strModelTag = L"Prototype_Component_Model_RoomLogo";
	mtDesc.Transform = Identity;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Model_Tester", &mtDesc);
	//Camera_Logo
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Camera_Logo", CCamera_Logo::Create(m_pDevice, m_pContext));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	CPlayerSelectModel::PSM psmDesc;
	psmDesc.strModelTag = L"Prototype_Component_Model_PSM_Serath";
	psmDesc.strIdleTag = "SerathGold.ao|ArmInLessBustyNeck";
	psmDesc.strSelectedTag = "SerathGold.ao|ArmInLessBustyNeck";
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerSelectModel_Serath", CPlayerSelectModel::Create(m_pDevice, m_pContext));
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_PlayerSelectModel_Serath", &psmDesc);

	psmDesc.strModelTag = L"Prototype_Component_Model_PSM_Frey";
	psmDesc.strIdleTag = "Greystone_Novaborn.ao|Idle";
	psmDesc.strSelectedTag = "Greystone_Novaborn.ao|Emote_Taunt_BringItOn_T1";
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerSelectModel_Frey", CPlayerSelectModel::Create(m_pDevice, m_pContext));
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_PlayerSelectModel_Frey", &psmDesc);

	psmDesc.strModelTag = L"Prototype_Component_Model_PSM_Hildr";
	psmDesc.strIdleTag = "Aurora_FrozenHearth.ao|Idle";
	psmDesc.strSelectedTag = "Aurora_FrozenHearth.ao|ShaderNodetreeAction";
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerSelectModel_Hildr", CPlayerSelectModel::Create(m_pDevice, m_pContext));
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_PlayerSelectModel_Hildr", &psmDesc);

	psmDesc.strModelTag = L"Prototype_Component_Model_PSM_Run";
	psmDesc.strIdleTag = "Murdock_Shogun_GDC.ao|Idle_A_Loop";
	psmDesc.strSelectedTag = "Murdock_Shogun_GDC.ao|Emote_Taunt_WatchingYou_T1";
	m_pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerSelectModel_Run", CPlayerSelectModel::Create(m_pDevice, m_pContext));
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_PlayerSelectModel_Run", &psmDesc);

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	{
		Load_Trails(m_pGameInstance, m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CrosshairPoint"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshairPoint%d.png"), 2))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Crosshair"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshair%d.png"), 4))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BeatMarker"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshairEdge%d.png"), 5))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/HpBar/Segment%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Items"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Item/item%d.png"), ITEM_END))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Weapons"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Weapons/Weapon%d.png"), WEAPON_END))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Amo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Amo/Bullet%d.png"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ability"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Abilities/Ablility%d.png"), ABILITY_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Level_Title"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Level_Title/Level_Title%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Title"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Boss_Title/Boss_%d.png"), 6))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Coin_Key"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Coin_Key/HUD%d.png"), 3))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridRoom%d.png"), ROOM_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room_Edge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridEdge/GridRoom%d.png"), RP_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MinimapPlayer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridRoomPlayer.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SwapItem"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Item/SwapItems.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/BossHp/BossBar%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MuzzleFlash"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Muzzle/MuzzleBC%d.png"), 2))))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MuzzleMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Muzzle/T_Boris_Impact_01.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Orb"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/images/T_Rift_Orb_flibbook_%d.png"), 64))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cone_Soft_Normal"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/T_Cone_Soft_N.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sphere_Ambient"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/T_Sphere_A.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeTrail"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_Silky_Smoke_Trail.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ThrillWall"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/ThrillWall/T_Fire_Lava_2.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChargeMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Charge/images/T_Magic_Smoke_0%d.png"), 4))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Charge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Charge/T_Plasma_Zinx_Seamless.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SummonRune"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/SummonRune/T_Star_Runes_Undertow.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Corona"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/SummonRune/T_Aurora_Corona_D.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowStar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_GlowStar_12pt.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/SnowFlake/T_Snowflakes_D_00.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Plasma"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/Plasma/T_Plasma_Wisps_%d.png"), 65))))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_lightning_burst_pack.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeParticle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_smokeSingle_01.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake_Green"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Snowflakes_Green.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake_Red"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Snowflakes_Red.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Scorch"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Scorch.png"), 1))))
			return E_FAIL; 
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ThreeRune"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/ThreeRune/Rune%d.png"), 3))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room_Clear"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Room_Clear.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LightnigArc"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/LightningArc/images/T_Lightning_Arc_4x1_0%d.png"), 9))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RadialGreen"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Radial_Green.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emission_Halo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_FX_Halo_MD_01.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emission_Corona"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_Aurora_Corona_D.png"), 1))))
			return E_FAIL;
	}
	
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	thread t1(Load_Gameplay_Extra, m_pGameInstance, m_pDevice, m_pContext);
#pragma region ROOM
	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_1",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_1.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_2",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_2.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_3",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_3.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));


	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_1",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_1.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_2",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_2.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_3",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_3.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));


	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_1",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_1.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_2",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_2.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_3",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_3.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	//
	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_4",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_4.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_1_a",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_1_a.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_1_b",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_1_b.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_2",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_2.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_3",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_3.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_4",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_4.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Armory",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Armory.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Boss",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Boss.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	//
	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Shop",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Shop.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Start_1",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Start_1.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Treasure",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Treasure.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
#pragma endregion
#pragma region Monster
	thread t2(Load_Gameplay_Monster1, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Gameplay_Monster2, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Gameplay_Room);
	thread t5(Load_Items, m_pGameInstance, m_pDevice, m_pContext);
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));
#pragma region UI
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CrossHair"),
		CUI_CrossHair::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CrossHairPoint"),
		CUI_CrossHairPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BeatMarker"),
		CUI_BeatMarker::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpBar"),
		CUI_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Hp"),
		CUI_Hp::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Equipment"),
		CUI_Equipment::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_EquipSlot"),
		CUI_EquipSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSlot"),
		CUI_WeaponSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Amo"),
		CUI_Amo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Ability"),
		CUI_Ability::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AbilitySlot"),
		CUI_AbilitySlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Level_Title"),
		CUI_LevelTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Score"),
		CUI_Score::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MinimapGrid"),
		CUI_MinimapGrid::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MiniMap"),
		CUI_MiniMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SwapItem"),
		CUI_SwapItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_RoomClear"),
		CUI_RoomClear::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossTitle"),
		CUI_BossTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP"),
		CUI_BossHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Rect"),
		CUI_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	CParticle::PARTICLE_DESC PDESC{};
	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::ORBITAL;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 20;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Plasma";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 3.5f, 5.f };
	PDESC.vRange = _float3{ 3.2f,3.2f,3.2f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.1f, 0.3f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Plasma_Orbit"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 30;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_GlowStar";
	//PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.05f, 0.2f };
	PDESC.vRange = _float3{ 1.f,1.f,1.f };
	PDESC.vPivot = _float3{ 0.f,0.f,6.f };
	PDESC.vScale = _float2{ 0.2f, 0.3f };
	PDESC.vSpeed = _float2{ 10.0f, 30.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_GlowStar_Tail"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 15;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.05f, 0.2f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.f,6.f };
	PDESC.vScale = _float2{ 0.1f, 0.2f };
	PDESC.vSpeed = _float2{ 5.0f, 10.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_SnowFlake_Tail"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = false;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::FOUNTAIN;
	PDESC.fMaxLifeTime = 0.5f;
	PDESC.iNumInstance = 30;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Lightning";
	PDESC.vCenter = _float3{ 0.f,0.f,0.5f };
	PDESC.vLifeTime = _float2{ 0.1f, 0.5f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.1f };
	PDESC.vPivot = _float3{ 0.f,0.0f,-0.5f };
	PDESC.vScale = _float2{ 0.2f, 0.25f };
	PDESC.vSpeed = _float2{ 10.0f, 20.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = false;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark"))))
			return E_FAIL;
	}

	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.vRange = _float3{ 0.5f,0.6f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.0f, 0.0f };
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread"))))
			return E_FAIL;
	}

	PDESC.strTextureTag = L"Prototype_Component_Texture_RadialGreen";
	PDESC.vRange = _float3{ 10.f,1.0f,10.f };
	PDESC.iNumInstance = 60;
	PDESC.vSpeed = _float2{ 0.05f, 0.1f };
	PDESC.vLifeTime = _float2{ 1.5f, 2.5f };
	PDESC.fMaxLifeTime = 2.5f;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_RadialGreen_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_RadialGreen_Spread"))))
			return E_FAIL;
	}

	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 20;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SmokeParticle";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.3f, 0.6f };
	PDESC.vRange = _float3{ 1.0f,1.0f,1.0f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.1f, 0.5f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Smoke_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::RISING;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 25;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::DecalToDiffuse;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake_Green";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.5f, 1.6f };
	PDESC.vRange = _float3{ 2.0f,2.0f,2.0f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.05f, 0.1f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_PI, XM_PI, XM_PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Heal_Rising"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake_Red";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Buff_Rising"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	//
	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 1.f;
	PDESC.iNumInstance = 3;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Explosion";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.5f, 1.0f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.2f, 0.8f };
	PDESC.vSpeed = _float2{ 0.5f, 1.f };
	PDESC.vTorque = _float3{ XM_PIDIV2, XM_PIDIV2, XM_PIDIV2 };
	PDESC.bInfinite = false;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Impact"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Impact"))))
			return E_FAIL;
	}
	if(s_ePC != PC_RUN)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TPS_Player"),
			CTPS_Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShopKeeper"),
		CShopKeeper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_MuzzleFlash"),
		CEffect_MuzzleFlash::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_MuzzleFlash"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Projectile"),
		CEffect_Projectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Projectile"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Teardrop"),
		CEffect_Teardrop::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Teardrop"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Wall"),
		CEffect_Wall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Wall"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Charge"),
		CEffect_Charge::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Charge"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Sweep"),
		CEffect_Sweep::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Sweep"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Spike"),
		CEffect_Spike::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Spike"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Fragment"),
		CEffect_Fragment::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 100; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Fragment"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Hex_Shield"),
		CEffect_Hex_Shield::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_RuneField"),
		CEffect_RuneField::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_RuneField"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Aura"),
		CEffect_Aura::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Aura"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Space"),
		CEffect_Space::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Space"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SummonRune"),
		CEffect_SummonRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_SummonRune"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Pheonix"),
		CEffect_Pheonix::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Pheonix"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_RuneSpear"),
		CEffect_RuneSpear::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_RuneSpear"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Scorch"),
		CEffect_Scorch::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Scorch"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DraugrRune"),
		CEffect_DraugrRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 4; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_DraugrRune"))))
			return E_FAIL;
	}
	for(_uint j=0;j<4;++j)
	{
		_wstring strTag = _wstring(L"Prototype_GameObject_Effect_StatueFragment") + to_wstring(j);
		if (FAILED(m_pGameInstance->Add_Prototype(strTag,
			CEffect_StatueFragment::Create(m_pDevice, m_pContext, j))))
			return E_FAIL;
		for (_uint i = 0; i < 3; ++i)
		{
			if (FAILED(m_pGameInstance->Pool_Object(strTag)))
				return E_FAIL;
		}
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_NidhoggField"),
		CEffect_NidhoggField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_FireBall"),
		CEffect_FireBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_FireBall"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Explosion"),
		CEffect_Explosion::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Explosion"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DiscImpact"),
		CEffect_DiscImpact::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_DiscImpact"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ThreeRune"),
		CEffect_ThreeRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_LightningArc"),
		CEffect_LightningArc::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 100; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_LightningArc"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ThunderClap"),
		CEffect_ThunderClap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 32; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_ThunderClap"))))
			return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_FireWall"),
		CEffect_FireWall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_FireWall"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Emission_Halo"),
		CEffect_Emission::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Emission_Corona"),
		CEffect_Emission::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	_wstring strTemp = L"Prototype_Component_Texture_Emission_Corona";
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Emission_Corona"), &strTemp)))
			return E_FAIL;
	}
	strTemp = L"Prototype_Component_Texture_Emission_Halo";
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Emission_Halo"), &strTemp)))
			return E_FAIL;
	}
	
	//for (_uint i = 0; i < 5; ++i)
	//{
	//	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Hex_Shield"))))
	//		return E_FAIL;
	//}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Ally"),
		CEffect_Ally::Create(m_pDevice, m_pContext))))
		throw;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_Ally", nullptr);
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_Ally", nullptr);
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_AllyProjectile"),
		CEffect_AllyProjectile::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 50; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_AllyProjectile", nullptr);
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ShockWave"),
		CEffect_ShockWave::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 30; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_ShockWave", nullptr);
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SharpSweep"),
		CEffect_SharpSweep::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 30; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_SharpSweep", nullptr);
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DashAura"),
		CEffect_DashAura::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 5; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_DashAura", nullptr);
	}

	//Prototype_Component_Model_DashIndicator
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DashIndicator"),
		CEffect_DashIndicator::Create(m_pDevice, m_pContext))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ShockImpact"),
		CEffect_ShockImpact::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_ShockImpact"))))
			return E_FAIL;
	}

#pragma region Chest
	CChest::CHEST_DESC ChestDesc;
	//Mythic
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Ring_SixShoot";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Ring_SixShoot"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_GiantSlayer";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_GiantSlayer"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_BestDefence";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_BestDefence"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Rage";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Rage"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	//Mythic End

	//Epic
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Masquerade";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Masquerade"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Ogre";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Ogre"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Sunspot";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Sunspot"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Tumbler";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Tumbler"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	//Epic End

	//Rare
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Death";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Death"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Rock";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Rock"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Horned";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Horned"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	//Rare End

	//Uncommon
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Svalinn";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Svalinn"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Hermod";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Hermod"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Converter";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Converter"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	//Uncommon End

	//Common
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Coin";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Coin"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Shield25";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Shield25"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Key";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Key"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Heal25";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Heal25"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Heal50";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Heal50"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	//Common End

#pragma endregion

#pragma region Weapon

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Revolver"),
		CWeapon_Revolver::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Weapon_Revolver"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Pistol"),
		CWeapon_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Weapon_Pistol"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Gatling"),
		CWeapon_Gatling::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Weapon_Gatling"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_RocketLauncher"),
		CWeapon_RocketLauncher::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Weapon_RocketLauncher"))))
		return E_FAIL;
#pragma endregion
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Statue"),
		CStatue::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for(_uint i = 0;i<3;++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Statue"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Portal"),
		CBoss_Portal::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 1; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Boss_Portal"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MagmaField"),
		CMagmaField::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkeletonKey"),
		CSkeletonKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkeletonAlter"),
		CSkeletonAlter::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("방 생성 중 입니다."));
	CRoomManager::Get_Instance()->Resize_Room();
	CRoomManager::Get_Instance()->Generate_Room();

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Hel_Level()
{
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	thread t1(Load_Helhime_Monster1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Helhime_Monster2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Helhime_Monster3, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Hel_Room);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	
	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_BigFly"),
		CMonster_BigFly::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_ClockWork"),
		CMonster_ClockWork::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Golem"),
		CMonster_Golem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Luminous"),
		CMonster_Luminous::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_ScorpionQueen"),
		CMonster_ScorpionQueen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
	//	CMonster_Reaper::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//m_pGameInstance->Pool_Object(L"Prototype_GameObject_Monster_Reaper");
	//m_pGameInstance->Pool_Object(L"Prototype_GameObject_Monster_Reaper");
	//m_pGameInstance->Pool_Object(L"Prototype_GameObject_Monster_Reaper");

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Nidhog"),
		CMonster_Nidhog::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("방 생성 중 입니다."));
	CRoomManager::Get_Instance()->Resize_Room();
	CRoomManager::Get_Instance()->Generate_Hel();


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_For_Vana_Level()
{
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	thread t1(Load_Vana_Monster1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Vana_Monster2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Vana_Monster3, m_pGameInstance, m_pDevice, m_pContext);
	t1.join();
	thread t4(Load_Vana_Room);

	t2.join();
	t3.join();
	t4.join();

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wing"),
		CWing::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_DarkKnight"),
		CMonster_DarkKnight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Demon"),
		CMonster_Demon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_DemonBrute"),
		CMonster_DemonBrute::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
		CMonster_Reaper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Gullveig"),
		CMonster_Gullveig::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Sharp_Projectile"),
		CEffect_Sharp_Projectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for(_uint i = 0;i<10;++i)
	{
		m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Sharp_Projectile"), nullptr);
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Gullveig_Field"),
		CEffect_Gullveig_Field::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 10; ++i)
	{
		m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Gullveig_Field"), nullptr);
	}
	//
	for (_uint i = 0; i < 1; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Boss_Portal"))))
			return E_FAIL;
	}
	//
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Gullveig_LightWall"),
		CEffect_Gullveig_LightWall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 1; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Gullveig_LightWall"))))
			return E_FAIL;
	}
	lstrcpy(m_szLoadingText, TEXT("방 생성 중 입니다."));
	CRoomManager::Get_Instance()->Resize_Room();
	CRoomManager::Get_Instance()->Generate_Vana();


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_For_Svart_Level()
{
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	Load_Svart_Monster(m_pGameInstance, m_pDevice, m_pContext);

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Mistcalf"),
		CMonster_Mistcalf::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 1; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Monster_Mistcalf"))))
			return E_FAIL;
	}

	CRoomManager::Get_Instance()->Ready_Rooms_Svart();
	CRoomManager::Get_Instance()->Resize_Room();

	lstrcpy(m_szLoadingText, TEXT("방 생성 중 입니다."));
	CRoomManager::Get_Instance()->Generate_Svart();
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));


	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_BossRush_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	{
		Load_Trails(m_pGameInstance, m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CrosshairPoint"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshairPoint%d.png"), 2))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Crosshair"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshair%d.png"), 4))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BeatMarker"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Crosshair/NewCrosshairEdge%d.png"), 5))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/HpBar/Segment%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Items"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Item/item%d.png"), ITEM_END))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Weapons"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Weapons/Weapon%d.png"), WEAPON_END))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Amo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Amo/Bullet%d.png"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ability"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Abilities/Ablility%d.png"), ABILITY_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Level_Title"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Level_Title/Level_Title%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Title"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Boss_Title/Boss_%d.png"), 6))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Coin_Key"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Coin_Key/HUD%d.png"), 3))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridRoom%d.png"), ROOM_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room_Edge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridEdge/GridRoom%d.png"), RP_END))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MinimapPlayer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/MiniMap/GridRoomPlayer.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SwapItem"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Item/SwapItems.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/BossHp/BossBar%d.png"), 4))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MuzzleFlash"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Muzzle/MuzzleBC%d.png"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MuzzleMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Muzzle/T_Boris_Impact_01.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Orb"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/images/T_Rift_Orb_flibbook_%d.png"), 64))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cone_Soft_Normal"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/T_Cone_Soft_N.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sphere_Ambient"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Orb/T_Sphere_A.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeTrail"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_Silky_Smoke_Trail.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ThrillWall"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/ThrillWall/T_Fire_Lava_2.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChargeMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Charge/images/T_Magic_Smoke_0%d.png"), 4))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Charge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Charge/T_Plasma_Zinx_Seamless.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SummonRune"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/SummonRune/T_Star_Runes_Undertow.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Corona"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/SummonRune/T_Aurora_Corona_D.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowStar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_GlowStar_12pt.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/SnowFlake/T_Snowflakes_D_00.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Plasma"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/Plasma/T_Plasma_Wisps_%d.png"), 65))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_lightning_burst_pack.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeParticle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_smokeSingle_01.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Scorch"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/Scorch.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ThreeRune"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/ThreeRune/Rune%d.png"), 3))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Room_Clear"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/Room_Clear.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LightnigArc"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/LightningArc/images/T_Lightning_Arc_4x1_0%d.png"), 9))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RadialGreen"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Radial_Green.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RadialRed"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Radial_Red.png"), 1))))
			return E_FAIL;
		//
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emission_Halo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_FX_Halo_MD_01.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emission_Corona"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_Aurora_Corona_D.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LaserWarning"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/LaserWarning.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenFlare"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_FlareWide_05.png"),1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GideonRune"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Effect/T_Star_Runes_M%d.png"), 2))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake_Green"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Snowflakes_Green.png"), 1))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowFlake_Red"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Particle/T_Snowflakes_Red.png"), 1))))
			return E_FAIL;

	}

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));
	thread t1(Load_BossRush_EXT, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_BossRush_Boss_1, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Boss_Room);
	thread t4(Load_BossRush_Boss_2, m_pGameInstance, m_pDevice, m_pContext);
	thread t5(Load_BossRush_Boss_3, m_pGameInstance, m_pDevice, m_pContext);
	thread t6(Load_BossRush_Boss_4, m_pGameInstance, m_pDevice, m_pContext);

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩 중 입니다."));
#pragma region UI
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CrossHair"),
		CUI_CrossHair::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CrossHairPoint"),
		CUI_CrossHairPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BeatMarker"),
		CUI_BeatMarker::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpBar"),
		CUI_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Hp"),
		CUI_Hp::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Equipment"),
		CUI_Equipment::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_EquipSlot"),
		CUI_EquipSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSlot"),
		CUI_WeaponSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Amo"),
		CUI_Amo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Ability"),
		CUI_Ability::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AbilitySlot"),
		CUI_AbilitySlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Level_Title"),
		CUI_LevelTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Score"),
		CUI_Score::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MinimapGrid"),
		CUI_MinimapGrid::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MiniMap"),
		CUI_MiniMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SwapItem"),
		CUI_SwapItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_RoomClear"),
		CUI_RoomClear::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossTitle"),
		CUI_BossTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP"),
		CUI_BossHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Rect"),
		CUI_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	CParticle::PARTICLE_DESC PDESC{};
	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::ORBITAL;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 20;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Plasma";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 3.5f, 5.f };
	PDESC.vRange = _float3{ 3.2f,3.2f,3.2f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.1f, 0.3f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Plasma_Orbit"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))

	PDESC.bLoop = true;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 30;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_GlowStar";
	//PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.05f, 0.2f };
	PDESC.vRange = _float3{ 1.f,1.f,1.f };
	PDESC.vPivot = _float3{ 0.f,0.f,6.f };
	PDESC.vScale = _float2{ 0.2f, 0.3f };
	PDESC.vSpeed = _float2{ 10.0f, 30.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_GlowStar_Tail"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 15;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.05f, 0.2f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.f,6.f };
	PDESC.vScale = _float2{ 0.1f, 0.2f };
	PDESC.vSpeed = _float2{ 5.0f, 10.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_SnowFlake_Tail"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = false;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::FOUNTAIN;
	PDESC.fMaxLifeTime = 0.5f;
	PDESC.iNumInstance = 30;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Lightning";
	PDESC.vCenter = _float3{ 0.f,0.f,0.5f };
	PDESC.vLifeTime = _float2{ 0.1f, 0.5f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.1f };
	PDESC.vPivot = _float3{ 0.f,0.0f,-0.5f };
	PDESC.vScale = _float2{ 0.2f, 0.25f };
	PDESC.vSpeed = _float2{ 10.0f, 20.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = false;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark"))))
			return E_FAIL;
	}

	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.vRange = _float3{ 0.5f,0.6f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.0f, 0.0f };
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread"))))
			return E_FAIL;
	}

	PDESC.strTextureTag = L"Prototype_Component_Texture_RadialGreen";
	PDESC.vRange = _float3{ 10.f,1.0f,10.f };
	PDESC.iNumInstance = 60;
	PDESC.vSpeed = _float2{ 0.05f, 0.1f };
	PDESC.vLifeTime = _float2{ 1.5f, 2.5f };
	PDESC.fMaxLifeTime = 2.5f;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_RadialGreen_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_RadialGreen_Spread"))))
			return E_FAIL;
	}
	PDESC.strTextureTag = L"Prototype_Component_Texture_RadialRed";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_RadialRed_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_RadialRed_Spread"))))
			return E_FAIL;
	}
	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 20;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SmokeParticle";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.3f, 0.6f };
	PDESC.vRange = _float3{ 1.0f,1.0f,1.0f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.1f, 0.5f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Smoke_Spread"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = false;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 200;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_RadialRed";
	//PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.05f, 0.1f };
	PDESC.vRange = _float3{ 2.f,2.f,2.f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.2f, 0.3f };
	PDESC.vSpeed = _float2{ 60.0f, 80.f };
	PDESC.vTorque = _float3{ XM_2PI, XM_2PI, XM_2PI };
	PDESC.bInfinite = true;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Explosion0"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Explosion1"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Explosion2"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Explosion3"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Explosion4"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::RISING;
	PDESC.fMaxLifeTime = 100000.f;
	PDESC.iNumInstance = 25;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::DecalToDiffuse;
	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake_Green";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.5f, 1.6f };
	PDESC.vRange = _float3{ 2.0f,2.0f,2.0f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.05f, 0.1f };
	PDESC.vSpeed = _float2{ 0.3f, 12.f };
	PDESC.vTorque = _float3{ XM_PI, XM_PI, XM_PI };
	PDESC.bInfinite = true;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Heal_Rising"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.strTextureTag = L"Prototype_Component_Texture_SnowFlake_Red";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Buff_Rising"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;

	PDESC.bLoop = true;
	PDESC.bTextureAnim = true;
	PDESC.eMovementType = PARTICLE_MOVEMENT::SPREAD;
	PDESC.fMaxLifeTime = 1.f;
	PDESC.iNumInstance = 3;
	PDESC.iRenderPass = (_uint)SHADER_VTXPOINTINSTANCE_PASS::Blend;
	PDESC.strTextureTag = L"Prototype_Component_Texture_Explosion";
	PDESC.vCenter = _float3{ 0.f,0.f,0.f };
	PDESC.vLifeTime = _float2{ 0.5f, 1.0f };
	PDESC.vRange = _float3{ 0.5f,0.5f,0.5f };
	PDESC.vPivot = _float3{ 0.f,0.f,0.f };
	PDESC.vScale = _float2{ 0.2f, 0.8f };
	PDESC.vSpeed = _float2{ 0.5f, 1.f };
	PDESC.vTorque = _float3{ XM_PIDIV2, XM_PIDIV2, XM_PIDIV2 };
	PDESC.bInfinite = false;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Particle_Impact"),
		CParticle_Point::Create(m_pDevice, m_pContext, PDESC))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Particle_Impact"))))
			return E_FAIL;
	}
	//
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	if (s_ePC != PC_RUN)
	{
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TPS_Player"),
			CTPS_Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShopKeeper"),
		CShopKeeper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_MuzzleFlash"),
		CEffect_MuzzleFlash::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_MuzzleFlash"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Projectile"),
		CEffect_Projectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Projectile"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Teardrop"),
		CEffect_Teardrop::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Teardrop"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Wall"),
		CEffect_Wall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Wall"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Charge"),
		CEffect_Charge::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Charge"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Sweep"),
		CEffect_Sweep::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Sweep"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Spike"),
		CEffect_Spike::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Spike"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Fragment"),
		CEffect_Fragment::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 100; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Fragment"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Hex_Shield"),
		CEffect_Hex_Shield::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_RuneField"),
		CEffect_RuneField::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_RuneField"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Aura"),
		CEffect_Aura::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Aura"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Space"),
		CEffect_Space::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Space"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SummonRune"),
		CEffect_SummonRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_SummonRune"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Pheonix"),
		CEffect_Pheonix::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Pheonix"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_RuneSpear"),
		CEffect_RuneSpear::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_RuneSpear"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Scorch"),
		CEffect_Scorch::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Scorch"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DraugrRune"),
		CEffect_DraugrRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 4; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_DraugrRune"))))
			return E_FAIL;
	}
	for (_uint j = 0; j < 4; ++j)
	{
		_wstring strTag = _wstring(L"Prototype_GameObject_Effect_StatueFragment") + to_wstring(j);
		if (FAILED(m_pGameInstance->Add_Prototype(strTag,
			CEffect_StatueFragment::Create(m_pDevice, m_pContext, j))))
			return E_FAIL;
		for (_uint i = 0; i < 3; ++i)
		{
			if (FAILED(m_pGameInstance->Pool_Object(strTag)))
				return E_FAIL;
		}
	}

	for (_uint j = 0; j < 5; ++j)
	{
		_wstring strTag = _wstring(L"Prototype_GameObject_Effect_AlvisRock_Fragment_") + to_wstring(j);
		if (FAILED(m_pGameInstance->Add_Prototype(strTag,
			CEffect_AlvisRock::Create(m_pDevice, m_pContext, j))))
			return E_FAIL;
		for (_uint i = 0; i < 64; ++i)
		{
			if (FAILED(m_pGameInstance->Pool_Object(strTag)))
				return E_FAIL;
		}
	}
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Effect_AlvisRock",
		CEffect_AlvisRock::Create(m_pDevice, m_pContext, -1))))
		return E_FAIL;
	for (_uint i = 0; i < 16; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_AlvisRock")))
			return E_FAIL;
	}

	for (_uint j = 0; j < 3; ++j)
	{
		_wstring strTag = _wstring(L"Prototype_GameObject_Effect_SurtRock_Fragment_") + to_wstring(j);
		if (FAILED(m_pGameInstance->Add_Prototype(strTag,
			CEffect_SurtRockFrag::Create(m_pDevice, m_pContext, j))))
			return E_FAIL;
		for (_uint i = 0; i < 64; ++i)
		{
			if (FAILED(m_pGameInstance->Pool_Object(strTag)))
				return E_FAIL;
		}
	}
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Effect_SurtRock",
		CEffect_SurtRock::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 16; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_SurtRock")))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_NidhoggField"),
		CEffect_NidhoggField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_FireBall"),
		CEffect_FireBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_FireBall"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Explosion"),
		CEffect_Explosion::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Explosion"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DiscImpact"),
		CEffect_DiscImpact::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_DiscImpact"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ThreeRune"),
		CEffect_ThreeRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_LightningArc"),
		CEffect_LightningArc::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 100; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_LightningArc"))))
			return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ThunderClap"),
		CEffect_ThunderClap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 32; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_ThunderClap"))))
			return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_AlvisImpact"),
		CEffect_AlvisImpact::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 11; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_AlvisImpact"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_FireWall"),
		CEffect_FireWall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_FireWall"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Emission_Halo"),
		CEffect_Emission::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Emission_Corona"),
		CEffect_Emission::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	_wstring strTemp = L"Prototype_Component_Texture_Emission_Corona";
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Emission_Corona"), &strTemp)))
			return E_FAIL;
	}
	strTemp = L"Prototype_Component_Texture_Emission_Halo";
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_Emission_Halo"), &strTemp)))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SurtLaser_Warning"),
		CEffect_SurtLaser_Warning::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_SurtLaser_Warning"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SurtLaser"),
		CEffect_SurtLaser::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_SurtLaser"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_SurtRune"),
		CEffect_SurtRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_SurtRune"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_VortexFire"),
		CEffect_VortexFire::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_VortexFire"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonSpear"),
		CEffect_GideonSpear::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 90; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_GideonSpear"))))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonField"),
		CEffect_GideonField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_GideonField"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonPortal"),
		CEffect_GideonPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_GideonPortal"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_FlowerBurst"),
		CEffect_FlowerBurst::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_FlowerBurst"))))
			return E_FAIL;
	}
	//Effect_VortexFire

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Pistol"),
		CWeapon_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Weapon_Pistol"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Portal"),
		CBoss_Portal::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 1; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Boss_Portal"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonBeam"),
		CEffect_GideonBeam::Create(m_pDevice, m_pContext, -1))))
		return E_FAIL;
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_GideonBeam"))))
			return E_FAIL;
	}

	for (_uint j = 0; j < 4; ++j)
	{
		_wstring strTag = L"Prototype_GameObject_Effect_GideonBeam_Fragment_";
		strTag += to_wstring(j);
		if (FAILED(m_pGameInstance->Add_Prototype(strTag,
			CEffect_GideonBeam::Create(m_pDevice, m_pContext, j))))
			return E_FAIL;
		for (_uint i = 0; i < 3; ++i)
		{
			if (FAILED(m_pGameInstance->Pool_Object(strTag)))
				return E_FAIL;
		}
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonRune"),
		CEffect_GideonRune::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonBlackHole"),
		CEffect_GideonBlackHole::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_GideonBlackHole"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Ally"),
		CEffect_Ally::Create(m_pDevice, m_pContext))))
		throw;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_Ally", nullptr);
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_AllyProjectile"),
		CEffect_AllyProjectile::Create(m_pDevice, m_pContext))))
		throw;
	for(_uint i = 0;i <50; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_AllyProjectile", nullptr);
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_GideonSlowProjectile"),
		CEffect_GideonSlowProjectile::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 50; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_GideonSlowProjectile", nullptr);
	}
	//
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_BlockLight"),
		CEffect_BlockLight::Create(m_pDevice, m_pContext))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DashAura"),
		CEffect_DashAura::Create(m_pDevice, m_pContext))))
		throw;
	for (_uint i = 0; i < 5; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_DashAura", nullptr);
	}
	for (_uint i = 0; i < 10; ++i)
	{
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Effect_BlockLight", L"Prototype_Component_Texture_ThrillWall");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_DashIndicator"),
		CEffect_DashIndicator::Create(m_pDevice, m_pContext))))
		throw;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ShockImpact"),
		CEffect_ShockImpact::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	for (_uint i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Effect_ShockImpact"))))
			return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Alvis"),
		CMonster_Alvis::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Surt"),
		CMonster_Surt::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Gideon"),
		CMonster_Gideon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Draugr"),
		CMonster_Draugr::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_pGameInstance->Pool_Object(L"Prototype_GameObject_Monster_Draugr", nullptr);

	lstrcpy(m_szLoadingText, TEXT("방 생성 중 입니다."));
	CRoomManager::Get_Instance()->Resize_Room();
	CRoomManager::Get_Instance()->Generate_BossRush();

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	auto pImguiManager = CImGui_Manager::Get_Instance();

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/UI/BossHp/BossBar%d.png"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Rect"),
		CUI_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP"),
		CUI_BossHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩 중 입니다."));

	thread tRoom{Load_Room_Tool , m_pGameInstance, m_pDevice, m_pContext, pImguiManager };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_MagmaField",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/MagmaField/MagmaField.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Null",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Effect/Teardrop/Teardrop.txt", XMMatrixRotationRollPitchYaw(XM_PIDIV2, XM_PI, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_PortalModels.push_back("Prototype_Component_Model_Null");
	pImguiManager->m_PortalModels.push_back("Prototype_Component_Model_Asgard_Door");
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Fly",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Fly/Fly.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Fly"),
		CMonster_Fly::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Fly");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Spider",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Spider/Spider.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Spider"),
		CMonster_Spider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Spider");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Worm",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ClawWorm/Claw_Worm.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Worm"),
		CMonster_Worm::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Worm");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Bat",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Bat/Bat.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	{
		MSG_BOX(L"USUCK");
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Bat"),
		CMonster_Bat::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Bat");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_BigFly",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/BigFly/BigFly.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_BigFly"),
	//	CMonster_BigFly::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_BigFly");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ClockWork",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ClockWork/ClockWork.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_ClockWork"),
	//	CMonster_ClockWork::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_ClockWork");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Golem",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Golem/Golem.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.04f, 0.04f, 0.04f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Golem"),
	//	CMonster_Golem::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Golem");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Luminous",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Luminous/Luminous.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Luminous"),
	//	CMonster_Luminous::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Luminous");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ScorpionQueen",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/ScorpionQueen/ScorpionQueen.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_ScorpionQueen"),
	//	CMonster_ScorpionQueen::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_ScorpionQueen");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Nidhog",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Nidhog/Nidhog.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Nidhog"),
	//	CMonster_Nidhog::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Nidhog");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Surt",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Surt/Surt.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Surt"),
	//	CMonster_Surt::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Surt");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Reaper",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Reaper/Reaper.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
	//	CMonster_Reaper::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Reaper");


	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_DarkKnight",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/DarkKnight/DarkKnight.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_DarkKnight"),
	//	CMonster_DarkKnight::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_DarkKnight");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Demon",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Demon/Demon.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Demon"),
	//	CMonster_Demon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Demon");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_DemonBrute",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/DemonBrute/DemonBrute.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_DemonBrute"),
	//	CMonster_DemonBrute::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_DemonBrute");
	////
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Gullveig",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Gullveig/Gullveig.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.03f, 0.03f, 0.03f)))))
	//	throw;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Gullveig"),
	//	CMonster_Gullveig::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Gullveig");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Draugr",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Monster/Draugr/Draugr.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Draugr"),
	//	CMonster_Draugr::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_MonsterPrototype.push_back("Prototype_GameObject_Monster_Draugr");

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_ShopKeeper",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/NPC/ShopKeeper.txt", XMMatrixRotationRollPitchYaw(XM_PI, 0.f, XM_PIDIV2) * XMMatrixScaling(0.1f, 0.1f, 0.1f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShopKeeper"),
	//	CShopKeeper::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_ShopKeeper");
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonKey",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/SkeletonKey/SkeletonKey.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_SkeletonAlter",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/SkelKeyAlter/SkelKeyAlter.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		throw;
	}
#pragma region Consume
	CItem_Consume::CONSUME_DESC ConsumeDesc;
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Heal25",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Heal_25.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_HEAL_25;
	ConsumeDesc.strName = L"25 HP";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Heal25";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal25"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Heal25");
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal25_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Heal25_Shop");
	//Routine
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Heal50",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Heal_50.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_HEAL_50;
	ConsumeDesc.strName = L"50 HP";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Heal50";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal50"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Heal50");
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Heal50_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Heal50_Shop");
	//Routine
	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Converter",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Converter.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_CONVERTER;
	ConsumeDesc.strName = L"Converter";
	ConsumeDesc.strDesc = L"Convert Shield To HP";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Converter";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Converter"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Converter");
	ConsumeDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Converter_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Converter_Shop");
	//Routine

	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Key",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Key.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_KEY;
	ConsumeDesc.strName = L"Key";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Key";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Key"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Key");
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Key_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Key_Shop");
	//Routine


	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Shield25",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Shield_25.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_CONSUME_SHIELD;
	ConsumeDesc.strName = L"25 Shield";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Shield25";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Shield25"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Shield25");
	ConsumeDesc.iPrice = 4;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Shield25_Shop"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Shield25_Shop");
	//Routine

	//Routine
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Consume_Coin",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pickup/Coin.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	ConsumeDesc.eID = ITEM_END;
	ConsumeDesc.strName = L"";
	ConsumeDesc.strDesc = L"";
	ConsumeDesc.strModelTag = L"Prototype_Component_Model_Item_Consume_Coin";
	ConsumeDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Consume_Coin"),
		CItem_Consume::Create(m_pDevice, m_pContext, ConsumeDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Consume_Coin");
	//Routine
#pragma endregion
#pragma region EQUIP
	CItem_Equip::EQUIP_DESC EquipDesc;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Svalinn",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Svalinn.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_TALLSHIELD;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Generate Shield If You Stand Still";
	EquipDesc.strName = L"Svalinn";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Svalinn";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Svalinn"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Svalinn");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Svalinn_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Svalinn_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Sunspot",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Sunspot.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SUN_CHEST;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Release Fire On Shot";
	EquipDesc.strName = L"SunSpot";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Sunspot";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Sunspot"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Sunspot");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Sunspot_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Sunspot_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Masquerade",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Masquerade.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_BLANK;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Cast Ult";
	EquipDesc.strName = L"Masquerade";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Masquerade";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Masquerade"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Masquerade");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Masquerade_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Masquerade_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Gungnir",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Gungnir.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_RANGE;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Increase Range";
	EquipDesc.strName = L"Gungnir";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Gungnir";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Gungnir"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Gungnir");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Gungnir_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Gungnir_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_Death",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_Death.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SKULL_CHEST;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Doubled Damage";
	EquipDesc.strName = L"Death Mask";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_Death";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Death"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Death");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_Death_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_Death_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Shield_BestDefence",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Shield_BestDefence.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_CHEST_INFINITE;
	EquipDesc.eSlotID = SLOT_CHEST;
	EquipDesc.strDesc = L"Infinite Ammo";
	EquipDesc.strName = L"The Best Defence";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Shield_BestDefence";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_BestDefence"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_BestDefence");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Shield_BestDefence_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Shield_BestDefence_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Rage",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Rage.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_RAGE_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Multiply Damage With Your Coolness";
	EquipDesc.strName = L"Helm of Rage";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Rage";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Rage"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Rage");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Rage_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Rage_Shop");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Ogre",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Ogre.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAD_ORC;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"For the top";
	EquipDesc.strName = L"Helm of Ogre";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Ogre";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Ogre"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Ogre");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Ogre_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Ogre_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Horned",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Horned.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SHOTS_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Your Bullet Will Explode";
	EquipDesc.strName = L"Horned Helm";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Horned";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Horned"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Horned");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Horned_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Horned_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Fluffy",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Fluffy.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAD_FLUFFY;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Reload And Shoot";
	EquipDesc.strName = L"Fluffy";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Fluffy";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fluffy"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Fluffy");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fluffy_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Fluffy_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Helm_Fedora",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Helm_Fedora.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_SWAP_HEAD;
	EquipDesc.eSlotID = SLOT_HEAD;
	EquipDesc.strDesc = L"Rotate Slots";
	EquipDesc.strName = L"Fedora";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Helm_Fedora";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fedora"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Fedora");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Helm_Fedora_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Helm_Fedora_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Tumbler",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Tumbler.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_DOC_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Shot to Evade";
	EquipDesc.strName = L"Tumbler Boots";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Tumbler";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Tumbler"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Tumbler");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Tumbler_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Tumbler_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Rock",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Rock.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_HEAVY_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Stomp Your Enemy";
	EquipDesc.strName = L"Rock Boots";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Rock";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Rock"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Rock");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Rock_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Rock_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_Hermod",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_Hermod.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_FOOT_SANDALS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Speed Up";
	EquipDesc.strName = L"Grave of Hermod";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_Hermod";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Hermod"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Hermod");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_Hermod_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_Hermod_Shop");


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Boots_GiantSlayer",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Boots_GiantSlayer.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_KINGHT_BOOTS;
	EquipDesc.eSlotID = SLOT_BOOTS;
	EquipDesc.strDesc = L"Damage Per Max Hp";
	EquipDesc.strName = L"Giant Slayer";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Boots_GiantSlayer";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_GiantSlayer"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_GiantSlayer");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Boots_GiantSlayer_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Boots_GiantSlayer_Shop");

	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Item_Equip_Ring_SixShoot",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Item/Ring_SixShoot.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	{
		MSG_BOX(L"USUCK");
	}
	EquipDesc.eID = ITEM_GLOVE_SIXSHOOTER;
	EquipDesc.eSlotID = SLOT_ARM;
	EquipDesc.strDesc = L"Double Beat";
	EquipDesc.strName = L"Six Shooter";
	EquipDesc.strModelTag = L"Prototype_Component_Model_Item_Equip_Ring_SixShoot";
	EquipDesc.iPrice = 0;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Ring_SixShoot"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Ring_SixShoot");
	EquipDesc.iPrice = 7;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Equip_Ring_SixShoot_Shop"),
		CItem_Equip::Create(m_pDevice, m_pContext, EquipDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Item_Equip_Ring_SixShoot_Shop");

#pragma endregion
	//..Bin/Resource/Room/Room_Asgard_Armory.txt
#pragma region CHEST

	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Mythic",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_06.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Epic",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_08.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Rare",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_18.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Uncommon",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_19.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Chest_Common",
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_13.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
		{
			MSG_BOX(L"USUCK");
		}
	}

	CChest::CHEST_DESC ChestDesc;
	//Mythic
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Ring_SixShoot";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Ring_SixShoot"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Ring_SixShoot");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_GiantSlayer";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_GiantSlayer"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Boots_GiantSlayer");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_BestDefence";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_BestDefence"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Shield_BestDefence");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Rage";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Mythic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Rage"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Helm_Rage");

	//Mythic End

	//Epic
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Masquerade";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Masquerade"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Shield_Masquerade");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Ogre";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Ogre"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Helm_Ogre");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Sunspot";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Sunspot"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Shield_Sunspot");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Tumbler";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Epic";

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Tumbler"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Boots_Tumbler");
	//Epic End

	//Rare
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Death";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Death"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Shield_Death");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Rock";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Rock"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Boots_Rock");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Helm_Horned";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Rare";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Helm_Horned"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Helm_Horned");
	//Rare End

	//Uncommon
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Shield_Svalinn";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Shield_Svalinn"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Shield_Svalinn");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Equip_Boots_Hermod";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Boots_Hermod"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Boots_Hermod");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Converter";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Uncommon";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Converter"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Converter");
	//Uncommon End
	
	//Common
	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Coin";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Coin"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Coin");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Shield25";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Shield25"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Shield25");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Key";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Key"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Key");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Heal25";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Heal25"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Heal25");

	ChestDesc.strContainTag = L"Prototype_GameObject_Item_Consume_Heal50";
	ChestDesc.strModelTag = L"Prototype_Component_Model_Chest_Common";
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_Consume_Heal50"),
		CChest::Create(m_pDevice, m_pContext, ChestDesc))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Chest_Consume_Heal50");
	//Common End

#pragma endregion

#pragma region WEAPON
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Revolver",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/revolver/revolver.txt",
			//XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.0f, XM_PI, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, -XM_PI * 0.0866f, 0.f), XMVectorSet(0.f, -0.03f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Pistol",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Pistol/BA_pistol.txt",
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Gatling",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/gatling/gatling.txt",
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RocketLauncher",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Rocket_launcher/rocket_launcher.txt",
			//XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.0f, XM_PI, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
			XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XM_PI * 1.5f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f))))))
		throw;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Revolver"),
		CWeapon_Revolver::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Weapon_Revolver");

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Pistol"),
		CWeapon_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Weapon_Pistol");


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Gatling"),
		CWeapon_Gatling::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Weapon_Gatling");


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_RocketLauncher"),
		CWeapon_RocketLauncher::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Weapon_RocketLauncher");


	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Wing",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Wing/Wing.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f)));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wing"),
		CWing::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_Wing");
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MagmaField"),
		CMagmaField::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_MagmaField");

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkeletonKey"),
		CSkeletonKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_SkeletonKey");
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkeletonAlter"),
		CSkeletonAlter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pImguiManager->m_ObjectPrototype.push_back("Prototype_GameObject_SkeletonAlter");
#pragma endregion


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Chest_06",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Chest/chest_06.txt", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f)))))
	//{
	//	MSG_BOX(L"USUCK");
	//}
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chest_"),
	//	CCamera_Free::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	tRoom.join();
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Ending_Level()
{
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
