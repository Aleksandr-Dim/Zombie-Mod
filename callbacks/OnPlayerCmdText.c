#include "GameVars.h"

char *NoClass="NoClass";

char *formatDS="%d %[\1-\377]";
char *formatDDS="%d%d %[\1-\377]";

#define rcmd(cmd, length) if(!strncasecmp(cmd, cmdtext, length) && (cmdtext[length] == '\0' || cmdtext[length] == ' '))

static char *reason1="Health Hack";
static char *reason2="Weapon Hack";
static char *reason3="AirBrake";
static char *reason4="Speed Hack";
static char *reason5="Teleport Hack";
static char *reason6="Parkour Mode";
static char *reason7="Fly Hack";

static char *ReasonExpander(char *reason)
{
	if(!strcasecmp(reason, "HH"))return reason1;
	if(!strcasecmp(reason, "WH"))return reason2;
	if(!strcasecmp(reason, "AB"))return reason3;
	if(!strcasecmp(reason, "SH"))return reason4;
	if(!strcasecmp(reason, "TH"))return reason5;
	if(!strcasecmp(reason, "PM"))return reason6;
	if(!strcasecmp(reason, "FH"))return reason7;	
	return reason;
}

static char *reason8="Spawn Damage";
static char *reason9="Spawn Camping";
static char *reason10="Team Attack";
static char *reason11="Out Of Map";

static char *ReasonExpanderForWarns(char *reason)
{
	if(!strcasecmp(reason, "SD"))return reason8;
	if(!strcasecmp(reason, "SC"))return reason9;
	if(!strcasecmp(reason, "TA"))return reason10;
	if(!strcasecmp(reason, "OOM"))return reason11;	
	return reason;
}


static inline void SendGangMessage(int gangid, char message[])
{
	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Clan == gangid)
		{
			SendClientMessage(i, 0xFF00FFAA, message);
		}
	}
}

int OnPlayerCommandText(int playerid, char cmdtext[])
{
 	if(!za[playerid].Log)return 1;
	if((time(NULL)-za[playerid].CMD) < 1)
	{
		GameTextForPlayer(playerid, "~g~Wait 1 second!", 1000, 5);
		return 1;
	}
	za[playerid].CMD=time(NULL);

	if(!strcasecmp("/cureall", cmdtext))
	{
		if(za[playerid].UCID != 33)return SendClientMessage(playerid, 0xFFFF00AA , ">> You are not a senior medic!");
		if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)return 1;
		if(temp[playerid].Antidote > 14)return SendClientMessage(playerid,0xFFFF00AA,">> Oppss! There are no antidotes anymore.");
		if(temp[playerid].CureTime > time(NULL))return SendClientMessage(playerid,0xFFFF00AA,">> You are still preparing..");
	
		float x, y, z;	int count=0;
		GetPlayerPos(playerid, &x, &y, &z);
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].UCID == -38 && IsPlayerInRangeOfPoint(i, 25.0, x, y, z))
			{
				return 1;
			}
		}
		
		for(int i=0; i < AMOUNT; i++)
		{
	    	if(temp[i].Infection &&	za[i].Log == true)
	    	{
				if(IsPlayerInRangeOfPoint(i, 12.0, x, y, z))
				{
					if(temp[i].HumanRogue == false)SetPlayerColor(i, 0x00CCCCAA);
					
					TextDrawHideForPlayer(i, t_Red);
					SetPlayerDrunkLevel(i, 0);
					temp[i].Infection=false;
					temp[i].Slow=false;
					temp[i].TransferInfection=0;
					count++;
				}
				
			}
		}
		
		if(count == 0)return SendClientMessage(playerid,0xFFFF00AA,">> No infected players in range!");

		ApplyAnimation(playerid, "MEDIC", "CPR", 1.5, 0, 0, 0, 0, 0, 0);
		char str[128];
		snprintf(str, sizeof(str), ">> Senior medic %s has cured %d people", za[playerid].Name, count);
		SendClientMessageToAll(0xADFF2FAA, str);
		temp[playerid].Antidote++;
		temp[playerid].CureTime=time(NULL)+4;
		
		GameTextForPlayer(playerid, "~n~~n~~n~~y~+5~y~~n~~n~~n~", 1000, 5);
		SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
		return 1;
	}
	if(!strcasecmp("/class", cmdtext))
	{
		ShowPlayerDialog(playerid, DIALOG_CLASS, 2,"Classes", "Humans\' Classes\nZombies\' Classes", "Ok", "Cancel");
		return 1;
	}
	if(!strcasecmp("/xpmult", cmdtext))
	{
		char str[128];
		snprintf(str, sizeof(str), "Mult: %d", xpmult);
		SendClientMessage(playerid, -1, str);
		return 1;
	}
	if(!strcasecmp("/shop", cmdtext))
	{
		ShowPlayerDialog(playerid, DIALOG_SHOP, 2, "Shop", "Smoke Machine - 40 XP\nAnti-Seeker - 50 XP\nAnti-Freezer - 90 XP\nCamera - 5 XP\nArmour - 100 XP\nAnti-Burning - 80 XP\nMorphine - 90 EXP (No Leg Breaks)\nAnti-Screamer - 10 EXP\nAnti-Blind - 80 EXP\nHelmet - 10 EXP (No headshots)", "Buy", "Cancel");
		return 1;
	}
	if(strcasecmp("/maps", cmdtext) == 0)
	{
	    ShowPlayerDialog(playerid, DIALOG_MAP, 2, "Maps", MapList, "Ok", "Cancel");
		return 1;
	}
	if(strcasecmp("/weapons", cmdtext) == 0)
	{
		ShowPlayerDialog(playerid, DIALOG_NOANSWER, 0, "Weapons", 
		"Grenade\t\t\tID:16\n9mm\t\t\t\tID:22\nSilenced 9mm\t\t\tID:23\nDesert Eagle\t\t\tID:24\nShotgun\t\t\tID:25\nSawnoff Shotgun\t\tID:26\nCombat Shotgun\t\tID:27\nAK-47\t\t\t\tID:30\nM4\t\t\t\tID:31\nCountry Rifle\t\t\tID:33\nSniper Rifle\t\t\tID:34\nRPG\t\t\t\tID:35\nMinigun\t\t\tID:38\nSatchel Charge\t\tID:39",
		"Ok", "");
		return 1;
	}
	if(strcasecmp("/cmds", cmdtext) == 0)
	{
		SendClientMessage(playerid,0xC0C0C0AA,"/help /class /stats /cure(all) /report /votekick /rules /pm /r /shop /admins /unstuck");
		SendClientMessage(playerid,0xC0C0C0AA,"/changename /changepassword /animlist /off(switch music off) /settings /enter /leave");
		SendClientMessage(playerid,0xC0C0C0AA,"/knockback /viplist /donate /weapons /maps /att /saveatt /duel /skin /radio");
		return 1;
	}
	if(strcasecmp("/help", cmdtext) == 0)
	{
		ShowPlayerDialog(playerid, DIALOG_NOANSWER, 0, "Help",
		"{EC3B83}If you are a human, your task to survive and evacuate. (Humans are blue)\n{33CCFF}If you are a zombie, your task to kill all humans. (Zombies are green)\n{FFFFFF} - How to be Human?\n{B0B0B0}Answer : It\'s random every round\n{FFFFFF} - How to see my Stats ?\n{B0B0B0}Answer : By following this cmd /stats or Press Tab\n{FFFFFF} - How to earn XP ?\n{B0B0B0}Answer : Kill your opponent team and get xp.\n{FFFFFF} - How to become Admin ?\n{B0B0B0}Answer : Be Active, Don't break the rules, and Apply for Admin at forum\n\n{FFFFFF}View also: /cmds",
		"Ok", "");
		return 1;
	}
	if(strcasecmp("/donate", cmdtext) == 0)
	{
		ShowPlayerDialog(playerid, DIALOG_NOANSWER, 0, "How to donate",
		"1. Check the thread \"Donations\" in the Announcements & Information section in the forum.\n2. Go To www.sampdm.net/payment.php", 
		"Ok", "");
		return 1;
	}
	if(strcasecmp("/radio", cmdtext) == 0)
	{
		SendClientMessage(playerid, 0xC0C0C0AA,">> Radio has been switched on! Use {FF0000}/off {FFFFFF}to switch it off!");
		PlayAudioStreamForPlayer(playerid, "http://ep128server.streamr.ru:8030/ep128", 0.0, 0.0, 0.0, 0.0, 0);
		return 1;
	}
	if(strcasecmp("/off", cmdtext) == 0)
	{
		SendClientMessage(playerid, 0xC0C0C0AA,">> Music has been disabled! {FF0000}Off.");
		StopAudioStreamForPlayer(playerid);
		return 1;
	}
	if(strcasecmp("/rules", cmdtext) == 0)
	{
		ShowPlayerDialog(playerid, DIALOG_NOANSWER, 0, "Rules.",
		"1.1. Players automatically agree with the server rules when they join the server.\n\n \
{FFFFFF}You {FF0000}MAY NOT{FFFFFF}:\n \
3.2. You may not use any cheats, hacks.\n \
3.3. You are not allowed to abuse map/character/server/any-other GTA or SAMP bug(s) to gain any advantage or ruin players' gameplay.\n \
3.4. You may not lie/trick/blackmail/impersonate others to gain any profit or anything other.\n \
3.5. You may not spam, flood, insult, provoke or annoy someone in any language, directly or indirectly.\n \
3.8. You may not help any human as zombie and vice versa by any means intentionally. (by animation blocking, AFK and so on)\n \
3.9. You may not block a path to stop the movement of your team by any means. (Using animations, boxes, standing or using a vehicle)\n \
3.13. You may not launder EXP earned by rule-violation by paying them to someone, depositing them or by any other means.\n \
3.11.1. Don't spawn kill or spawn damage.\n \
3.11.2. Don't camp near the zombie spawn area.\n \
3.11.3. Don't leave the map.\n \
3.11.5. Don't kill or damage your own team by any means.\n \
\n \
You {00FF00}MAY{FFFFFF}:\n \
4.2. You may block a path to stop the movement of the opposite team without violating rule 3.9.\n \
4.3. You can ask for EXP without violating rule 3.5.\n \
4.4. Zombies can run back to zombie spawn positions.\n \
4.6. You are allowed to use bad words if you are not insulting someone even indirectly.\n \
4.11. You may jump before using LALT by any zombie class. (BH as demon and etc)\n \
4.12. You may shoot from zombie spawn positions as an armed zombie. (Spitter Zombie Abusing and etc)\n \
\n\nFull Rules In The Thread \"The Server Rules\" at the forum!",
		"Ok", "");
		return 1;
	}
	if(!strcasecmp("/settings", cmdtext))
	{
		char str[200];
		char *yes="{00FF00}YES{FFFFFF}";
		char *no="{FF0000}NO{FFFFFF}";
		snprintf(str, sizeof(str), "Block PMs\t\t\t\t[%.19s]\nDing\t\t\t\t\t[%.19s]\nAuto-Login\t\t\t\t[%.19s]\nBlock Duel Requests\t\t\t[%.19s]\n\"Ability Is Ready\" Sound\t\t[%.19s]", ((za[playerid].Settings)&0x1) ? yes:no, ((za[playerid].Settings)&0x2) ? no:yes, ((za[playerid].Settings)&0x4) ? no:yes, ((za[playerid].Settings)&0x8) ? yes:no, ((za[playerid].Settings)&0x10) ? no:yes);
	
		ShowPlayerDialog(playerid, DIALOG_SETTINGS, 2, "Settings", str, "Ok", "Cancel");
		return 1;
	}
	if(strcasecmp("/hide", cmdtext) == 0)
	{
		if(za[playerid].Admin > 0)
		{
			if(za[playerid].Hide == false)
			{
				za[playerid].Hide=true;
				SendClientMessage(playerid, COLOR_ANTIHACK, "You're unhidden.");
			}
			else
			{
				za[playerid].Hide=false;
				SendClientMessage(playerid, COLOR_ANTIHACK, "You're hidden.");
			}
		}
		return 1;
	}
	if(!strcasecmp("/admins", cmdtext))
	{
		if(za[playerid].Admin > 0)
		{
			SendClientMessage(playerid, COLOR_ANTIHACK, "Online Admins:");
		
			int C=0;
			for(int i= 0; i < AMOUNT; i++)
			{
				if(za[i].Admin > 0)
				{
					char mess[128];
					snprintf(mess, sizeof(mess), "> %s (Level %d)%s%s%s", za[i].Name, za[i].Admin, (za[i].AdminDuty == true) ? " - {FF4000}ON DUTY" : "",(za[i].Unconnected == true) ? " - {FFFF00}UNCONNECTED" : "",(GetPlayerState(i) == PLAYER_STATE_SPECTATING) ? " - {00FF00}SPECTATING" : "");
					SendClientMessage(playerid, -1, mess);
					C++;
				}
			}
			if (C == 0) SendClientMessage(playerid,0xDC0000FF,"There are no admins online!");
			SendClientMessage(playerid, -1, "--------------------------------------");
		}
		else
		{
			SendClientMessage(playerid, COLOR_ANTIHACK, "Online Admins:");
			SendClientMessage(playerid, 0xDC0000FF, "There are no admins online!");
			SendClientMessage(playerid, -1, "--------------------------------------");
		}
		return 1;
	}
	if(!strcasecmp("/knockback", cmdtext))
	{
	    if(za[playerid].VIP == 3)
	    {
			if(za[playerid].KnockBack == true)
			{
			    za[playerid].KnockBack=false;
			    SendClientMessage(playerid, COLOR_GRAYWHITE, "Knockback {ff0000}disabled!");
			}
			else if(za[playerid].KnockBack == false)
			{
			    za[playerid].KnockBack=true;
			    SendClientMessage(playerid, COLOR_GRAYWHITE, "Knockback {00ff00}enabled!");
			}
		}
		return 1;
	}
	if(!strcasecmp("/jetpack", cmdtext))
	{
		if(za[playerid].Admin < 2)return SendClientMessage(playerid, -1, "You have no right to use it.");
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
		return 1;
	}
	if(!strcasecmp("/saveexp", cmdtext))
	{
		if(za[playerid].Admin != 6)return 0;
		for(int i=0; i < AMOUNT; i++)
		{
			Save(i);

			if(za[i].Admin)
			{			
				char str[128];
				snprintf(str, sizeof(str), "UPDATE AA SET Day%d=Day%d+? WHERE ID=? AND Week=?", CurrentDay, CurrentDay);
		
				sqlite3_stmt *stmt;
		    	sqlite3_prepare_v2(logs_db, str, 47, &stmt, NULL);
		    	sqlite3_bind_int(stmt, 1, time(NULL)-AA[i].Time-AA[i].AFK);
				sqlite3_bind_int(stmt, 2, za[i].AccountID);
				sqlite3_bind_int(stmt, 3, CurrentWeek);
					
				if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(logs_db) == 0)
				{
					snprintf(str, sizeof(str), "INSERT INTO AA (ID, Time, Week, Day%d) VALUES (?, ?, ?, ?)", CurrentDay);
		
					sqlite3_stmt *stmt2;
			    	sqlite3_prepare_v2(logs_db, str, 57, &stmt2, NULL);
			    	sqlite3_bind_int(stmt2, 1, za[i].AccountID);
			    	sqlite3_bind_int(stmt2, 2, time(NULL));
			    	sqlite3_bind_int(stmt2, 3, CurrentWeek);
			    	sqlite3_bind_int(stmt2, 4, time(NULL)-AA[i].Time-AA[i].AFK);
					sqlite3_step(stmt2);
					sqlite3_finalize(stmt2);
				}
				sqlite3_finalize(stmt);
			
				AA[i].AFK=0;
				AA[i].Time=time(NULL);
			}
		}

		SendClientMessage(playerid, -1, ">> Done!");
		return 1;
	}	
	if(!strcasecmp("/acmds", cmdtext))
	{
		if(za[playerid].Admin > 0)
		{
			SendClientMessage(playerid,0xC0C0C0AA,"Level 1: /mute /cc /atext /aduty /hide");//Helper
			if(za[playerid].Admin > 1)SendClientMessage(playerid,0xC0C0C0AA,"Level 2: /kick /tban /warn /slap /mslap /spec /jetpack");//Trial Moderator
			
			if(za[playerid].Admin > 2)SendClientMessage(playerid,0xC0C0C0AA,"Level 3: /timeout /goto /explode /ostats");
			if(za[playerid].Admin > 2)SendClientMessage(playerid,0xC0C0C0AA,"Level 3: /ban /mz /getinfo /otban /unconnect");
			
			if(za[playerid].Admin > 3)SendClientMessage(playerid,0xC0C0C0AA,"Level 4: /(un)freeze /ann /mh /unban /veh /newround /music");//Manager
			if(za[playerid].Admin > 3)SendClientMessage(playerid,0xC0C0C0AA,"Level 4: /time /weather /unbanip /banname /get /banip ");
			
			if(za[playerid].Admin > 4)SendClientMessage(playerid,0xC0C0C0AA,"Level 5: /makeadmin /makeadminname /exp /fine /adminlist /baniprange");
			if(za[playerid].Admin > 4)SendClientMessage(playerid,0xC0C0C0AA,"Level 5: /announce_event /start_event /end_event /freezeall /unfreezeall /getall");
			
			if(za[playerid].Admin == 6)SendClientMessage(playerid,0xC0C0C0AA, "Level 6: /saveexp");
		}
		return 1;
	}
	if(!strcasecmp("/specoff", cmdtext))
	{
		if(za[playerid].Admin)
		{
			TogglePlayerSpectating(playerid, 0);
			za[playerid].Spec=0;
			
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Health);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Ping);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Team);
			PlayerTextDrawHide(playerid, za[playerid].TDs.t_Weapons);
		}
		else
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	if(!strcasecmp("/cc", cmdtext))
	{
		if(za[playerid].Admin == 0)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");

		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && za[i].Admin == 0)
			{
				for(int k=0; k < 95; k++)SendClientMessage(i, -1, " ");	
			}
		}
		
		char str[128];
		snprintf(str, sizeof(str), "The chat was cleared by %s.", za[playerid].Name);
		SendClientMessageToAll(0xFFFF00AA, " ");
		SendClientMessageToAll(0xFFFF00AA, str);
		return 1;
	}
	if(!strcasecmp("/aduty", cmdtext))
	{
		if(za[playerid].Admin < 1)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(za[playerid].AdminDuty == false)
		{
			ResetPlayerWeapons(playerid);
			za[playerid].Team=true;
			za[playerid].UCID=INVALID_CLASS_ID;
			za[playerid].ClassDesc=NoClass;
			za[playerid].AdminDuty=true;
			za[playerid].Killstreaks=0;
			SendClientMessage(playerid,0xFFFF00AA, "AdminDuty has been switched {00FF00}On.");
			SetPlayerSkin(playerid, 217);
			TogglePlayerControllable(playerid, 1);
			SetPlayerArmour(playerid, 0.0);
			SetPlayerHealth(playerid, 700.0);
			SetPlayerColor(playerid, 0x99CC0000);
			
			for(int i=0; i < 10; i++)RemovePlayerAttachedObject(playerid, i);
			
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true)
				{
					if(!za[i].Admin)ShowPlayerNameTagForPlayer(i, playerid, false);
					else SetPlayerMarkerForPlayer(i, playerid, 0xFF0000FF);
				}
			}
		}
		else
		{
			za[playerid].AdminDuty=false;
			SendClientMessage(playerid,0xFFFF00AA, "AdminDuty has been {FF0000}disabled");
			SpawnPlayer(playerid);
		}
		return 1;
	}
	if(!strcasecmp("/viplist", cmdtext))
	{
		int C=0; char str[128];
		SendClientMessage(playerid, COLOR_ANTIHACK, "Online Vips:");
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].VIP > 0)
			{
				snprintf(str, sizeof(str), "> %s (Level %d)", za[i].Name, za[i].VIP);
				SendClientMessage(playerid, -1, str);
				
				C++;
			}
		}
		if (C == 0) SendClientMessage(playerid,0xDC0000FF,"There are no vips online!");
		SendClientMessage(playerid, -1, "--------------------------------------");
		return 1;
	}
	if(!strcasecmp("/pmspy", cmdtext))
	{
		if(za[playerid].Admin < 5)return 1;
		
		if(za[playerid].PMSPY == true)
		{
			za[playerid].PMSPY=false;
			SendClientMessage(playerid,0xFFFF00AA, "PMSPY has been{FF0000} disabled");
		} else
		{
			za[playerid].PMSPY=true;
			SendClientMessage(playerid,0xFFFF00AA, "PMSPY has been {00FF00}enabled");
		}
		return 1;
	}
	if(!strcasecmp("/unstuck", cmdtext))
	{
		if(temp[playerid].Unstucked == false)
		{
			if(za[playerid].Team == true)
			{
				float x, y, z;
				GetPlayerVelocity(playerid, &x, &y, &z);
				if(za[playerid].Duel == false && za[playerid].InEvent == false && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && (z < 0.1 && z > -0.1))
				{
					if(EvacTime > -20)
					{
						TogglePlayerControllable(playerid, 0);
						SetCameraBehindPlayer(playerid);
						SendClientMessage(playerid,-1,">> You will be spawned in {FF0000}15 {FFFFFF}seconds!");
						PlayerPlaySound(playerid, 1056);
						SetTimerEx("Unstuck", 15000, playerid);
						temp[playerid].Unstucked=true;
					}
					else
					{
						SendClientMessage(playerid,0xC0C0C0AA,">> Wait for a new round!");
					}
				}
				else
				{
					SendClientMessage(playerid,0xC0C0C0AA,">> You may not use it now!");
				}
			}
			else
			{
				SendClientMessage(playerid,0xC0C0C0AA,">> Only for zombies!");
			}
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You may not use it anymore until new round!");
		}
		return 1;
	}
	if(!strcasecmp("/yes", cmdtext))
	{
		if(Votekick.VoteOpen == false)return SendClientMessage(playerid,0xFFFF00AA,">> There is no votekick!");
		if(Votekick.Vote[playerid] == true)return SendClientMessage(playerid,0xFFFF00AA,">> You've already voted!");

		Votekick.Vote[playerid]=true;		
		Votekick.VoteTime=time(NULL)+50;
		Votekick.Counted++;
		
		char str[128];
		snprintf(str, sizeof(str), ">> %s voted for a kick (%d/8)", za[playerid].Name, Votekick.Counted);
		SendClientMessageToAll(0xFFFF00AA, str);
		
		if(Votekick.Counted == 8)
		{
			snprintf(str, sizeof(str), ">> %s has been votekicked.", za[Votekick.ToBeKicked].Name);
			SendClientMessageToAll(0xFFFF00AA, str);
			
			sqlite3_stmt *stmt;
		    sqlite3_prepare_v2(db, "UPDATE Data SET Time=? WHERE ID=?", 33, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, 300+(int)time(NULL));
			sqlite3_bind_int(stmt, 2, za[Votekick.ToBeKicked].AccountID);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			
			Votekick.VoteOpen=false;
			SetTimerEx("PlayerKick", 500, Votekick.ToBeKicked);
			Votekick.TimeLimit=time(NULL);
			BlockIpAddress(za[Votekick.ToBeKicked].IP, 290000);
		}
		return 1;
	}
	if(!strcasecmp("/no", cmdtext))
	{
		if(Votekick.VoteOpen == false)return SendClientMessage(playerid,0xFFFF00AA,">> There is no votekick!");
		if(Votekick.Vote[playerid] == true)return SendClientMessage(playerid,0xFFFF00AA,">> You already voted!");

		Votekick.Vote[playerid]=true;		
		Votekick.VoteTime=time(NULL)+50;
		if(Votekick.Counted > 0)Votekick.Counted--;
		Votekick.VoteNO++;
		
		char str[128];
		snprintf(str, sizeof(str), ">> %s voted against a kick (%d/8)",  za[playerid].Name, Votekick.Counted);
		SendClientMessageToAll(0xFFFF00AA, str);
		
		if(Votekick.Counted == 0 && Votekick.VoteNO > 1)
		{
			Votekick.VoteOpen=false;
			SendClientMessageToAll(0xFFFF00AA,">> Votekicked has been aborted.");
			Votekick.TimeLimit=time(NULL);
		}
		return 1;
	}
	if(!strcasecmp("/y", cmdtext))
	{
		if(za[playerid].DuelInviteID == -1) return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You aren't invited!");
		if(!IsPlayerConnected(za[playerid].DuelInviteID) || za[za[playerid].DuelInviteID].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> He has left the server!");
		if(za[za[playerid].DuelInviteID].Duel == true)return SendClientMessage(playerid,0xC0C0C0AA,">> He is already there! You're late!");
		if(za[playerid].Duel == true)return SendClientMessage(playerid,0xC0C0C0AA,">> You can't!");
		if(EvacTime < -40)return SendClientMessage(playerid,0xC0C0C0AA,">> Wait for a new round!");
		if(GetPlayerState(playerid) == 7)return SendClientMessage(playerid,0xC0C0C0AA,">> You're dead! Try again when you are alive.");
		if(GetPlayerState(za[playerid].DuelInviteID) == 7)return SendClientMessage(playerid,0xC0C0C0AA,">> He's dead! Try again a bit later.");

		TextDrawHideForPlayer(playerid, t_Red);
		TextDrawHideForPlayer(playerid, t_Black);
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
		SetPlayerDrunkLevel(playerid, 0);

		TextDrawHideForPlayer(za[playerid].DuelInviteID, t_Red);
		TextDrawHideForPlayer(za[playerid].DuelInviteID, t_Black);
		TextDrawHideForPlayer(za[playerid].DuelInviteID, t_Blue);
		TextDrawHideForPlayer(za[playerid].DuelInviteID, t_Yellow);
		TextDrawHideForPlayer(za[playerid].DuelInviteID, t_Magenta);

		temp[za[playerid].DuelInviteID].Infection=false;
		temp[za[playerid].DuelInviteID].Slow=false;
		temp[za[playerid].DuelInviteID].Leg=false;
		temp[za[playerid].DuelInviteID].TransferInfection=0;
		temp[za[playerid].DuelInviteID].Burned=false;
		temp[za[playerid].DuelInviteID].HumanRogue=false;
		temp[za[playerid].DuelInviteID].Poison=0;
		SetPlayerDrunkLevel(za[playerid].DuelInviteID, 0);

		if(za[playerid].Team == false)za[playerid].Killstreaks=0;
		if(za[za[playerid].DuelInviteID].Team == false)za[za[playerid].DuelInviteID].Killstreaks=0;
	
		za[playerid].AdminDuty=false;
		za[za[playerid].DuelInviteID].AdminDuty=false;
	
		za[playerid].Duel=true;
		za[za[playerid].DuelInviteID].Duel=true;

		za[playerid].DuelFalsePositive=true;
		za[za[playerid].DuelInviteID].DuelFalsePositive=true;
	
		za[playerid].Team=true;
		za[za[playerid].DuelInviteID].Team=true;
	
		za[playerid].UCID=INVALID_CLASS_ID;
		za[playerid].ClassDesc=NoClass;
		za[za[playerid].DuelInviteID].UCID=INVALID_CLASS_ID;
		za[za[playerid].DuelInviteID].ClassDesc=NoClass;
		
		char WeaponName[32];
		GetWeaponName(za[playerid].DuelInviteWID, WeaponName, 32);
	
		char str[128];
		snprintf(str, sizeof(str), ">> %s started a duel with %s. Weapon: %s (ID %d)", za[za[playerid].DuelInviteID].Name, za[playerid].Name, WeaponName, za[playerid].DuelInviteWID);
		SendClientMessageToAll(0xFFE4B5AA, str);
	
		SetPlayerInterior(playerid, 1);
		SetPlayerInterior(za[playerid].DuelInviteID, 1);
	
		SetPlayerVirtualWorld(playerid, playerid);
		SetPlayerVirtualWorld(za[playerid].DuelInviteID, playerid);
	
		SetPlayerPos(playerid, 1359.7341, -19.3410, 1000.9219);
		SetPlayerFacingAngle(playerid, 270.0);
	
		SetPlayerPos(za[playerid].DuelInviteID, 1418.4573, -19.2926, 1000.9272);
		SetPlayerFacingAngle(za[playerid].DuelInviteID, 90.0);
	
		ResetPlayerWeapons(playerid);
		ResetPlayerWeapons(za[playerid].DuelInviteID);
	
		GivePlayerWeapon(playerid, za[playerid].DuelInviteWID, 3000);
		GivePlayerWeapon(za[playerid].DuelInviteID, za[playerid].DuelInviteWID, 3000);
	
		SetPlayerTeam(playerid, 0);
		SetPlayerTeam(za[playerid].DuelInviteID, 1);
	
		SetPlayerSkin(playerid, 217);
		TogglePlayerControllable(playerid, 1);
	
		SetPlayerSkin(za[playerid].DuelInviteID, 217);
		TogglePlayerControllable(za[playerid].DuelInviteID, 1);
	
		SetPlayerHealth(playerid, 99.0);
		SetPlayerArmour(playerid, 50.0);
		SetPlayerColor(playerid, COLOR_INDIGO);
	
		SetPlayerHealth(za[playerid].DuelInviteID, 99.0);
		SetPlayerArmour(za[playerid].DuelInviteID, 50.0);
		SetPlayerColor(za[playerid].DuelInviteID, COLOR_INDIGO);
	
		za[playerid].DuelVictimID=za[playerid].DuelInviteID;
		za[za[playerid].DuelInviteID].DuelVictimID=playerid;
	
		za[playerid].DuelInviteTime=0;
		za[playerid].DuelInviteID=-1;
		za[playerid].DuelInviteWID=0;	
		return 1;
	}
	if(!strcasecmp("/n", cmdtext))
	{
		if(za[playerid].DuelInviteID == -1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You aren't invited!");
		
		char str[128];
		snprintf(str, sizeof(str), ">> %s has denied the duel with you!", za[playerid].Name);
		SendClientMessage(za[playerid].DuelInviteID, 0xFFE4B5AA, str);
		SendClientMessage(playerid, 0xFFE4B5AA, ">> You have denied the duel!");
		
		za[playerid].DuelInviteTime=0;
		za[playerid].DuelInviteID=-1;
		za[playerid].DuelInviteWID=0;	
		return 1;
	}


	if(!strcasecmp("/announce_event", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		
		if(Event.IsEvent == false)
		{
			Event.IsEvent=true;
			Event.Closed=false;
			float x, y, z, fa;
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &fa);
			Event.Spawn[0]=x;
			Event.Spawn[1]=y;
			Event.Spawn[2]=z;
			Event.Spawn[3]=fa;

			SendClientMessageToAll(-1, "\"----------------------------------------------\"");
			SendClientMessageToAll(0x9be56dAA, ">> {2388d6}The event {9be56d}is going to be started.  {FFFFFF}Use {FFA500}/enter {FFFFFF}to join to the event!");
			SendClientMessageToAll(-1, "\"----------------------------------------------\"");
			
			SendClientMessage(playerid, -1, ">> Use {FF0000}/announce_event {FFFFFF}again to reset the spawn positions!");
			SendClientMessage(playerid, -1, ">> Use {FF0000}/start_event {FFFFFF} to close entry for players!");			
			SendClientMessage(playerid, -1, ">> Use {FF0000}/end_event {FFFFFF}to finish the event!");
		
			za[playerid].InEvent=true;
			SpawnPlayer(playerid);
		}
		else
		{
			float x, y, z, fa;
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &fa);
			Event.Spawn[0]=x;
			Event.Spawn[1]=y;
			Event.Spawn[2]=z;
			Event.Spawn[3]=fa;
			SendClientMessage(playerid, -1, ">> The spawn positions were reset!");	
		}		
		return 1;
	}
	if(!strcasecmp("/start_event", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You should announce an event to start it!");
		if(Event.Closed == true)return SendClientMessage(playerid,0xC0C0C0AA,">> The event is closed already!");

		Event.Closed=true;

		SendClientMessageToAll(-1, "\"----------------------------------------------\"");
		SendClientMessageToAll(0x9be56dAA, ">> {2388d6}The event {9be56d}has been started!!");
		SendClientMessageToAll(-1, "\"----------------------------------------------\"");
		return 1;		
	}
	if(!strcasecmp("/end_event", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You should start an event to finish it!");
		
		Event.IsEvent=false;
		SendClientMessageToAll(-1, "\"----------------------------------------------\"");
		SendClientMessageToAll(0x9be56dAA, ">> {2388d6}The event {9be56d}has been finished!");
		SendClientMessageToAll(-1, "\"----------------------------------------------\"");
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].InEvent == true)
			{
				za[i].InEvent=false;
				SpawnPlayer(i);
			}
		}
		return 1;
	}
	if(!strcasecmp("/enter", cmdtext))
	{
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> There's no an event for now!");
		if(Event.Closed == true)return SendClientMessage(playerid,0xC0C0C0AA,">> You may not enter anymore!");
		if(za[playerid].InEvent == true)return SendClientMessage(playerid,0xC0C0C0AA,">> You have already joined!");
		
		za[playerid].InEvent=true;
		SpawnPlayer(playerid);
		
		char str[128];
		snprintf(str, sizeof(str), ">> {FF0000}%s {9be56d}has joined to {2388d6}the event!", za[playerid].Name);
		SendClientMessageToAll(0x9be56dAA, str);
		SendClientMessage(playerid, -1, ">> Use {FF0000}/leave {FFFFFF}to leave {2388d6}the event");
		return 1;
	}
	if(!strcasecmp("/leave", cmdtext))
	{
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> There's no an event for now!");
		if(za[playerid].InEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You have not joined to leave!");
		za[playerid].InEvent=false;
		SpawnPlayer(playerid);
		
		char str[128];
		snprintf(str, sizeof(str), ">> {FFFF00}%s {9be56d}has left to {2388d6}the event!", za[playerid].Name);
		SendClientMessageToAll(0x9be56dAA, str);
		return 1;
	}
	if(!strcasecmp("/freezeall", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> There's no an event for now!");
		if(Event.Closed == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You should start an event to freeze players!");
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].InEvent == true && i != playerid)
			{
				TogglePlayerControllable(i, 0);
				GameTextForPlayer(i, "~b~frozen..", 1000, 5);
			}
		}

		SendClientMessageToAll(COLOR_ANTIHACK, ">> Event players have been frozen!");
		return 1;
	}
	if(!strcasecmp("/unfreezeall", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> There's no an event for now!");
		if(Event.Closed == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You should start an event to freeze players!");
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].InEvent == true)
			{
				TogglePlayerControllable(i, 1);
			}
		}

		SendClientMessageToAll(COLOR_ANTIHACK, ">> Event players have been unfrozen!");
		return 1;
	}
	if(!strcasecmp("/getall", cmdtext))
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(Event.IsEvent == false)return SendClientMessage(playerid,0xC0C0C0AA,">> There's no an event for now!");
		if(Event.Closed == false)return SendClientMessage(playerid,0xC0C0C0AA,">> You should start an event to freeze players!");
		
		float x, y, z, fa;
		GetPlayerPos(playerid, &x, &y, &z);
		GetPlayerFacingAngle(playerid, &fa);
		x+=2.0*cos(((fa+90.0)/180)*3.14159265358979);
		y+=2.0*sin(((fa+90.0)/180)*3.14159265358979);
		fa+=180.0;
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].InEvent == true && i != playerid)
			{
				SetPlayerPos(i, x, y, z+0.8);
				SetPlayerFacingAngle(i, fa);
			}
		}
		return 1;
	}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
	if(!strcasecmp(cmdtext, "/gang help"))
	{
		SendClientMessage(playerid,0xADFF2FAA,"+-- GANG HELP --+");
		SendClientMessage(playerid,-1,"/gang create [name] - Create a gang for 70,000 XP");
		SendClientMessage(playerid,-1,"/gang tag [tag] - Set clan's tag");
		SendClientMessage(playerid,-1,"/gang name [name] - Set clan's name for 10,000 XP");
		SendClientMessage(playerid,-1,"/gang rankname [1-6] [Name] - Set Rank Name");
		SendClientMessage(playerid,-1,"/gang join [Gang ID] - Request to join a gang");
		SendClientMessage(playerid,-1,"/gang promote/demote [ID] - Demote/promote gang member");
		SendClientMessage(playerid,-1,"/gang leave - Leave Gang");
		SendClientMessage(playerid,-1,"/gang list - List gangs to join");
		SendClientMessage(playerid,-1,"/gang info - To get info about gang");
		SendClientMessage(playerid,-1,"/gang ranks - List ranks in gang");
		SendClientMessage(playerid,-1,"/gang pay - Pay gang members");
		SendClientMessage(playerid,-1,"/gang odemote - offline demote/fire");
		SendClientMessage(playerid,-1,"/gang members - member list");
		SendClientMessage(playerid,-1,"/gang deposit - deposit EXP.");
		SendClientMessage(playerid,-1,"/gang setleader - to set 6 rank.");

		SendClientMessage(playerid,0xC0C0C0AA,"+---------------+");
		return 1;
	}
	if(!strcasecmp(cmdtext, "/gang ranks"))
	{
		int gang=za[playerid].Clan;
		if(gang < 1 || gang > (GANGS-1))return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");
		if(Gang[gang].IsCreated == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");
		SendClientMessage(playerid,0xADFF2FAA,"+--- RANKS ---+");
		char str[128];
		snprintf(str, sizeof(str), ">> Rank 1 >> %s", Gang[gang].Rank1);
		SendClientMessage(playerid, -1, str);
		snprintf(str, sizeof(str), ">> Rank 2 >> %s", Gang[gang].Rank2);
		SendClientMessage(playerid, -1, str);
		snprintf(str, sizeof(str), ">> Rank 3 >> %s", Gang[gang].Rank3);
		SendClientMessage(playerid, -1, str);
		snprintf(str, sizeof(str), ">> Rank 4 >> %s", Gang[gang].Rank4);
		SendClientMessage(playerid, -1, str);
		snprintf(str, sizeof(str), ">> Rank 5 >> %s", Gang[gang].Rank5);
		SendClientMessage(playerid, -1, str);
		snprintf(str, sizeof(str), ">> Rank 6 (LEADER) >> %s", Gang[gang].Rank6);
		SendClientMessage(playerid, -1, str);
		SendClientMessage(playerid,-1,"+----------------+");
		return 1;
	}
	if(!strcasecmp(cmdtext, "/gang list"))
	{
		SendClientMessage(playerid,0xADFF2FAA,"+------ GANGS ------+");
		for(int i=1; i < GANGS; i++)
		{
			if(Gang[i].IsCreated == true)
			{
			   	char str[128];
			   	snprintf(str, sizeof(str), "(ID %d) >> %s (%s) - XP POT : %d", i, Gang[i].Name, Gang[i].Tag, Gang[i].Score);
			   	SendClientMessage(playerid, -1, str);
			}
		}
		SendClientMessage(playerid, 0xADFF2FAA, "+------------------+");
		return 1;
	}
	rcmd("/gang info", 10)
	{
		int gid;
		if(sscanf((char *)&cmdtext[10], "%d", &gid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang info [ID]");
		if(gid < 1 || gid > (GANGS-1))return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");
		if(Gang[gid].IsCreated == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");

		char str[128];
		snprintf(str, sizeof(str), "+--- %s (%d XP) ----------+", Gang[gid].Name, Gang[gid].Score);
		SendClientMessage(playerid, 0xADFF2FAA, str);
		snprintf(str, sizeof(str), "{D3D3D3}Gangwar wins: %d - loses: %d", Gang[gid].Wins, Gang[gid].Losses);
		SendClientMessage(playerid, -1, str);
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Clan == gid)
			{
				snprintf(str, sizeof(str), "(Rank %d) >> %s (ID %d)", za[i].Rank, za[i].Name, i);
				SendClientMessage(playerid, -1, str);
			}
		}
		return 1;
	}
	rcmd("/gang tag", 9)
	{	
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		if(za[playerid].Rank < 5) return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're not the gangleader!");
		if(cmdtext[9] == 0)return SendClientMessage(playerid, 0xC0C0C0AA, "[Server] Usage : /gang tag [tag]");
		char *tag=&cmdtext[10];
		int len=strlen(tag);
		if(len > 4 || len < 1) return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid Length (1-4 chars)");

		SendClientMessage(playerid, -1, "Tag Changed.");
		strncpy(Gang[gid].Tag, tag, 4);

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Gangs SET Tag=? WHERE ID=?", 33, &stmt, NULL);
   		sqlite3_bind_text(stmt, 1, tag, len, NULL);
		sqlite3_bind_int(stmt, 2, gid);
    	sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/gang name", 10)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		if(za[playerid].Rank < 6)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're not the gangleader!");
		if(GetPlayerScore(playerid) < 10000)return SendClientMessage(playerid, 0xC0C0C0AA, "You need atleast 10000 XP to use this command");
		if(cmdtext[10] == 0)return SendClientMessage(playerid, 0xC0C0C0AA, "[Server] Usage : /gang name [name]");
		char *name=&cmdtext[11];
		int len=strlen(name);
		if(len > 31 || len < 2) return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid Length (2-31 chars)");
		
		strncpy(Gang[gid].Name, name, 31);

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Gangs SET Name=? WHERE ID=?", 34, &stmt, NULL);
   		sqlite3_bind_text(stmt, 1, name, len, NULL);
		sqlite3_bind_int(stmt, 2, gid);
    	sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		SendClientMessage(playerid, -1, "Name Changed. (-10,000 XP)");
		SetPlayerScore(playerid, GetPlayerScore(playerid)-10000);
		return 1;
	}
	rcmd("/gang rankname", 14)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		if(za[playerid].Rank < 6)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're not the gangleader!");	
		int rankid; char name[128];
		if(sscanf((char *)&cmdtext[14], formatDS, &rankid, name) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /gang rankname [rankid] [name]");
		if(rankid <= 0 || rankid > 6) return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Ranks are between 1-6");
		int len=strlen(name);
		if(len < 1 || len > 31)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Length must be between 1-31");

		strncpy((char *)(&Gang[gid].Rank1[(rankid-1)*32]), name, 31);
		
		char str[40];
		snprintf(str, sizeof(str), "UPDATE Gangs SET Rank%d=? WHERE ID=%d", rankid, gid);
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, str, -1, &stmt, NULL);
	   	sqlite3_bind_text(stmt, 1, name, len, NULL);
    	sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		
		SendClientMessage(playerid, -1, "Rankname Changed.");
		return 1;
	}
	if(!strcasecmp(cmdtext, "/gang leave"))
	{
		if(za[playerid].Clan == 0)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're not in a gang!");
		if(za[playerid].Rank == 6)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You own this gang. You must /gang setleader first");

		SendClientMessage(playerid,-1,"You have left your gang. Say bai.");
		za[playerid].Rank=0;
		za[playerid].Clan=0;

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Data SET Rank=0, Clan=0 WHERE ID=?", 41, &stmt, NULL);
   		sqlite3_bind_int(stmt, 1, za[playerid].AccountID);
    	sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/gang members", 13)
	{
		int gid, rid;
		if(sscanf((char *)&cmdtext[13], "%d%d", &gid, &rid) != 2)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang members [gangid] [rank]");
		if(gid < 1 || gid > (GANGS-1))return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");
		if(Gang[gid].IsCreated == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong Gang!");
		if(rid < 1 || rid > 6)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Rank should be between 1&6.");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT Name, Rank, LastTime FROM Data WHERE Clan=? AND Rank=? LIMIT 95", 70, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, gid);
		sqlite3_bind_int(stmt, 2, rid);
		SendClientMessage(playerid, -1, "+-------------------------------+");
		
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			char str[100], date[15];
			time_t raw=sqlite3_column_int(stmt, 2);
			strftime(date, sizeof(date), "%dD/%mM/%YY", localtime(&raw));			
			snprintf(str, sizeof(str), ">> %s   {FFFFFF}Rank: %d   {FFFF99}Last Activity: %s", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1), date);
			SendClientMessage(playerid, 0xFF9999AA, str);
		}

		sqlite3_finalize(stmt);
		SendClientMessage(playerid,-1,"+-------------------------------+");
		return 1;
	}
	rcmd("/gang pay", 9)
	{
		if(za[playerid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something Wrong. Try to reconnect!");
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		if(za[playerid].Rank < 5)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You do not have permission.");
		int pid, amt;
		if(sscanf((char *)&cmdtext[9], "%d%d", &pid, &amt) != 2)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang pay [id] [score]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid ID!");
		if(za[pid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something wrong. That player should reconnect!");
		if(amt > 1000 || amt < 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] 1000 EXP is max at once!");
		if(amt > Gang[gid].Score)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You do not have this much in your bank!");

	    char str[300];
		
        Gang[gid].Score-=amt;

		int taxed=amt-(amt/3);
		SetPlayerScore(pid, GetPlayerScore(pid)+taxed);

		snprintf(str, sizeof(str), ">> %s has paid you %d XP from the gang XP pot. With taxes: %d", za[playerid].Name, amt, taxed);
		SendClientMessage(pid, 0xFF00FFAA, str);
		snprintf(str, sizeof(str), ">> %s has paid %s %d XP from the gang XP pot. With taxes: %d", za[playerid].Name, za[pid].Name, amt, taxed);
		SendGangMessage(gid, str);
		
		Save(pid);

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Gangs SET Score=? WHERE ID=?", 35, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, Gang[za[playerid].Clan].Score);
		sqlite3_bind_int(stmt, 2, za[playerid].Clan);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		
		snprintf(str, sizeof(str), "INSERT INTO `ganglog` (`who`,`tid`,`action`,`time`, `exp`) VALUES ('%s','%s','Gang Payment(%d)', %d, %d)", za[playerid].Name, za[pid].Name, za[playerid].Clan, (int)time(NULL), taxed);
		mysql_tquery(mysql, str, "", 0);
		return 1;
	}
	rcmd("/gang deposit", 13)	
	{
		if(za[playerid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something Wrong. Try to reconnect!");
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		int amt;
		if(sscanf((char *)&cmdtext[13], "%d", &amt) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang deposit [score]");
		if(amt < 100)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] 100 EXP is min!");
		if(GetPlayerScore(playerid) < amt)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You have no enough!");

	    char str[400];

		SetPlayerScore(playerid, GetPlayerScore(playerid)-amt);
		int taxed=amt-(amt/3);
		Gang[gid].Score+=taxed;

		snprintf(str, sizeof(str), ">> You deposited %d XP to the gang XP pot. Total with taxes: %d", amt, taxed);
		SendClientMessage(playerid, 0xFF00FFAA, str);
		snprintf(str, sizeof(str), ">> %s has deposited %d XP to the gang XP pot. Total with taxes: %d", za[playerid].Name, amt, taxed);
		SendGangMessage(gid, str);
		
		Save(playerid);

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Gangs SET Score=? WHERE ID=?", 35, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, Gang[za[playerid].Clan].Score);
		sqlite3_bind_int(stmt, 2, za[playerid].Clan);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		
		snprintf(str, sizeof(str), "INSERT INTO `ganglog` (`who`,`tid`,`action`,`time`, `exp`) VALUES ('%s','Clan %d','Deposit', %d, %d)", za[playerid].Name, za[playerid].Clan,(int)time(NULL), taxed);
		mysql_tquery(mysql, str, "", 0);
		return 1;
	}
	rcmd("/gang promote", 13)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;		
		int pid;
		if(sscanf((char *)&cmdtext[13], "%d", &pid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang promote [id]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid ID!");
		if(gid != za[pid].Clan)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] This player is not for you to promote");
		if(za[playerid].Rank < 4)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You must be rank 4+ to demote/promote people");
		if(za[pid].Rank >= 5 || pid == playerid || (za[pid].Rank > 2 && za[playerid].Rank == 4))return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You may not promote this person further.");

		za[pid].Rank++;
		
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "UPDATE Data SET Rank=? WHERE ID=?", 33, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, za[pid].Rank);
		sqlite3_bind_int(stmt, 2, za[pid].AccountID);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		char str[200];
		snprintf(str, sizeof(str), ">> %s has promoted %s to rank %d!", za[playerid].Name, za[pid].Name, za[pid].Rank);
		SendGangMessage(gid, str);
		return 1;
	}
	rcmd("/gang demote", 12)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;		
		int pid;
		if(sscanf((char *)&cmdtext[12], "%d", &pid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang demote [id]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid ID!");
		if(gid != za[pid].Clan)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] This player is not for you to demote");
		if(za[playerid].Rank < 4)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You must be rank 4+ to demote/promote people");
	
		if(playerid == pid && za[playerid].Rank == 6)
		{
			sqlite3_stmt *stmt;
			sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Data WHERE Clan=? AND Rank=6", 49, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, za[pid].Clan);
			sqlite3_step(stmt);

			if(sqlite3_column_int(stmt, 0) > 1)
			{
				sqlite3_stmt *stmt2;
				sqlite3_prepare_v2(db, "UPDATE Data SET Rank=5 WHERE ID=?", 42, &stmt2, NULL);
				sqlite3_bind_int(stmt2, 1, za[pid].AccountID);
				sqlite3_step(stmt2);
				sqlite3_finalize(stmt2);

				za[playerid].Rank=5;
				SendClientMessage(playerid,0xC0C0C0AA,"[Server] You have given away your ownership.");
			}
			else
			{
				SendClientMessage(playerid,0xC0C0C0AA,"[Server] You are only owner. Use /gang setleader to give ownership. Then use this cmd.");
			}
			sqlite3_finalize(stmt);
			return 1;
		}
		
		if(za[playerid].Rank <= za[pid].Rank)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You can't demote this person");
		za[pid].Rank--;

		if(za[pid].Rank == 0)
		{
			sqlite3_stmt *stmt3;
			sqlite3_prepare_v2(db, "UPDATE Data SET Rank=0, Clan=0 WHERE ID=?", 41, &stmt3, NULL);
			sqlite3_bind_int(stmt3, 1, za[pid].AccountID);
			sqlite3_step(stmt3);
			sqlite3_finalize(stmt3);

			za[pid].Clan=0;

			char str[128];
			snprintf(str, sizeof(str), ">> %s has been fired from your clan by %s!", za[pid].Name, za[playerid].Name);
			SendGangMessage(gid, str);
		}
		else
		{
			sqlite3_stmt *stmt3;
			sqlite3_prepare_v2(db, "UPDATE Data SET Rank=? WHERE ID=?", 33, &stmt3, NULL);
			sqlite3_bind_int(stmt3, 1, za[pid].Rank);
			sqlite3_bind_int(stmt3, 2, za[pid].AccountID);
			sqlite3_step(stmt3);
			sqlite3_finalize(stmt3);

			char str[128];
			snprintf(str, sizeof(str), ">> %s has demoted %s to rank %d!", za[playerid].Name, za[pid].Name, za[pid].Rank);
			SendGangMessage(gid, str);
		}
		return 1;
	}
	rcmd("/gang setleader", 15)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;			
		if(za[playerid].Rank != 6)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're not level 6!");
		int pid;
		if(sscanf((char *)&cmdtext[15], "%d", &pid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang setleader [id]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid ID.");
		if(gid != za[pid].Clan)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] This player is not for you to promote.");
		if(za[pid].Rank != 5)return SendClientMessage(playerid, 0xC0C0C0AA, "[Server] He/She must have 5 level."); 

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "UPDATE Data SET Rank=6 WHERE ID=?", 33, &stmt, NULL);		
		sqlite3_bind_int(stmt, 1, za[pid].AccountID);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		
		za[pid].Rank=6;

		char str[128];
		snprintf(str, sizeof(str), ">> The Owner %s has set ownership to %s", za[playerid].Name, za[pid].Name);
		SendGangMessage(gid, str);
		SendClientMessage(playerid, 0xFF9999AA, "Use /gang demote, to demote myself.");
		return 1;
	}
	rcmd("/gang odemote", 13)
	{
		int gid=za[playerid].Clan;
		if(gid < 1 || gid > (GANGS-1))return 1;
		if(Gang[gid].IsCreated == false)return 1;
		if(za[playerid].Rank < 5)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You must be rank 5+ to demote/promote people");
		char name[128];
		if(sscanf(&cmdtext[13], "%s", name) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang odemote [name]");
		int len=strlen(name);
		if(len > 20 || len < 3)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] [3-20 symbols].");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT Clan, Rank FROM Data WHERE Name=?", 40, &stmt, NULL);		
		sqlite3_bind_text(stmt, 1, name, len, 0);
		sqlite3_step(stmt);
		
		if(sqlite3_column_int(stmt, 0) == gid)
		{
			int rank=sqlite3_column_int(stmt, 1);
			
			if(rank == 6)return SendClientMessage(playerid, 0xC0C0C0AA, "[Server] Wrong demotion.");
			if(rank == 5 && za[playerid].Rank == 5)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Wrong demotion.");

			if(rank == 1)
			{
				sqlite3_stmt *stmt2;
				sqlite3_prepare_v2(db, "UPDATE Data SET Rank=0, Clan=0 WHERE Name=?", 43, &stmt2, NULL);		
				sqlite3_bind_text(stmt2, 1, name, len, 0);
				sqlite3_step(stmt2);
				sqlite3_finalize(stmt2);
		
				SendClientMessage(playerid, 0xC0C0C0AA, "[Server] This player has been fired.");
			}
			else
			{
				sqlite3_stmt *stmt2;
				sqlite3_prepare_v2(db, "UPDATE Data SET Rank=Rank-1 WHERE Name=?", 40, &stmt2, NULL);		
				sqlite3_bind_text(stmt2, 1, name, len, 0);
				sqlite3_step(stmt2);
				sqlite3_finalize(stmt2);
				
				SendClientMessage(playerid,0xC0C0C0AA,"[Server] This player has been demoted.");
			}
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,"[Server] He/She is not in your clan.");
		}
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/gang create", 12)
	{
		if(GetPlayerScore(playerid) < 70000)return SendClientMessage(playerid,0xC0C0C0AA,"Cost is 70,000 EXP");
		if(za[playerid].Clan > 0)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're already in a gang (use /gang leave)");
		if(za[playerid].Unconnected == true)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] OMG! umad, facepalm, lennyface. Get the f*ck out!");
		char name[128];
		if(sscanf(&cmdtext[12], "%s", name) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang create [name] Cost is 70,000 EXP");
		int len=strlen(name);
		if(len > 31 || len < 3)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Min 3, Max 32 symbols.");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT ID FROM Gangs WHERE Created=0", 36, &stmt, NULL);

		int rc=sqlite3_step(stmt);
		if(rc == SQLITE_DONE)
		{
			SendClientMessage(playerid,0xC0C0C0AA,"No empty gang slot!");
			sqlite3_finalize(stmt);
			return 1;
		}
		else if(rc == SQLITE_ROW)
		{
			int gang=sqlite3_column_int(stmt, 0);
			if(gang > (GANGS-1))
			{
				SendClientMessage(playerid,0xC0C0C0AA,"No empty gang slot!");
				sqlite3_finalize(stmt);
				return 1;	
			}

			sqlite3_stmt *stmt2;
			sqlite3_prepare_v2(db, "UPDATE Gangs SET Name=?, Created=1, Tag='---', Score=0, Wins=0, Losses=0, Rank1='Rank1', Rank2='Rank2', Rank3='Rank3', Rank4='Rank4', Rank5='Rank5', Rank6='Rank6' WHERE ID=?", 173, &stmt2, NULL);
			sqlite3_bind_text(stmt2, 1, name, len, 0);
			sqlite3_bind_int(stmt2, 2, gang);
			sqlite3_step(stmt2);
			sqlite3_finalize(stmt2);

			sqlite3_prepare_v2(db, "UPDATE Data SET Rank=6, Clan=? WHERE ID=?", 41, &stmt2, NULL);
			sqlite3_bind_int(stmt2, 1, gang);
			sqlite3_bind_int(stmt2, 2, za[playerid].AccountID);
			sqlite3_step(stmt2);
			sqlite3_finalize(stmt2);
		
			strcpy(Gang[gang].Rank1, "Rank1");
			strcpy(Gang[gang].Rank2, "Rank2");
			strcpy(Gang[gang].Rank3, "Rank3");
			strcpy(Gang[gang].Rank4, "Rank4");
			strcpy(Gang[gang].Rank5, "Rank5");
			strcpy(Gang[gang].Rank6, "Rank6");
			strcpy(Gang[gang].Tag, "---");
			strncpy(Gang[gang].Name, name, 31);
			
			Gang[gang].Score=0;
			Gang[gang].Wins=0;
			Gang[gang].Losses=0;
			Gang[gang].IsCreated=true;
			
			za[playerid].Clan=gang;
			za[playerid].Rank=6;

			SendClientMessage(playerid,0xFF00FFAA,"Congratulations! Use /gang tag to set your tag!");
			SendClientMessage(playerid,0xFF00FFAA,"Use /gang rankname to set your ranks!");
			SetPlayerScore(playerid,GetPlayerScore(playerid)-70000);
		}
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/gang join", 10)
	{
		if(za[playerid].Clan > 0)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You're already in a gang (use /gang leave)");
		if(za[playerid].Unconnected == true)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Nice try. Reconnect!");
		int gid;
		if(sscanf(&cmdtext[10], "%d", &gid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang join [id]");
		if(gid < 1 || gid > (GANGS-1))return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid Gang ID.");
		if(Gang[gid].IsCreated == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Invalid Gang ID.");
		if(time(NULL)-za[playerid].GangRequestTime < 300)return SendClientMessage(playerid,0xC0C0C0AA,">> Don't spam!");

		int online=0;
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Clan == gid && za[i].Rank > 2)
			{
				online=1;
				char str[128];
				snprintf(str, sizeof(str), ">> %s has requested to join your gang!", za[playerid].Name);
				SendClientMessage(i, 0xFF00FFAA, str);
				snprintf(str, sizeof(str), "Use '/gang accept %d' to accept them.", playerid);
				SendClientMessage(i, -1, str);
			}
		}
		if(online == 0)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] No high rank online to accept you.");
		
		za[playerid].GangRequest=gid;
		za[playerid].GangRequestTime=time(NULL);
		SendClientMessage(playerid, -1, "Join request sent.");
		return 1;
	}
	rcmd("/gang accept", 12)
	{
		if(za[playerid].Rank < 3)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] You do not have permission to accept people (rank 3+)");
		int pid;
		if(sscanf(&cmdtext[12], "%d", &pid) != 1)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] Usage : /gang accept [id]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] That player is not online!");
		if(za[pid].GangRequest != za[playerid].Clan)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] That player has not requested to join you!");

		char str[128];
		snprintf(str, sizeof(str), ">> You have been accepted into the '%s'", Gang[za[pid].GangRequest].Name);
		SendClientMessage(pid, -1, str);
		
		za[pid].GangRequest=0;
		za[pid].Clan=za[playerid].Clan;
		za[pid].Rank=1;
	
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "UPDATE Data SET Rank=1, Clan=? WHERE ID=?", 41, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, za[playerid].Clan);
		sqlite3_bind_int(stmt, 2, za[pid].AccountID);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		snprintf(str, sizeof(str), ">> %s has just been accepted into gang by %s", za[pid].Name, za[playerid].Name);
		SendGangMessage(za[playerid].Clan, str);
		return 1;
	}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

	rcmd("/unconnect", 10)
	{
		if(za[playerid].Admin < 3)return 0;
		char name[128];
		if(sscanf((char *)&cmdtext[10], "%s", name) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /unconnect [name]");
		int len=strlen(name);
		if(len < 3 || len > 20)return SendClientMessage(playerid, 0xC0C0C0AA, ">> [NAME] 3..20 symbols.");
	
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT ID FROM Data WHERE Name=?", 32, &stmt, NULL);
		sqlite3_bind_text(stmt, 1, name, len, 0);
	
		if(sqlite3_step(stmt) != SQLITE_DONE)
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Already someone has the same registered name!!" );
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Choose unregistered name!!" );
			sqlite3_finalize(stmt);
			return 1;
		}
		sqlite3_finalize(stmt);
	
	    Save(playerid);
	
		if(SetPlayerName(playerid, name) == 1)
		{
			SendClientMessage(playerid,COLOR_ANTIHACK,"[Server] Stats have been saved! Relog to be back!");
			SendClientMessage(playerid,COLOR_ANTIHACK,"[Server] Successfully hidden! Your scores won't be saved!");
			
	
			char str[256];
			snprintf(str, sizeof(str), "*** %s has left the Server [Leaving]", za[playerid].Name);
			SendClientMessageToAll(0xC0C0C0AA, str);

			snprintf(str, sizeof(str), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','Unconnected to %s','%d')", za[playerid].Name, name, (int)time(NULL));
			mysql_tquery(mysql, str, "", 0);

			GetPlayerName(playerid, za[playerid].Name);
			TogglePlayerSpectating(playerid, 1);
			
			za[playerid].Unconnected=true;
			za[playerid].UnconnectedID=10000+rand()%200000;
			za[playerid].Kills=rand()%1000;
			SetPlayerScore(playerid, za[playerid].Kills*10+(rand()%5)*1000);
			za[playerid].Killstreaks=0;
			za[playerid].Clan=0;
			za[playerid].Rank=0;
			za[playerid].Wins=rand()%20;
			za[playerid].DuelWins=rand()%30;
			za[playerid].DuelLoses=rand()%30;
		    SetTimerEx("KindaConnected", 2000+(rand()%3)*1000, playerid);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,"[Server] Only valid characters [0-9], [a-z], [A-Z], [], (), $ @ . _ and = only.");
			SendClientMessage(playerid,0xC0C0C0AA,"[Server] or someone has this name online.");
		}
		return 1;
	}
	rcmd("/changename", 11)
	{
		if(za[playerid].Admin != 6)
		{
			if(za[playerid].Unconnected == true)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You may not use it!");
			if(za[playerid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something wrong. Try to reconnect!");
			char name[128];
			if(sscanf((char *)&cmdtext[11], "%s", name) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /changename [new name] Cost: 1,400 EXP; Free for VIP, new players;");
			int len=strlen(name);
			if(len > 20 || len < 3)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Min:3 & Max:20 symbols!");
			if(za[playerid].NameIsChanged == true)return SendClientMessage(playerid,0xC0C0C0AA,">> You've already changed name. You have to rejoin for changing name!");
			if((time(NULL)-za[playerid].NameChange) < 86400)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You can change a name every 24 hours only!");
			
			int newbie=false;
			if(za[playerid].Kills < 40)
			{
				newbie=true;
			}
			else if(GetPlayerScore(playerid) < 1400)
			{
				return SendClientMessage(playerid, 0xC0C0C0AA, ">> You have no enough EXP!");
			}
			
			sqlite3_stmt *stmt;
			sqlite3_prepare_v2(db, "SELECT ID FROM Data WHERE Name=?", 32, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, name, len, 0);
	
			if(sqlite3_step(stmt) != SQLITE_DONE)
			{
				SendClientMessage(playerid, 0xC0C0C0AA, "Already someone has the same name!!" );
				sqlite3_finalize(stmt);
				return 1;
			}
			sqlite3_finalize(stmt);
			
			if(SetPlayerName(playerid, name) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, "Allowed symbols: [0-9] [a-z] _ $ @ [ ] ( ) . or someone has this name.");
				
			char buffer[25];
			strncpy(buffer, za[playerid].Name, 21);
			GetPlayerName(playerid, za[playerid].Name);
			
			za[playerid].NameChange=time(NULL);
			sqlite3_prepare_v2(db, "UPDATE Data SET Name=?, NameChange=? WHERE Name=?", 49, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, za[playerid].Name, strlen(za[playerid].Name), 0);
			sqlite3_bind_int(stmt, 2, za[playerid].NameChange);
			sqlite3_bind_text(stmt, 3, buffer, strlen(buffer), 0);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		
			char str[128];
			snprintf(str, sizeof(str), "[ADMIN] %s will now be known as %s", buffer, za[playerid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			SendClientMessage(playerid, 0xE6BC16AA, ">> Your name has been changed. Change your name in the SAMP-Client too!");
			
			za[playerid].NameIsChanged=true;
			if(za[playerid].VIP == 0 && newbie == false)
			{
				SendClientMessage(playerid, COLOR_ANTIHACK, "You have changed the name for 1400 EXP.");
				SetPlayerScore(playerid, GetPlayerScore(playerid)-1400);
			}

			char query[300];
			snprintf(query, sizeof(query), "INSERT INTO `cnHistory` (`Old`,`New`,`String`,`time`,`ID`) VALUES ('%s','%s','%s',%d,%d)", buffer, za[playerid].Name, str, (int)time(NULL),za[playerid].AccountID);
			mysql_tquery(mysql, query, "", 0);
		
			snprintf(query, sizeof(query), "UPDATE List SET unbanned=1 WHERE Name LIKE BINARY '%s'", za[playerid].Name);
			mysql_tquery(mysql, query, "", 0);
		}
		else
		{
			int pid; char name[128];
			if(sscanf((char *)&cmdtext[11], "%d%s", &pid, name) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /changename [playerid] [new name]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			if(za[pid].Unconnected == true)return SendClientMessage(playerid, 0xC0C0C0AA, ">> That player should reconnect to change a name!");
			if(za[pid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> That player should reconnect to change a name!");
			int len=strlen(name);
			if(len > 20 || len < 3)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Min:3 & Max:20 symbols!");
			if(za[pid].NameIsChanged == true)return SendClientMessage(playerid,0xC0C0C0AA,">> He's already changed name. He has to rejoin for changing name!");
			
			sqlite3_stmt *stmt;
			sqlite3_prepare_v2(db, "SELECT ID FROM Data WHERE Name=?", 32, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, name, len, 0);
	
			if(sqlite3_step(stmt) != SQLITE_DONE)
			{
				SendClientMessage(playerid, 0xC0C0C0AA, "Already someone has the same name!!" );
				sqlite3_finalize(stmt);
				return 1;
			}
			sqlite3_finalize(stmt);
			
			if(SetPlayerName(pid, name) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, "Allowed symbols: [0-9] [a-z] _ $ @ [ ] ( ) . or someone has this name.");

			char buffer[25];
			strncpy(buffer, za[pid].Name, 21);
			GetPlayerName(pid, za[pid].Name);
			
			sqlite3_prepare_v2(db, "UPDATE Data SET Name=? WHERE Name=?", 35, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, za[pid].Name, strlen(za[pid].Name), 0);
			sqlite3_bind_text(stmt, 2, buffer, strlen(buffer), 0);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		
			char str[128];
			snprintf(str, sizeof(str), "[ADMIN] %s will now be known as %s", buffer, za[pid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			SendClientMessage(pid, 0xE6BC16AA, ">> Your name has been changed. Change your name in the SAMP-Client too!");
			
			za[pid].NameIsChanged=true;
			
			char query[300];
			snprintf(query, sizeof(query), "INSERT INTO `cnHistory` (`Old`,`New`,`String`,`time`,`ID`) VALUES ('%s','%s','%s',%d,%d)", buffer, za[pid].Name, str, (int)time(NULL), za[pid].AccountID);
			mysql_tquery(mysql, query, "", 0);

			snprintf(query, sizeof(query), "UPDATE List SET unbanned=1 WHERE Name LIKE BINARY '%s'", za[playerid].Name);
			mysql_tquery(mysql, query, "", 0);
		}
		return 1;
	}
	rcmd("/pay", 4)
	{
		int pid, number;
		if(sscanf((char *)&cmdtext[4], "%d%d", &pid, &number) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /pay [ID] [Score]");
		if(number < 90)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Min Score is 90 EXP!");
		if(number > 90000)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You can transfer 80k EXP at once only!");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Invalid ID!");
		if(GetPlayerScore(playerid) < number)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You have no enough!");
		if(playerid == pid)return SendClientMessage(playerid, 0xC0C0C0AA, ">> umad, it's pointless to pay myself!");
		if(za[playerid].Unconnected == true)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You may not use!");
		if(za[playerid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something wrong. Try to reconnect!");
		if(za[pid].BanCheck == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Something wrong. That player should reconnect!");
			
		char str[200];

		SetPlayerScore(playerid, GetPlayerScore(playerid)-number);
		int taxed=number-(number/3);	
		SetPlayerScore(pid, GetPlayerScore(pid)+taxed);

		snprintf(str, sizeof(str), "%s paid to %s %d EXP. Total sent with taxes: %d", za[playerid].Name, za[pid].Name, number, taxed);
		SendClientMessageToAll(0xADFF2FAA, str);
	
		GameTextForPlayer(pid, "~y~You ~g~got the ~r~payment!", 2000, 5);
		Save(playerid);
		Save(pid);

		snprintf(str, sizeof(str), "INSERT INTO ganglog (who, tid, `action`, `time`, exp) VALUES ('%s', '%s', 'Payment', %d, %d)", za[playerid].Name, za[pid].Name, (int)time(NULL), taxed);
		mysql_tquery(mysql, str, "", 0);
		return 1;
	}
	rcmd("/changepassword", 15)
	{
		char oldpassword[128], newpassword[128];
		if(sscanf((char *)&cmdtext[15], "%s %s", oldpassword, newpassword) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /changepassword [password] [newpassword]");
		int oldlength=strlen(oldpassword);
		if(oldlength > 11 || oldlength < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> Incorrect Old Password!");
		int newlength=strlen(newpassword);
		if(newlength > 11 || newlength < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> Min 4, Max 11!");

		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "SELECT Password FROM Data WHERE ID=?", 36, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, za[playerid].AccountID);
		sqlite3_step(stmt);
	
		char buffer[65];
		SHA256_PassHash(oldpassword, buffer);
			
		if(!strncmp((const char *)sqlite3_column_text(stmt, 0), buffer, 64))
		{
			sqlite3_stmt *stmt2;
	    	sqlite3_prepare_v2(db, "UPDATE Data SET Password=? WHERE ID=?", 37, &stmt2, NULL);
			SHA256_PassHash(newpassword, buffer);
			sqlite3_bind_text(stmt2, 1, buffer, 64, 0);
			sqlite3_bind_int(stmt2, 2, za[playerid].AccountID);
			sqlite3_step(stmt2);
			sqlite3_finalize(stmt2);
	
	        char str[128];
			snprintf(str, sizeof(str), ">> Password changed. New password: {FFFFFF}%s", newpassword);
			SendClientMessage(playerid, -1, "'---------------------------------------------------------------------'");
			SendClientMessage(playerid, 0x009900aa, str);
			SendClientMessage(playerid, COLOR_ORANGE, ">> Please {FFFFFF}remember {FF9900}your password. We {FF0000}DON'T REFUND {FF9900}passwords!");
			SendClientMessage(playerid, -1, "'---------------------------------------------------------------------'");
		}
		else
		{
			SendClientMessage(playerid,0xADFF2FAA,">> Server: Incorrect Password!");
		}
	
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/duel", 5)
	{
		int pid, wid;
		if(sscanf((char *)&cmdtext[5], "%d%d", &pid, &wid) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /duel [playerid] [weaponid]");
		if(playerid == pid)return SendClientMessage(playerid,0xC0C0C0AA,">> It is pointless to fight yourself..");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> Invalid ID!");	
		if(za[pid].Settings&0x8)return SendClientMessage(playerid,0xC0C0C0AA,">> He/She has blocked duels!");
		if(za[pid].Duel == true || za[playerid].Duel == true)return SendClientMessage(playerid,0xC0C0C0AA,">> Someone is already in a duel!");
		if(wid < 22 || wid > 39 || wid == 37 || wid == 36 || wid == 35)return SendClientMessage(playerid,0xC0C0C0AA,">> Allowed Weapons ID: [22-36] [38, 39]. Use /weapons.");
		if((time(NULL)-za[playerid].DuelRequestPause) < 10)return SendClientMessage(playerid,0xC0C0C0AA,">> You can send duel requests only every 10s!");

		char str[128]; char WeaponName[32];
		snprintf(str, sizeof(str), ">> Duel request sent to %s!", za[pid].Name);
		SendClientMessage(playerid, 0xFFE4B5AA, str);
		
		za[playerid].DuelRequestPause=time(NULL);		
	
		GetWeaponName(wid, WeaponName, 32);
		snprintf(str, sizeof(str), ">> %s is inviting you to duel with weapon: %s (ID: %d).", za[playerid].Name, WeaponName, wid);
		SendClientMessage(pid, 0xFFE4B5AA, str);
		SendClientMessage(pid, 0xFFE4B5AA, "Type {00FF00}/y{FFE4B5} to agree and {FF0000}/n{FFE4B5} to deny.");		
		
		GameTextForPlayer(pid, "~y~Duel request received", 2000, 5);
		
		za[pid].DuelInviteTime=time(NULL);
		za[pid].DuelInviteID=playerid;
		za[pid].DuelInviteWID=wid;
		return 1;
	}
	rcmd("/cure", 5)
	{
		if(za[playerid].UCID != 3 && za[playerid].UCID != 4 && za[playerid].UCID != 23 && za[playerid].UCID != 33)
			return SendClientMessage(playerid, 0xFFFF00AA , ">> You are not a medic!");
		if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)return 1;
		int pid;
		if(sscanf((char *)&cmdtext[5], "%d", &pid) != 1)return SendClientMessage(playerid, 0xFFFF00AA, ">> Usage: /cure [playerid]");
		if(!IsPlayerConnected(pid))return SendClientMessage(playerid,0xFFFF00AA,">> Invalid ID!");
		if(temp[pid].Infection == false)return SendClientMessage(playerid,0xFFFF00AA,">> The player isn't infected!");
		if(temp[playerid].Antidote > 14)return SendClientMessage(playerid,0xFFFF00AA,">> Oppss! There are no antidotes anymore.");
		if(temp[playerid].CureTime > time(NULL))return SendClientMessage(playerid,0xFFFF00AA,">> You are still preparing..");
	
		float x, y, z;
		GetPlayerPos(playerid, &x, &y, &z);
		
		for(int i=0; i < AMOUNT; i++)
			if(za[i].UCID == -38 && IsPlayerInRangeOfPoint(i, 25.0, x, y, z))
			{
				SendClientMessage(playerid, 0xFFFF00AA, ">> Cure Blocker is too close to you!");
				return 1;
			}
		
		
		if(!IsPlayerInRangeOfPoint(pid, 8.0, x, y, z))
			return SendClientMessage(playerid,0xFFFF00AA,">> The player is too far away!");
		
		temp[playerid].Antidote++;
		temp[playerid].CureTime=time(NULL)+4;
		GameTextForPlayer(playerid, "~n~~n~~n~~y~+5~y~~n~~n~~n~", 1000, 5);
		SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
		ApplyAnimation(playerid, "MEDIC", "CPR", 1.5, 0, 0, 0, 0, 0, 0);
		char str[128];
		snprintf(str, sizeof(str), ">> %s has been cured by medic %s", za[pid].Name, za[playerid].Name);
		SendClientMessageToAll(0xADFF2FAA, str);
		
		if(temp[pid].HumanRogue == false)
			SetPlayerColor(pid, 0x00CCCCAA);
		temp[pid].Infection=false;
		temp[pid].Slow=false;
		temp[pid].TransferInfection=0;
		TextDrawHideForPlayer(pid, t_Red);
		SetPlayerDrunkLevel(pid, 0);
		return 1;
	}
	rcmd("/votekick", 9)
	{
		if(GetPlayerScore(playerid) < 50000) return SendClientMessage(playerid,0xC0C0C0AA,">> You need at least 50k exp to use this command");
		int pid; char reason[128];
		if(sscanf((char *)&cmdtext[9], formatDS, &pid, reason) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /votekick [playerid] [reason]");
		if(!IsPlayerConnected(pid))return SendClientMessage(playerid, 0xC0C0C0AA,">> Invalid ID!");
		if((time(NULL)-Votekick.TimeLimit) < 180)return SendClientMessage(playerid,0xC0C0C0AA,">> Wait for some time!");

		int c=0;
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Admin)
			{
				c++;
			}
		}
		if(c)return SendClientMessage(playerid, 0xC0C0C0AA,">> Use /report. You can't start.");
		if(Votekick.VoteOpen)return SendClientMessage(playerid,0xFFFF00AA,">> Votekick already opened! Vote with /yes or /no.");

		char str[144];
		snprintf(str, sizeof(str), ">> %s started a votekick on %s [Reason: %.60s]", za[playerid].Name, za[pid].Name, reason);
		SendClientMessageToAll(0xFFFF00AA, str);
		SendClientMessageToAll(0xFFFF00AA, "Vote with {00FF00}/yes{FFFF00} or {FF0000}/no{FFFF00}.");
		
		memset(&(Votekick.Vote[0]), 0, sizeof(Votekick.Vote[0])*AMOUNT);
		Votekick.VoteOpen=true;
		Votekick.ToBeKicked=pid;
		Votekick.VoteTime=time(NULL)+50;
		Votekick.Counted=0;
		Votekick.VoteNO=0;
		return 1;
	}
	rcmd("/skin", 5)
	{
		if(!za[playerid].VIP)return SendClientMessage(playerid, 0xC0C0C0AA, ">> You are not VIP!");
		int kid;
		if(sscanf((char *)&cmdtext[5],"%d", &kid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /skin [skinid]");
		
		if(kid == 0)
		{
			za[playerid].vSkin=0;
			SendClientMessage(playerid, 0xC0C0C0AA, ">> You've switched {FF0000}auto-change {C0C0C0}off!");
			return 1;
		}
	
		float x, y, z;
		GetPlayerVelocity(playerid, &x, &y, &z);
		if(!(x == 0.0 && y == 0.0 && z == 0.0))return SendClientMessage(playerid, 0xC0C0C0AA, ">> You should stay to use it!");
	
		if(kid < 1 || kid > 311)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Limits: 1 - 311");
		if(kid == 217)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Use another one!");//admin skin
	
		if(kid == 99 || kid == 92 || kid == 74 || kid == 29)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Use another one!");
		if(kid == 189 && za[playerid].InEvent == true)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Use another one!");
		
		za[playerid].vSkin=kid;
		ClearAnimations(playerid, 0);
		SetPlayerSkin(playerid, kid);
		TogglePlayerControllable(playerid, 1);
		
		SendClientMessage(playerid, 0xC0C0C0AA, ">> Use {FF0000}/skin 0 {C0C0C0}to switch auto-change off!");
		return 1;
	}
	rcmd("/report", 7)
	{
		int pid; char reason[128];
		if(sscanf((char *)&cmdtext[7], formatDS, &pid, reason) != 2)return SendClientMessage(playerid, -1, ">> Usage: /report [playerid] [reason]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid, 0xC0C0C0AA, ">> That player is not online!");
		if((za[playerid].ReportTime+20) > time(NULL))return SendClientMessage(playerid, 0xC0C0C0AA, ">> Don't spam please!");
		
		SendClientMessage(playerid, COLOR_ANTIHACK, ">> Your report has been sent.");
		
		char str[144];
		snprintf(str, sizeof(str), "[REPORT] %s(%d) reported %s(%d): %s", za[playerid].Name, playerid, za[pid].Name, pid, reason);
		SendClientMessageToAdmins(str);
		
		za[playerid].Hacker=pid;
		za[playerid].HackerTime=time(NULL);
		za[playerid].ReportTime=za[playerid].HackerTime;
		return 1;
	}
	rcmd("/stats", 6)
	{
		int pid=0;
		if(sscanf((char *)&cmdtext[6], "%d", &pid) != 1)
		{
			pid=playerid;
		}
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> Invalid ID!");

		char str[128];
		snprintf(str, sizeof(str), ">> Name: %s - Kills: %d -  Killstreak: %d (best: %d)", za[pid].Name, za[pid].Kills, za[pid].Killstreaks, za[pid].personalKS);
		SendClientMessage(playerid, COLOR_ANTIHACK, "*******Player Status*******");
		SendClientMessage(playerid, -1, str);

		char vip[50];
		if(za[pid].VIP == 1)
		{
			struct tm *localtm=localtime((time_t *)&(za[pid].VipTime));
			snprintf(vip, sizeof(vip), "Bronze(1) - VIP Expire: %dD/%dM", localtm->tm_mday, (localtm->tm_mon)+1);
		}
		else if(za[pid].VIP == 2)
		{
			struct tm *localtm=localtime((time_t *)&(za[pid].VipTime));
			snprintf(vip, sizeof(vip), "Silver(2) - VIP Expire: %dD/%dM", localtm->tm_mday, (localtm->tm_mon)+1);
		}
		else if(za[pid].VIP == 3)
		{
			struct tm *localtm=localtime((time_t *)&(za[pid].VipTime));
			snprintf(vip, sizeof(vip), "Gold(3) - VIP Expire: %dD/%dM", localtm->tm_mday, (localtm->tm_mon)+1);
		}
		else
		{
			snprintf(vip, sizeof(vip), "-");
		}
		snprintf(str, sizeof(str), ">> Wins: %d - Duel wins: %d - Duel loses: %d", za[pid].Wins, za[pid].DuelWins, za[pid].DuelLoses);
		SendClientMessage(playerid, -1, str);
	
		snprintf(str, sizeof(str), ">> EXP: %d - VIP: %s", GetPlayerScore(pid), vip);
		SendClientMessage(playerid,-1, str);
	
		snprintf(str, sizeof(str), ">> Clan: (ID %d)(Rank : %d)   {E8ACBB}Account ID: %d", za[pid].Clan, za[pid].Rank, (za[pid].Unconnected == false) ? za[pid].AccountID : za[pid].UnconnectedID);
		SendClientMessage(playerid, -1, str);
	
		if(pid == playerid)
		{
			time_t rawtime;
			rawtime=time(NULL);
			struct tm *info=localtime(&rawtime);
			char sign[65], toHash[40];
			
			if(za[pid].Unconnected == false)
			{
				snprintf(toHash, sizeof(toHash), "%d%s", GetPlayerScore(pid)+za[pid].Kills+za[pid].AccountID+info->tm_mday, za[pid].Name);
				SHA256_PassHash(toHash, sign);
			}
			else
			{
				memset(sign, 'F', sizeof(sign));
				sign[64]=0;
			}

			snprintf(str, sizeof(str), ">> Date: %dD/%dM/%dY  Sign: %s", info->tm_mday, (info->tm_mon)+1, (info->tm_year)+1900, &sign[48]);
			SendClientMessage(playerid, -1, str);			
		}
		else
		{
			time_t rawtime;
			rawtime=time(NULL);
			strftime(str, sizeof(str), ">> Date: %dD/%mM/%YY", localtime(&rawtime));
			SendClientMessage(playerid, -1, str);
		}
		return 1;
	}
	rcmd("/pm", 3)
	{
		int pid; char message[128];
		if(sscanf((char *)&cmdtext[3], formatDS, &pid, message) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /pm [playerid] [text]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> Invalid ID!");
		if(playerid == pid)return SendClientMessage(playerid,0xC0C0C0AA,">> There is no point in sending yourself a pm");
		if(za[playerid].Mute > time(NULL))return SendClientMessage(playerid,0xC0C0C0AA,">> You are muted!");
		if(((za[pid].Settings)&0x1) == 1)return SendClientMessage(playerid,0xC0C0C0AA,">> He/She blocked PMs!");
		
		char str[144];
	   	snprintf(str, sizeof(str), "PM to %s: %s", za[pid].Name, message);
		SendClientMessage(playerid, 0xFFFF00AA, str);
	
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(logs_db, "INSERT INTO PMChat (Time, AccountID, Name, Message, ToID, ToName) VALUES(?, ?, ?, ?, ?, ?)", 90, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, (int)time(NULL));
		sqlite3_bind_int(stmt, 2, za[playerid].AccountID);
		sqlite3_bind_text(stmt, 3, (char *)&za[playerid].Name, strlen((char *)&za[playerid].Name), NULL);
		sqlite3_bind_text(stmt, 4, message, strlen(message), NULL);
		sqlite3_bind_int(stmt, 5, za[pid].AccountID);
		sqlite3_bind_text(stmt, 6, (char *)&za[pid].Name, strlen((char *)&za[pid].Name), NULL);
		sqlite3_step(stmt);		
		sqlite3_finalize(stmt);
	
		if(TextContainsIP(message))
		{
			snprintf(str, sizeof(str), ">> Possible PM advertiser %s[%d] to %s[%d]: %s", za[playerid].Name, playerid, za[pid].Name, pid, message);
			SendClientMessageToAdmins(str);
			return 1;
		}
	
		snprintf(str, sizeof(str), "PM from %s(%d): %s", za[playerid].Name, playerid, message);
		SendClientMessage(pid, 0xFFFF00AA, str);
	    PlayerPlaySound(pid, 1056);
	    za[pid].ReplyID=playerid;
	    
		snprintf(str, sizeof(str), "[PMSpy] %s(%d) to %s(%d): %s", za[playerid].Name, playerid, za[pid].Name, pid, message);
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].PMSPY == true && playerid != i && pid != i)
			{
				SendClientMessage(i, 0xFF8000AA, str);
			}
		}
		return 1;
	}
	rcmd("/r", 2)
	{
		char message[128];
		if(sscanf((char *)&cmdtext[2], "%[\1-\377]", message) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /r [text]");
		int pid=za[playerid].ReplyID;
		if(pid == INVALID_PLAYER_ID)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Invalid ID. Use /pm");
		if(za[playerid].Mute > time(NULL))return SendClientMessage(playerid, 0xC0C0C0AA, ">> You are muted!");
		if(((za[pid].Settings)&0x1) == 1)return SendClientMessage(playerid,0xC0C0C0AA,">> He/She blocked PMs!");
		
		char str[144];
	   	snprintf(str, sizeof(str), "Reply to %s: %s", za[pid].Name, &message[1]);
		SendClientMessage(playerid, 0xFFFF00AA, str);
	
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(logs_db, "INSERT INTO PMChat (Time, AccountID, Name, Message, ToID, ToName) VALUES(?, ?, ?, ?, ?, ?)", 90, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, (int)time(NULL));
		sqlite3_bind_int(stmt, 2, za[playerid].AccountID);
		sqlite3_bind_text(stmt, 3, (char *)&za[playerid].Name, strlen((char *)&za[playerid].Name), NULL);
		sqlite3_bind_text(stmt, 4, &message[1], strlen(&message[1]), NULL);
		sqlite3_bind_int(stmt, 5, za[pid].AccountID);
		sqlite3_bind_text(stmt, 6, (char *)&za[pid].Name, strlen((char *)&za[pid].Name), NULL);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	
		if(TextContainsIP(&message[1]))
		{
			snprintf(str, sizeof(str), ">> Possible PM advertiser %s[%d] to %s[%d]: %s", za[playerid].Name, playerid, za[pid].Name, pid, &message[1]);
			SendClientMessageToAdmins(str);
			return 1;
		}
	
		snprintf(str, sizeof(str), "PM from %s(%d): %s", za[playerid].Name, playerid, &message[1]);
		SendClientMessage(pid, 0xFFFF00AA, str);
	    PlayerPlaySound(pid, 1056);
	    za[pid].ReplyID=playerid;
	    
		snprintf(str, sizeof(str), "[PMSpy] %s(%d) to %s(%d): %s", za[playerid].Name, playerid, za[pid].Name, za[playerid].ReplyID, &message[1]);
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].PMSPY == true && playerid != i && pid != i)
			{
				SendClientMessage(i, 0xFF8000AA, str);
			}
		}
		return 1;
	}
	
	rcmd("/spec", 5)
	{
		if(za[playerid].Admin)
		{
			int pid;
			if(sscanf((char *)&cmdtext[5], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /spec [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");

			SendClientMessage(playerid, 0xC0C0C0AA, "|: Type /specoff to stop spectating");

			za[playerid].UCID=INVALID_CLASS_ID;
			za[playerid].ClassDesc=NoClass;
			if(za[playerid].Team == false)za[playerid].SpawnedAsHuman=true;
			SetPlayerInterior(playerid, GetPlayerInterior(pid));
			SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(pid));
			TogglePlayerSpectating(playerid, 1);
			
			int ret2;
			if(IsPlayerInAnyVehicle(pid))ret2=PlayerSpectateVehicle(playerid, GetPlayerVehicleID(pid), 1);
			else ret2=PlayerSpectatePlayer(playerid, pid, 1);
	
			if(ret2)
			{
				char str[128];
				snprintf(str, sizeof(str), "[ADMCHAT] %s started to spectate %s", za[playerid].Name, za[pid].Name);
				SendClientMessageToAdmins(str);
		
				PlayerTextDrawShow(playerid, za[playerid].TDs.t_Ping);
				PlayerTextDrawShow(playerid, za[playerid].TDs.t_Health);
				PlayerTextDrawShow(playerid, za[playerid].TDs.t_Team);
				PlayerTextDrawShow(playerid, za[playerid].TDs.t_Weapons);

				za[playerid].Spec=pid+1;
			}
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/kick", 5)
	{
		if(za[playerid].Admin > 0)
		{
			int pid; char reason[128];
			if(sscanf((char *)&cmdtext[5], formatDS, &pid, reason) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /kick [playerid] [reason]");
			if(!IsPlayerConnected(pid))return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			
			char str[144];
			snprintf(str, sizeof(str), ">> Admin %s has kicked %s [Reason: %.60s]", za[playerid].Name, za[pid].Name, reason);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			SetTimerEx("PlayerKick", 500, pid);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/warn", 5)
	{
		if(za[playerid].Admin < 1)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		
		int Warns=1, pid[5]={0}; char reason[128];
		
		if(sscanf((char *)&cmdtext[5], "%d %d %d %d %d %[\1-\377]", &pid[0], &pid[1], &pid[2], &pid[3], &pid[4], reason) == 6)Warns=5;
		else if(sscanf((char *)&cmdtext[5], "%d %d %d %d %[\1-\377]", &pid[0], &pid[1], &pid[2], &pid[3], reason) == 5)Warns=4;
		else if(sscanf((char *)&cmdtext[5], "%d %d %d %[\1-\377]", &pid[0], &pid[1], &pid[2], reason) == 4)Warns=3;
		else if(sscanf((char *)&cmdtext[5], "%d %d %[\1-\377]", &pid[0], &pid[1], reason) == 3)Warns=2;
		else if(sscanf((char *)&cmdtext[5], formatDS, &pid, reason) != 2)
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /warn [playerid] (playerid) (playerid) (playerid) (playerid) [reason]");
			return 1;
		}

		for(int i=0; i < Warns; i++)
		{
			if(!IsPlayerConnected(pid[i]) || za[pid[i]].Log == false)
			{
				SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
				continue;
			}
			
			za[pid[i]].Warning++;
			
			char str[144];
			snprintf(str, sizeof(str), ">> Admin %s has given %s a warning (%d/3) [Reason: %.45s]", za[playerid].Name, za[pid[i]].Name, za[pid[i]].Warning, ReasonExpanderForWarns(reason));
			SendClientMessageToAll(COLOR_ANTIHACK, str);
			
			ShowPlayerDialog(pid[i], DIALOG_NOANSWER, 0, "Warning", ReasonExpanderForWarns(reason) , "Ok", "");
			
			if(za[pid[i]].Warning == 3)
			{
				za[pid[i]].Warning=0;
				snprintf(str, sizeof(str), ">> %s has been kicked from the server [Reason: 3 warnings]", za[pid[i]].Name);
				SendClientMessageToAll(COLOR_ANTIHACK, str);
				SetTimerEx("PlayerKick", 500, pid[i]);
			}
		}
		return 1;
	}
	rcmd("/mute", 5)
	{
		if(za[playerid].Admin == 0)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid, tid; char reason[128];
		if(sscanf((char *)&cmdtext[5], formatDDS, &pid, &tid, reason) != 3)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /mute [playerid] [time] [reason]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		if(tid > 30 || tid < 0)return SendClientMessage(playerid,0xC0C0C0AA,">> 0-30 mins!");
		
		za[pid].Mute=tid*60+time(NULL);
		
		char str[144];
		snprintf(str, sizeof(str), ">> %s muted %s for %d min(s). [Reason: %.57s]", za[playerid].Name, za[pid].Name, tid, reason);
		SendClientMessageToAll(COLOR_ANTIHACK, str);
		return 1;
	}
	rcmd("/atext", 6)
	{
		if(za[playerid].Admin == 0)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");

		char str[144];
		snprintf(str, sizeof(str), ">> Announcement: {FFFFFF}%s", (char *)&cmdtext[6]);
		SendClientMessageToAll(COLOR_ORANGE, str);
		snprintf(str, sizeof(str), "[ATEXT] by %s", za[playerid].Name);
		SendClientMessageToAdmins(str);
		return 1;
	}
	rcmd("/explode", 8)
	{
		if(za[playerid].Admin > 2)
		{
			int pid;
			if(sscanf((char *)&cmdtext[8], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /explode [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");

			float x, y, z, health;
			GetPlayerPos(pid, &x, &y, &z);
			CreateExplosion(x, y, z, 12, 3.5);
			GetPlayerHealth(pid, &health);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s has been exploded by %s [%.2f health]", za[pid].Name, za[playerid].Name, health);
			SendClientMessageToAdmins(str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/slap", 5)
	{
		if(za[playerid].Admin > 0)
		{
			int pid;
			if(sscanf((char *)&cmdtext[5], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /slap [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			
			float x, y, z, health;
			GetPlayerHealth(pid, &health);
			GetPlayerPos(pid, &x, &y, &z);
			SetPlayerPos(pid, x, y, z+8.0);
			
			char str[128];
			snprintf(str, sizeof(str), ">> Admin %s slapped %s [%.2f health]", za[playerid].Name, za[pid].Name, health);
			SendClientMessageToAdmins(str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/mslap", 6)
	{
		if(za[playerid].Admin > 0)
		{
			int pid;
			if(sscanf((char *)&cmdtext[6], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /slap [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			
			float x, y, z, health;
			GetPlayerHealth(pid, &health);
			GetPlayerPos(pid, &x, &y, &z);
			SetPlayerPos(pid, x, y, z+14.0);
			
			char str[128];
			snprintf(str, sizeof(str), ">> Admin %s monster slapped %s [%.2f health]", za[playerid].Name, za[pid].Name, health);
			SendClientMessageToAdmins(str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/freeze", 7)
	{
		if(za[playerid].Admin > 3)
		{
			int pid;
			if(sscanf((char *)&cmdtext[7], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /freeze [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			TogglePlayerControllable(pid, 0);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s has been frozen by %s", za[pid].Name, za[playerid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/unfreeze", 9)
	{
		if(za[playerid].Admin > 3)
		{
			int pid;
			if(sscanf((char *)&cmdtext[9], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /unfreeze [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			TogglePlayerControllable(pid, 1);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s has been unfrozen by %s", za[pid].Name, za[playerid].Name);
			SendClientMessageToAll(COLOR_ANTIHACK, str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/goto", 5)
	{
		if(za[playerid].Admin > 2)
		{
			int pid;
			if(sscanf((char *)&cmdtext[5], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /goto [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			
			float x, y, z;
			GetPlayerPos(pid, &x, &y, &z);
			SetPlayerPos(playerid, x+0.5, y, z);
			SetPlayerInterior(playerid, GetPlayerInterior(pid));
		}
		return 1;
	}
	rcmd("/get", 4)
	{
		if(za[playerid].Admin > 2)
		{
			int pid;
			if(sscanf((char *)&cmdtext[4], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /get [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
			
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			SetPlayerPos(pid, x+0.5, y, z);
		}
		return 1;
	}
	rcmd("/veh", 4)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int vid;
		if(sscanf((char *)&cmdtext[4], "%d", &vid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /veh [vehicleid]");
		if((401 > vid || vid > 611) && vid != 0)return SendClientMessage(playerid,0xC0C0C0AA,">> Vehicles available 401-610; 0 = destroy car;");
		
		if(vid == 0 && za[playerid].Vehicle)
		{
			DestroyVehicle(za[playerid].Vehicle);
			za[playerid].Vehicle=0;
		}
		else
		{
			if(za[playerid].Vehicle)
				DestroyVehicle(za[playerid].Vehicle);
			
			float x, y, z, a;
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &a);
			
			za[playerid].Vehicle=CreateVehicle(vid, x, y, z, a, -1, -1, -1, 0);
			LinkVehicleToInterior(za[playerid].Vehicle, GetPlayerInterior(playerid));
			SetVehicleVirtualWorld(za[playerid].Vehicle, GetPlayerVirtualWorld(playerid));
			PutPlayerInVehicle(playerid, za[playerid].Vehicle, 0);
		}
		return 1;
	}
	rcmd("/getinfo", 8)
	{
		if(za[playerid].Admin > 0)
		{
			int pid;
			if(sscanf((char *)&cmdtext[8], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /getinfo [playerid]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">>[Server] Invalid ID!");
			char str[128];
			float health, armour;	
			GetPlayerHealth(pid, &health);
			GetPlayerArmour(pid, &armour);
			
			snprintf(str, sizeof(str), ">> %s Information (IP %s)", za[pid].Name, za[pid].IP);
			SendClientMessage(playerid, COLOR_ANTIHACK, str);
			snprintf(str, sizeof(str), ">> Health: %.2f  Armour: %.2f", health, armour);
			SendClientMessage(playerid, -1, str);
	
			snprintf(str, sizeof(str),">> Team: %s  Class: %s", (za[pid].Team == false) ? "Human" : "Zombie", za[pid].ClassDesc);
			SendClientMessage(playerid, -1, str);
	
			union
			{
				float val1;
				int val2;
			}Some;
			Some.val2=NetStats_PacketLossPercent(pid);
			
			int afk=IsPlayerPaused(pid)/1000;
			if(afk > 10)
			{
				snprintf(str, sizeof(str),">> Packet Loss: %.2f  In AFK: Yes(%d seconds)", Some.val1, afk);
			}
			else
			{
				snprintf(str, sizeof(str),">> Packet Loss: %.2f  In AFK: No", Some.val1);
			}
			SendClientMessage(playerid, -1, str);
	
			int weap, ammo;
			char WeaponName[32];
			for(int i=0; i < 12; i++)
			{
				GetPlayerWeaponData(pid, i, &weap, &ammo);
				if(weap != 0)
				{
					GetWeaponName(weap, WeaponName, sizeof(WeaponName));
					snprintf(str, sizeof(str),">> Weapon: %s  Ammo: %d", WeaponName, ammo);
					SendClientMessage(playerid, -1, str);
				}
			}
	
			snprintf(str, sizeof(str), ">> Clan: (ID %d)(Rank : %d)", za[pid].Clan, za[pid].Rank);
			SendClientMessage(playerid,-1, str);
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		}
		return 1;
	}
	rcmd("/checkip", 8)
	{
		if(za[playerid].Admin < 2)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		char ip[128];
		if(sscanf((char *)&cmdtext[8], "%s", ip) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /checkip [ip]");
		int pip=strlen(ip);
		if(pip < 6 || pip > 15) return SendClientMessage(playerid, 0xC0C0C0AA, ">> Invalid IP!");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT Name FROM Data WHERE IP LIKE ?", 37, &stmt, NULL);
		strcat(ip, "%");
		sqlite3_bind_text(stmt, 1, ip, pip+1, 0);
	
		SendClientMessage(playerid,-1,"+-------------------------------+");
		char str[128];
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			snprintf(str, sizeof(str), ">> %s", sqlite3_column_text(stmt, 0));
			SendClientMessage(playerid, 0xFF9999AA, str);
		}
		SendClientMessage(playerid,-1,"+-------------------------------+");

		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/tban", 5)
	{
		if(za[playerid].Admin < 1)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid, tm; char reason[128];
		if(sscanf((char *)&cmdtext[5], formatDDS, &pid, &tm, reason) != 3)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /tempban [playerid] [time] [reason]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		if(za[pid].Admin > za[playerid].Admin)return SendClientMessage(playerid,0xC0C0C0AA,">> The player is admin!");
		if(pid == playerid)return SendClientMessage(playerid,0xC0C0C0AA,">> Do not myself!");
		if(tm < 1 || tm > 200)return SendClientMessage(playerid,0xC0C0C0AA,">> Min 1, Max 200!");
		if(za[playerid].Admin == 1 && tm > 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You cannot ban more than 24 hours!");

		BanPlayer(pid, za[playerid].Name, ReasonExpander(reason), tm);
		DHS[playerid].Bans++;
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && za[i].Hacker == pid && (za[i].HackerTime+50) > time(NULL))
			{
				SendClientMessage(i, 0xFF0000AA, ">> Thanks for report. {00FF00}+20 EXP!");
				SetPlayerScore(i, GetPlayerScore(i)+20);
				PlayerPlaySound(i, 1056);
			}
		}
		return 1;
	}
	rcmd("/otban", 6)
	{
		if(za[playerid].Admin < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int tm; char name[128], reason[128];
		char *format="%d %s %[\1-\377]";
		if(sscanf((char *)&cmdtext[6], format, &tm, name, reason) != 3)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /offlinetempban [time] [name] [reason]");
		if(tm < 1 || tm > 200)return SendClientMessage(playerid,0xC0C0C0AA,">> Min 1, Max 200!");
		int len=strlen(name);
		if(len < 3 || len > 20)return SendClientMessage(playerid,0xC0C0C0AA,">> Ivalid Name: Min 3, Max 20!");

	    sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Data SET Time=? WHERE Name=?", 35, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, tm*3600+(int)time(NULL));
		sqlite3_bind_text(stmt, 2, name, len, 0);
		if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) == 1)
		{
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> Done!");
			DHS[playerid].Bans++;
			
			char Ereason[128];
			mysql_escape_string(reason, Ereason, sizeof(Ereason), mysql);
			Ereason[127]=0;
			
			char str[250];
			snprintf(str, sizeof(str), "INSERT INTO `List` (`Name`,`Reason`,`Admin`,`Type`,`unbanned`,`unixtime`) VALUES ('%s','%s','%s','Temporary (%d Hour(s))', 1, %d)", name, Ereason, za[playerid].Name, tm, (int)time(NULL));
			mysql_tquery(mysql, str, "", 0);
		}
		else
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Name not found.");
		}
		sqlite3_finalize(stmt);		
		return 1;
	}
	
	rcmd("/ban", 4)
	{
		if(za[playerid].Admin < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid; char reason[128];
		if(sscanf((char *)&cmdtext[4], formatDS, &pid, reason) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /ban [playerid] [reason]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		if(za[pid].Admin > za[playerid].Admin)return SendClientMessage(playerid,0xC0C0C0AA,">> The player is admin!");
		if(pid == playerid)return SendClientMessage(playerid,0xC0C0C0AA,">> Do not ban myself!");
		if(strlen(reason) > 50)return SendClientMessage(playerid,0xC0C0C0AA,">> Length of string is too big!");
		
		if(za[pid].Admin == 0)
		{
			SetPlayerScore(playerid, GetPlayerScore(playerid)+20);
			GameTextForPlayer(playerid,"~n~~n~~n~~y~+20", 1000, 5);
		}

		BanPlayer(pid, za[playerid].Name, ReasonExpander(reason), 0);
		DHS[playerid].Bans++;

		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && za[i].Hacker == pid && (za[i].HackerTime+50) > time(NULL))
			{
				SendClientMessage(i, 0xFF0000AA, ">> Thanks for report. {00FF00}+20 EXP!");
				SetPlayerScore(i, GetPlayerScore(i)+20);
				PlayerPlaySound(i, 1056);
			}
		}
		return 1;
	}
	rcmd("/unban", 6)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		char name[128];
		if(sscanf((char *)&cmdtext[6], "%s", name) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /unban [name]");
		int len=strlen(name);
		if(len > 20 || len < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> [Name] Min 3; Max 20!");
		
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Data SET Time=0 WHERE Name=?", 35, &stmt, NULL);
		sqlite3_bind_text(stmt, 1, name, len, 0);
		
		if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) == 1)
		{
			sqlite3_stmt *stmt2;
	    	sqlite3_prepare_v2(db, "SELECT IP FROM Data WHERE Name=?", 32, &stmt2, NULL);
			sqlite3_bind_text(stmt2, 1, name, len, 0);
			sqlite3_step(stmt2);

			char str[350];
			snprintf(str, sizeof(str), "unbanip %s", sqlite3_column_text(stmt2, 0));
			SendRconCommand(str);
			SendRconCommand("reloadbans");
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> The player has been unbanned!");
			sqlite3_finalize(stmt2);
			
			snprintf(str, sizeof(str), "UPDATE List SET unbanned=1 WHERE Name LIKE BINARY '%s'", name);
			mysql_tquery(mysql, str, "", 0);

			snprintf(str, sizeof(str), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','Unbanned %s','%d')", za[playerid].Name, name, (int)time(NULL));
			mysql_tquery(mysql, str, "", 0);
		}
		else
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Name Not Found!");
		}
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/banname", 8)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		char name[128], reason[128];
		char *format="%s %[\1-\377]";
		if(sscanf((char *)&cmdtext[8], format, name, reason) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /banname [name] [reason]");
		int len=strlen(name);
		if(len > 20 || len < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> Length of name is too big!");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, "SELECT ID FROM Data WHERE Name=?", 32, &stmt, NULL);
		sqlite3_bind_text(stmt, 1, name, len, 0);

		if(sqlite3_step(stmt) == SQLITE_ROW)
		{
			char Ereason[128];
			mysql_escape_string(reason, Ereason, 125, mysql);
	
			char str[600];
			snprintf(str, sizeof(str), "INSERT INTO `List` (`Name`,`Reason`, Admin,`Type`, unbanned, `unixtime`) VALUES ('%s','%s','%s', 'Permanent', 0, %d)", name, Ereason, za[playerid].Name, (int)time(NULL));
			mysql_tquery(mysql, str, "", 0);
	
	    	SendClientMessage(playerid,COLOR_ANTIHACK,"Banned!");
	    	DHS[playerid].Bans++;
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,"Name not found!");
		}
		sqlite3_finalize(stmt);
		return 1;
	}
	rcmd("/banip", 6)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid;
		if(sscanf((char *)&cmdtext[6], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /banip [ID], use /baniprange for other cases.");
		if(!IsPlayerConnected(pid))return SendClientMessage(playerid,0xC0C0C0AA,">> Invalid ID!");
		
		char str[128];
		snprintf(str, sizeof(str), "[ADMIN] %s has banned %s by IP", za[playerid].Name, za[pid].Name);
		SendClientMessageToAdmins(str);
		
		snprintf(str, sizeof(str), "banip %s", za[pid].IP);
		SendRconCommand(str);
		return 1;
	}
	rcmd("/unbanip", 8)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if(strlen((char *)&cmdtext[8]) > 15 || strlen((char *)&cmdtext[8]) < 7)return SendClientMessage(playerid,0xC0C0C0AA,">> Use right format!");

		char str[128];
		snprintf(str, sizeof(str), "unbanip %s", (char *)&cmdtext[8]);
		SendRconCommand(str);
		SendRconCommand("reloadbans");
		SendClientMessage(playerid, COLOR_ANTIHACK, ">> Done!");
		return 1;
	}


	rcmd("/baniprange", 11)
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int length=strlen((char *)&cmdtext[11]);
		if(length > 16 || length < 10)return SendClientMessage(playerid,0xC0C0C0AA,">> Usage /baniprange [IP]!");
		
		char str[128];
		snprintf(str, sizeof(str), "[ADMIN] %s has banned ip %s", za[playerid].Name, (char *)&cmdtext[11]);
		SendClientMessageToAdmins(str);
		
		snprintf(str, sizeof(str), "banip %s", (char *)&cmdtext[11]);
		SendRconCommand(str);
		return 1;
	}
	
	rcmd("/timeout", 8)
	{		
		if(za[playerid].Admin < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid;
		if(sscanf((char *)&cmdtext[8], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /timeout [playerid]");
		if(!IsPlayerConnected(pid))return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		
		char str[128];
		snprintf(str, sizeof(str), "[ADMIN] %s has timed out %s", za[playerid].Name, za[pid].Name);
		SendClientMessageToAdmins(str);
		
		BlockIpAddress(za[pid].IP, 15000);
		return 1;
	}


	rcmd("/ostats", 7)
	{
		if(za[playerid].Admin < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int tid; char name[128];
		if(sscanf((char *)&cmdtext[7], "%d %s", &tid, name) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /ostats [0 - Account ID, 1 - Name] [Appropriate Account ID/Name]");
		
		sqlite3_stmt *stmt;
		int length=strlen(name);
		
		if(tid == 1)//Name
		{	
			if(length > 20 || length < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> Name length is improper!");
	
			sqlite3_prepare_v2(db, "SELECT ID, EXP, Kills, Clan, Rank, LastTime, IP, Time, Vip, VipTime, Name, Admin FROM Data WHERE Name=?", 103, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, name, length, NULL);
		}
		else if(tid == 0)//Account ID
		{
			int ID=atoi(name);
			if(ID == 0)return SendClientMessage(playerid,0xC0C0C0AA,">> Please enter proper Account ID!");
		
			sqlite3_prepare_v2(db, "SELECT ID, EXP, Kills, Clan, Rank, LastTime, IP, Time, Vip, VipTime, Name, Admin FROM Data WHERE ID=?", 101, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, ID);	
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> Valid Types: 0(Acc. ID), 1(Name)!");
			return 1;	
		}
		
		if(sqlite3_step(stmt) == SQLITE_ROW)
		{
			SendClientMessage(playerid,-1,"+-------------------------------+");
			
			char str[128];
			snprintf(str, sizeof(str), "Information For The Last Activity: {FFFFFF}%s  {33CCFF}Account ID: {FFFFFF}%d", sqlite3_column_text(stmt, 10), sqlite3_column_int(stmt, 0));
			SendClientMessage(playerid, COLOR_ANTIHACK, str);

			int VipTime=sqlite3_column_int(stmt, 9);
			if(VipTime > time(NULL))
			{
				struct tm *localtm=localtime((time_t *)&VipTime);				
				snprintf(str, sizeof(str), "EXP: %d  Kills: %d  Clan: %d  Rank: %d  VIP: %d  VIP Expire: %dD/%dM", sqlite3_column_int(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3), sqlite3_column_int(stmt, 4), sqlite3_column_int(stmt, 8), localtm->tm_mday, (localtm->tm_mon)+1);
				SendClientMessage(playerid, -1, str);
			}
			else
			{
				snprintf(str, sizeof(str), "EXP: %d  Kills: %d  Clan: %d  Rank: %d  VIP: -", sqlite3_column_int(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3), sqlite3_column_int(stmt, 4));
				SendClientMessage(playerid, -1, str);
			}	
			
			time_t raw=sqlite3_column_int(stmt, 5);
			char LastActivity[18], TempBan[18];
			strftime(LastActivity, sizeof(LastActivity), "%dD/%mM/%YY", localtime(&raw));
			raw=sqlite3_column_int(stmt, 7);
			strftime(TempBan, sizeof(TempBan), "%dD/%mM/%YY", localtime(&raw));
			
			snprintf(str, sizeof(str), "Last Activity: %s  Admin: %d", LastActivity, sqlite3_column_int(stmt, 11));
			SendClientMessage(playerid, -1, str);
			snprintf(str, sizeof(str), "Last IP: %s", sqlite3_column_text(stmt, 6));
			SendClientMessage(playerid, -1, str);
			snprintf(str, sizeof(str), "Tempbanned until: %s", TempBan);
			SendClientMessage(playerid, -1, str);

			SendClientMessage(playerid,-1,"+-------------------------------+");
		}
		else SendClientMessage(playerid, -1, ">> Not Found!");
		
		sqlite3_finalize(stmt);
		return 1;
	}


	rcmd("/mh", 3)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid;
		if(sscanf((char *)&cmdtext[3], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /mh [playerid]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		if(za[pid].VIP && za[playerid].Admin != 6)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You may not use it!");
		if(za[pid].Killstreaks > 20 && za[playerid].Admin != 6)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You may not use it!");

		if(IsPlayerInAnyVehicle(pid))
		{
			SetVehicleToRespawn(GetPlayerVehicleID(pid));
		}
		za[pid].Team=false;
		za[pid].SpawnedAsHuman=true;
		SpawnPlayer(pid);
		za[pid].Killstreaks=0;
		
		char str[128];
		snprintf(str, sizeof(str), "[ADMIN] %s made %s a human", za[playerid].Name, za[pid].Name);
		SendClientMessageToAdmins(str);
		return 1;
	}
	
	rcmd("/mz", 3)
	{
		if(za[playerid].Admin < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid;
		if(sscanf((char *)&cmdtext[3], "%d", &pid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /mz [playerid]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		
		if(IsPlayerInAnyVehicle(pid))
		{
			SetVehicleToRespawn(GetPlayerVehicleID(pid));
		}
		za[pid].Team=true;
		SpawnPlayer(pid);
		za[pid].Killstreaks=0;
		
		char str[128];
		snprintf(str, sizeof(str), "[ADMIN] %s made %s a zombie", za[playerid].Name, za[pid].Name);
		SendClientMessageToAdmins(str);
		return 1;
	}
	
	rcmd("/newround", 9)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		if (EvacTime < -30)return SendClientMessage(playerid,0xC0C0C0AA,">> You can't do that now.");
		int mid;
		if(sscanf((char *)&cmdtext[9], "%d", &mid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /newround [mapid]");
		if(mid < 1 || mid > 200) return SendClientMessage(playerid,0xC0C0C0AA,">> Invalid map ID");
		
		map=mid-1;
		EvacTime=-50;
		return 1;
	}
	
	rcmd("/adminlist", 10)
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,"[Server] lalala!");
		int level;
		if(sscanf((char *)&cmdtext[10], "%d", &level) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /adminlist [level]");
		if(level < 1 || level > 6)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Level should be between 1 and 6;");

		sqlite3_stmt *stmt;
		char query[60];
		snprintf(query, sizeof(query), "SELECT Name, Admin, LastTime FROM Data WHERE Admin=%d", level);
		sqlite3_prepare_v2(db, query, 53, &stmt, NULL);

		SendClientMessage(playerid,-1,"+-------------------------------+");
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			char str[128], date[15];
			time_t raw=sqlite3_column_int(stmt, 2);
			strftime(date, sizeof(date), "%dD/%mM/%YY", localtime(&raw));			
			snprintf(str, sizeof(str), ">> %s   {FFFFFF}Level: %d   {FFFF99}Last Activity: %s", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1), date);
			SendClientMessage(playerid, 0xFF9999AA, str);
		}

		sqlite3_finalize(stmt);
		SendClientMessage(playerid,-1,"+-------------------------------+");
		return 1;
	}
	
	rcmd("/ann", 4)
	{
		if(za[playerid].Admin < 4)return 1;
		if(za[playerid].Admin != 6)
		{
			if(strstr((char *)&cmdtext[4], "~"))
			{
				SendClientMessage(playerid, 0xC0C0C0AA, ">> [Server] You may not use '~'.");
				return 1;
			}
		}
		GameTextForAll((char *)&cmdtext[4], 7500, 5);
		return 1;
	}
	
	rcmd("/weather", 8)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You do not have the right admin permissions for this command!");
		int wid;
		if(sscanf((char *)&cmdtext[8], "%d", &wid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /weather [weatherid]");
		if(wid < 0 || wid > 150)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Valid Weather: 0-150;");
		SetWeather(wid);
		MapData.Weather=wid;
		return 1;
	}
	
	rcmd("/time", 5)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You do not have the right admin permissions for this command!");
		int tid;
		if(sscanf((char *)&cmdtext[5], "%d", &tid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /time [time]");
		if(tid < 0 || tid > 23)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Valid Time: 0-23;");
		SetWorldTime(tid);
		MapData.Time=tid;
		return 1;
	}
	
	rcmd("/music", 6)
	{
		if(za[playerid].Admin < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		char str[140];

		if(cmdtext[6] == 0)
		{
			for(int i=0; i < AMOUNT; i++)StopAudioStreamForPlayer(i);

			snprintf(str, sizeof(str), "[ADMIN] %s has switched music off!", za[playerid].Name);
			SendClientMessageToAdmins(str);
		}
		else
		{
			for(int i=0; i < AMOUNT; i++)
			{
				SendClientMessage(i, 0xC0C0C0AA, ">> Music has been switched on! Use {FF0000}/off {FFFFFF}to switch this nonsense off!");
				PlayAudioStreamForPlayer(i, &cmdtext[7], 0.0, 0.0, 0.0, 0.0, 0);		
			}

			snprintf(str, sizeof(str), "[ADMIN] %s has switched music on!", za[playerid].Name);
			SendClientMessageToAdmins(str);
			SendClientMessage(playerid, 0xC0C0C0AA, ">> You can disable the music for everyone by typing just {FF0000}/music");		
		}
		return 1;
	}

	rcmd("/makeadmin", 10)
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		int pid, level;
		if(sscanf((char *)&cmdtext[10], "%d%d", &pid, &level) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /makeadmin [playerid] [level]");
		if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
		if(level > 4 || level < 0)return SendClientMessage(playerid,0xC0C0C0AA,">> Level must be between 0 & 4!");
	
		sqlite3_stmt *stmt;
    	sqlite3_prepare_v2(db, "UPDATE Data SET Admin=? WHERE ID=?", 34, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, level);
		sqlite3_bind_int(stmt, 2, za[pid].AccountID);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		za[pid].Admin=level;
		AA[pid].Time=time(NULL);

		char str[300];
		snprintf(str, sizeof(str), "Adminlevel changed for %s (new level: %d)", za[pid].Name, level);
		SendClientMessage(playerid, COLOR_ANTIHACK, str);

		snprintf(str, sizeof(str), ">> %s made you an admin level %d. Use /acmds to check all commands and use @ to chat with other admins", za[playerid].Name, level);
		SendClientMessage(pid, COLOR_ANTIHACK, str);
		PlayerPlaySound(pid, 1056);

		snprintf(str, sizeof(str), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','%s made %s an admin level %d','%d')", za[playerid].Name, za[playerid].Name, za[pid].Name, level, (int)time(NULL));
		mysql_tquery(mysql, str, "", 0);	
		return 1;
	}

	rcmd("/makeadminname", 14)
	{
		if(za[playerid].Admin < 5)return SendClientMessage(playerid,0xC0C0C0AA,">> You do not have the right admin permissions for this command!");
		char name[128]; int level;
		if(sscanf((char *)&cmdtext[14], "%d%s", &level, name) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /makeadminname [level] [name]");
		int len=strlen(name);
		if(len > 20 || len < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> [Name] Min 3; Max 20!");
		if(level < 0 || level > 4)return SendClientMessage(playerid,0xC0C0C0AA,">> Levels: 0&4 only!");
	
		sqlite3_stmt *stmt;
    	sqlite3_prepare_v2(db, "UPDATE Data SET Admin=? WHERE Name=?", 36, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, level);
		sqlite3_bind_text(stmt, 2, name, len, 0);
		if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) == 1)
		{
			char str[300];
			snprintf(str, sizeof(str), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','%s made %s an admin level %d (offline)','%d')", za[playerid].Name, za[playerid].Name, name, level, (int)time(NULL));
			mysql_tquery(mysql, str, "", 0);
			
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> Done!");
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA,">> Name not found.");
		}
		sqlite3_finalize(stmt);
		return 1;
	}
/*
	rcmd("/setpassword", 12)
	{
	   	if(za[playerid].Admin < 5)return 0;
		char name[128], password[128];
		if(sscanf((char *)&cmdtext[12], "%s%s", name, password) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /setpassword [name] [password]");
		int len=strlen(password);
		if(len > 11 || len < 4)return SendClientMessage(playerid,0xC0C0C0AA,">> [Password] Min 4, Max 11!");
		int lenname=strlen(name);
		if(lenname > 20 || lenname < 3)return SendClientMessage(playerid,0xC0C0C0AA,">> [Name] Min 3, Max 20!");
	
		char hash[65];
		SHA256_PassHash(password, hash);
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "UPDATE Data SET Password=? WHERE Name=?", 108, &stmt, NULL);
		sqlite3_bind_text(stmt, 1, hash, 64, 0);
		sqlite3_bind_text(stmt, 2, name, lenname, 0);
	
		if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) == 1)
		{
			char str[100];
			snprintf(str, sizeof(str), ">> Password changed for %s.", name);
			SendClientMessage(playerid, 0x009900aa, str);
			
			char query[350];
			snprintf(query, sizeof(query), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','%s','%d')", za[playerid].Name, str, (int)time(NULL));
			mysql_tquery(mysql, query, "", 0);
		}
		else
		{
			SendClientMessage(playerid, 0xC0C0C0AA, ">> Name not found.");
		}
		
		sqlite3_finalize(stmt);
		return 1;
	}
*/
	rcmd("/exp", 4)
	{
		if(za[playerid].Admin == 6)
		{
			int pid, exp;
			if(sscanf((char *)&cmdtext[4], "%d%d", &pid, &exp) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /exp [playerid] [score]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
	        if(exp > 1000000 || exp < -1000000)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid EXP Value.");
			SetPlayerScore(pid, GetPlayerScore(pid)+exp);
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> [EXP] Done!");
			return 1;
		}
		else if(za[playerid].Admin == 5)
		{
			int pid, exp;
			if(sscanf((char *)&cmdtext[4], "%d%d", &pid, &exp) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /exp [playerid] [score]");
			if(!IsPlayerConnected(pid) || za[pid].Log == false)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid ID!");
	        if(exp > 1000000 || exp < -1000000)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid  EXP Value.");
			if(exp > -1)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You can't give EXP.");	
			if((GetPlayerScore(pid)+exp) < 0)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You wanna take too much. Reason: Negative score.");
	
			SetPlayerScore(pid, GetPlayerScore(pid)+exp);
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> [EXP] Done!");

			char query[350];
			snprintf(query, sizeof(query), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','has taken %d EXP from %s','%d')", za[playerid].Name, exp, za[pid].Name, (int)time(NULL));
			mysql_tquery(mysql, query, "", 0);
			return 1;
		}
		return 0;
	}
	rcmd("/fine", 5)
	{
		if(za[playerid].Admin < 5)return 0;
		char name[128]; int exp;
		if(sscanf((char *)&cmdtext[5], "%d%s", &exp, name) != 2)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /fine [score] [name]");
		if(exp > 1000000 || exp < -1000000)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] Invalid EXP Input.");
		if(exp > -1)return SendClientMessage(playerid,0xC0C0C0AA,">> [Server] You may not give EXP.");
		int len=strlen(name);
		if(len < 3 || len > 20)return SendClientMessage(playerid,0xC0C0C0AA,">> [NAME] 3..20 symbols.");

		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Log == true && !strncmp(za[i].Name, name, 22))
			{
				SendClientMessage(playerid, 0xC0C0C0AA, ">> [Server] He/She's online. Use /exp.");
				return 1;
			}
		}

		sqlite3_stmt *stmt;
    	sqlite3_prepare_v2(db, "UPDATE Data SET EXP=EXP+? WHERE Name=?", 38, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, exp);
		sqlite3_bind_text(stmt, 2, name, len, 0);
		if(sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) == 1)
		{
			char query[350];
			snprintf(query, sizeof(query), "INSERT INTO adminlog (`username`,`action`,`time`) VALUES ('%s','has taken %d EXP from %s','%d')", za[playerid].Name, exp, name, (int)time(NULL));
			mysql_tquery(mysql, query, "", 0);	
				
			SendClientMessage(playerid, COLOR_ANTIHACK, ">> Done!");
		}
		else
		{
			SendClientMessage(playerid,0xC0C0C0AA, ">> Name not found.");
		}
		sqlite3_finalize(stmt);	
		return 1;
	}

	rcmd("/query", 6)
	{
		if(za[playerid].Admin == 6)
		{
			char *errmsg, str[128];
			sqlite3_exec(db, &cmdtext[6], NULL, NULL, &errmsg);
			
			if(errmsg != NULL)
			{
    			snprintf(str, sizeof(str), ">> Query was sent: {FFFFFF}%s!", errmsg);
    			SendClientMessage(playerid, COLOR_ANTIHACK, str);
    			sqlite3_free(errmsg);
			}
			else
			{
				SendClientMessage(playerid, COLOR_ANTIHACK, ">> Query was sent: {FFFFFF}No Error!");
			}
			return 1;
		}
		return 0;
	}
	
	rcmd("/aa", 3)
	{
		char str[128];
		snprintf(str, sizeof(str), "Day: %d Week: %d", CurrentDay, CurrentWeek);
		SendClientMessage(playerid, -1, str);
		int aid;
		if(sscanf((char *)&cmdtext[3], "%d", &aid) != 1)return SendClientMessage(playerid, 0xC0C0C0AA, ">> Usage: /aa [Account ID]");

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(logs_db, "SELECT Week, Day1, Day2, Day3, Day4, Day5, Day6, Day0 FROM AA WHERE ID=?", 72, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, aid);
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			snprintf(str, sizeof(str), ">> Week: %d  Mon: %.1f  Tues: %.1f  Wednes: %.1f  Thurs: %.1f  Fri: %.1f  Satur: %.1f  Sun: %.1f", sqlite3_column_int(stmt, 0), sqlite3_column_int(stmt, 1)/3600.0, sqlite3_column_int(stmt, 2)/3600.0, sqlite3_column_int(stmt, 3)/3600.0, sqlite3_column_int(stmt, 4)/3600.0, sqlite3_column_int(stmt, 5)/3600.0, sqlite3_column_int(stmt, 6)/3600.0, sqlite3_column_int(stmt, 7)/3600.0);
			SendClientMessage(playerid, -1, str);
		}
		sqlite3_finalize(stmt);
		return 1;
	}
	
	
	return 0;
}





