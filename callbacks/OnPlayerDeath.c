#include "GameVars.h"

void OnPlayerDeath(int playerid, int killerid, int reason)
{
	za[playerid].Fakekill++;
	if(za[playerid].Fakekill > 1)
	{
		return;
	}

	if(killerid != INVALID_PLAYER_ID)
	{
		if(playerid == killerid){Kick(playerid);return;}
		if(killerid > (AMOUNT-1) || killerid < 0){Kick(playerid);return;}

		if(za[playerid].FakekillSpawn == false)
		{
			Kick(playerid);
			return;
		}
		za[playerid].FakekillSpawn=false;
		
		//Anti-Fake Kill;
		if(temp[killerid].Assist[playerid] == 0.0 && za[playerid].Duel == false && za[playerid].InEvent == false)
		{
			return;
		}
		
		DHS[killerid].Kills++;

	    if(za[killerid].Clan > 0)
	    {
	    	Gang[za[killerid].Clan].Score+=+1;
	    }
		
		if(za[killerid].Team == false)
		{
			DHS[killerid].HumanKills++;
		}
		else
		{
			DHS[killerid].ZombieKills++;
		}
		
		//Human Rogue
		if(za[killerid].UCID == 26 && temp[killerid].HumanRogue == true)
		{
			if(temp[killerid].Infection == false)SetPlayerColor(killerid, 0x00CCCCAA);
			else SetPlayerColor(killerid, 0xFF8080AA);
		}
		//Zombie Rogue
		else if(za[killerid].UCID == -8 && temp[killerid].ZombieRogue == true)SetPlayerColor(killerid, 0x99CC00AA);
	
		//Lasthuman killed
	    if(lasthuman == true && za[killerid].Team == true && za[playerid].Team == false)
	    {
	    	char str[128];
	        snprintf(str, sizeof(str), ">>> %s has killed the last human %s. He gains additional 25exp", za[killerid].Name, za[playerid].Name);
		    SendClientMessageToAll(COLOR_GRAYWHITE, str);
			SetPlayerScore(killerid, GetPlayerScore(killerid)+25);
		    lasthuman=false;
	    }
	 	
		//Anti-Spawn Kill
		if(za[killerid].Team == false)
		{
			if(IsPlayerInRangeOfPoint(playerid, 5.0, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1)||
			IsPlayerInRangeOfPoint(playerid, 5.0, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2)||
			IsPlayerInRangeOfPoint(playerid, 5.0, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3)||
			(MapData.ZombieSpawn > 3 && Undertaker[3].Occupied == false && IsPlayerInRangeOfPoint(playerid, 5.0, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4))||
			(MapData.ZombieSpawn > 4 && Undertaker[4].Occupied == false && IsPlayerInRangeOfPoint(playerid, 5.0, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5)))
			{
				if(temp[playerid].Poison == 0 && temp[playerid].Burned == false)
				{
					SetPlayerHealth(killerid, 0.0);
					char str[128];
					snprintf(str, sizeof(str), ">> %s has been killed for spawn killing..", za[killerid].Name);
					SendClientMessageToAll(COLOR_ANTIHACK, str);
					
					za[killerid].Killstreaks=-1;
			
					int exp=GetPlayerScore(killerid);
					if(exp > 10000)
					{
						SendClientMessage(killerid, 0xFF0000, ">> -150 EXP!");
						SetPlayerScore(killerid, GetPlayerScore(killerid)-150);
					}
					else if(exp > 1000)
					{
						SendClientMessage(killerid, 0xFF0000, ">> -80 EXP!");
						SetPlayerScore(killerid, GetPlayerScore(killerid)-80);					
					}
				}
			}
		}
	}

	//Boss Zombie;
	if(BossZombie == true && BZID == playerid && za[playerid].Duel == false)
    {
    	BossZombie=false;
    	
    	if(killerid != INVALID_PLAYER_ID)
		{
	    	char str[128];
	    	snprintf(str, sizeof(str), ">>> %s has killed the bosszombie %s and get 40 EXP!", za[killerid].Name, za[playerid].Name);
	    	SendClientMessageToAll(0x33AA33AA, str);	
	    	SetPlayerScore(killerid, GetPlayerScore(killerid)+40);
    	}
	}

	za[playerid].Team=true;
	za[playerid].IsDead=true;
	SendDeathMessage(killerid, playerid, reason);

	if(killerid != INVALID_PLAYER_ID)
	{
		//Human Rogue
		if(za[killerid].UCID == 26 && temp[killerid].HumanRogue == true)
		{
			SetPlayerColor(killerid, 0x99CC00AA);
		}
		//Zombie Rogue
		else if(za[killerid].UCID == -8 && temp[killerid].ZombieRogue == true)
		{
			SetPlayerColor(killerid, 0x00B0B0AA);
		}
		//Joker
		if(za[playerid].UCID == -41)
		{
			if(GetPlayerScore(killerid) > 800)
			{
				char str[128];
			    snprintf(str, sizeof(str), ">>> %s the joker has stolen %d exp from %s", za[playerid].Name, xpmult*20, za[killerid].Name);
			    SendClientMessageToAll(0x33AA33AA,str);
				SetPlayerScore(killerid, GetPlayerScore(killerid)-20*xpmult);
			    SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			}
		}
		
		if(za[killerid].Duel == false)
		{
			char str[128];	
			if(za[killerid].VIP == 3)//Gold VIP: x2
			{
				snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*20);
				GameTextForPlayer(killerid, str, 1000, 5);
	
				SetPlayerScore(killerid, GetPlayerScore(killerid)+20*xpmult);
			}
			else if(za[killerid].VIP == 2)//Silver VIP: x1.5
			{
				snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*15);
				GameTextForPlayer(killerid, str, 1000, 5);
					
				SetPlayerScore(killerid, GetPlayerScore(killerid)+15*xpmult);
			}
			else//Usual Player
			{
				if(za[killerid].UCID == 45)//Businessman Human;
				{
					snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*15);
					GameTextForPlayer(killerid, str, 1000, 5);
					
					SetPlayerScore(killerid, GetPlayerScore(killerid)+15*xpmult);
				}
				else if(za[killerid].UCID == -27)//Businessman Zombie;
				{	
					snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*20+10);
					GameTextForPlayer(killerid, str, 1000, 5);
					
					SetPlayerScore(killerid, GetPlayerScore(killerid)+(20*xpmult+10));
				}
				else if(za[killerid].Team == true)//Zombie gets more;
				{
					snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*20);
					GameTextForPlayer(killerid, str, 1000, 5);
					
					SetPlayerScore(killerid, GetPlayerScore(killerid)+20*xpmult);
				}
				else//Just Human
				{
					snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*10);
					GameTextForPlayer(killerid, str, 1000, 5);
					
					SetPlayerScore(killerid, GetPlayerScore(killerid)+10*xpmult);
				}
			}
			
			za[killerid].Kills++;
			za[killerid].Killstreaks++;
			
			if(za[killerid].Killstreaks && ((za[killerid].Killstreaks)%5) == 0)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s has achieved a %d killstreak! {FFFFFF}(+%d XP)", za[killerid].Name, za[killerid].Killstreaks, za[killerid].Killstreaks);
				SendClientMessageToAll(0x33AA33AA, str);
					
				SetPlayerScore(killerid, GetPlayerScore(killerid)+za[killerid].Killstreaks);
				PlayerPlaySound(killerid, 1056);
			}
			if(za[playerid].Killstreaks > 20)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s earned %d exp for killing %s on %d kills killstreak!", za[killerid].Name, za[playerid].Killstreaks, za[playerid].Name, za[playerid].Killstreaks);
				SendClientMessageToAll(0x33AA33AA, str);
	
				SetPlayerScore(killerid, GetPlayerScore(killerid)+za[playerid].Killstreaks);
			}
		}//if duel is false
		
		//Fly Zombie;
		if(za[killerid].UCID == -49 && reason == 50 && temp[killerid].Fly)
		{
			temp[killerid].Pause=time(NULL)+80;
			SendClientMessage(killerid, -1, ">> Your wings were damaged and destroyed! It will take more time to recovery!");
		
			float x, y, z, fa;
			GetVehiclePos(temp[killerid].Fly, &x, &y, &z);
			GetVehicleZAngle(temp[killerid].Fly, &fa);
			SetPlayerPos(killerid, x, y, z);
			SetPlayerFacingAngle(killerid, fa);

			for(int i=0; i < 8; i++)
			{
				DestroyObject(temp[killerid].FlyObjects[i]);
			}
			DestroyVehicle(temp[killerid].Fly);
			temp[killerid].Fly=0;
		}
		
		//Assist Killing
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true)
			{
				if(temp[i].Assist[playerid] > 35.0 && i != killerid)
				{
					char str[32];
					snprintf(str, sizeof(str), "~n~Assist:~n~+%d", xpmult*4);
					GameTextForPlayer(i, str, 1000, 5);
					SetPlayerScore(i, GetPlayerScore(i)+4*xpmult);
					
					temp[i].Assist[playerid]=0;
				}
				else
				{
					temp[i].Assist[playerid]=0;
				}
			}
		}//for
	}

	if(za[playerid].Killstreaks > za[playerid].personalKS)
	{
		char str[128];
		snprintf(str, sizeof(str), ">> %s has achieved a new personal highest killstreak (%d kills in a row - old: %d kills)", za[playerid].Name, za[playerid].Killstreaks, za[playerid].personalKS);
		SendClientMessageToAll(0xabcdef66, str);
		za[playerid].personalKS=za[playerid].Killstreaks;
	}
	za[playerid].Killstreaks=0;

	float x, y, z;
	GetPlayerPos(playerid, &x, &y, &z);
	
	//Boomer Zombie
	if(za[playerid].UCID == -5)
	{
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].UCID == 27 && i != playerid && IsPlayerInRangeOfPoint(i, 10.0, x, y, z))return;
		}
		
		CreateExplosion(x, y, z, 12, 10);
		
		int Numbers=0;
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 7.0, x, y, z))
			{
				if(Infect(i, playerid) == 1)Numbers++;
			}
		}
			
		if(Numbers > 3)
		{
			char str[128];
			snprintf(str, sizeof(str), ">> %s the boomer has blown up and infected {FFFFFF}WHOPPING {FF0000}%d people!", za[playerid].Name, Numbers);
			SendClientMessageToAll(0x009900aa, str);
			GameTextForPlayer(playerid, "~n~~y~+10", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+10);
		}
		else if(Numbers != 0)
		{
			char str[128];
			snprintf(str, sizeof(str), ">> %s the boomer has blown up and infected {FFFFFF}%d people!", za[playerid].Name, Numbers);
			SendClientMessageToAll(0x009900aa, str);
			GameTextForPlayer(playerid, "~n~~y~+5", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
		}
	}
	//Demon Zombie
	else if(za[playerid].UCID == -21)
	{
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].UCID == 27 && i != playerid && IsPlayerInRangeOfPoint(i, 10.0, x, y, z))return;
		}
		
		CreateExplosion(x, y, z, 12, 10);
		
		int Numbers=0;
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && za[i].Team == false && temp[i].AntiBurning == false && za[i].UCID != 9 && za[i].UCID != 41 && IsPlayerInRangeOfPoint(i, 7.0, x, y, z))
			{
				Numbers++;
				temp[i].Burned=true;
				SetPlayerAttachedObject(i, 9, 18689, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0);
			}
		}
		
		if(Numbers > 3)
		{
			char str[128];
			snprintf(str, sizeof(str), ">> %s the demon has blown up and burnt {FFFFFF}WHOPPING {FF0000}%d people!", za[playerid].Name, Numbers);
			SendClientMessageToAll(0x009900aa, str);
			GameTextForPlayer(playerid, "~n~~y~+10", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+10);
		}
		else if(Numbers != 0)
		{
			char str[128];
			snprintf(str, sizeof(str), ">> %s the demon has blown up and burnt {FFFFFF}%d people!", za[playerid].Name, Numbers);
			SendClientMessageToAll(0x009900aa, str);
			GameTextForPlayer(playerid, "~n~~y~+5", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
		}
	}
	//Flesher Zombie
	else if(za[playerid].UCID == -24)
	{
		temp[playerid].Flesher=CreatePickup(2804, 8, x, y, z, -1);
	}
	//Reincarnate Zombie
	else if(za[playerid].UCID == -19)
	{
		GetPlayerPos(playerid, &(temp[playerid].reinPos[0]), &(temp[playerid].reinPos[1]), &(temp[playerid].reinPos[2]));
	}
	//Cockroach Zombie
	else if(za[playerid].UCID == -37)
	{
		temp[playerid].Cockroach=false;
	}
	//Gun Jammer Zombie
	else if(za[playerid].UCID == -46)
	{
		temp[playerid].JammerZombie=0;
	}
	
	//Tornado Zombie
	int IsTornado=0;
	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].UCID == -17 &&	IsPlayerInRangeOfPoint(i, 23.0, x, y, z))
		{
			GameTextForPlayer(i, "~n~~n~~n~~y~+2", 1000, 5);
			SetPlayerScore(i, GetPlayerScore(i)+2);
			IsTornado=1;
			break;
		}			
	}
	if(IsTornado == 0)
	{
		CreatePickup(2804, 8, x, y, z, -1);
	}
	
	//Fly Zombie
	if(temp[playerid].Fly)
	{
		for(int i=0; i < 8; i++)
		{
			DestroyObject(temp[playerid].FlyObjects[i]);
		}
		DestroyVehicle(temp[playerid].Fly);
		temp[playerid].Fly=0;
	}
	
	
	if(za[playerid].Duel == true && za[playerid].DuelFalsePositive == true)
	{
		ResetPlayerWeapons(playerid);
		ResetPlayerWeapons(za[playerid].DuelVictimID);

		za[playerid].DuelLoses++;
		za[za[playerid].DuelVictimID].DuelWins++;

		char str[128];
		snprintf(str, sizeof(str), ">> %s has won the duel against %s!", za[za[playerid].DuelVictimID].Name, za[playerid].Name);
		SendClientMessageToAll(0xFFE4B5AA, str);

		SpawnPlayer(za[playerid].DuelVictimID);

		za[playerid].DuelFalsePositive=false;
		za[za[playerid].DuelInviteID].DuelFalsePositive=false;
		SetTimerEx("OnPlayerDuel", 3000, za[playerid].DuelVictimID);
		SetTimerEx("OnPlayerDuel", 3000, playerid);
	}
	return;
}
