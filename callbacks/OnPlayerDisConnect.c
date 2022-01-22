#include "GameVars.h"

void OnPlayerConnect(int playerid)
{
	GetPlayerIp(playerid, za[playerid].IP);	
	GetPlayerName(playerid, za[playerid].Name);
	SetPlayerColor(playerid, 0x000000AA);
	SetPlayerVirtualWorld(playerid, VIRTUALWORLD_CONNECTED);
	za[playerid].Timer=SetTimerExForPlayer(playerid);
	za[playerid].ReplyID=INVALID_PLAYER_ID;
	za[playerid].BanCheck=false;
	za[playerid].JustConnected=true;
	
	int IPs=0;
	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Log == true && i != playerid)
		{
			if(!strcmp(za[playerid].IP, za[i].IP))
			{
				IPs++;
			}
		}
	}
	if(IPs > 2)//Maximum 3 players from one IP;
	{
		Kick(playerid);
		return;
	}

	char str[128];
	snprintf(str, sizeof(str), "SELECT Reason, Admin FROM List WHERE unbanned=0 AND Name LIKE BINARY '%s'", za[playerid].Name);
	mysql_tquery(mysql, str, "CheckUserBan", playerid);
	
	TextDrawShowForPlayer(playerid, t_zombie);
	TextDrawShowForPlayer(playerid, t_human);
	TextDrawShowForPlayer(playerid, t_forum);
	TextDrawShowForPlayer(playerid, t_site);
	TextDrawShowForPlayer(playerid, t_evac);	
	TextDrawShowForPlayer(playerid, t_timer);

	TextDrawShowForPlayer(playerid, t_welcome);
	TextDrawShowForPlayer(playerid, t_za);
	TextDrawShowForPlayer(playerid, t_server);
	TextDrawShowForPlayer(playerid, t_help);
	TextDrawShowForPlayer(playerid, t_rules);
	TextDrawShowForPlayer(playerid, t_add);

	za[playerid].TDs.t_Ping=CreatePlayerTextDraw(playerid, 575.0, 20.0, "Ping: NaN");
	PlayerTextDrawTextSize(playerid, za[playerid].TDs.t_Ping, 20.0, 40.0);
	PlayerTextDrawAlignment(playerid, za[playerid].TDs.t_Ping, 2);
	PlayerTextDrawFont(playerid, za[playerid].TDs.t_Ping, 1);
	PlayerTextDrawLetterSize(playerid, za[playerid].TDs.t_Ping, 0.55, 1.2);
	PlayerTextDrawColor(playerid, za[playerid].TDs.t_Ping, 0xAFAFAFAA);
	PlayerTextDrawSetProportional(playerid,za[playerid].TDs.t_Ping, 1);
	
	za[playerid].TDs.t_Health=CreatePlayerTextDraw(playerid, 505.0, 90.0, "Health: 0.00");
	PlayerTextDrawTextSize(playerid, za[playerid].TDs.t_Health, 20.0, 40.0);
	PlayerTextDrawAlignment(playerid, za[playerid].TDs.t_Health, 2);
	PlayerTextDrawFont(playerid, za[playerid].TDs.t_Health, 1);
	PlayerTextDrawLetterSize(playerid, za[playerid].TDs.t_Health, 0.55, 1.2);
	PlayerTextDrawColor(playerid, za[playerid].TDs.t_Health, 0xAFAFAFAA);
	PlayerTextDrawSetProportional(playerid, za[playerid].TDs.t_Health, 1);
	
	
	za[playerid].TDs.t_Team=CreatePlayerTextDraw(playerid, 505.0, 20.0, "Team: None~n~Class: None");
	PlayerTextDrawTextSize(playerid, za[playerid].TDs.t_Team, 20.0, 40.0);
	PlayerTextDrawAlignment(playerid, za[playerid].TDs.t_Team, 2);
	PlayerTextDrawFont(playerid, za[playerid].TDs.t_Team, 1);
	PlayerTextDrawLetterSize(playerid, za[playerid].TDs.t_Team, 0.55, 1.2);
	PlayerTextDrawColor(playerid, za[playerid].TDs.t_Team, 0x5D8AFFFF);
	PlayerTextDrawSetProportional(playerid, za[playerid].TDs.t_Team, 1);
	
	
	za[playerid].TDs.t_Weapons=CreatePlayerTextDraw(playerid, 505.0, 256.0, "Pistol: NaN~n~Shotgun: NaN~n~Assault Rifle: NaN");
	PlayerTextDrawTextSize(playerid, za[playerid].TDs.t_Weapons, 40.0, 40.0);
	PlayerTextDrawAlignment(playerid, za[playerid].TDs.t_Weapons, 2);
	PlayerTextDrawFont(playerid, za[playerid].TDs.t_Weapons, 1);
	PlayerTextDrawLetterSize(playerid, za[playerid].TDs.t_Weapons, 0.55, 1.2);
	PlayerTextDrawColor(playerid, za[playerid].TDs.t_Weapons, 0xFF1493FF);
	PlayerTextDrawSetProportional(playerid, za[playerid].TDs.t_Weapons, 1);

	///////////////////////////////////////DUEL OBJECTS////////////////////////////////
	CreatePlayerObject(playerid, 3565,1389.1,-16.4,1001.3,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (1)
	CreatePlayerObject(playerid, 3565,1389.1,-19.0,1001.3,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (2)
	CreatePlayerObject(playerid, 3565,1389.1,-21.6,1001.3,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (3)
	CreatePlayerObject(playerid, 3565,1389.1,-24.2,1001.3,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (4)
	CreatePlayerObject(playerid, 3565,1389.2,-24.2,1003.9,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (5)
	CreatePlayerObject(playerid, 3565,1389.2,-21.7,1003.9,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (6)
	CreatePlayerObject(playerid, 3565,1389.2,-19.1,1003.9,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (7)
	CreatePlayerObject(playerid, 3565,1389.2,-16.5,1003.9,0.0,0.0,0.0, 300.0); //object(lasdkrt1_la01) (8)
	///////////////////////////////////////////DUEL OBJECTS/////////////////////////////

	///////////////////////////////////////////EVAC OBJECTS/////////////////////////////
	CreatePlayerObject(playerid, 944,966.2999,-52.8,1001.0,0.0,0.0,0.0, 300.0); //object(packing_carates04) (1)
	CreatePlayerObject(playerid, 944,966.2999,-52.8,1002.2,0.0,0.0,0.0, 300.0); //object(packing_carates04) (2)
	CreatePlayerObject(playerid, 944,966.2999,-53.6,1001.0,0.0,0.0,0.0, 300.0); //object(packing_carates04) (3)
	CreatePlayerObject(playerid, 944,966.2999,-53.6,1002.20,0.0,0.0,0.0, 300.0); //object(packing_carates04) (4)
	CreatePlayerObject(playerid, 944,966.2999,-52.8,1003.1,0.0,0.0,0.0, 300.0); //object(packing_carates04) (5)
	CreatePlayerObject(playerid, 944,966.2999,-53.6,1003.1,0.0,0.0,0.0, 300.0); //object(packing_carates04) (6)
	CreatePlayerObject(playerid, 4108,960.5,-33.0,994.2999,0.0,0.0,0.0, 300.0); //object(roads01b_lan) (1)
	///////////////////////////////////////////EVAC OBJECTS/////////////////////////////
	
	RegisterOrLogin(playerid);
	
	for(int i = 0; i < AMOUNT; i++)
	{
		if(za[i].Admin)
		{
			char str[128];
			snprintf(str, sizeof(str), ">> %s (ID: %d) has joined the server.  [IP %s]", za[playerid].Name, playerid, za[playerid].IP);
			SendClientMessage(i, 0xC0C0C0AA, str);
		}
		else
		{
			char str[100];
			snprintf(str, sizeof(str), ">> %s (ID: %d) has joined the server.", za[playerid].Name, playerid);
			SendClientMessage(i, 0xC0C0C0AA, str);
		}
	}
	return;
}

void OnPlayerDisconnect(int playerid, int reason)
{
	TimerKill(za[playerid].Timer);
	Save(playerid);

	char str[128];
	switch(reason)
	{
		case 0:
		{
			snprintf(str, sizeof(str), "*** %s has left the Server [Lost Connection]", za[playerid].Name);
			break;
		}
		case 1:
		{
			snprintf(str, sizeof(str), "*** %s has left the Server [Leaving]", za[playerid].Name);
			break;
		}
		case 2:
		{
			snprintf(str, sizeof(str), "*** %s has left the Server [Kicked]", za[playerid].Name);
			break;
		}
	}
	SendClientMessageToAll(0xC0C0C0AA, str);

	if(Votekick.VoteOpen == true && Votekick.ToBeKicked == playerid)
	{
		SendClientMessageToAll(0xFFFF00AA,">> The player left the server!");
		Votekick.VoteOpen=false;
		Votekick.TimeLimit=time(NULL);
	}
	
	if(BossZombie == true && BZID == playerid)
	{
		BossZombie=false;
	}
	
	//If admin used vehicle.
	if(za[playerid].Vehicle)DestroyVehicle(za[playerid].Vehicle);
	
	//If Fly Zombie has vehicle and objects;
	if(temp[playerid].Fly)
	{
		for(int i=0; i < 8; i++)
		{
			DestroyObject(temp[playerid].FlyObjects[i]);
		}
		DestroyVehicle(temp[playerid].Fly);
		temp[playerid].Fly=0;
	}

	if(za[playerid].Log == true)
	{
		if(za[playerid].Duel == true)
		{
			ResetPlayerWeapons(za[playerid].DuelVictimID);
			SpawnPlayer(za[playerid].DuelVictimID);
			za[za[playerid].DuelVictimID].DuelFalsePositive=false;
			SetTimerEx("OnPlayerDuel", 3000, za[playerid].DuelVictimID);

			char str[128];
			snprintf(str, sizeof(str), ">>  %s has left the server. %s has won the duel!", za[playerid].Name, za[za[playerid].DuelVictimID].Name);
			SendClientMessageToAll(0xFFE4B5AA, str);
		}
		
		if((time(NULL)-DHS[playerid].Time) > 300 || DHS[playerid].Bans > 0)
		{
			DHS[playerid].Admin=za[playerid].Admin;
			strncpy(DHS[playerid].name, za[playerid].Name, 24);
			
			char str2[128];
			snprintf(str2, sizeof(str2), "SELECT * From HighScoresTable Where Name='%s'", DHS[playerid].name);
			mysql_tquery(mysql, str2, "DayilyHighScores", playerid);
		}
		
		if(za[playerid].Admin)
		{			
			char str[128];
			snprintf(str, sizeof(str), "UPDATE AA SET Day%d=Day%d+? WHERE ID=? AND Week=?", CurrentDay, CurrentDay);

			sqlite3_stmt *stmt;
	    	sqlite3_prepare_v2(logs_db, str, 47, &stmt, NULL);
	    	sqlite3_bind_int(stmt, 1, time(NULL)-AA[playerid].Time-AA[playerid].AFK);
			sqlite3_bind_int(stmt, 2, za[playerid].AccountID);
			sqlite3_bind_int(stmt, 3, CurrentWeek);
			
			if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(logs_db) == 0)
			{
				snprintf(str, sizeof(str), "INSERT INTO AA (ID, Time, Week, Day%d) VALUES (?, ?, ?, ?)", CurrentDay);

				sqlite3_stmt *stmt2;
		    	sqlite3_prepare_v2(logs_db, str, 57, &stmt2, NULL);
		    	sqlite3_bind_int(stmt2, 1, za[playerid].AccountID);
		    	sqlite3_bind_int(stmt2, 2, time(NULL));
		    	sqlite3_bind_int(stmt2, 3, CurrentWeek);
		    	sqlite3_bind_int(stmt2, 4, time(NULL)-AA[playerid].Time-AA[playerid].AFK);
				sqlite3_step(stmt2);
				sqlite3_finalize(stmt2);
			}
			sqlite3_finalize(stmt);

			AA[playerid].AFK=0;
		}
	}

	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Log == true && za[i].ReplyID == playerid)
		{
			za[i].ReplyID=INVALID_PLAYER_ID;
		}
	}
	
	memset(&za[playerid], 0, sizeof(za[playerid]));
	memset(&temp[playerid], 0, sizeof(temp[playerid]));
	LoginError[playerid]=0;
    return;
}


