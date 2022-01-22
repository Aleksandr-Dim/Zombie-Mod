#include "GameVars.h"

int TextContainsIP(const char text[])
{
	int digits=0, dotcount=0;

	for(int i=0; i < strlen(text); i++)
	{
		if(text[i] >= '0' && text[i] <= '9')digits++;
		else if(text[i] == '.')dotcount++;
	}

	if((digits > 6) && (dotcount >= 3))return 1;
    return 0;
}



void OnPlayerText(int playerid, char text[])
{
	if(!za[playerid].Log)return;
	if((time(NULL)-za[playerid].Text) < 1)
	{
		GameTextForPlayer(playerid, "~g~Wait 1 second!", 1000, 5);
		return;
	}
	za[playerid].Text=time(NULL);
	
	if(Ask.Opened == true)
	{
		if(Ask.CaseSensitive == false)
		{
			if(!strcasecmp(text, Ask.Answer))
			{
				Ask.Opened=false;
				SetPlayerScore(playerid, GetPlayerScore(playerid)+Ask.EXP);
				char str[128];
				snprintf(str, sizeof(str), ">> %s was the first one to answer the question (Answer : %s) and earns %d XP!", za[playerid].Name, Ask.Answer, Ask.EXP);
				SendClientMessageToAll(0xFF00FFAA, str);
			}
		}
		else
		{
			if(!strcmp(text, Ask.Answer))
			{
				Ask.Opened=false;
				SetPlayerScore(playerid, GetPlayerScore(playerid)+Ask.EXP);
				char str[128];
				snprintf(str, sizeof(str), ">> %s was the first one to answer the question (Answer : %s) and earns %d XP!", za[playerid].Name, Ask.Answer, Ask.EXP);
				SendClientMessageToAll(0xFF00FFAA, str);
			}	
		}
	}
	
	if(za[playerid].Mute > time(NULL))
	{
		GameTextForPlayer(playerid, "~g~You're muted!", 1000, 5);
		return;
	}

	//Anti-Caps Off;
	int CapitalLetters=0;
	for(int i=0; text[i] != 0; i++)
	{
		if(text[i] >= 'A' && text[i] <= 'Z')CapitalLetters++;
	}
	if(CapitalLetters > 8)
	{
		for(int i=1; text[i] != 0; i++) 
		{
			if(isupper(text[i]) && text[i-1] != ' ')
			{
				text[i]=tolower(text[i]);
			}
		}
	}

	char *string=0; int pos=0;
	for(int i=0; i < 10; i++)
	{
		if((string=strstr(&text[pos], "{0x")) != NULL && (pos=string-text) < 110 && text[pos+9] == '}')
		{
			strncpy(&text[pos+1], &text[pos+3], 126-pos);
			pos=pos+10;
		}
	}

	if(text[0] == '@' && za[playerid].Admin > 0)
	{
		char str[144];
		snprintf(str, sizeof(str), "[ADMCHAT] %s(%d): %s", za[playerid].Name, playerid, &text[1]);

		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Admin > 0)
			{
				SendClientMessage(i, 0x33CCFFAA, str);
			}
		}
		
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(logs_db, "INSERT INTO AdminChat (Time, AccountID, Name, Message) VALUES(?, ?, ?, ?)", 73, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, (int)time(NULL));
		sqlite3_bind_int(stmt, 2, za[playerid].AccountID);
		sqlite3_bind_text(stmt, 3, (char *)&za[playerid].Name, strlen((char *)&za[playerid].Name), NULL);
		sqlite3_bind_text(stmt, 4, &(text[1]), strlen(&(text[1])), NULL);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return;
	}

	if(text[0] == '!' && za[playerid].Clan)
	{
		char str[144];
	 	snprintf(str, sizeof(str), "[%s] %s(%d): %s", (char *)(&Gang[za[playerid].Clan].Rank1[((za[playerid].Rank)-1)*32]), za[playerid].Name, playerid, &text[1]);
		
		if(TextContainsIP(text))
		{
			SendClientMessage(playerid, 0xFF00FFAA, str);
			char str2[144];
			snprintf(str2, sizeof(str2), ">> Possible advertiser in clan-chat %s[%d]: %s", za[playerid].Name, playerid, text);
			SendClientMessageToAdmins(str2);
			return;
		}
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].Clan == za[playerid].Clan)
			{
				SendClientMessage(i, 0xFF00FFAA, str);
			}
		}
		return;
	}

	if(text[0] == '#' && za[playerid].VIP)
	{
		char str[128];
		snprintf(str, sizeof(str), "[VIPChat] %s(%d): %s", za[playerid].Name, playerid, &text[1]);
		
		if(TextContainsIP(text))
		{
			SendClientMessage(playerid, COLOR_ORANGE, str);
			char str2[144];
			snprintf(str2, sizeof(str2), ">> Possible advertiser in VIP-chat %s[%d]: %s", za[playerid].Name, playerid, text);
			SendClientMessageToAdmins(str2);
			return;
		}
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].VIP)
			{
				SendClientMessage(i, COLOR_ORANGE, str);
			}
		}
		return;
	}

	char str[144];
	char color[9], clan[8], vip[14], admin[15];
	memset(color, 0, sizeof(color));
	memset(clan, 0, sizeof(clan));
	memset(vip, 0, sizeof(vip));
	memset(admin, 0, sizeof(admin));
	
	//Human/Zombie Rogue
	if(temp[playerid].HumanRogue)strncpy(color, "{99CC00}", sizeof(color)-1);
	else if(temp[playerid].ZombieRogue)strncpy(color, "{00B0B0}", sizeof(color)-1);
	else if(za[playerid].Team == false)
	{
		if(temp[playerid].Infection)strncpy(color, "{FF8080}", sizeof(color)-1);
		else strncpy(color, "{00CCCC}", sizeof(color)-1);
	}
	else strncpy(color, "{99CC00}", sizeof(color)-1);

	if(za[playerid].Clan > 0)snprintf(clan, sizeof(clan), "[%s]", Gang[za[playerid].Clan].Tag);

	if(za[playerid].VIP == 1)strncpy(vip, "{CD7F32}(VIP)", sizeof(vip)-1);
	else if(za[playerid].VIP == 2)strncpy(vip, "{C0C0C0}(VIP)", sizeof(vip)-1);
	else if(za[playerid].VIP == 3)strncpy(vip, "{FFD700}(VIP)", sizeof(vip)-1);
	
	if(za[playerid].Hide == true)
	{
		if(za[playerid].Admin == 1)strncpy(admin, "{F5ED1E}(H)", sizeof(admin)-1);
		else if(za[playerid].Admin == 2)strncpy(admin, "{50FF00}(M)", sizeof(admin)-1);
		else if(za[playerid].Admin == 3)strncpy(admin, "{33CCff}(A)", sizeof(admin)-1);
		else if(za[playerid].Admin == 4)strncpy(admin, "{FF0000}(LA)", sizeof(admin)-1);
		else if(za[playerid].Admin == 5)strncpy(admin, "{9D03A8}(HA)", sizeof(admin)-1);
		else if(za[playerid].Admin == 6)strncpy(admin, "{EC3B83}(BOSS)", sizeof(admin)-1);
	}

	snprintf(str, sizeof(str), "%s%s%s%s%s{FFFFFF}(%d): %s", clan, vip, admin, color, za[playerid].Name, playerid, text);

	//Anti-hacker id in the main chat;
	if(za[playerid].Admin == 0 && (strstr(text, "id") != NULL && (strstr(text, "hacker") != NULL || strstr(text, "hack") != NULL || strstr(text, "cheat") != NULL || strstr(text, "cheater") != NULL)))
	{
		SendClientMessage(playerid, -1, ">> Use {FFFF00}/report {FFFFFF}to get {FF0000}EXP {FFFFFF}for banned hackers and cheaters!!");
		SendClientMessage(playerid, -1, str);
		return;
	}

	//Anti-Spam
	if((za[playerid].LastSpamMessageTime+10) > za[playerid].Text)
	{
		if(!strcmp(text, LastSpamMessage[playerid]))
		{
			za[playerid].LastSpamMessageCount++;
			
			if(za[playerid].LastSpamMessageCount > 1)
			{
				SendClientMessage(playerid, -1, str);
				za[playerid].LastSpamMessageTime=za[playerid].Text;
				return;
			}
		}
		else
		{
			za[playerid].LastSpamMessageCount=0;		
			strncpy(LastSpamMessage[playerid], text, 127);
			LastSpamMessage[playerid][127]=0;
		}
	}
	else
	{
		za[playerid].LastSpamMessageCount=0;
		strncpy(LastSpamMessage[playerid], text, 127);
		LastSpamMessage[playerid][127]=0;
	}
	za[playerid].LastSpamMessageTime=za[playerid].Text;

	//Anti-Advertisements;
	if(!TextContainsIP(text))SendClientMessageToAll(-1, str);	
	else
	{
		SendClientMessage(playerid, -1, str);
		char str2[144];
		snprintf(str2, sizeof(str2), ">> Possible advertiser in main-chat %s[%d]: %s", za[playerid].Name, playerid, text);
		SendClientMessageToAdmins(str2);
	}
	return;
}


void SendClientMessageToAdmins(char str[])
{
	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Admin > 0)
		{
			SendClientMessage(i, COLOR_BLUE, str);
		}
	}
}
