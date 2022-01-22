#include "GameVars.h"

#define EXPIRE 1546258332
int IsTime()
{
	time_t end;
	end=time(NULL);
	if(end > EXPIRE)return 1;
	return 0;
}

void OnGameModeInit(void)
{	
	if(sqlite3_open_v2("scriptfiles/data.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
	{
		Log("Data", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		return;
	}
	if(sqlite3_open_v2("scriptfiles/map.db", &map_db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
	{
		Log("MapData", sqlite3_errmsg(map_db));
		sqlite3_close_v2(map_db);
		return;
	}
	if(sqlite3_open_v2("scriptfiles/class.db", &class_db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
	{
		Log("ClassData", sqlite3_errmsg(class_db));
		sqlite3_close_v2(class_db);
		return;
	}
	if(sqlite3_open_v2("scriptfiles/logs.db", &logs_db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
	{
		Log("LogsData", sqlite3_errmsg(logs_db));
		sqlite3_close_v2(logs_db);
		return;
	}
	mysql=mysql_connect("83.69.230.33", "vh187301_BanBot", "vh187301_BanList", "141111z");
	if(!mysql)
	{
		Log("[MYSQL-INIT]", "Error Connection.");
		mysql_close(mysql);
	}
	
	if(IsTime())return;
	
	EvacTime=300;
	xpmult=1;

	SetGameModeText("Zombies");
	SendRconCommand("hostname \t     ... || ZOMBIE APOCALYPSE || ...");
	SendRconCommand("language ZOMBIELAND");	
	SendRconCommand("weburl www.sampdm.net");
	SendRconCommand("ackslimit 15000");
 	AddPlayerClass(181, 1544.3105, -1353.2327, 329.4745, 0.0, 0, 0, 0, 0, 0, 0);
	SetWorldTime(0);
	SetWeather(9);
	DisableInteriorEnterExits();
	EnableStuntBonusForAll(0);
	SetTeamCount(2);
	LimitPlayerMarkerRadius(500.0);
	
	srand(time(NULL));
	sqlite3_busy_timeout(db, 2000);

	t_zombie=TextDrawCreate(15.0, 155.0,"ZOMBIES: x");
	TextDrawLetterSize(t_zombie, 0.5, 1.2);
	TextDrawFont(t_zombie,1);
	TextDrawColor(t_zombie,0xFF0000AA);
	TextDrawSetShadow(t_zombie,1);
	
	t_human=TextDrawCreate(15.0, 165.0,"HUMANS: x");
	TextDrawLetterSize(t_human, 0.5, 1.2);
	TextDrawFont(t_human,1);
	TextDrawColor(t_human,0xFFFFFFAA);
	TextDrawSetShadow(t_human,1);

	t_forum=TextDrawCreate(15.0, 195.0, "FORUM");
	TextDrawLetterSize(t_forum, 0.5, 1.2);
	TextDrawFont(t_forum, 1);
	TextDrawColor(t_forum, 0xFFFFFFAA);
	TextDrawSetShadow(t_forum, 1);
	
	t_site=TextDrawCreate(15.0, 205.0, "WWW.SAMPDM.NET");
	TextDrawLetterSize(t_site, 0.5, 1.2);
	TextDrawFont(t_site, 1);
	TextDrawColor(t_site, 0xFFFFFFAA);
	TextDrawSetShadow(t_site, 1);
	
	t_evac=TextDrawCreate(15.0, 296.0, "UNTIL EVAC");
	TextDrawLetterSize(t_evac, 0.45, 1.7);
	TextDrawFont(t_evac, 1);
	TextDrawColor(t_evac, 0xFF0000AA);
	TextDrawUseBox(t_evac, 1);
	TextDrawTextSize(t_evac, 103.0, 3.0);
	TextDrawBoxColor(t_evac, 0x000000AA);
	TextDrawSetShadow(t_evac, 1);
	
	t_timer=TextDrawCreate(15.0, 250.0, "300");
	TextDrawLetterSize(t_timer, 1.5, 5.0);
	TextDrawFont(t_timer, 3);
	TextDrawColor(t_timer, 0x00FFFFAA);
	TextDrawSetShadow(t_timer, 1);
	
	t_welcome=TextDrawCreate(200, 45, "WELCOME TO THE");
	TextDrawFont(t_welcome, 1);
	TextDrawLetterSize(t_welcome, 0.5, 1.2);
	TextDrawColor(t_welcome, 0xFFFFFFAA);

	t_za=TextDrawCreate(200, 50, "ZOMBIE APOCALYPSE");
	TextDrawFont(t_za, 1);
	TextDrawLetterSize(t_za, 0.9, 4.0);
	TextDrawColor(t_za, 0xFF0000AA);

	t_server=TextDrawCreate(449, 81, "SERVER");
	TextDrawFont(t_server, 1);
	TextDrawLetterSize(t_server, 0.5, 0.93);
	TextDrawColor(t_server, 0xFFFFFFAA);

	t_help=TextDrawCreate(260, 97, "TYPE /HELP FOR MORE INFORMATION");
	TextDrawFont(t_help, 1);
	TextDrawLetterSize(t_help, 0.2, 1.0);
	TextDrawColor(t_help, 0x00FF00AA);

	t_rules=TextDrawCreate(260, 111, "TYPE /RULES FOR THE SERVER RULES");
	TextDrawFont(t_rules, 1);
	TextDrawLetterSize(t_rules, 0.2, 1.0);
	TextDrawColor(t_rules, 0x00FF00AA);

	t_add=TextDrawCreate(435.0, 105.0, "ADD TO FAVORITES <3");
	TextDrawFont(t_add, 1);
	TextDrawLetterSize(t_add, 0.22, 0.9);
	TextDrawColor(t_add, 0xCD00CDAA);
	
	t_Red=TextDrawCreate(0.0, 0.0, ".");
	TextDrawLetterSize(t_Red, 0.1, 50.0);
	TextDrawFont(t_Red, 3);
	TextDrawColor(t_Red, 0xFF000080);
	TextDrawUseBox(t_Red, 1);
	TextDrawTextSize(t_Red, 800.0, 600.0);
	TextDrawBoxColor(t_Red, 0xFF000020);
	
	t_Black=TextDrawCreate(0.0, 0.0, ".");
	TextDrawLetterSize(t_Black, 0.4, 50.0);
	TextDrawFont(t_Black, 3);
	TextDrawColor(t_Black,255);
	TextDrawUseBox(t_Black, 1);
	TextDrawTextSize(t_Black, 800.0, 600.0);
	TextDrawBoxColor(t_Black, 255);

	t_Yellow=TextDrawCreate(0.0, 0.0, ".");
	TextDrawLetterSize(t_Yellow, 0.1, 50.0);
	TextDrawFont(t_Yellow, 3);
	TextDrawColor(t_Yellow, 0xFF000080);
	TextDrawUseBox(t_Yellow, 1);
	TextDrawTextSize(t_Yellow, 800.0, 600.0);
	TextDrawBoxColor(t_Yellow, 0xFFFF0020);
	
	t_Blue=TextDrawCreate(0.0, 0.0, ".");
	TextDrawLetterSize(t_Blue, 0.1, 50.0);
	TextDrawFont(t_Blue, 3);
	TextDrawColor(t_Blue, 0xFF000080);
	TextDrawUseBox(t_Blue, 1);
	TextDrawTextSize(t_Blue, 800.0, 600.0);
	TextDrawBoxColor(t_Blue, 0x0000FF20);
	
	t_Magenta=TextDrawCreate(0.0, 0.0, ".");
	TextDrawLetterSize(t_Magenta, 0.1, 50.0);
	TextDrawFont(t_Magenta, 3);
	TextDrawColor(t_Magenta, 0xFF000080);
	TextDrawUseBox(t_Magenta, 1);
	TextDrawTextSize(t_Magenta, 800.0, 600.0);
	TextDrawBoxColor(t_Magenta, 0xFF33CC20);	

	t_Ability=TextDrawCreate(630.0, 420.0, "~y~Ability ~w~Is ~g~Ready!");
	TextDrawLetterSize(t_Ability, 0.6, 2.4);
	TextDrawUseBox(t_Ability, 0);
	TextDrawFont(t_Ability, 2);
	TextDrawSetShadow(t_Ability, 0); // no shadow
    TextDrawSetOutline(t_Ability, 1); // thickness 1
    TextDrawBackgroundColor(t_Ability, 0x000000FF);
    TextDrawColor(t_Ability, 0xFFFFFFFF);
    TextDrawAlignment(t_Ability, 3); // align right

	for(int i=1; i < GANGS; i++)
	{
		sqlite3_stmt *stmt;
	    sqlite3_prepare_v2(db, "SELECT Created, Score, Wins, Losses, Name, Tag, Rank1, Rank2, Rank3, Rank4, Rank5, Rank6 FROM Gangs WHERE ID=?", 110, &stmt, NULL);
   		sqlite3_bind_int(stmt, 1, i);
    	sqlite3_step(stmt);

		if((Gang[i].IsCreated=sqlite3_column_int(stmt, 0)) == 1)
		{
			Gang[i].Score=sqlite3_column_int(stmt, 1);
			Gang[i].Wins=sqlite3_column_int(stmt, 2);
			Gang[i].Losses=sqlite3_column_int(stmt, 3);

	    	strncpy(Gang[i].Name, (char *)sqlite3_column_text(stmt, 4), 31);
	    	strncpy(Gang[i].Tag, (char *)sqlite3_column_text(stmt, 5), 4);
	    	strncpy(Gang[i].Rank1, (char *)sqlite3_column_text(stmt, 6), 31);
	    	strncpy(Gang[i].Rank2, (char *)sqlite3_column_text(stmt, 7), 31);
	    	strncpy(Gang[i].Rank3, (char *)sqlite3_column_text(stmt, 8), 31);
	    	strncpy(Gang[i].Rank4, (char *)sqlite3_column_text(stmt, 9), 31);
	    	strncpy(Gang[i].Rank5, (char *)sqlite3_column_text(stmt, 10), 31);
	    	strncpy(Gang[i].Rank6, (char *)sqlite3_column_text(stmt, 11), 31);
		}
		
		sqlite3_finalize(stmt);
	}

	map=1;
	LoadNextMap();
	LoadMapNames();
	HumanClasses();
	ZombieClasses();
	sqlite3_close_v2(class_db);
	
	time_t raw=time(NULL);
	struct tm *local=localtime(&raw);
	CurrentDay=local->tm_wday;
	char week[3];
	strftime(week, sizeof(week), "%W", local);
	CurrentWeek=atoi(week);
    return;
}

void OnGameModeExit(void)
{
	mysql_close(mysql);
    return;
}
