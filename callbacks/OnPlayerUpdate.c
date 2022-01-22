#include "GameVars.h"

#define KEY_UP					(-128)
#define KEY_DOWN				(128)
#define KEY_LEFT				(-128)
#define KEY_RIGHT				(128)
#define KEY_ANALOG_UP			(2048)
#define KEY_ANALOG_DOWN			(4096)
#define KEY_ANALOG_LEFT			(8192)
#define KEY_ANALOG_RIGHT		(16384)
#define KEY_SPRINT				(8)
#define KEY_JUMP				(32)
#define KEY_FIRE				(4)

static int OnSpecUpdate[AMOUNT];

int OnPlayerUpdate(int playerid)
{
	if(za[playerid].AFKhuman > 2)
	{
		za[playerid].Killstreaks=0;	
		SpawnPlayer(playerid);
	}
	
	if(za[playerid].AFKzombie > 1)SetPlayerVirtualWorld(playerid, 0);

	za[playerid].AFKhuman=0;
	za[playerid].AFKzombie=0;
	za[playerid].uAFK=GetCount();

	if(za[playerid].Spec)
	{
		OnSpecUpdate[playerid]+=1;
		if(OnSpecUpdate[playerid] > 8)
		{
			if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
			{
				float x, y, z;
		        GetPlayerPos(za[playerid].Spec-1, &x, &y, &z);
		        if(!IsPlayerInRangeOfPoint(playerid, 300.0, x, y, z))
		        {
					PlayerSpectatePlayer(playerid, za[playerid].Spec-1, 1);
				}
				
				OnSpecUpdate[playerid]=0;
				
				char str[128]; float health;
				GetPlayerHealth(za[playerid].Spec-1, &health);
				
				snprintf(str, sizeof(str), "Health: %.2f", health);
				PlayerTextDrawSetString(playerid, za[playerid].TDs.t_Health, str);
					
				snprintf(str, sizeof(str), "Ping: %d", GetPlayerPing(za[playerid].Spec-1));
				PlayerTextDrawSetString(playerid, za[playerid].TDs.t_Ping, str);

				if(za[za[playerid].Spec-1].Team == false)
					snprintf(str, sizeof(str), "Team: ~g~Human~n~~p~Class: ~y~%s", za[za[playerid].Spec-1].ClassDesc);
				else
					snprintf(str, sizeof(str), "Team: ~r~Zombie~n~~p~Class: ~y~%s", za[za[playerid].Spec-1].ClassDesc);		

				PlayerTextDrawSetString(playerid, za[playerid].TDs.t_Team, str);	
				
				int weapon[3][2]; char name[3][128];
		        GetPlayerWeaponData(za[playerid].Spec-1, 2, &weapon[0][0], &weapon[0][1]);//pistols
		        GetPlayerWeaponData(za[playerid].Spec-1, 3, &weapon[1][0], &weapon[1][1]);//shotgun
		        GetPlayerWeaponData(za[playerid].Spec-1, 5, &weapon[2][0], &weapon[2][1]);//assault rifle
				if(weapon[0][0])GetWeaponName(weapon[0][0], name[0], sizeof(name[0]));
				if(weapon[1][0])GetWeaponName(weapon[1][0], name[1], sizeof(name[1]));
				if(weapon[2][0])GetWeaponName(weapon[2][0], name[2], sizeof(name[2]));
					
				snprintf(str, sizeof(str), "Pistol:~w~ %s (%d)~n~~r~Shotgun:~w~ %s (%d)~n~~g~Assault Rifle:~w~ %s (%d)", 
				(weapon[0][0]) ? (name[0]) : ("NaN"), weapon[0][1], (weapon[1][0]) ? (name[1]) : ("NaN"), weapon[1][1], (weapon[2][0]) ? (name[2]) : ("NaN"), weapon[2][1]);
				PlayerTextDrawSetString(playerid, za[playerid].TDs.t_Weapons, str);
			}
			else
			{
				PlayerTextDrawHide(playerid, za[playerid].TDs.t_Health);
				PlayerTextDrawHide(playerid, za[playerid].TDs.t_Ping);
				PlayerTextDrawHide(playerid, za[playerid].TDs.t_Team);
				PlayerTextDrawHide(playerid, za[playerid].TDs.t_Weapons);	
				za[playerid].Spec=0;
			}
		}
	}
	else
	{
		//Cockroach Zombie
		if(za[playerid].UCID == -37 && temp[playerid].Cockroach == true)
		{
			int Keys, UpDown, LeftRight;
			GetPlayerKeys(playerid, &Keys, &UpDown, &LeftRight);
	
			float x, y, z, fa;
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &fa);
	
			if(UpDown == KEY_UP)
			{
				x+=(0.5*sin(-(fa/180)*3.14159265));
				y+=(0.5*cos(-(fa/180)*3.14159265));
	
				SetPlayerPos(playerid, x, y, z);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
	
			if(UpDown == KEY_DOWN)
			{
				x-=(0.5*sin(-(fa/180)*3.14159265));
				y-=(0.5*cos(-(fa/180)*3.14159265));
	
				SetPlayerPos(playerid, x, y, z);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
	
			if(LeftRight == KEY_RIGHT)
			{
				fa+=-90;
				x+=sin(-(fa/180)*3.14159265)*0.5;
				y+=cos(-(fa/180)*3.14159265)*0.5;
	
				SetPlayerPos(playerid, x, y, z);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
	
			if(LeftRight == KEY_LEFT)
			{
				fa+=90;
				x+=sin(-(fa/180)*3.14159265)*0.5;
				y+=cos(-(fa/180)*3.14159265)*0.5;
	
				SetPlayerPos(playerid, x, y, z);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
	
			if(Keys & KEY_ANALOG_RIGHT)
			{
				SetPlayerFacingAngle(playerid, fa-10.0);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
	
			if(Keys & KEY_ANALOG_LEFT)
			{
				SetPlayerFacingAngle(playerid, fa+10.0);
				SetCameraBehindPlayer(playerid);
				return 1;
			}
		}
		//Leg Breaker or Slow Zombie
		else if(temp[playerid].Leg || temp[playerid].Slow)
		{
			int Keys, ud, lr;
			GetPlayerKeys(playerid, &Keys, &ud, &lr);
	
			if(Keys & KEY_JUMP)
			{
				ApplyAnimation(playerid, "PED", "getup_front", 4.0, 0, 0, 1, 0, 0, 0);
			}
	
			if((Keys & KEY_SPRINT) && (ud < 0 || lr < 0))
			{
				ApplyAnimation(playerid, "PED", "WOMAN_RUNFATOLD", 4.0, 0, 0, 0, 0, 0, 0);
			}
		}
		//Juggernaut	
		else if(za[playerid].UCID == -47)
		{
			int Keys, ud, lr;
			GetPlayerKeys(playerid, &Keys, &ud, &lr);
	
			if(Keys & KEY_JUMP)
			{
				ClearAnimations(playerid, 0);
			}
		}
		
		if(za[playerid].UCID == 46)
		{
			int weap=GetPlayerWeapon(playerid);
			if(weap == 0)
			{
				if(temp[playerid].TazerCurrentWeapon != 0)
				{
					RemovePlayerAttachedObject(playerid, 9);
					SetPlayerAttachedObject(playerid, 9, 18642, 6, 0.06, 0.01, 0.08, 180.0, 0.0, 0.0, 1.4, 1.4, 1.4, 0, 0);
					temp[playerid].TazerCurrentWeapon=0;
				}
			}
			else if(weap == 24 && temp[playerid].TazerCurrentWeapon != 24)
			{
				if(temp[playerid].Tazer == true)
				{
					RemovePlayerAttachedObject(playerid, 9);
					SetPlayerAttachedObject(playerid, 9, 18642, 6, 0.302, 0.015, 0.07, 73.300071, -97.500663, 93.699928, 1.241999, 2.866999, 1.08, 0, 0);
				}
				else
				{
					RemovePlayerAttachedObject(playerid, 9);
				}
				temp[playerid].TazerCurrentWeapon=24;
			}			
		}
		
		if(temp[playerid].IsPlayerJammed == true)
		{
			if(temp[playerid].IsPlayerJammedTime > time(NULL))
			{
				SetPlayerArmedWeapon(playerid, 0);
			}
			else
			{
				temp[playerid].IsPlayerJammed=false;
			}
		}

		//Ability
		if(temp[playerid].AbilityNotReady == true && ((++temp[playerid].AbilityIdleTick)%3) == 0 && temp[playerid].Pause <= time(NULL))
		{
			temp[playerid].AbilityNotReady=false;
			
			TextDrawShowForPlayer(playerid, t_Ability);
			if(!((za[playerid].Settings)&0x10))PlayerPlaySound(playerid, 6401);
			SetTimerEx("HideAbility", 800, playerid);
		}
	}
	return 1;
}
