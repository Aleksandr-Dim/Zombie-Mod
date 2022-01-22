#include "GameVars.h"

int OnPlayerRequestClass(int playerid, int classid)
{
	SetPlayerPos(playerid, 743.8432,-555.6884,18.012);
	SetPlayerFacingAngle(playerid, 359.528);
	SetPlayerCameraPos(playerid, 743.9,-550.554,18.008);
	SetPlayerCameraLookAt(playerid, 743.8432,-555.6884,18.012);	
	return 1;
}

int OnPlayerRequestSpawn(int playerid)
{
	if(za[playerid].Log == false)
	{
		SendClientMessage(playerid, 0xC0C0C0AA, "You cannot spawn. Register/Login please.");
		return 0;
	}
	return 1;
}

void OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart)
{
	if(damagedid > (AMOUNT-1) || damagedid < 0){Kick(playerid);return;}

	if((weaponid == 0 || weaponid == 8 || weaponid == 9) && za[playerid].Team == true && za[damagedid].Team == false && za[damagedid].Log == true)
	{
		if(GetPlayerState(damagedid) == PLAYER_STATE_ONFOOT && IsPlayerPaused(damagedid) > 1000)
		{
			temp[damagedid].DamageAFK+=amount;
			
			if(temp[damagedid].DamageAFK > 60.0)
			{
				temp[playerid].Assist[damagedid]+=60.0;
				OnPlayerDeath(damagedid, playerid, weaponid);
				SpawnPlayer(damagedid);

				char str[128];
				snprintf(str, sizeof(str), ">> %s has been set to zombie team [Reason: AFK to Avoid Damage]", za[damagedid].Name);
				SendClientMessageToAll(COLOR_ANTIHACK, str);
			}
		}
	}
	return;
}

void OnPlayerStreamIn(int playerid, int forplayerid)
{
	if(za[playerid].UCID == 15 || za[playerid].UCID == -23)
	{
		ShowPlayerNameTagForPlayer(forplayerid, playerid, false);
	}
	if(za[forplayerid].UCID == 15 || za[forplayerid].UCID == -23)
	{
		ShowPlayerNameTagForPlayer(playerid, forplayerid, false);
	}

	if(za[playerid].AdminDuty == true)
	{
		if(za[forplayerid].Admin == 0)ShowPlayerNameTagForPlayer(forplayerid, playerid, false);
		else SetPlayerMarkerForPlayer(forplayerid, playerid, 0xFF0000FF);
    }
    if(za[forplayerid].AdminDuty == true)
	{
		if(za[playerid].Admin == 0)ShowPlayerNameTagForPlayer(playerid, forplayerid, false);
		else SetPlayerMarkerForPlayer(playerid, forplayerid, 0xFF0000FF);
    }
	return;
}

void OnVehicleStreamIn(int vehicleid, int forplayerid)
{
	if(za[forplayerid].Admin < 1)
    {
        SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, 1);
    }
	return;
}

void OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid)
{
	for(int i=0; i < AMOUNT; i++)
	{
	    if(playerid == (za[i].Spec-1))
	    {
	 		SetPlayerInterior(i, GetPlayerInterior(playerid));
	    }
	}
	
	return;
}

int OnPlayerCommandText(int playerid, char cmdtext[]);

void OnPlayerClickPlayer(int playerid, int clickedplayerid, int source)
{
	if(clickedplayerid < 0 && clickedplayerid > (AMOUNT-1))
	{
		Kick(playerid);
		return;
	}
	
	char str[128];
	if(za[playerid].Admin == 0)
	{
		snprintf(str, sizeof(str), "/stats %d", clickedplayerid);
		OnPlayerCommandText(playerid, str);
	}
	else
	{
		if(clickedplayerid != playerid)
		{
			snprintf(str, sizeof(str), "/getinfo %d", clickedplayerid);
			OnPlayerCommandText(playerid, str);
			za[playerid].CMD=0;
			snprintf(str, sizeof(str), "/spec %d", clickedplayerid);
			OnPlayerCommandText(playerid, str);
		}
		else
		{
			TogglePlayerSpectating(playerid, 0);
			za[playerid].Spec=0;
			
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Health);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Ping);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Team);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Weapons);	
		}
	}
	return;
}

void OnPlayerStateChange(int playerid, int newstate, int oldstate)
{
	if((newstate == PLAYER_STATE_PASSENGER || newstate == PLAYER_STATE_DRIVER) && za[playerid].Admin == 0 && za[playerid].UCID != -49)
	{
		ClearAnimations(playerid, 0);
	}
}

void OnPlayerEnterVehicle(int playerid, int vehicleid, int ispassenger)
{
	if(za[playerid].Admin == 0)
	{
		ClearAnimations(playerid, 0);
	}
}

void OnPlayerEnterCheckpoint(int playerid);
void OnPlayerEnterRaceCheckpoint(int playerid)
{
	OnPlayerEnterCheckpoint(playerid);
}

void OnPlayerEnterCheckpoint(int playerid)
{
	if(za[playerid].Team == false && za[playerid].UCID != INVALID_CLASS_ID)
	{
		PlayerPlaySound(playerid, 5848);
		za[playerid].Wins++;
		DHS[playerid].Wins++;
		temp[playerid].Evac=true;
		SetPlayerColor(playerid, 0xFFFF00AA);
		SetPlayerDrunkLevel(playerid, 0);
		SetPlayerInterior(playerid, 3);

		SetPlayerPos(playerid, 961.0551, -53.1878, 1001.1172);
		SetPlayerFacingAngle(playerid, 275.0);
		DisablePlayerCheckpoint(playerid);
		DisablePlayerRaceCheckpoint(playerid);
		
		char str[128];
		snprintf(str, sizeof(str), ">> {00FF00}%s {FFFF00}has made to the evacuation point!", za[playerid].Name);
		SendClientMessageToAll(0xFFFF00AA, str);
		
		TextDrawHideForPlayer(playerid, t_Red);
		TextDrawHideForPlayer(playerid, t_Black);
		TextDrawHideForPlayer(playerid, t_Blue);
		TextDrawHideForPlayer(playerid, t_Yellow);
		TextDrawHideForPlayer(playerid, t_Magenta);
	
		temp[playerid].Infection=false;
		temp[playerid].Burned=false;
		temp[playerid].HumanRogue=false;
		temp[playerid].Poison=0;
		temp[playerid].Slow=false;
		temp[playerid].Leg=false;
		temp[playerid].TransferInfection=0;
		SetPlayerDrunkLevel(playerid, 0);
	
		//tourist human
		if(za[playerid].UCID == 34)
		{
		    SetPlayerScore(playerid, GetPlayerScore(playerid)+90);
			GameTextForPlayer(playerid, "~n~~n~~n~~y~+90~y~~n~~n~~n~", 1000, 5);
		}
		else
		{
			SetPlayerScore(playerid, GetPlayerScore(playerid)+20);
			GameTextForPlayer(playerid, "~n~~n~~n~~y~+20~y~~n~~n~~n~", 1000, 5);
		}
		
		if(lasthuman == true)
		{
			SetPlayerScore(playerid, GetPlayerScore(playerid)+50);
			SendClientMessage(playerid, 0xFFFF00AA, "[LASTHUMAN] +50 EXP!");
		}
		
	}
}

int OnDialogResponse(int playerid, int dialogid, int response, int listitem, char inputtext[])
{	
	if(dialogid == DIALOG_REGISTER)
	{
		if(za[playerid].Log == true)return Kick(playerid);
		
		char str[80];
		if(response)
		{//From 4 to 11;
			if(strlen(inputtext) > 3 && strlen(inputtext) < 12)
			{
				if(strstr(inputtext, " "))
				{
					snprintf(str, sizeof(str), "Welcome, %s!", za[playerid].Name);
	        		ShowPlayerDialog(playerid, DIALOG_REGISTER, 3, "Registration.", str, "Ok", "Cancel");
					SendClientMessage(playerid, 0xC0C0C0AA, ">> [Server] You may not use space in password: ' '.");
					return 1;
				}		
				
				Register(playerid, inputtext);
			}
			else
			{		
				snprintf(str, sizeof(str), "Welcome, %s!", za[playerid].Name);
		        ShowPlayerDialog(playerid, DIALOG_REGISTER, 3, "Registration.", str, "Ok", "Cancel");
				SendClientMessage(playerid, 0xC0C0C0AA, ">> Server: Password Length should be between {00FF00}4 {C0C0C0}and {FF0000}11");
			}
		}
		else
		{
			snprintf(str, sizeof(str), "Welcome, %s!", za[playerid].Name);
	        ShowPlayerDialog(playerid, DIALOG_REGISTER, 3, "Registration.", str, "Ok", "Cancel");
		}
		return 1;
	}
	else if(dialogid == DIALOG_LOGIN)
	{
		if(za[playerid].Log == true)return Kick(playerid);
		//From 3 to 18; Back-Compatibility to 3.0;
		if(response && strlen(inputtext) > 2 && strlen(inputtext) < 19)
		{
			Login(playerid, inputtext);
		}
		else
		{
			char str[80];
			snprintf(str, sizeof(str), "Welcome back, %s!", za[playerid].Name);
			ShowPlayerDialog(playerid, DIALOG_LOGIN, 3, "Login in.", str, "Login", "Cancel");
		}
		return 1;
	}
	else if(dialogid == DIALOG_CLASS)
	{
		if(response)
		{
			if(listitem == 0)
			{
				ShowPlayerDialog(playerid, DIALOG_HUMAN, 2, "Humans\' Classes", za[playerid].HumanString, "Select", "Back");
			}
			else
			{
				ShowPlayerDialog(playerid, DIALOG_ZOMBIE, 2, "Zombies\' Classes", za[playerid].ZombieString, "Select", "Back");
			}
		}
		return 1;
	}
	else if(dialogid == DIALOG_HUMAN)
	{
		if(response)
		{	
			if(listitem < 0 || listitem > (MAX_CLASS_NUMBER-1))return 1;
		
			if(!humanclasses[listitem].VIP)
			{
				if(GetPlayerScore(playerid) >= humanclasses[listitem].EXP)
				{
					za[playerid].Hclass=listitem;
					SendClientMessage(playerid,0xFFFF00AA,">> Class Set, will change next round!");
					if(EvacTime > 275 && za[playerid].Team == false)
					{
						za[playerid].SpawnedAsHuman=true;
						SpawnPlayer(playerid);
					}
				}
				else SendClientMessage(playerid, 0xFFFF00AA, ">> You have no enough XP!");
			}
			else
			{
				if(za[playerid].VIP >= humanclasses[listitem].VIP)
				{
					za[playerid].Hclass=listitem;
					SendClientMessage(playerid,0xFFFF00AA,">> Class Set, will change next round!");
					if(EvacTime > 275 && za[playerid].Team == false)
					{
						za[playerid].SpawnedAsHuman=true;
						SpawnPlayer(playerid);
					}
				}
				else
				{
					char str[80];
					snprintf(str, sizeof(str), ">> You should have %d+ VIP level!", humanclasses[listitem].VIP);
					SendClientMessage(playerid, 0xFFFF00AA, str);
				}
			}
		}
		else ShowPlayerDialog(playerid, DIALOG_CLASS, 2,"Classes", "Humans\' Classes\nZombies\' Classes", "Ok", "Cancel");
		return 1;
	}
	else if(dialogid == DIALOG_ZOMBIE)
	{
		if(response)
		{
			if(listitem < 0 || listitem > (MAX_CLASS_NUMBER-1))return 1;
			
			if(!zombieclasses[listitem].VIP)
			{
				if(GetPlayerScore(playerid) >= zombieclasses[listitem].EXP)
				{
					za[playerid].Zclass=listitem;
					SendClientMessage(playerid,0xFFFF00AA,">> Class Set, will change next round!");
					if(EvacTime > 275 && za[playerid].Team == true)SpawnPlayer(playerid);
				}
				else SendClientMessage(playerid, 0xFFFF00AA, ">> You have no enough XP!");
			}
			else
			{
				if(za[playerid].VIP >= zombieclasses[listitem].VIP)
				{
					za[playerid].Zclass=listitem;
					SendClientMessage(playerid,0xFFFF00AA,">> Class Set, will change next round!");
					if(EvacTime > 275 && za[playerid].Team == true)SpawnPlayer(playerid);
				}
				else
				{
					char str[80];
					snprintf(str, sizeof(str), ">> You should have %d+ VIP level!", zombieclasses[listitem].VIP);
					SendClientMessage(playerid, 0xFFFF00AA, str);
				}
				
			}
		}
		else ShowPlayerDialog(playerid, DIALOG_CLASS, 2,"Classes", "Humans\' Classes\nZombies\' Classes", "Ok", "Cancel");
		return 1;
	}
	else if(dialogid == DIALOG_SETTINGS)
	{
		if(response)
		{
			if(listitem == 0)//BlockPMs
			{
				if((za[playerid].Settings)&0x1)
				{
					za[playerid].Settings=za[playerid].Settings&0xFFFE;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've unblocked PMs!");
				}
				else
				{
					za[playerid].Settings=za[playerid].Settings|0x1;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've blocked PMs!");
				}
			}
			else if(listitem == 1)//Ding
			{
				if((za[playerid].Settings)&0x2)
				{
					za[playerid].Settings=(za[playerid].Settings)&0xFFFD;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the ding {00FF00}on");
				}
				else
				{
					za[playerid].Settings=(za[playerid].Settings)|0x2;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the ding {FF0000}off");
				}
			}
			else if(listitem == 2)//Auto-Login
			{
				if((za[playerid].Settings)&0x4)
				{
					za[playerid].Settings=za[playerid].Settings&0xFFFB;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the Auto-Login {00FF00}on");
				}
				else
				{
					za[playerid].Settings=za[playerid].Settings|0x4;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the Auto-Login {FF0000}off");
				}
			}
			else if(listitem == 3)//Accept Duel
			{
				if((za[playerid].Settings)&0x8)
				{
					za[playerid].Settings=za[playerid].Settings&0xFFF7;
					SendClientMessage(playerid, 0xC0C0C0AA, "Now you can {00FF00}accept {C0C0C0}duels.");
				}
				else
				{
					za[playerid].Settings=za[playerid].Settings|0x8;
					SendClientMessage(playerid, 0xC0C0C0AA, "Now you {FF0000}deny {C0C0C0}automatically duels.");
				}
			}
			else if(listitem == 4)//"Ability Is Ready" Sound
			{
				if((za[playerid].Settings)&0x10)//Is OFF then ON
				{
					za[playerid].Settings=za[playerid].Settings&0xFFEF;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the sound {00FF00}on {C0C0C0}for \"Ability Is Ready\".");
				}
				else//Is ON then OFF
				{
					za[playerid].Settings=za[playerid].Settings|0x10;
					SendClientMessage(playerid, 0xC0C0C0AA, "You've turned the sound {FF0000}off {C0C0C0}for \"Ability Is Ready\".");
				}
			}
			
			
		}
		return 1;
	}
	else if(dialogid == DIALOG_SHOP)
	{
		if(response)
		{
			//Smoke Machine
			if(listitem == 0)
			{
				if(GetPlayerScore(playerid) >= 40)
				{
					SetPlayerScore(playerid, GetPlayerScore(playerid)-40);
					float x, y, z;
					GetPlayerPos(playerid, &x, &y, &z);
					CreateObject(2780, x, y, z-2, 0, 0, 0, 200.0);
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Anti-Seeker
			else if(listitem == 1)
			{
				if(GetPlayerScore(playerid) >= 50)
				{
				    if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
				    if(temp[playerid].AntiSeeker == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've bought already.");
					
					temp[playerid].AntiSeeker=true;
					SetPlayerScore(playerid, GetPlayerScore(playerid)-50);
					SendClientMessage(playerid,0xFFFF00AA,">> You've got anti-seeker.");
				}
				else
				{
						SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Anti-Freezer	
			else if(listitem == 2)
			{
				if(GetPlayerScore(playerid) >= 90)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					if(temp[playerid].AntiFreezer == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've bought already.");
					
					SetPlayerScore(playerid, GetPlayerScore(playerid)-90);
					temp[playerid].AntiFreezer=true;
					SendClientMessage(playerid,0xFFFF00AA,">> You've got anti-freezer.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//camera
			else if(listitem == 3)
			{
				if(GetPlayerScore(playerid) >= 5)
				{
					SetPlayerScore(playerid,GetPlayerScore(playerid)-5);
					GivePlayerWeapon(playerid, 43, 40);
					SendClientMessage(playerid,0xFFFF00AA,">> You've got a camera.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Armour
			else if(listitem == 4)
			{
				if(GetPlayerScore(playerid) >= 100)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					
					SetPlayerScore(playerid, GetPlayerScore(playerid)-100);
					SetPlayerArmour(playerid, 50.0);
					SendClientMessage(playerid, 0xFFFF00AA, ">> You've got an armour.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Anti-Burning
			else if(listitem == 5)
			{
				if(GetPlayerScore(playerid) >= 80)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					if(za[playerid].UCID == 9)return SendClientMessage(playerid,0xFFFF00AA,">> You are firemaniac.");
					if(temp[playerid].AntiBurning == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've bought already.");
					
					temp[playerid].AntiBurning=true;
					TextDrawHideForPlayer(playerid, t_Yellow);
					temp[playerid].Burned=false;
					RemovePlayerAttachedObject(playerid, 9);
					
					SetPlayerScore(playerid, GetPlayerScore(playerid)-80);
					SendClientMessage(playerid, 0xFFFF00AA, ">> You've got anti-burning.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Morphine
			else if(listitem == 6)
			{
				if(GetPlayerScore(playerid) >= 90)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					if(temp[playerid].Morphine == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've bought already.");
				
					temp[playerid].Morphine=true;
					temp[playerid].Leg=false;
					temp[playerid].Slow=false;
					
					SetPlayerScore(playerid,GetPlayerScore(playerid)-90);
					SendClientMessage(playerid,0xFFFF00AA,">> You've bought morphine.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Anti-Screamer
			else if(listitem == 7)
			{
				if(GetPlayerScore(playerid) >= 10)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					if(temp[playerid].AntiScreamer == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've bought already.");
					
					temp[playerid].AntiScreamer=true;
					SetPlayerScore(playerid,GetPlayerScore(playerid)-10);
					SendClientMessage(playerid,0xFFFF00AA,">> You've bought anti-screamer.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Anti-Blind	
			else if(listitem == 8)
			{
				if(GetPlayerScore(playerid) >= 80)
				{
					if(za[playerid].Team == true)return SendClientMessage(playerid,0xFFFF00AA,">> Only for humans.");
					
					temp[playerid].AntiBlind=true;
					temp[playerid].IsBlinded=false;
					TextDrawHideForPlayer(playerid, t_Black);
					SetPlayerWeather(playerid, MapData.Weather);
					SetPlayerTime(playerid, MapData.Time);
					
					SetPlayerScore(playerid, GetPlayerScore(playerid)-80);
					SendClientMessage(playerid, 0xFFFF00AA, ">> You've bought anti-blind.");
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}
			//Helmet	
			else if(listitem == 9)
			{
				if(GetPlayerScore(playerid) >= 10)
				{
					if(za[playerid].Team == false)return SendClientMessage(playerid,0xFFFF00AA,">> You are not a zombie.");
					if(temp[playerid].Helmet == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've already bought a helmet.");
					 	
					temp[playerid].Helmet=true;
					SetPlayerScore(playerid, GetPlayerScore(playerid)-10);
					SendClientMessage(playerid, 0xFFFF00AA, ">> You've bought helmet. -10 EXP!");
					SetPlayerAttachedObject(playerid,9,19103,2,0.1949,0.0099,0.0,0.0,0.0,0.0,1.298998, 1.116, 1.1799, 0, 0);
				}
				else
				{
					SendClientMessage(playerid,0xFFFF00AA,">> You do not have enough XP!");
				}
			}	
		}
		return 1;
	}
	
	
	return 0;
}

void Engines(int playerid)
{
	RemovePlayerAttachedObject(playerid, 5);
	RemovePlayerAttachedObject(playerid, 6);
}

void Black(int playerid)
{
	TextDrawHideForPlayer(playerid, t_Black);
	SetPlayerWeather(playerid, MapData.Weather);
	SetPlayerTime(playerid, MapData.Time);
}

void Smoke(int value)
{
	DestroyObject(value);
}

void Space(int playerid)
{
	RemovePlayerAttachedObject(playerid, 8);
	RemovePlayerAttachedObject(playerid, 9);
	SetPlayerVirtualWorld(playerid, VIRTUALWORLD_SPACEBREAKER);
}

void Slender(int playerid)
{
	RemovePlayerAttachedObject(playerid, 8);
	RemovePlayerAttachedObject(playerid, 7);
	RemovePlayerAttachedObject(playerid, 6);
	RemovePlayerAttachedObject(playerid, 5);
	RemovePlayerAttachedObject(playerid, 4);
	RemovePlayerAttachedObject(playerid, 3);
}

void Screamer(int playerid)
{
	ClearAnimations(playerid, 1);
}

void Freeze(int playerid)
{
	TextDrawHideForPlayer(playerid, t_Blue);
	TogglePlayerControllable(playerid, 1);
}

void KickPlayer(int playerid)
{
	Kick(playerid);
}

void Spores(int playerid)
{
	temp[playerid].Spore=false;
	GameTextForPlayer(playerid, "~r~deactivated!", 2000, 5);
}

void GunJammer(int playerid)
{
	if(temp[playerid].JammerZombie != 0)
	{
		GameTextForPlayer(playerid, "~r~deactivated!", 2000, 5);
	}
}

void Stung(int playerid)
{
	ClearAnimations(playerid, 1);
	TextDrawHideForPlayer(playerid, t_Magenta);
}

void cBug(int playerid)
{
	temp[playerid].cBug=false;
}

void OnPlayerDuel(int playerid)
{
	za[playerid].Duel=false;
}

void Tazer(int playerid)
{
	ClearAnimations(playerid, 1);
	TogglePlayerControllable(playerid, 1);
	temp[playerid].Tazered=false;
}

void TazerObject(int objectid)
{
	DestroyObject(objectid);
}

void KindaConnected(int playerid)
{
	char str[100];
	snprintf(str, sizeof(str), ">> %s (ID: %d) has joined the server.", za[playerid].Name, playerid);
	SendClientMessageToAll(0xC0C0C0AA, str);
}

void Unstuck(int playerid)
{
	if(za[playerid].Team == true && za[playerid].Duel == false && za[playerid].InEvent == false && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		char str[120];
		snprintf(str, sizeof(str), ">> %s has used {FF0000}/unstuck{ffffcc}.", za[playerid].Name);
		SendClientMessageToAll(0xffffccaa, str);
		
		SpawnPlayer(playerid);
	}
}

void HideAbility(int playerid)
{
	TextDrawHideForPlayer(playerid, t_Ability);
}

void BanPlayer(int banid, char admin[], char reason[], int duration)
{
	char str[600]; char type[25];

	if(duration)//Temporary Ban;
	{
		snprintf(str, sizeof(str), "|: %s has been banned by %s for %d hours [Reason: %s]", za[banid].Name, admin, duration, reason);
		snprintf(type, sizeof(type), "Temporary (%d Hour(s))", duration);

	    sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Data SET Time=? WHERE ID=?", 33, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, duration*3600+(int)time(NULL));
		sqlite3_bind_int(stmt, 2, za[banid].AccountID);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		snprintf(str, sizeof(str), "|: %s has been banned by %s [Reason: %s]", za[banid].Name, admin, reason);
		strcpy(type, "Permanent");
	
		char ipcmd[30];
		snprintf(ipcmd, sizeof(ipcmd), "banip %s", za[banid].IP);
		SendRconCommand(ipcmd);
	}
		
	SendClientMessageToAdmins(str);
	SendClientMessage(banid, COLOR_ANTIHACK, str);
		
	SendClientMessage(banid, 0x009900AA,"|: Unfair banned? Go to our website www.sampdm.net and write a Ban Appeal. {FFFFFF}SCREENSHOT {FFFF00}with {FF0000}F8!");
	SetTimerEx("PlayerKick", 500, banid);
	
	char Ereason[128];
	mysql_escape_string(reason, Ereason, sizeof(Ereason), mysql);
	Ereason[127]=0;
	
	snprintf(str, sizeof(str), "INSERT INTO `List` (`Name`,`Reason`,`Admin`,`Type`,`unbanned`,`unixtime`,`ip`) VALUES ('%s','%s','%s','%s', %d, %d, '%s')", za[banid].Name, Ereason, admin, type, (type[0] == 'T')?1:0, (int)time(NULL), za[banid].IP);
	mysql_tquery(mysql, str, "", 0);	
}

void UnbanPlayer(int playerid)
{
    if(cache_affected_rows(mysql))
   		SendClientMessage(playerid, COLOR_ANTIHACK, ">> The player has been unbanned!");
	else
		SendClientMessage(playerid, 0xC0C0C0AA,">> [UNBAN] Name not found!");
}

void CheckUserBan(int playerid)
{	
	int num=cache_num_rows(mysql);
	if(num > 0)
	{
		char str[128];
		snprintf(str, sizeof(str), ">> %s has attempted to join the server whilst serving a ban.", za[playerid].Name);
		SendClientMessageToAdmins(str);
	    SendClientMessage(playerid, -1, ">>> Congratulations, you're {FF0000}banned!");

		char reason[128], admin[24];
		cache_get_field_content(num-1, "Reason", reason, mysql, sizeof(reason));
	    cache_get_field_content(num-1, "Admin", admin, mysql, sizeof(admin));
	    snprintf(str, sizeof(str), ">>> Reason: %s | Admin: %s", reason, admin);
		SendClientMessage(playerid, -1, str);

		SendClientMessage(playerid, -1, ">>> Appeal at www.sampdm.net!");
		SetTimerEx("PlayerKick", 500, playerid);
	}
	else
	{
		za[playerid].BanCheck=true;
	}
	return;
}

void DayilyHighScores(int playerid)
{
	if(cache_num_rows(mysql) > 0)//UPDATE
	{
		int Kills=cache_get_field_content_int(0, "Kills", mysql);
		int Wins=cache_get_field_content_int(0, "Wins", mysql);
		int Time=cache_get_field_content_int(0, "Time", mysql);
		int AdminTime=cache_get_field_content_int(0, "AdminTime", mysql);
		int HumanKills=cache_get_field_content_int(0, "HumanKills", mysql);
		int ZombieKills=cache_get_field_content_int(0, "ZombieKills", mysql);
		int Bans=cache_get_field_content_int(0, "Bans", mysql);

		char query[500];
		snprintf(query, sizeof(query), 
		"UPDATE HighScoresTable SET Kills=%d, Wins=%d, `Time`=%d, AdminTime=%d, HumanKills=%d, ZombieKills=%d, Bans=%d WHERE Name='%s'", DHS[playerid].Kills+Kills, DHS[playerid].Wins+Wins, ((int)time(NULL)-DHS[playerid].Time-DHS[playerid].AFKtime)+Time, (DHS[playerid].Admin)?((int)time(NULL)-DHS[playerid].Time-DHS[playerid].AFKtime)+AdminTime:0, DHS[playerid].HumanKills+HumanKills, DHS[playerid].ZombieKills+ZombieKills, DHS[playerid].Bans+Bans, DHS[playerid].name);
		mysql_tquery(mysql, query, "", 0);
	}
	else//INSERT
	{
		char query[400];
		snprintf(query, sizeof(query), "INSERT INTO HighScoresTable (`Name`, Kills, Wins, `Time`, AdminTime, HumanKills, ZombieKills, Bans) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d)", DHS[playerid].name, DHS[playerid].Kills, DHS[playerid].Wins, (int)time(NULL)-DHS[playerid].Time-DHS[playerid].AFKtime, (DHS[playerid].Admin)?((int)time(NULL)-DHS[playerid].Time-DHS[playerid].AFKtime):0, DHS[playerid].HumanKills, DHS[playerid].ZombieKills, DHS[playerid].Bans);
		mysql_tquery(mysql, query, "", 0);
	}

	memset(&DHS[playerid], 0, sizeof(DHS[playerid]));
	return;
}


unsigned IsPlayerPaused(int playerid)
{
	unsigned int now=GetCount();
	return now-za[playerid].uAFK;
}

