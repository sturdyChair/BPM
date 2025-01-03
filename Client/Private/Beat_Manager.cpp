#include "Beat_Manager.h"
#include "GameInstance.h"
#include "Texture.h"

IMPLEMENT_SINGLETON(CBeat_Manager)

CBeat_Manager::CBeat_Manager()
{
}

void CBeat_Manager::Tick(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	for (_uint i = 0; i < 8; ++i)
	{
		m_fNextBeats[i] -= fTimeDelta;
		if (m_fNextBeats[i] <= 0.f)
		{
			m_fNextBeats[i] += m_fBeat * 8.f;
			if (i % 2)
			{
				m_eNextBeatTypes[i] = BM_DEFAULT;
			}
			else
			{
				m_eNextBeatTypes[i] = BM_SMALL;
			}
		}
	}
	if (m_fTimer >= m_fBeat)
	{
		++m_iBeatCount;
		m_fTimer -= m_fBeat;

		if (m_bSequenceChange)
		{
			Start_Sequence();
		}
		if(m_pCurrSequence)
		if (m_iBeatCount >= m_pCurrSequence->iBeat)
		{
			Proceed();
		}

	}
}

void CBeat_Manager::Initialize()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pGameInstance = pGameInstance;
	Safe_AddRef(m_pGameInstance);
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Fill_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Loop 1_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Loop 2_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Loop 3_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Loop 3_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Loop 4_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Loop 4 Low Health_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Boss Loop 5_001", L"../Bin/Resource/Sound/Music/Asgard 1_Boss Loop 5 Low Health_001.wav");//16
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Fill_001" , 8});
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Loop 1_001" , 128});
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Loop 2_001" , 128});
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Loop 3_001" , 128});
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Loop 4_001" , 128});
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Boss Loop 5_001" , 128});

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Intro_001", L"../Bin/Resource/Sound/Music/Asgard 1_Intro_001.wav");//6
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Main Loop 1_001", L"../Bin/Resource/Sound/Music/Asgard 1_Main Loop 1_001.wav");//82
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Main Loop 2_001", L"../Bin/Resource/Sound/Music/Asgard 1_Main Loop 2_001.wav");//82
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Main Loop 3_001", L"../Bin/Resource/Sound/Music/Asgard 1_Main Loop 3_001.wav");//82
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Intro_001" , 48 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Main Loop 1_001" , 672 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Main Loop 2_001" , 672 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Main Loop 3_001" , 672 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Loading Ambience_001", L"../Bin/Resource/Sound/Music/Asgard 1_Loading Ambience_001.wav");
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Loading Ambience_001" , 672 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Cool Down Loop_001", L"../Bin/Resource/Sound/Music/Asgard 1_Cool Down Loop_001.wav");//16
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 1_Cool Down Loop_001" , 32 });


	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 1_Intro_001", L"../Bin/Resource/Sound/Music/Helheim 1_Intro_001.wav");//64
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 1_Loop 1_001", L"../Bin/Resource/Sound/Music/Helheim 1_Loop 1_001.wav");//80
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 1_Loop 2_001", L"../Bin/Resource/Sound/Music/Helheim 1_Loop 2_001.wav");//80
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 1_Loop 3_001", L"../Bin/Resource/Sound/Music/Helheim 1_Loop 3_001.wav");//80
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 1_Intro_001" , 64 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 1_Loop 1_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 1_Loop 2_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 1_Loop 3_001" , 640 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 2_Boss Fill_001",	 L"../Bin/Resource/Sound/Music/Helheim 2_Boss Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 2_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Helheim 2_Boss Loop 1_001.wav");//24
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 2_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Helheim 2_Boss Loop 2_001.wav");//24
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 2_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 2_Boss Loop 1_001" , 192 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 2_Boss Loop 2_001" , 192 });


	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Helheim 1_Loading Ambience_001", L"../Bin/Resource/Sound/Music/Helheim 1_Loading Ambience_001.L.wav");
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Helheim 1_Loading Ambience_001" , 672 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vanaheim 1_Boss Ambience_001", L"../Bin/Resource/Sound/Music/Vanaheim 1_Boss Ambience_001.wav");
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vanaheim 1_Boss Ambience_001" , 672 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svart 2_Boss Fill_001", L"../Bin/Resource/Sound/Music/Svartalfheim 2_Boss Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svart 2_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Svartalfheim 2_Boss Loop 1_001.wav");//17
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svart 2_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Svartalfheim 2_Boss Loop 2_001.wav");//17
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svart 2_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svart 2_Boss Loop 1_001" , 136 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svart 2_Boss Loop 2_001" , 136 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Hel 1_Boss Fill_001", L"../Bin/Resource/Sound/Music/Helheim 1_Boss Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Hel 1_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Helheim 1_Boss Loop 1_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Hel 1_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Helheim 1_Boss Loop 2_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Hel 1_Boss Loop 3_001", L"../Bin/Resource/Sound/Music/Helheim 1_Boss Loop 3 LOW HEALTH_001.wav");//16

	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Hel 1_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Hel 1_Boss Loop 1_001" , 128 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Hel 1_Boss Loop 2_001" , 128 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Hel 1_Boss Loop 3_001" , 128 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 2_Boss Fill_001", L"../Bin/Resource/Sound/Music/Asgard 2_Boss Intro_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 2_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Asgard 2_Boss Loop 1_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 2_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Asgard 2_Boss Loop 2 Low Health_001.wav");//16
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 2_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 2_Boss Loop 1_001" , 128 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Asgard 2_Boss Loop 2_001" , 128 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Crypt 2_Boss Fill_001", L"../Bin/Resource/Sound/Music/Crypt_Boss Chord Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Crypt 2_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Crypt_Boss Loop 1_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Crypt 2_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Crypt_Boss Loop 2_001.wav");//12
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Crypt 2_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Crypt 2_Boss Loop 1_001" , 128 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Crypt 2_Boss Loop 2_001" , 96 });



	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"MenuMusic", L"../Bin/Resource/Sound/Music/MenuMusic.wav");
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"MenuMusic" , 672 });


	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Boss Fill_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Boss Fill_001.wav");//1
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Boss Loop 1_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Boss Loop 1_001.wav");//16
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Boss Loop 2_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Boss Loop 2 Low Health_001.wav");//8
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Boss Loop 3_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Boss Loop 3 Low Health_001.wav");//8

	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Boss Loop 1_001" , 128 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Boss Loop 2_001" , 64 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Boss Loop 3_001" , 64 });


	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Main Loop 1_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Main Loop 1_001.wav");//640
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Main Loop 2_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Main Loop 2_001.wav");//640
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Vana 2_Main Loop 3_001", L"../Bin/Resource/Sound/Music/Vanaheim 2_Main Loop 3_001.wav");//640

	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Main Loop 1_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Main Loop 2_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Vana 2_Main Loop 3_001" , 640 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svartalfheim 1_Loop 1_001", L"../Bin/Resource/Sound/Music/Svartalfheim 1_Loop 1_001.wav");//640
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svartalfheim 1_Loop 2_001", L"../Bin/Resource/Sound/Music/Svartalfheim 1_Loop 2_001.wav");//640
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svartalfheim 1_Loop 3_001", L"../Bin/Resource/Sound/Music/Svartalfheim 1_Loop 3_001.wav");//640

	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svartalfheim 1_Loop 1_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svartalfheim 1_Loop 2_001" , 640 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svartalfheim 1_Loop 3_001" , 640 });

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svartalfheim 1_Boss Fill_001", L"../Bin/Resource/Sound/Music/Svartalfheim 1_Boss Fill_001.wav");//640
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Svartalfheim 1_Boss Loop_001", L"../Bin/Resource/Sound/Music/Svartalfheim 1_Boss Loop_001.wav");//640
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svartalfheim 1_Boss Fill_001" , 8 });
	m_vecBGMSequence.push_back(BGM_SEQUENCE{ L"Svartalfheim 1_Boss Loop_001" , 128 });

	m_vecBGMSequence[0].pNext = &m_vecBGMSequence[1];
	m_vecBGMSequence[1].pNext = &m_vecBGMSequence[2];
	m_vecBGMSequence[2].pNext = &m_vecBGMSequence[3];
	m_vecBGMSequence[3].pNext = &m_vecBGMSequence[4];
	m_vecBGMSequence[4].pNext = &m_vecBGMSequence[5];
	m_vecBGMSequence[5].pNext = &m_vecBGMSequence[1];
	m_mapBGM.emplace(_wstring(Asgard_1_Boss), &m_vecBGMSequence[0]);

	m_vecBGMSequence[6].pNext = &m_vecBGMSequence[7];
	m_vecBGMSequence[7].pNext = &m_vecBGMSequence[8];
	m_vecBGMSequence[8].pNext = &m_vecBGMSequence[9];
	m_vecBGMSequence[9].pNext = &m_vecBGMSequence[7];
	m_mapBGM.emplace(_wstring(Asgard_1), &m_vecBGMSequence[6]);

	m_vecBGMSequence[10].pNext = &m_vecBGMSequence[10];
	m_mapBGM.emplace(L"Asgard_Loading", &m_vecBGMSequence[10]);

	m_vecBGMSequence[11].pNext = &m_vecBGMSequence[11];
	m_mapBGM.emplace(L"Asgard_CoolDown", &m_vecBGMSequence[11]);

	m_vecBGMSequence[12].pNext = &m_vecBGMSequence[13];
	m_vecBGMSequence[13].pNext = &m_vecBGMSequence[14];
	m_vecBGMSequence[14].pNext = &m_vecBGMSequence[15];
	m_vecBGMSequence[15].pNext = &m_vecBGMSequence[13];
	m_mapBGM.emplace(_wstring(L"Helheim"), &m_vecBGMSequence[12]);

	m_vecBGMSequence[16].pNext = &m_vecBGMSequence[17];
	m_vecBGMSequence[17].pNext = &m_vecBGMSequence[18];
	m_vecBGMSequence[18].pNext = &m_vecBGMSequence[17];
	m_mapBGM.emplace(_wstring(L"Helheim_Boss"), &m_vecBGMSequence[16]);

	m_vecBGMSequence[19].pNext = &m_vecBGMSequence[19];
	m_mapBGM.emplace(L"Helheim_Loading", &m_vecBGMSequence[19]);

	m_vecBGMSequence[20].pNext = &m_vecBGMSequence[20];
	m_mapBGM.emplace(L"Helheim_CoolDown", &m_vecBGMSequence[20]);

	m_vecBGMSequence[21].pNext = &m_vecBGMSequence[22];
	m_vecBGMSequence[22].pNext = &m_vecBGMSequence[23];
	m_vecBGMSequence[23].pNext = &m_vecBGMSequence[22];
	m_mapBGM.emplace(_wstring(L"Alvis"), &m_vecBGMSequence[21]);

	m_vecBGMSequence[24].pNext = &m_vecBGMSequence[25];
	m_vecBGMSequence[25].pNext = &m_vecBGMSequence[26];
	m_vecBGMSequence[26].pNext = &m_vecBGMSequence[27];
	m_vecBGMSequence[27].pNext = &m_vecBGMSequence[25];
	m_mapBGM.emplace(_wstring(L"Surt"), &m_vecBGMSequence[24]);

	m_vecBGMSequence[28].pNext = &m_vecBGMSequence[29];
	m_vecBGMSequence[29].pNext = &m_vecBGMSequence[29];
	m_vecBGMSequence[30].pNext = &m_vecBGMSequence[29];
	m_mapBGM.emplace(_wstring(L"Gideon_1"), &m_vecBGMSequence[28]);

	m_vecBGMSequence[31].pNext = &m_vecBGMSequence[32];
	m_vecBGMSequence[32].pNext = &m_vecBGMSequence[33];
	m_vecBGMSequence[33].pNext = &m_vecBGMSequence[32];
	m_mapBGM.emplace(_wstring(L"Gideon_2"), &m_vecBGMSequence[31]);

	m_vecBGMSequence[34].pNext = &m_vecBGMSequence[34];
	m_mapBGM.emplace(_wstring(L"Menu"), &m_vecBGMSequence[34]);

	m_vecBGMSequence[35].pNext = &m_vecBGMSequence[36];
	m_vecBGMSequence[36].pNext = &m_vecBGMSequence[37];
	m_vecBGMSequence[37].pNext = &m_vecBGMSequence[38];
	m_vecBGMSequence[38].pNext = &m_vecBGMSequence[37];
	m_mapBGM.emplace(_wstring(L"Vana_Boss"), &m_vecBGMSequence[35]);

	m_vecBGMSequence[39].pNext = &m_vecBGMSequence[40];
	m_vecBGMSequence[40].pNext = &m_vecBGMSequence[41];
	m_vecBGMSequence[41].pNext = &m_vecBGMSequence[39];
	m_mapBGM.emplace(_wstring(L"Vana"), &m_vecBGMSequence[39]);

	m_vecBGMSequence[42].pNext = &m_vecBGMSequence[43];
	m_vecBGMSequence[43].pNext = &m_vecBGMSequence[44];
	m_vecBGMSequence[44].pNext = &m_vecBGMSequence[42];
	m_mapBGM.emplace(_wstring(L"Svart"), &m_vecBGMSequence[42]);

	m_vecBGMSequence[45].pNext = &m_vecBGMSequence[46];
	m_vecBGMSequence[46].pNext = &m_vecBGMSequence[46];
	m_mapBGM.emplace(_wstring(L"Mistcalf"), &m_vecBGMSequence[45]);

#pragma region SFX
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink", L"../Bin/Resource/Sound/SFX/Player_Blink.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_MissedBeat", L"../Bin/Resource/Sound/SFX/Player_MissedBeat.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Hurt0", L"../Bin/Resource/Sound/SFX/Player_Hurt0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Hurt1", L"../Bin/Resource/Sound/SFX/Player_Hurt1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Hurt2", L"../Bin/Resource/Sound/SFX/Player_Hurt2.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_TakeDamage", L"../Bin/Resource/Sound/SFX/Player_TakeDamage.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_TakeDamage0", L"../Bin/Resource/Sound/SFX/Player_TakeDamage0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_LowHealth", L"../Bin/Resource/Sound/SFX/Player_LowHealth.wav");
	
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Block", L"../Bin/Resource/Sound/SFX/Player_Block.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Block_Start", L"../Bin/Resource/Sound/SFX/Player_Block_Start.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Jump0", L"../Bin/Resource/Sound/SFX/Player_Jump0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Jump1", L"../Bin/Resource/Sound/SFX/Player_Jump1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Jump2", L"../Bin/Resource/Sound/SFX/Player_Jump2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastFireball", L"../Bin/Resource/Sound/SFX/Player_CastFireball.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastMagic", L"../Bin/Resource/Sound/SFX/Player_CastMagic.wav");
	pGameInstance->Add_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fireball_Explosion", L"../Bin/Resource/Sound/SFX/Fireball_Explosion.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_Empty", L"../Bin/Resource/Sound/SFX/Weapon_Empty.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Weapon_LowAmo", L"../Bin/Resource/Sound/SFX/Weapon_LowAmo.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Fire1", L"../Bin/Resource/Sound/SFX/Revolver_Fire1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Fire2", L"../Bin/Resource/Sound/SFX/Revolver_Fire2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Fire3", L"../Bin/Resource/Sound/SFX/Revolver_Fire3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Equip", L"../Bin/Resource/Sound/SFX/Revolver_Equip.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_Reload", L"../Bin/Resource/Sound/SFX/Revolver_Reload.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_ReloadStart", L"../Bin/Resource/Sound/SFX/Revolver_ReloadStart.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Revolver_ReloadEnd", L"../Bin/Resource/Sound/SFX/Revolver_ReloadEnd.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_Fire1", L"../Bin/Resource/Sound/SFX/Pistol_Fire1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_Fire2", L"../Bin/Resource/Sound/SFX/Pistol_Fire2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_Fire3", L"../Bin/Resource/Sound/SFX/Pistol_Fire3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_ReloadStart", L"../Bin/Resource/Sound/SFX/Pistol_ReloadStart.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pistol_ReloadEnd", L"../Bin/Resource/Sound/SFX/Pistol_ReloadEnd.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_Fire1", L"../Bin/Resource/Sound/SFX/Minigun_Fire1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_Fire2", L"../Bin/Resource/Sound/SFX/Minigun_Fire2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_Fire3", L"../Bin/Resource/Sound/SFX/Minigun_Fire3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_Equip", L"../Bin/Resource/Sound/SFX/Minigun_Equip.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_ReloadStart", L"../Bin/Resource/Sound/SFX/Minigun_ReloadStart.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Minigun_ReloadEnd", L"../Bin/Resource/Sound/SFX/Minigun_ReloadEnd.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_Fire1", L"../Bin/Resource/Sound/SFX/RPG_Fire1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_Fire2", L"../Bin/Resource/Sound/SFX/RPG_Fire2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLauncher_Fire3", L"../Bin/Resource/Sound/SFX/RPG_Fire3.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLaunche_ReloadStart", L"../Bin/Resource/Sound/SFX/RPG_ReloadStart.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLaunche_Reload", L"../Bin/Resource/Sound/SFX/RPG_Reload.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"RocketLaunche_ReloadEnd", L"../Bin/Resource/Sound/SFX/RPG_ReloadEnd.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Coin", L"../Bin/Resource/Sound/SFX/Pickup_Coin.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_CoinBag", L"../Bin/Resource/Sound/SFX/Pickup_CoinBag.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Heal", L"../Bin/Resource/Sound/SFX/Pickup_Health.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Key", L"../Bin/Resource/Sound/SFX/Pickup_Key.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_MaxHp", L"../Bin/Resource/Sound/SFX/Pickup_MaxHp.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Pickup_Shield", L"../Bin/Resource/Sound/SFX/Pickup_Shield.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Chest_Open", L"../Bin/Resource/Sound/SFX/Chest_Open.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Coin_Drop", L"../Bin/Resource/Sound/SFX/Coin_Drop.wav");
	//pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"LowHealth", L"../Bin/Resource/Sound/SFX/LowHealth_Warn.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Bat_Death", L"../Bin/Resource/Sound/SFX/Bat_Death.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile", L"../Bin/Resource/Sound/SFX/Draugr_Projectile.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Dance", L"../Bin/Resource/Sound/SFX/Draugr_Dance.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Dance_Start", L"../Bin/Resource/Sound/SFX/Draugr_Dance1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Death", L"../Bin/Resource/Sound/SFX/Draugr_Death.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Hammer", L"../Bin/Resource/Sound/SFX/Draugr_Hammer.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Attacked", L"../Bin/Resource/Sound/SFX/Spider_Attack.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Charge",   L"../Bin/Resource/Sound/SFX/Spider_Charge.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Death",    L"../Bin/Resource/Sound/SFX/Spider_Death.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Death", L"../Bin/Resource/Sound/SFX/ScorpionQueen_Death.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Fire", L"../Bin/Resource/Sound/SFX/ScorpionQueen_Fire.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Hurt", L"../Bin/Resource/Sound/SFX/ScorpionQueen_Hurt.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Luminous_Hurt", L"../Bin/Resource/Sound/SFX/Luminous_Hurt.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Luminous_Death", L"../Bin/Resource/Sound/SFX/Luminous_Death.wav");
	
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"BigFly_Hurt", L"../Bin/Resource/Sound/SFX/BigFly_Hurt.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"BigFly_Death", L"../Bin/Resource/Sound/SFX/BigFly_Death.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Golem_Death", L"../Bin/Resource/Sound/SFX/Golem_Death.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Golem_Stomp", L"../Bin/Resource/Sound/SFX/Golem_Stomp.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Room_Clear",    L"../Bin/Resource/Sound/SFX/Room_Clear.wav");

	pGameInstance->Add_Sound_3D(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spike", L"../Bin/Resource/Sound/SFX/Spike.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Fire", L"../Bin/Resource/Sound/SFX/fire-magic.mp3");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Birth", L"../Bin/Resource/Sound/SFX/Nidhog_Birth.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Death", L"../Bin/Resource/Sound/SFX/Nidhog_Death.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Stinger", L"../Bin/Resource/Sound/SFX/Nidhog_Stinger.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Spread", L"../Bin/Resource/Sound/SFX/Nidhog_Spread.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Bump", L"../Bin/Resource/Sound/SFX/Alvis_Bump.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact0", L"../Bin/Resource/Sound/SFX/Alvis_Impact0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1", L"../Bin/Resource/Sound/SFX/Alvis_Impact1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact2", L"../Bin/Resource/Sound/SFX/Alvis_Impact2.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderGuitar1", L"../Bin/Resource/Sound/SFX/ThunderGuitar1.ogg");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderGuitar2", L"../Bin/Resource/Sound/SFX/ThunderGuitar2.ogg");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderGuitar3", L"../Bin/Resource/Sound/SFX/ThunderGuitar3.ogg");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap", L"../Bin/Resource/Sound/SFX/ThunderClap.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Roar", L"../Bin/Resource/Sound/SFX/Surt_Roar.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Slice", L"../Bin/Resource/Sound/SFX/Surt_Slice.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Laser", L"../Bin/Resource/Sound/SFX/Surt_Laser.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Gideon_Warn", L"../Bin/Resource/Sound/SFX/Gideon_Warn.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Gideon_Fire", L"../Bin/Resource/Sound/SFX/Gideon_Fire.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigDeath", L"../Bin/Resource/Sound/SFX/GullveigDeath.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigPain", L"../Bin/Resource/Sound/SFX/GullveigPain.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigTalk1", L"../Bin/Resource/Sound/SFX/GullveigTalk.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigTalk2", L"../Bin/Resource/Sound/SFX/GullveigTalk2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigTalk3", L"../Bin/Resource/Sound/SFX/GullveigTalk3.wav");


	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Demon_Cast", L"../Bin/Resource/Sound/SFX/DemonCast.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Demon_Death", L"../Bin/Resource/Sound/SFX/DemonDeath.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Demon_Floor", L"../Bin/Resource/Sound/SFX/DemonFloor.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ChestLocked", L"../Bin/Resource/Sound/SFX/ChestLocked.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Equip_Item", L"../Bin/Resource/Sound/SFX/Equip_Item.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Burn", L"../Bin/Resource/Sound/SFX/Burn.ogg");

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord0", L"../Bin/Resource/Sound/Music/Draugr_Chord0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord1", L"../Bin/Resource/Sound/Music/Draugr_Chord1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord2", L"../Bin/Resource/Sound/Music/Draugr_Chord2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord3", L"../Bin/Resource/Sound/Music/Draugr_Chord3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord4", L"../Bin/Resource/Sound/Music/Draugr_Chord4.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord0", L"../Bin/Resource/Sound/Music/Nidhog_Chord0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord1", L"../Bin/Resource/Sound/Music/Nidhog_Chord1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord2", L"../Bin/Resource/Sound/Music/Nidhog_Chord2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord3", L"../Bin/Resource/Sound/Music/Nidhog_Chord3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord4", L"../Bin/Resource/Sound/Music/Nidhog_Chord4.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord5", L"../Bin/Resource/Sound/Music/Nidhog_Chord5.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord6", L"../Bin/Resource/Sound/Music/Nidhog_Chord6.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord7", L"../Bin/Resource/Sound/Music/Nidhog_Chord7.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord8", L"../Bin/Resource/Sound/Music/Nidhog_Chord8.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord0", L"../Bin/Resource/Sound/Music/Alvis_Chord0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord1", L"../Bin/Resource/Sound/Music/Alvis_Chord1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord2", L"../Bin/Resource/Sound/Music/Alvis_Chord2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord3", L"../Bin/Resource/Sound/Music/Alvis_Chord3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord4", L"../Bin/Resource/Sound/Music/Alvis_Chord4.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord0", L"../Bin/Resource/Sound/Music/Surt_Chord0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord1", L"../Bin/Resource/Sound/Music/Surt_Chord1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord2", L"../Bin/Resource/Sound/Music/Surt_Chord2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord3", L"../Bin/Resource/Sound/Music/Surt_Chord3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord4", L"../Bin/Resource/Sound/Music/Surt_Chord4.wav");

	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord0", L"../Bin/Resource/Sound/Music/GullveigChord0.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord1", L"../Bin/Resource/Sound/Music/GullveigChord1.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord2", L"../Bin/Resource/Sound/Music/GullveigChord2.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord3", L"../Bin/Resource/Sound/Music/GullveigChord3.wav");
	pGameInstance->Add_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord4", L"../Bin/Resource/Sound/Music/GullveigChord4.wav");
	
#pragma endregion

	for (_uint i = 0; i < 8; ++i)
	{
		if (i % 2)
		{
			m_eNextBeatTypes[i] = BM_DEFAULT;
		}
		else
		{
			m_eNextBeatTypes[i] = BM_SMALL;
		}
	}


}

void CBeat_Manager::Kill_Instance()
{
	m_pInstance->Free();
	m_pInstance->Destroy_Instance();
}

void CBeat_Manager::Stop_Sequence()
{
	m_bSequenceStoped = true;
	if (m_pCurrSequence)
	{
		m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, m_pCurrSequence->strTag);
	}
}

void CBeat_Manager::Start_Sequence()
{
	m_bSequenceChange = false;
	if (m_pCurrSequence)
	{
		m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, m_pCurrSequence->strTag);
	}
	m_pCurrSequence = m_mapBGM[m_strNextSequenceTag];
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, m_pCurrSequence->strTag);
	m_iBeatCount = 0;
	for (_uint i = 0; i < 8; ++i)
	{
		m_fNextBeats[i] = (_float)i * m_fBeat;
	}
	m_fTimer = 0.f;
}

void CBeat_Manager::Proceed()
{
	if (m_bSequenceStoped)
	{
		return;
	}
	m_pCurrSequence = m_pCurrSequence->pNext;
	if (m_pCurrSequence)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, m_pCurrSequence->strTag);
	}
	m_iBeatCount = 0;
	for (_uint i = 0; i < 8; ++i)
	{
		m_fNextBeats[i] = (_float)i * m_fBeat;
	}
	m_fTimer = 0.f;
}

void CBeat_Manager::Set_N_BeatType(_uint iN, BM_TYPE eType)
{
	_float fLeast = 10.f;
	_uint iIdx = 0;
	for (_uint i = 0; i<8;++i)
	{
		if (fLeast > m_fNextBeats[i])
		{
			fLeast = m_fNextBeats[i];
			iIdx = i;
		}
	}

	_uint iNext = iIdx + iN;
	iNext %= 8;
	m_eNextBeatTypes[iNext] = eType;

}



void CBeat_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDefaultORM);
	Safe_Release(m_pDefaultNormal);
	Safe_Release(m_pNoise);
	__super::Free();
}
