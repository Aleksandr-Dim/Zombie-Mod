#include "GameVars.h"

#define COLUMN_ID  0
#define COLUMN_NAME  1
#define COLUMN_PASSWORD  2
#define COLUMN_EXP  3
#define COLUMN_KILLS  4
#define COLUMN_TIME  5
#define COLUMN_WINS  6
#define COLUMN_ADMIN  7
#define COLUMN_VIP  8
#define COLUMN_VIPTIME  9
#define COLUMN_RANK  10
#define COLUMN_CLAN  11
#define COLUMN_DUELWINS 12
#define COLUMN_DUELLOSSES  13
#define COLUMN_KILLSTREAKS  14
#define COLUMN_LASTTIME  15
#define COLUMN_IP 16
#define COLUMN_SETTINGS  17
#define COLUMN_NAMECHANGE  18

void RegisterOrLogin(int playerid)
{
	sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, "SELECT * FROM Data WHERE Name=?", 31, &stmt, NULL) !=  SQLITE_OK)Log("RorL_pre", sqlite3_errmsg(db));
	sqlite3_bind_text(stmt, 1, za[playerid].Name, strlen(za[playerid].Name), 0);
	
	int rc=sqlite3_step(stmt);
    if(rc == SQLITE_DONE)
    {
    	char str[80];
    	snprintf(str, sizeof(str), "Welcome, %s!", za[playerid].Name);
		ShowPlayerDialog(playerid, DIALOG_REGISTER, 3, "Registration.", str, "Ok", "Cancel");
	}
    else if(rc == SQLITE_ROW)//Registered
    {
		if(sqlite3_column_int(stmt, COLUMN_TIME) > time(NULL))
		{
			char str[144];
			snprintf(str, sizeof(str), ">> %s has attempted to join the server whilst serving a tempban.", za[playerid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			SendClientMessage(playerid, 0xFFFFFFAA, ">>> Congratulations, you're {00FF00}temp{FF0000}banned!");
			SetTimerEx("PlayerKick", 500, playerid);
			return;
		}
		////////////////////////////////////////////////////////////////////////////////////
		if((sqlite3_column_int(stmt, COLUMN_LASTTIME) > (time(NULL)-480)) && !(sqlite3_column_int(stmt, COLUMN_SETTINGS)&0x4) && !strcmp((const char *)sqlite3_column_text(stmt, COLUMN_IP), za[playerid].IP))
		{	
			za[playerid].AccountID=sqlite3_column_int(stmt, COLUMN_ID);
			SetPlayerScore(playerid, sqlite3_column_int(stmt, COLUMN_EXP));
			za[playerid].Kills=sqlite3_column_int(stmt, COLUMN_KILLS);
			za[playerid].Wins=sqlite3_column_int(stmt, COLUMN_WINS);
			za[playerid].Admin=sqlite3_column_int(stmt, COLUMN_ADMIN);
			za[playerid].Rank=sqlite3_column_int(stmt, COLUMN_RANK);
			za[playerid].Clan=sqlite3_column_int(stmt, COLUMN_CLAN);
			za[playerid].DuelWins=sqlite3_column_int(stmt, COLUMN_DUELWINS);
			za[playerid].DuelLoses=sqlite3_column_int(stmt, COLUMN_DUELLOSSES);
			za[playerid].personalKS=sqlite3_column_int(stmt, COLUMN_KILLSTREAKS);
			za[playerid].Settings=sqlite3_column_int(stmt, COLUMN_SETTINGS);				
			za[playerid].NameChange=sqlite3_column_int(stmt, COLUMN_NAMECHANGE);				

			za[playerid].VipTime=sqlite3_column_int(stmt, COLUMN_VIPTIME);
			if(za[playerid].VipTime > time(NULL))
			{
				za[playerid].VIP=sqlite3_column_int(stmt, COLUMN_VIP);
			}
			
			DHS[playerid].Time=time(NULL);
			if(za[playerid].Admin)AA[playerid].Time=DHS[playerid].Time;
			
			za[playerid].Team=true;
			za[playerid].Log=true;
			SendClientMessage(playerid, 0xFFFF00AA, ">> Successfully {00FF00}auto{FFFF00}-logged in!");
	        SetPlayerColor(playerid, 0xC0C0C0AA);
	        
	        HumanGenerate(playerid);
        	ZombieGenerate(playerid);
		}
		else
		{
			char str[80];
			snprintf(str, sizeof(str), "Welcome back, %s!", za[playerid].Name);
			ShowPlayerDialog(playerid, DIALOG_LOGIN, 3, "Login in.", str, "Login", "Cancel");
		}
		////////////////////////////////////////////////////////////////////////////////////
	}
	else Log("RorL_step", sqlite3_errmsg(db));
	
	sqlite3_finalize(stmt);
}

void Register(int playerid, char password[])
{
	char hash[65]; sqlite3_stmt *stmt;
	SHA256_PassHash(password, hash);
	if(sqlite3_prepare_v2(db, "INSERT INTO Data (Name, Password) VALUES(?, ?)", 46, &stmt, NULL) !=  SQLITE_OK)
	{
		Log("R_pre", sqlite3_errmsg(db));
		goto Reg_END;
	}
	sqlite3_bind_text(stmt, 1, za[playerid].Name, strlen(za[playerid].Name), 0);
	sqlite3_bind_text(stmt, 2, hash, strlen(hash), 0);
	
	if(sqlite3_step(stmt) != SQLITE_DONE)
	{
		Log("R_step", sqlite3_errmsg(db));
		goto Reg_END;
	}
	
	SendClientMessage(playerid, 0xFFFF00AA, ">> Successfully registered!");
	DHS[playerid].Time=time(NULL);
	za[playerid].Log=true;
	za[playerid].Team=true;
	SetPlayerColor(playerid, 0xC0C0C0AA);
	
	HumanGenerate(playerid);
    ZombieGenerate(playerid);
	Reg_END:
	sqlite3_finalize(stmt);
	
	sqlite3_prepare_v2(db, "SELECT ID FROM Data WHERE Name=?", 32, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, za[playerid].Name, strlen(za[playerid].Name), 0);	
	sqlite3_step(stmt);
	if((za[playerid].AccountID=sqlite3_column_int(stmt, 0)) == 0)Kick(playerid);
	sqlite3_finalize(stmt);
}

void Login(int playerid, char password[])
{
	char hash[65]; sqlite3_stmt *stmt;
	if(sqlite3_prepare_v2(db, "SELECT * FROM Data WHERE Name=?", 31, &stmt, NULL) !=  SQLITE_OK)
	{
		Log("L_pre", sqlite3_errmsg(db));
		goto Log_END;
	}
	sqlite3_bind_text(stmt, 1, za[playerid].Name, strlen(za[playerid].Name), 0);
	
	int rc=sqlite3_step(stmt);
	if(rc != SQLITE_ROW)
	{
		Log("L_step", sqlite3_errmsg(db));
		goto Log_END;
	}
	
	SHA256_PassHash(password, hash);
	if(!strncmp((const char *)sqlite3_column_text(stmt, COLUMN_PASSWORD), hash, 64))
	{		
		za[playerid].AccountID=sqlite3_column_int(stmt, COLUMN_ID);
		SetPlayerScore(playerid, sqlite3_column_int(stmt, COLUMN_EXP));
		za[playerid].Kills=sqlite3_column_int(stmt, COLUMN_KILLS);
		za[playerid].Wins=sqlite3_column_int(stmt, COLUMN_WINS);
		za[playerid].Admin=sqlite3_column_int(stmt, COLUMN_ADMIN);
		za[playerid].Rank=sqlite3_column_int(stmt, COLUMN_RANK);
		za[playerid].Clan=sqlite3_column_int(stmt, COLUMN_CLAN);
		za[playerid].DuelWins=sqlite3_column_int(stmt, COLUMN_DUELWINS);
		za[playerid].DuelLoses=sqlite3_column_int(stmt, COLUMN_DUELLOSSES);
		za[playerid].personalKS=sqlite3_column_int(stmt, COLUMN_KILLSTREAKS);
		za[playerid].Settings=sqlite3_column_int(stmt, COLUMN_SETTINGS);				
		za[playerid].NameChange=sqlite3_column_int(stmt, COLUMN_NAMECHANGE);

		za[playerid].VipTime=sqlite3_column_int(stmt, COLUMN_VIPTIME);
		if(za[playerid].VipTime > time(NULL))
		{
			za[playerid].VIP=sqlite3_column_int(stmt, COLUMN_VIP);
		}

		DHS[playerid].Time=time(NULL);
		if(za[playerid].Admin)AA[playerid].Time=DHS[playerid].Time;
		
		za[playerid].Log=true;
		za[playerid].Team=true;
		SendClientMessage(playerid, 0xFFFF00AA, ">> Successfully logged in!");
        SetPlayerColor(playerid,0xC0C0C0AA);
        
        HumanGenerate(playerid);
        ZombieGenerate(playerid);
	}
	else
	{
		char str[144];
		SendClientMessage(playerid,0xC0C0C0AA,">> Server: Incorrect Password!");
		LoginError[playerid]++;
		
		if(LoginError[playerid] == 3)
		{
			LoginError[playerid]=0;
			snprintf(str, sizeof(str), ">> %s has been kicked for 3 failed login attempts!", za[playerid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			SetTimerEx("PlayerKick", 500, playerid);
		}
		else
		{
			snprintf(str, sizeof(str), "Welcome back, %s!", za[playerid].Name);
	 		ShowPlayerDialog(playerid, DIALOG_LOGIN, 3, "Login in.", str, "Login", "Cancel");
		}
	}
	Log_END:
	sqlite3_finalize(stmt);
}

void Save(int playerid)
{	
	if(za[playerid].Log == true && za[playerid].Unconnected == false)
    {
        sqlite3_stmt *stmt;
    	if(sqlite3_prepare_v2(db, "UPDATE Data SET EXP=?, Kills=?, Wins=?, DuelWins=?, DuelLosses=?, Killstreaks=?, LastTime=?, IP=?, Settings=? WHERE ID=?", 120, &stmt, NULL) !=  SQLITE_OK)
		{
			Log("S_pre", sqlite3_errmsg(db));
			goto Save_End;
		}
		sqlite3_bind_int(stmt, 1, GetPlayerScore(playerid));
		sqlite3_bind_int(stmt, 2, za[playerid].Kills);
		sqlite3_bind_int(stmt, 3, za[playerid].Wins);
		sqlite3_bind_int(stmt, 4, za[playerid].DuelWins);
		sqlite3_bind_int(stmt, 5, za[playerid].DuelLoses);
		sqlite3_bind_int(stmt, 6, za[playerid].personalKS);
		sqlite3_bind_int(stmt, 7, time(NULL));
		sqlite3_bind_text(stmt, 8, za[playerid].IP, strlen(za[playerid].IP), 0);
		sqlite3_bind_int(stmt, 9, za[playerid].Settings);
		sqlite3_bind_int(stmt, 10, za[playerid].AccountID);
		
		if(sqlite3_step(stmt) != SQLITE_DONE)Log("L_step", sqlite3_errmsg(db));
		Save_End:
		sqlite3_finalize(stmt);
    }
}
