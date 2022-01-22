#include "GameVars.h"

void OnPlayerPickUpPickup(int playerid, int pickupid)
{
	DestroyPickup(pickupid);
	if(za[playerid].Duel == true || za[playerid].AdminDuty == true)return;

	if(za[playerid].Team == true)
	{
		//No: BossZombie, Freezer; Tank; Juggernaut, Boomer, Kamikadze, Demon;
		if((playerid == BZID && BossZombie == true) || za[playerid].UCID == -15 || za[playerid].UCID == -47 || za[playerid].UCID == -5 || za[playerid].UCID == -11 || za[playerid].UCID == -12 || za[playerid].UCID == -21)return;
		SetPlayerHealth(playerid, 99.0);
	}
	else
	{
		for(int i=0; i < AMOUNT; i++)
		{
			if(temp[i].Flesher == pickupid && temp[i].Flesher != 0 && temp[playerid].Infection == false)
			{
				Infect(playerid, i);
				temp[i].Flesher=0;
				return;
			}
		}
	
		
		if(temp[playerid].Infection && ((rand()%3) == 0))
		{
			GameTextForPlayer(playerid, "~g~ANTEDOTE TAKEN!", 1000, 5);
			temp[playerid].Infection=false;
			temp[playerid].Slow=false;
			temp[playerid].TransferInfection=0;
			SetPlayerDrunkLevel(playerid, 0);
			SetPlayerColor(playerid, 0x00B0B0AA);
			
			TextDrawHideForPlayer(playerid, t_Red);
		}
		else
		{
			//Scavenger Human;
			if(za[playerid].UCID == 48)
			{
				if((rand()%10) < 8)
				{
					int weapon, ammo;
					GetPlayerWeaponData(playerid, 2, &weapon, &ammo);
					if(ammo)SetPlayerAmmo(playerid, 24, ammo+30);
					GetPlayerWeaponData(playerid, 3, &weapon, &ammo);
					if(ammo)SetPlayerAmmo(playerid, 25, ammo+20);
					GetPlayerWeaponData(playerid, 5, &weapon, &ammo);
					if(ammo)SetPlayerAmmo(playerid, 31, ammo+80);

					SendClientMessage(playerid, COLOR_GRAYWHITE, ">> You have found some ammo!");
				}

				if((rand()%10) < 8)
				{
					float armour;
					GetPlayerArmour(playerid, &armour);
					
					if(armour <= 40.0)
					{
						SetPlayerArmour(playerid, armour+10.0);
						SendClientMessage(playerid, COLOR_GRAYWHITE, ">> You have found some armour!");
					}
					else if(armour < 50.0)
					{
						SetPlayerArmour(playerid, 50.0);
						SendClientMessage(playerid, COLOR_GRAYWHITE, ">> You have found some armour!");
					}
				}
				
			}
			else if((rand()%3) == 0)
			{
				int weapon, ammo;
				GetPlayerWeaponData(playerid, 2, &weapon, &ammo);
				if(weapon == 23 && ammo)SetPlayerAmmo(playerid, 23, ammo+50);
				if(weapon == 24 && ammo)SetPlayerAmmo(playerid, 24, ammo+20);
				GetPlayerWeaponData(playerid, 3, &weapon, &ammo);
				if(weapon == 25 && ammo)SetPlayerAmmo(playerid, 25, ammo+40);
				GetPlayerWeaponData(playerid, 6, &weapon, &ammo);
				if(weapon == 33 && ammo)SetPlayerAmmo(playerid, 33, ammo+20);
				if(weapon == 34 && ammo)SetPlayerAmmo(playerid, 34, ammo+20);
				SendClientMessage(playerid, COLOR_GRAYWHITE, "|: You picked up meat and you've found some ammo");
			}
		}
		
	}
	return;
}

