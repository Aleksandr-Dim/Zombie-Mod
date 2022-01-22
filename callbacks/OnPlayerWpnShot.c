#include "GameVars.h"

#define BULLET_HIT_TYPE_NONE			0
#define BULLET_HIT_TYPE_PLAYER			1
#define BULLET_HIT_TYPE_VEHICLE			2
#define BULLET_HIT_TYPE_OBJECT			3
#define BULLET_HIT_TYPE_PLAYER_OBJECT	4

int OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ)
{	
//Anti-Crash: 0.3.7
if(weaponid < 22 || weaponid > 38 || hittype > 4 || hittype < 0)
{
	return 0;
}

//Anti-Crash: 0.3.7-R2;
if(hittype == BULLET_HIT_TYPE_PLAYER)
{
    if(fX > 20.0 || fY > 20.0 || fZ > 20.0 || fX < -20.0 || fY < -20.0 || fZ < -20.0) 
    {
        return 0;
    }
}
//Anti-Crash: 0.3.7;
float x, y, z;
GetPlayerPos(playerid, &x, &y, &z);
if(x > 10000.0 || x < -10000.0 || y > 10000.0 || y < -10000.0 || z > 10000.0 || z < -10000.0)
{
	return 0;
}

//if player is banned already, to prevent damage;
if(za[playerid].Kicked == true)return 0;
//Anti-Weapon Hack Entry;
if(za[playerid].AFKhuman == 0 && za[playerid].AFKzombie == 0 && za[playerid].Duel == false && za[playerid].InEvent == false)
{
	int state=GetPlayerState(playerid);
	if(state == PLAYER_STATE_ONFOOT || state == PLAYER_STATE_PASSENGER)
	{
		//Anti-Weapon Hack #3: For Humans;
		if(za[playerid].UCID == 1 && weaponid != 23 && weaponid != 25)
		{
			za[playerid].Kicked=true;
	
			char str[128], WeaponName[40];
			GetWeaponName(weaponid, WeaponName, 38);
			snprintf(str, sizeof(str), "Weapon Hack(%s) #3", WeaponName);
			BanPlayer(playerid, "Anti-Hack", str, 0);
			return 0;
		}
		//Anti-Weapon Hack #2: For Zombies;
		if(za[playerid].Team == true && za[playerid].UCID < 0 && za[playerid].UCID > -1000)
		{//Team-Class Sync;
			//Team-Color Sync;(Useless?)
			int color=GetPlayerColor(playerid);
			if(color == 0x99CC00AA || color == 0x99CC0000 || color == 0xFF9900AA)
			{
				//spitter zombie, zombie bee, adv. seeker;
				if(weaponid == 23 && (za[playerid].UCID == -44 || za[playerid].UCID == -51 || za[playerid].UCID == -53))
				{
				}
				else
				{
					za[playerid].Kicked=true;
			
					char str[128], WeaponName[40];
					GetWeaponName(weaponid, WeaponName, 38);
					snprintf(str, sizeof(str), "Weapon Hack(%s) #2", WeaponName);
				    BanPlayer(playerid, "Anti-Hack", str, 0);
					return 0;			
				}
			}
		}
	}
	else
	{
		return 0;
	}
}

//Anti-Ammo Cheat;
if(weaponid == 23 || weaponid == 24)//2
{
	Haxor[playerid].Ammo[0]--;
}
else if(weaponid == 25)//3
{
	Haxor[playerid].Ammo[1]--;
}
else if(weaponid == 30 || weaponid == 31)//5
{
	Haxor[playerid].Ammo[2]--;
}

//Anti-Rapid Fire;
if(Haxor[playerid].AntiRapidFire < time(NULL))
{
	Haxor[playerid].AntiRapidFire=time(NULL);
	Haxor[playerid].AntiRapidFireShots=0;
}

if(weaponid == 25 || weaponid == 33 || weaponid == 34)
{
	Haxor[playerid].AntiRapidFireShots++;
	if(Haxor[playerid].AntiRapidFireShots > 1)return 0;
}
else if(weaponid == 23 || weaponid == 24)
{
	Haxor[playerid].AntiRapidFireShots++;
	if(Haxor[playerid].AntiRapidFireShots > 2)return 0;	
}
else if(weaponid == 30 || weaponid == 31)	
{
	Haxor[playerid].AntiRapidFireShots++;
	if(Haxor[playerid].AntiRapidFireShots > 9)return 0;
}

if(hittype == BULLET_HIT_TYPE_OBJECT)
{
	//Engineers
	if(za[playerid].Team == false && temp[playerid].Boxes > 0)
	{
		for(int k=0; k < temp[playerid].Boxes; k++)
		{
			if(temp[playerid].Box[k] == hitid)
			{
				temp[playerid].Box[k]=0;
				DestroyObject(hitid);
				Delete3DTextLabel(temp[playerid].BoxText[k]);
				break;
			}
		}
	}
	//Adv.Seeker
	else if(za[playerid].UCID == -53)
	{
		if(time(NULL) > temp[playerid].Pause)
		{
			temp[playerid].Pause=time(NULL)+40;
			temp[playerid].AbilityNotReady=true;

			float oX, oY, oZ, fa;
			GetObjectPos(hitid, &oX, &oY, &oZ);
			oX+=fX;
			oY+=fY;
			oZ+=fZ;
			GetPlayerFacingAngle(playerid, &fa);
			oX-=0.4*cos(((fa+90.0)/180)*3.14159265358979);
			oY-=0.4*sin(((fa+90.0)/180)*3.14159265358979);
			SetPlayerPos(playerid, oX, oY, oZ+0.5);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s advanced seeker has teleported to somewhere!", za[playerid].Name);
			SendClientMessageToAll(0x009900aa, str);
		}
	}
}
else if(hittype == BULLET_HIT_TYPE_NONE)
{
	//Adv.Seeker
	if(za[playerid].UCID == -53 && fX != 0.0)
	{
		if(time(NULL) > temp[playerid].Pause)
		{
			temp[playerid].Pause=time(NULL)+40;
			temp[playerid].AbilityNotReady=true;

			float oX=fX, oY=fY, fa;
			GetPlayerFacingAngle(playerid, &fa);
			oX-=0.4*cos(((fa+90.0)/180)*3.14159265358979);
			oY-=0.4*sin(((fa+90.0)/180)*3.14159265358979);
			SetPlayerPos(playerid, oX, oY, fZ+0.5);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s advanced seeker has teleported to somewhere!", za[playerid].Name);
			SendClientMessageToAll(0x009900aa, str);
		}	
	}
}
else if(hittype == BULLET_HIT_TYPE_PLAYER)
{
	if(hitid < 0 || hitid > (AMOUNT-1))
	{
		Kick(playerid);
		return 0;
	}

	//VIP knockback
	if(za[playerid].KnockBack == true && za[playerid].Duel == false)
	{		
		if(za[playerid].UCID != 21 && za[playerid].UCID != 41 && za[hitid].Team == true && za[hitid].AdminDuty == false)
		{			
			float x, y, z;
			GetPlayerVelocity(hitid, &x, &y, &z);
			if(x != 0.0 && y != 0.0)
			{				
				if(weaponid == 23 || weaponid == 24 || weaponid == 25 || weaponid == 29 || weaponid == 31)
		   		{
		   			if(GetPlayerScore(hitid) < 1500)return 1;
					float fOriginX, fOriginY, fOriginZ,
				    fHitPosX, fHitPosY, fHitPosZ, AA, BB, CC;
				    GetPlayerLastShotVectors(playerid, &fOriginX, &fOriginY, &fOriginZ, &fHitPosX, &fHitPosY, &fHitPosZ);
					AA=fHitPosX-fOriginX;
					BB=fHitPosY-fOriginY;
					CC=VectorSize(AA, BB, 0.0);
					SetPlayerVelocity(hitid, AA/CC*0.26, BB/CC*0.26, 0.1);
				}
			}
	    }
	}
	
	//Enforcer
	if(za[hitid].UCID == -35)
	{
		if(weaponid == 24 || weaponid == 29 || weaponid == 31)
		{
			GameTextForPlayer(playerid, "~y~Bullet~r~-~g~Proof", 1000, 5);
			return 0;
		}
	}
	
	//Adv.Scout Bug; Reason is Unknown, maybe samp's bug;
	if(za[playerid].UCID == 15)
	{
		if(za[hitid].Team == false && weaponid == 34)return 0;
	}
	//Curing Rifle
	else if(za[playerid].UCID == 23)
	{
		if(weaponid == 33 && temp[hitid].Infection)
		{
			GameTextForPlayer(hitid, "~g~ANTEDOTE TAKEN!", 1000, 5);
			temp[hitid].Infection=false;
			temp[hitid].Slow=false;
			temp[hitid].TransferInfection=0;
			SetPlayerDrunkLevel(hitid, 0);
			SetPlayerColor(hitid, 0x00B0B0AA);
			TextDrawHideForPlayer(hitid, t_Red);
		
			GameTextForPlayer(playerid, "~n~~n~~n~~y~+5~y~~n~~n~~n~", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			char str[128];
			snprintf(str, sizeof(str), ">> %s has been cured by medic %s (shot)", za[hitid].Name, za[playerid].Name);
			SendClientMessageToAll(0xADFF2FAA, str);
		}
	}
	//Spitter Zombie
	else if(za[playerid].UCID == -44)
	{
		if(za[hitid].Team == false)
		{
			if(time(NULL) > temp[playerid].Pause)
			{
				temp[playerid].Pause=time(NULL)+45;
				temp[playerid].AbilityNotReady=true;
				
				if(temp[hitid].Infection == false)
				{
					Infect(hitid, playerid);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	//Bee Zombie;
	else if(za[playerid].UCID == -51)
	{
		if(za[hitid].Team == false)
		{
			if(time(NULL) > temp[playerid].Pause)
			{
				temp[playerid].Pause=time(NULL)+45;
				temp[playerid].AbilityNotReady=true;
	
				ApplyAnimation(hitid, "PED", "RUN_CIVI", 4.1, 1, 1, 1, 1, 0, 1);
				GameTextForPlayer(hitid, "~y~~h~You are stung", 2000, 5);
				TextDrawShowForPlayer(hitid, t_Magenta);
				SetTimerEx("Stung", 1900, hitid);
				char str[128];
				snprintf(str, sizeof(str), ">> %s has been stung by %s!", za[hitid].Name, za[playerid].Name);
				SendClientMessageToAll(0x009900aa, str);
				GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
				SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			}
			else
			{
				return 0;	
			}
		}
	}
	//Adv. Seeker;
	else if(za[playerid].UCID == -53)
	{
		if(za[hitid].AdminDuty == false)
		{
			if(za[hitid].Team == false && temp[hitid].AntiSeeker == true)
			{
				GameTextForPlayer(playerid, "~g~Anti-Seeker", 800, 5);
				return 0;
			}
			
			if(time(NULL) > temp[playerid].Pause)
			{
				temp[playerid].Pause=time(NULL)+40;
				temp[playerid].AbilityNotReady=true;
	
				float x, y, z, fa;
				GetPlayerPos(hitid, &x, &y, &z);			
				GetPlayerFacingAngle(playerid, &fa);

				x-=1.3*cos(((fa+90.0)/180)*3.14159265358979);
				y-=1.3*sin(((fa+90.0)/180)*3.14159265358979);
				SetPlayerPos(playerid, x, y, z);
				
				if(za[hitid].Team == false)
				{
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
				}
				
				char str[128];
				snprintf(str, sizeof(str), ">> %s advanced seeker has teleported to %s!", za[playerid].Name, za[hitid].Name);
				SendClientMessageToAll(0x009900aa, str);
			}
			else
			{
				return 0;	
			}
		}
	}
}
return 1;	
}
