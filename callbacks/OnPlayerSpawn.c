#include "GameVars.h"

int EventSkins[]={97, 101, 108, 109, 115, 147, 154, 155, 167, 171, 177, 180, 240, 252};
extern char *NoClass;

float RandomRange(int range)
{
	if(rand()%2 == 0)
	{
		return ((float)(rand()%range))/5;
	}
	else
	{
		return (((float)(rand()%range))/5)*-1;
	}
	
}

void OnPlayerSpawn(int playerid)
{
	za[playerid].FakekillSpawn=true;
	za[playerid].IsDead=false;

	if(za[playerid].JustConnected == true)
	{
		za[playerid].JustConnected=false;

		TextDrawHideForPlayer(playerid, t_welcome);
		TextDrawHideForPlayer(playerid, t_za);
		TextDrawHideForPlayer(playerid, t_server);
		TextDrawHideForPlayer(playerid, t_help);
		TextDrawHideForPlayer(playerid, t_rules);
		TextDrawHideForPlayer(playerid, t_add);
	}

	TextDrawHideForPlayer(playerid, t_Red);
	TextDrawHideForPlayer(playerid, t_Black);
	if(temp[playerid].IsBlinded == true)
	{
		SetPlayerWeather(playerid, MapData.Weather);
		SetPlayerTime(playerid, MapData.Time);
	}
	TextDrawHideForPlayer(playerid, t_Blue);
	TextDrawHideForPlayer(playerid, t_Yellow);
	TextDrawHideForPlayer(playerid, t_Magenta);
	
	temp[playerid].Infection=false;
	temp[playerid].Slow=false;
	temp[playerid].Leg=false;
	temp[playerid].TransferInfection=0;
	temp[playerid].Burned=false;
	temp[playerid].HumanRogue=false;
	temp[playerid].Poison=0;
	temp[playerid].IsPlayerJammed=false;
	temp[playerid].IsBlinded=false;
	
	SetPlayerDrunkLevel(playerid, 0);
	SetCameraBehindPlayer(playerid);
	SetPlayerVirtualWorld(playerid, 0);
	ResetPlayerWeapons(playerid);
	
	//Assist Killing
	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Log == true)
		{
			temp[i].Assist[playerid]=0;
		}
	}

	if(za[playerid].InEvent == true)
	{
		float x, y;
		x=Event.Spawn[0]+RandomRange(3);
		y=Event.Spawn[1]+RandomRange(3);
		SetPlayerPos(playerid, x, y, Event.Spawn[2]);
		SetPlayerFacingAngle(playerid, Event.Spawn[3]);
		SetPlayerVirtualWorld(playerid, VIRTUALWORLD_EVENT);
	
		SetPlayerArmour(playerid, 50.0);
		SetPlayerTeam(playerid, 0);
		za[playerid].UCID=INVALID_CLASS_ID;
		za[playerid].ClassDesc=NoClass;
		za[playerid].Team=true;
		
		if(za[playerid].Admin > 4)
		{
			SetPlayerSkin(playerid, 189);
		}
		else
		{
			SetPlayerSkin(playerid, EventSkins[rand()%(sizeof(EventSkins)/sizeof(int))]);
		}
		return;
	}

	if(za[playerid].Team == false)
	{
		//Anti-Death NOP
		if(za[playerid].SpawnedAsHuman == true)
		{
			za[playerid].SpawnedAsHuman=false;
		}
		else
		{
			za[playerid].Team=true;
			SpawnPlayer(playerid);
			za[playerid].Killstreaks=0;
			return;
		}
		
		SetPlayerTeam(playerid, 0);
        SetPlayerColor(playerid, 0x00B0B0AA);
		EnablePlayerCameraTarget(playerid, 0);
		
		za[playerid].PlayedAsZombie=0;
		
		float x, y, z, a;
		int rnd=rand()%MapData.HumanSpawn;
		x=*((&MapData.HumanX1)+rnd*4)+RandomRange(3);
		y=*((&MapData.HumanY1)+rnd*4)+RandomRange(3);
		z=*((&MapData.HumanZ1)+rnd*4);
		a=*((&MapData.HumanA1)+rnd*4);
	
		SetPlayerPos(playerid, x, y, z);
		SetPlayerFacingAngle(playerid, a);
		SetPlayerInterior(playerid, MapData.Interior);
		
		za[playerid].ClassDesc=(char *)&(humanclasses[za[playerid].Hclass].Name[0]);
		za[playerid].UCID=humanclasses[za[playerid].Hclass].UniqueID;
		za[playerid].CDP=za[playerid].Hclass;
		SetPlayerHealth(playerid, humanclasses[za[playerid].Hclass].health);
		
		if(za[playerid].VIP)SetPlayerArmour(playerid, 50.0);
		else SetPlayerArmour(playerid, humanclasses[za[playerid].Hclass].armour);
		
		if(humanclasses[za[playerid].Hclass].FightStyle)SetPlayerFightingStyle(playerid, humanclasses[za[playerid].Hclass].FightStyle);
		else SetPlayerFightingStyle(playerid, 15);
		
		for(int i=0; i < 5; i++)
		{
			int weap=humanclasses[za[playerid].Hclass].Weapon[i];
			if(weap)
			{
				GivePlayerWeapon(playerid, weap, humanclasses[za[playerid].Hclass].Ammo[i]);

				if(za[playerid].VIP > 0)
				{
					if(weap == 23)SetPlayerAmmo(playerid,23, 300);
					else if(weap == 24)SetPlayerAmmo(playerid,24, 300);
					else if(weap == 25)SetPlayerAmmo(playerid,25, 600);
					else if(weap == 29)SetPlayerAmmo(playerid,29, 500);
					else if(weap == 30)SetPlayerAmmo(playerid,30, 1000);
					else if(weap == 31)SetPlayerAmmo(playerid,31, 1000);
					else if(weap == 33)SetPlayerAmmo(playerid,33, 130);
					else if(weap == 34)SetPlayerAmmo(playerid,34, 130);
					else if(weap == 37)SetPlayerAmmo(playerid,37, 800);
					else if(weap == 39)SetPlayerAmmo(playerid,39, 50);
				}
			}
		}
		
		if(humanclasses[za[playerid].Hclass].Skins == 1)
		{
			SetPlayerSkin(playerid, humanclasses[za[playerid].Hclass].Skin[0]);	
		}
		else
		{
			SetPlayerSkin(playerid, humanclasses[za[playerid].Hclass].Skin[rand()%(humanclasses[za[playerid].Hclass].Skins)]);	
		}

		//Scout
		if(za[playerid].UCID == 5)
		{
			SetPlayerColor(playerid, 0x00B0B000);
		}
		//Adv.scout
		else if(za[playerid].UCID == 15)
		{
			SetPlayerColor(playerid, 0x00B0B000);
		}
		
		//Iron Man's Attachments;
		if(za[playerid].UCID == 31)
		{
			temp[playerid].Energy=150;
				
			SetPlayerAttachedObject(playerid,7,19142,1,0.058,0.0379,0.0,0.0,0.0,0.0,1.18,1.285,1.0,0,0);
			SetPlayerAttachedObject(playerid,8,19141,2,0.071,0.0,0.0,0.0,0.0,0.0,1.5889,1.2649,1.094,0,0);
			SetPlayerAttachedObject(playerid,9,18641,1,0.146,0.1279,0.0,102.9,0.0,0.0,1.5389,1.538,1.0,0,0);		
		}

		char str[144];
		snprintf(str, sizeof(str), ">> You have spawned as a %s, use /class to change this", humanclasses[za[playerid].Hclass].Name);
		SendClientMessage(playerid, 0xFFFF00AA, str);
	}//if gTeam
	else
	{
		SetPlayerTeam(playerid, 1);	
	
		float x, y, z, a;
		int rnd=0;
		
		int UTbegin=0, UT=0;
		for(int i=3; i < MapData.ZombieSpawn; i++)
		{
			if(Undertaker[i].Occupied == true)
			{
				if(UTbegin == 0)UTbegin=i;
				UT++;
			}
		}
		
		if(UTbegin == 0)
		{
			rnd=rand()%MapData.ZombieSpawn;
		}
		else
		{
			rnd=UTbegin+rand()%UT;
		}
		
		//Undertaker Zombie
		if(Undertaker[rnd].Occupied == true)
		{
			//If player disconnected, changed class, time expired;
			if(za[Undertaker[rnd].PlayerID].Log == false ||	za[Undertaker[rnd].PlayerID].UCID != -50 || (time(NULL)-Undertaker[rnd].Time) > 30)
			{
				Delete3DTextLabel(Undertaker[rnd].Text3D);
				Undertaker[rnd].Occupied=false;
				
				for(int i=rnd; i < 6; i++)
				{
					if(Undertaker[i+1].Occupied == true)
					{
						Undertaker[i].Occupied=true;
						Undertaker[i+1].Occupied=false;
						Undertaker[i].PlayerID=Undertaker[i+1].PlayerID;
						Undertaker[i].Text3D=Undertaker[i+1].Text3D;
						Undertaker[i].Time=Undertaker[i+1].Time;
					}
					else break;
				}
				
				MapData.ZombieSpawn--;
				rnd=rand()%MapData.ZombieSpawn;
			}
			else
			{
				GameTextForPlayer(Undertaker[rnd].PlayerID, "~n~~n~~n~~y~+2", 1000, 5);
				SetPlayerScore(Undertaker[rnd].PlayerID, GetPlayerScore(Undertaker[rnd].PlayerID)+2);	
			}
		}
		
		x=*((&MapData.ZombieX1)+rnd*4)+RandomRange(3);
		y=*((&MapData.ZombieY1)+rnd*4)+RandomRange(3);
		z=*((&MapData.ZombieZ1)+rnd*4);
		a=*((&MapData.ZombieA1)+rnd*4);
	
		SetPlayerPos(playerid, x, y, z);
		SetPlayerFacingAngle(playerid, a);
		SetPlayerInterior(playerid, MapData.Interior);
	
		if(za[playerid].AdminDuty == true)
		{
			ResetPlayerWeapons(playerid);
			za[playerid].UCID=INVALID_CLASS_ID;
			za[playerid].ClassDesc=NoClass;
			SetPlayerSkin(playerid, 217);
			SetPlayerHealth(playerid, 700.0);
			
			SendClientMessage(playerid, 0xFFFF00AA, ">> You are on {FF0000}adminduty{FFFF00}!");
		}
		else
		{
			SetPlayerColor(playerid, 0x99CC00AA);
			EnablePlayerCameraTarget(playerid, 1);
			GivePlayerWeapon(playerid, 9, 1);

			za[playerid].ClassDesc=(char *)&(zombieclasses[za[playerid].Zclass].Name[0]);
			za[playerid].UCID=zombieclasses[za[playerid].Zclass].UniqueID;
			za[playerid].CDP=za[playerid].Zclass;
		
			if(BossZombie == true && BZID == playerid)
			{
		    	SetPlayerHealth(playerid, 400.0);
		    	SetPlayerColor(playerid, 0xFF9900AA);
		    	SendClientMessage(BZID, -1, ">> You are {FF9900}BOSS {0000FF}zombie. {ADD8E6}You have increased health, go and kill all humans.");
				PlayerPlaySound(BZID, 1056);
				GameTextForPlayer(BZID, "~g~You are ~n~~y~boss zombie!", 5000, 5);
			}
			else SetPlayerHealth(playerid, zombieclasses[za[playerid].Zclass].health);
				
			SetPlayerArmour(playerid, zombieclasses[za[playerid].Zclass].armour);
	
			if(zombieclasses[za[playerid].Zclass].FightStyle)SetPlayerFightingStyle(playerid, zombieclasses[za[playerid].Zclass].FightStyle);
			else SetPlayerFightingStyle(playerid, 15);
				
			for(int i=0; i < 5; i++)
				if(zombieclasses[za[playerid].Zclass].Weapon[i])GivePlayerWeapon(playerid, zombieclasses[za[playerid].Zclass].Weapon[i], zombieclasses[za[playerid].Zclass].Ammo[i]);
		
			if(zombieclasses[za[playerid].Zclass].Skins == 1)
				SetPlayerSkin(playerid, zombieclasses[za[playerid].Zclass].Skin[0]);	
			else
				SetPlayerSkin(playerid, zombieclasses[za[playerid].Zclass].Skin[rand()%(zombieclasses[za[playerid].Zclass].Skins)]);	

			//Camoflauge Zombie.
			if(za[playerid].UCID == -23)
			{
				SetPlayerColor(playerid, 0x99CC0000);
			}
			
			//Slender Zombie's Attachments;
			if(za[playerid].UCID == -29)
			{
				SetPlayerAttachedObject(playerid, 9, 19036, 2,0.082,0.052,0.0,0.0,85.29,93.2,1.0,1.0,1.0,0,0);
			}
			//Bee Zombie's Attachments;
			else if(za[playerid].UCID == -51)
			{
				SetPlayerAttachedObject(playerid, 8, 18978, 2, 0.079999,0.003999,0.0,0.0,85.800003,92.799995,1.0,1.0,1.0, 0xFFFFBB1C, 0);
				SetPlayerAttachedObject(playerid, 9, 1092, 2, -0.584,-0.038,-0.147,-91.199935,-13.200003,77.900009,0.18,0.344,0.250999, 0xFFFFBB1C, 0);
			}
			//Helmet
			if(temp[playerid].Helmet == true)
			{
		 		SetPlayerAttachedObject(playerid,9,19103,2,0.1949,0.0099,0.00,0.0,0.0,0.0,1.2989,1.116,1.1799,0,0);
			}
		
			char str[144];
			snprintf(str, sizeof(str), ">> You have spawned as a %s, use /class to change this", zombieclasses[za[playerid].Zclass].Name);
			SendClientMessage(playerid, 0xFFFF00AA, str);	
		}
	}//else

	if(za[playerid].vSkin)
	{
	    SetPlayerSkin(playerid, za[playerid].vSkin);
	}

	if(temp[playerid].rein && za[playerid].UCID == -19 && temp[playerid].reinPos[0] != 0.0)
	{
		SetPlayerPos(playerid, temp[playerid].reinPos[0], temp[playerid].reinPos[1], temp[playerid].reinPos[2]);
	}

	return;
}


