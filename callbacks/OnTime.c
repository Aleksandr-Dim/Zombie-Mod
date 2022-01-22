#include "GameVars.h"

static int DefCounter;

char questions[49][2][110]=
{
	{"Which country is norther: \"Rome\" or \"New York\"?", "Rome"},
	{"Where would you find the Sea of Tranquility?", "Moon"},
	{"What item of clothing was named after its Scottish inventor?", "Mackintosh"},
	{"Name the seventh planet from the sun?", "Uranus"},
	{"Which is the only American state to begin with the letter 'p'?", "Pennsylvania"},
	{"Name the world's biggest island?", "Greenland"},
	{"What is the world's longest river?", "Amazon"},
	{"Name the world's largest ocean", "Pacific"},
	{"What is the capital city of Spain?", "Madrid"},
	{"Which kind of bulbs were once exchanged as a form of currency?", "Tulip"},
	{"Which chess piece can only move diagonally?", "Bishop"},
	{"Which Turkish city has the name of a cartoon character?", "Batman"},
	{"What is a very cold part of Russia?", "Siberia"},
	{"How many continents are there?", "Seven"},
	{"How many time zones are there in the world? (use numerals)", "24"},
	{"Which is the largest desert on earth?", "Sahara"},
	{"What is the largest state of the United States?", "Alaska"},
	{"What is the largest city in Canada?", "Toronto"},
	{"Who was the first man to fly around the earth with a spaceship?", "Gagarin"},
	{"Which planet is nearest the sun?", "Mercury"},
	{"In computing what is RAM short for?", "Random Access Memory"},
	{"How do you call a motorcycle with four wheels?", "Quad"},
	{"What does the abbreviation GPS mean?", "Global Positioning System"},
	{"What is the most spoken language in the world?", "Chinese"},
	{"What does the abbreviation SMS mean?", "Short Message Service"},
	{"What is the meaning of the following abbreviation: IBM?", "International Business Machines"},
	{"What colour to do you get when you mix red and white?", "Pink"},
	{"What colour to do you get when you mix red and blue?", "Purple"},
	{"What colour to do you get when you mix blue and yellow?", "Green"},
	{"What colour to do you get when you mix red and yellow?", "Orange"},		
	{"What colour to do you get when you mix red and green?", "Brown"},
	{"Which plant does the Canadian flag contain?", "Maple"},
	{"Are unicorns extinct?", "False"},
	{"What colour is the 'L' in the Google logo?", "Green"},
	{"What ended in 1896?", "1895"},
	{"What is the strongest force on earth? (Romantic)", "Love"},
	{"What do we call the science of classifying living things? (The first 'R')", "Racism"},
	{"Where was the American Declaration of Independence signed?", "At the bottom"},
	{"If there are three apples and you took two away, how many do you have?", "Two"},
	{"Which is heavier, 100 pounds of rocks or 100 pounds of gold??", "The same"},
	{"Can you spell 80 in two letters?", "AT"},
	{"How many sides does a circle have?", "Two"},
	{"What word in the English language is always spelled incorrectly?", "Incorrectly"},
	{"When do you stop at green and go at red?", "Watermelon"},
	{"What does below number rebus represents 123S468A037F189E0202T0888Y913?", "Safety"},
	{"It looks square from outside, round when opened, triangle when taken out. What is It?", "Pizza"},
	{"I am deaf dumb and blind but never lies. Who am I?", "Mirror"},
	{"If 77x=189x=345x. What is the value of x?", "0"},
	{"Man has 5 sons and each son have 1 sister. How many people are there in the family?", "7"}	
};


static inline void AskQuestion()
{
	Ask.Opened=true;
	Ask.EXP=rand()%25+7;

	switch(rand()%4)
	{
		case 0:
		{
			int Players[AMOUNT], Counter=0;
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true)
				{
					Players[Counter]=i;
					Counter++;
				}
			}
			
			if(Counter > 0)
			{	
				int answer=Players[rand()%Counter];
				snprintf(Ask.Answer, 4, "%d", answer);
				
				char str[128];
				snprintf(str, sizeof(str), "| RANDOM QUESTION FOR %d XP: What's the ID of {FFFFFF}%s? ", Ask.EXP, za[answer].Name);
	
				SendClientMessageToAll(-1, "\"---------------------------------------\"");
				SendClientMessageToAll(0xFF00FFAA, str);
				SendClientMessageToAll(-1, "\"---------------------------------------\"");
			}
			Ask.CaseSensitive=false;
			break;
		}
		case 1:
		{
			int C1,C2,C3,C4,C5;
			
			C1=rand()%100;
			C2=rand()%100;
			C3=rand()%100;
			C4=rand()%100;
			C5=rand()%100;

			int answer=C1-C2+C3-C4+C5;
			snprintf(Ask.Answer, 4, "%d", answer);

			char str[128];
			snprintf(str, sizeof(str), "| RANDOM QUESTION FOR %d XP: {FFFFFF}%d - %d + %d - %d + %d = ?", Ask.EXP, C1, C2, C3, C4, C5);
			
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			SendClientMessageToAll(0xFF00FFAA, str);
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			Ask.CaseSensitive=false;
			break;
		}
		case 2:
		{
			memset(&(Ask.Answer), 0, 15);
			char alphabet[52]="abcdefghijklmnopqrstuvqxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			for(int i=0; i < 10; i++)
			{
				Ask.Answer[i]=alphabet[rand()%52];
			}
			
			char str[128];
			snprintf(str, sizeof(str), "| RANDOM WRITING FOR %d XP: {FFFFFF}%s", Ask.EXP, Ask.Answer);
			
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			SendClientMessageToAll(0xFF00FFAA, str);
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			Ask.CaseSensitive=true;
			break;
		}
		case 3:
		{
			int RandomValue=rand()%49;
			snprintf((char *)&(Ask.Answer), 110, "%s", questions[RandomValue][1]);
			
			char str[128];
			snprintf(str, sizeof(str), "RANDOM QUIZ FOR %d XP: {FFFFFF}%s", Ask.EXP, questions[RandomValue][0]);
			
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			SendClientMessageToAll(0xFF00FFAA, str);
			SendClientMessageToAll(-1, "\"---------------------------------------\"");
			Ask.CaseSensitive=false;
			break;
		}	
	}
	return;
}


void OnTime()
{
	if(Votekick.VoteOpen == true && Votekick.VoteTime < time(NULL))
	{
		Votekick.VoteOpen=false;
		SendClientMessageToAll(0xFFFF00AA,">> Votekick has been aborted.");
		Votekick.TimeLimit=time(NULL);
	}

	int HM=0, ZM=0;
	int LastHumanID=0;

	for(int i=0; i < AMOUNT; i++)
	{
		if(za[i].Log == true)
		{
			if(za[i].Duel == false && za[i].InEvent == false)
			{
				if(za[i].Team == false)
				{
			        HM++;
			        LastHumanID=i;
				}
				else
				{
					ZM++;
				}
			}
			
			//Rogue Human
			if(za[i].UCID == 26 && temp[i].HumanRogue == true && time(NULL) > (temp[i].Pause-20))
			{
				temp[i].HumanRogue=false;
				SetPlayerSkin(i, 97);
				if(temp[i].Infection == false)SetPlayerColor(i, 0x00CCCCAA);
				else SetPlayerColor(i, 0xFF8080AA);
				TogglePlayerControllable(i, 1);
			}
			//Rogue Zombie
			else if(za[i].UCID == -8 && temp[i].ZombieRogue == true && time(NULL) > (temp[i].Pause-20))
			{
				temp[i].ZombieRogue=false;
				SetPlayerColor(i, 0x99CC00AA);
				SetPlayerSkin(i, 135);
				TogglePlayerControllable(i, 1);
			}

			if(za[i].Team == false)
			{
				za[i].AFKhuman++;
				if(za[i].AFKhuman > 2)
				{
					DHS[i].AFKtime+=5;
					za[i].Team=true;
				}
			}
			else
			{
				za[i].AFKzombie++;
				if(za[i].AFKzombie > 1)
				{
					DHS[i].AFKtime+=5;
					if(za[i].Admin)AA[i].AFK+=5;
					SetPlayerVirtualWorld(i, VIRTUALWORLD_AFK);
				}	
			}
	
			za[i].PingCounter++;
			if(za[i].PingCounter > 8)
			{
				za[i].PingCounter=1;
				za[i].Ping=0;
			}
			za[i].Ping+=GetPlayerPing(i);
				
			if((za[i].Ping/za[i].PingCounter) > 650 && za[i].PingCounter > 2)//3
			{
				SendClientMessage(i, COLOR_ANTIHACK, "[PING] Your ping is too high. Fix it or you will be kicked.");
			}
			if((za[i].Ping/za[i].PingCounter) >= 660 && za[i].PingCounter > 4)//5
			{
				char str[128];
				snprintf(str, sizeof(str), "%s has been kicked [Reason: Ping too high]", za[i].Name);
				SendClientMessageToAll(COLOR_ANTIHACK, str);
				SetTimerEx("PlayerKick", 500, i);
			}
			
			temp[i].AntiTA=0;
			temp[i].BunnyHop=0;
		
			if(!za[i].DuelInviteTime)
			{
				if(za[i].DuelInviteTime+40 < time(NULL))
				{
					za[i].DuelInviteTime=0;
					za[i].DuelInviteID=-1;
					za[i].DuelInviteWID=0;	
				}
			}
			
			
			//Auto-Deduction to detect health-hackers.
			if(EvacTime == 230 || EvacTime == 160 || EvacTime == 80)
			{
				if(za[i].AdminDuty == false && za[i].UCID != -15 && za[i].UCID != -47)
				{
					//No deducting already.
					if(temp[i].Infection == false && temp[i].Burned == false && temp[i].Poison == 0)
					{
						float health;
						GetPlayerHealth(i, &health);
						if(health > 11.0)
						{
							SetPlayerHealth(i, health-1.0);
						}
					}
				}
			}//Auto-Deduction
			
		}//if logged.
		
		za[i].Fakekill=0;
	}

	if(HM == 0 && ZM > 1)
	{
		if(EvacTime > -55)
		{
			SendClientMessageToAll(0xFF0000AA, ">> ZOMBIES WIN!");
			EvacTime=-50;
		}
	}
	else if(HM == 1 && ZM > 3 && lasthuman == false)
	{
		if(temp[LastHumanID].Evac == false)
		{
		    lasthuman=true;
		    GameTextForAll("~r~Kill ~w~the last human", 3000, 3);
			SendClientMessageToAll(0x00EEADDF, ">>> Kill the last human and receive extra exp!");
		}
	}

	if(EvacTime == 300)
	{
		Delete3DTextLabel(label[0]);
		Delete3DTextLabel(label[1]);
		Delete3DTextLabel(label[2]);
		Delete3DTextLabel(label[3]);
		Delete3DTextLabel(label[4]);

		label[0]=Create3DTextLabel("Spawn {00FF00}Zone", 0xFF0000CC, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1, 30.0, 0, 1);
		label[1]=Create3DTextLabel("Spawn {00FF00}Zone", 0xFF0000CC, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2, 30.0, 0, 1);
		if(MapData.ZombieSpawn > 2)label[2]=Create3DTextLabel("Spawn {00FF00}Zone", 0xFF0000CC, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3, 30.0, 0, 1);
		if(MapData.ZombieSpawn > 3)label[3]=Create3DTextLabel("Spawn {00FF00}Zone", 0xFF0000CC, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4, 30.0, 0, 1);
		if(MapData.ZombieSpawn > 4)label[4]=Create3DTextLabel("Spawn {00FF00}Zone", 0xFF0000CC, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5, 30.0, 0, 1);
	}
	else if(EvacTime == 240 && (HM+ZM) > 0)
	{
		AskQuestion();
	}
	else if(EvacTime == 100 && Ask.Opened == true)
	{
		Ask.Opened=false;
		SendClientMessageToAll(-1, ">> No one could answer!");
	}

	char str[40];
	snprintf(str, sizeof(str), "HUMANS: %d", HM);
    TextDrawSetString(t_human,str);

	snprintf(str, sizeof(str), "ZOMBIES: %d", ZM);
	TextDrawSetString(t_zombie, str);

 	EvacTime=EvacTime-5;
	
	if(EvacTime > 0)
	{
		snprintf(str, sizeof(str), "%d", EvacTime);
		TextDrawSetString(t_timer,str);	
	}
	else
	{
		TextDrawSetString(t_timer, "---");
		
		if(EvacTime == 0)
		{
			SendClientMessageToAll(0xFF0000AA, ">> An evacuation has arrived for humans!");
			SendClientMessageToAll(-1,"   Humans get to the checkpoint within a minute!");

			if(MapData.CPx != 0.0)
			{
				if(MapData.CPRaceType == 0)
				{
					for(int i=0; i < AMOUNT; i++)
					{
						if(za[i].Log == true)
						{
							SetPlayerCheckpoint(i, MapData.CPx, MapData.CPy, MapData.CPz, MapData.CPSize);
						}
					}
				}
				else
				{
					for(int i=0; i < AMOUNT; i++)
					{
						if(za[i].Log == true)
						{
							SetPlayerRaceCheckpoint(i, MapData.CPRaceType, MapData.CPx, MapData.CPy, MapData.CPz, MapData.CPRaceNextX, MapData.CPRaceNextY, MapData.CPRaceNextZ, MapData.CPSize);
						}
					}
				}
			}
			
			if(MapData.GateModel1 != 0)
				MoveObject(MapData.Gate1, MapData.GateMoveX1, MapData.GateMoveY1, MapData.GateMoveZ1, MapData.GateSpeed1);

			if(MapData.GateModel2 != 0)
				MoveObject(MapData.Gate2, MapData.GateMoveX2, MapData.GateMoveY2, MapData.GateMoveZ2, MapData.GateSpeed2);
		}
		
		if(EvacTime == -55)
		{
			SendClientMessageToAll(0xFFFF00AA, "ALL SURVIVORS EVACUATED!");
			SendClientMessageToAll(-1, "Beginning a new round...");
			
			if(MapData.CameraToX != 0)
			{
				for(int i=0; i < AMOUNT; i++)
				{
					if(za[i].Log == true && temp[i].Evac == false && za[i].Duel == false && za[i].InEvent == false)
					{
						SetPlayerCameraPos(i, MapData.CameraX, MapData.CameraY, MapData.CameraZ);
						SetPlayerCameraLookAt(i, MapData.CameraToX, MapData.CameraToY, MapData.CameraToZ);
					}
				}
			}
			
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Duel == false && za[i].InEvent == false)
				{
					TextDrawShowForPlayer(i, t_welcome);
					TextDrawShowForPlayer(i, t_za);
					TextDrawShowForPlayer(i, t_server);
					TextDrawShowForPlayer(i, t_help);
					TextDrawShowForPlayer(i, t_rules);
					TextDrawShowForPlayer(i, t_add);
					GameTextForPlayer(i, "~r~ROUND OVER!~r~~n~~w~STARTING NEW ROUND...~w~", 5000, 5);
				}
			}
		}
		if(EvacTime == -60)
		{

			if((map%4) == 0)
			{
				for(int i=1; i < GANGS; i++)
				{
					if(Gang[i].IsCreated == true)
					{
						sqlite3_stmt *stmt;
	    				sqlite3_prepare_v2(db, "UPDATE Gangs SET Score=?, Wins=?, Losses=? WHERE ID=?", 53, &stmt, NULL);
						sqlite3_bind_int(stmt, 1, Gang[i].Score);
						sqlite3_bind_int(stmt, 2, Gang[i].Wins);
						sqlite3_bind_int(stmt, 3, Gang[i].Losses);
						sqlite3_bind_int(stmt, 4, i);
						sqlite3_step(stmt);
						sqlite3_finalize(stmt);
					}
				}
			}
			
			map++;
			LoadNextMap();
			LoadMapNames();
			SetWorldTime(MapData.Time);
			SetWeather(MapData.Weather);

			char str[80];
        	snprintf(str, sizeof(str), "|: Entering Map #%d (%s)", map, MapName);
        	SendClientMessageToAll(COLOR_BROWN, str);

			snprintf(str, sizeof(str), "language %s", MapName);
			SendRconCommand(str);

			if(MapData.GateModel1 != 0)
				MapData.Gate1=CreateObject(MapData.GateModel1, MapData.GateX1, MapData.GateY1, MapData.GateZ1, MapData.GateRotX1, MapData.GateRotY1, MapData.GateRotZ1, 300.0);

			if(MapData.GateModel2 != 0)
				MapData.Gate2=CreateObject(MapData.GateModel2, MapData.GateX2, MapData.GateY2, MapData.GateZ2, MapData.GateRotX2, MapData.GateRotY2, MapData.GateRotZ2, 300.0);

			int NumberOfPlayers[AMOUNT], Count=0;
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true)
				{
					za[i].AdminDuty=false;
					
					if(za[i].Duel == false && za[i].InEvent == false)
					{
						za[i].Team=false;
						za[i].SpawnedAsHuman=true;
						NumberOfPlayers[Count]=i;
						Count++;
					}
					else
					{
						za[i].Team=true;
					}
				}
			}

			int Zombies=(Count/3)+2;
			if(Count < 11)Zombies=Count/2;

			int k=0, Players=Count, zombs[Count];
			
			if(Zombies > 0)
			{
				for(int i=0; i < Count; i++)
				{		
					int ZombieID=rand()%Players;
					int ChoosenZombie=NumberOfPlayers[ZombieID];
	
					if(za[ChoosenZombie].PlayedAsZombie < 2)
					{
						za[ChoosenZombie].PlayedAsZombie++;
						za[ChoosenZombie].Team=true;
						za[ChoosenZombie].SpawnedAsHuman=false;
						
						zombs[k]=ChoosenZombie;
						k++;
					}
	
					if(k == Zombies)
					{
						break;
					}
	
					Players--;
					for(int i=ZombieID; i < Players; i++)
					{
						NumberOfPlayers[i]=NumberOfPlayers[i+1];
					}
					
					if(Players == 0)
					{
						break;
					}
				}
			}

			if(Count > 13)
			{
				BossZombie=true;
				BZID=zombs[rand()%Zombies];
				char str[128];
				snprintf(str, sizeof(str),">>> {FF0000}%s {FFFFFF}was chosen to be the bosszombie. Kill him to get additional {FFD000}40exp!", za[BZID].Name);
				SendClientMessageToAll(-1, str);
			}

			lasthuman=false;
			EvacTime=300;
			TextDrawSetString(t_timer, "300");
			
			memset(&Undertaker, 0, sizeof(Undertaker));//Reset Undertakers' Positions;
			
			for(int i=0; i < AMOUNT; i++)
			{
				memset(&temp[i], 0, sizeof(temp[i]));
				
				DisablePlayerCheckpoint(i);
				DisablePlayerRaceCheckpoint(i);
				
				if(za[i].Log == true && za[i].Duel == false && za[i].InEvent == false)
				{
					if(IsPlayerInAnyVehicle(i))SetVehicleToRespawn(GetPlayerVehicleID(i));
					GameTextForPlayer(i, "~r~TRY TO STAY~n~~w~ALIVE", 5000, 6);
					
					if(za[i].IsDead == false)
					{
						ResetPlayerWeapons(i);
						SetCameraBehindPlayer(i);
						SpawnPlayer(i);
					}
				}
			}

			TextDrawHideForAll(t_welcome);
			TextDrawHideForAll(t_za);
			TextDrawHideForAll(t_server);
			TextDrawHideForAll(t_help);
			TextDrawHideForAll(t_rules);
			TextDrawHideForAll(t_add);

			SendClientMessageToAll(0xFFFF00AA,"Creating objects...");
		}
	}//if(EvacTime > 0) ... else ...
}

void SetVip()
{
   	int i=0; int num=cache_num_rows(mysql); int AccID, level;
	while(i < num)
	{
		sqlite3_stmt *stmt;
    	sqlite3_prepare_v2(db, "UPDATE Data SET Vip=?, VipTime=? WHERE ID=?", 43, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, (level=cache_get_field_content_int(i, "level", mysql)));
		sqlite3_bind_int(stmt, 2,  time(NULL)+31*24*3600);
		sqlite3_bind_int(stmt, 3, (AccID=cache_get_field_content_int(i, "AccID", mysql)));
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		for(int k=0; k < AMOUNT; k++)
		{
		    if(AccID == za[k].AccountID)
		    {
				za[k].VIP=level;
		    }
		}

		i++;
	}

	mysql_tquery(mysql, "DELETE FROM zz", "", 0);
	return;
}

static int cleanup=0;

void Defender()
{
	DefCounter++;
	
	if((DefCounter%7) == 0)
	{
		char *str="";
		
		switch(rand()%11)
		{
			case 0: 
			{
				str=">> As a Human, you must survive. As a Zombie, you must kill humans.";
				break;
			}
			case 1:
			{
				str=">> Zombies can infect people using LEFT ALT. This will slowly kill humans.";
				break;
			}
			case 2:
			{
				str=">> Use /report to report rulebreakers.";
				break;
			}
			case 3:
			{
				str=">> Change your class using /class!";
				break;
			}						
			case 4:
			{
				str=">> You can save your attachments with /saveatt";
				break;
			}
			case 5:
			{
				str=">> Try out new gang system. Check /gang help";
				break;
			}			
			case 6:
			{
				str=">> You can block pms if you dont want to receive any with /settings";
				break;
			}			
			case 7:
			{
				str=">> Use /weapons if you dont know the ID of a specific weapon.";
				break;
			}			
			case 8:
			{
				str=">> Use /maps to see map list.";
				break;
			}
			case 9:
			{
				str=">> You can use /pay to transfer EXP!";
				break;
			}
			case 10:
			{
				str=">> You can earn EXP by suggesting a map at the forum!";
				break;
			}
			case 11:
			{
				str=">> Use /unstuck if you are stuck in the map!";
				break;
			}
		}
		SendClientMessageToAll(0xADFF2FAA, str);
		
		time_t rawtime=time(NULL);
		struct tm *ServerTime=localtime(&rawtime);
		int WeekDay=ServerTime->tm_wday;
		int MonthDay=ServerTime->tm_mday;
		int Month=ServerTime->tm_mon;

		//Weekends, Halloween, New Year;
		if((WeekDay == 0 || WeekDay == 6) || (Month == 9 && MonthDay == 31) || (Month == 11 && (MonthDay == 25 || MonthDay == 30 || MonthDay == 31)) || (Month == 0 && (MonthDay == 1 || MonthDay == 2 || MonthDay == 7)))
		{
			xpmult=2;
		}
		else
		{
			xpmult=1;
		}	
	}
	if((DefCounter%14) == 0)
	{
		mysql_tquery(mysql, "SELECT * FROM zz", "SetVip", 0);

		time_t raw=time(NULL);
		struct tm *local=localtime(&raw);
		
		if(cleanup == 0 && local->tm_hour == 23 && local->tm_min < 20)
		{
			//Removing useless player accounts.
		 	sqlite3_stmt *stmt;
		   	sqlite3_prepare_v2(db, "DELETE FROM Data WHERE Kills < 20 AND EXP < 200 AND LastTime < ?", 64, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, (int)raw-3600*24*7);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			//Removing old logs.
			int time=(int)raw-3600*24*30;
			sqlite3_prepare_v2(logs_db, "DELETE FROM AdminChat WHERE Time < ?", 36, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, time);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
				
			sqlite3_prepare_v2(logs_db, "DELETE FROM PMChat WHERE Time < ?", 33, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, time);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			
//			sqlite3_prepare_v2(logs_db, "DELETE FROM AA WHERE Time < ?", 33, &stmt, NULL);
//			sqlite3_bind_int(stmt, 1, time-3600*24*31*3);
//			sqlite3_step(stmt);
//			sqlite3_finalize(stmt);
				
			cleanup=1;
		}
		else if(cleanup < 2 && local->tm_hour == 0 && local->tm_min < 20)
		{				
			for(int i=0; i < AMOUNT; i++)
			{
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
				
			CurrentDay=local->tm_wday;
			if(CurrentDay == 1)
			{
				char week[3];
				strftime(week, sizeof(week), "%W", local);
				CurrentWeek=atoi(week);
			}
				
			cleanup=2;
		}
		else if(cleanup == 2 && local->tm_hour == 0 && local->tm_min > 20)
		{
			cleanup=0;
		}
	}

	int IsAdminOnline=0;
	for(int i=0; i < AMOUNT; i++)if(za[i].Admin)IsAdminOnline++;

	for(int i=0; i < AMOUNT; i++)
	{
	    if(za[i].Log == true)
	    {
	    	//Anti-Armour Hack #1;
			float armour;
			GetPlayerArmour(i, &armour);
			if(armour > 50.0)
			{
			    BanPlayer(i, "Anti-Hack", "Armour Hack", 0);
				continue;
			}
			//Anti-Jetpack Hack;
			if(GetPlayerSpecialAction(i) == 2 && za[i].Admin < 1)
			{
			    BanPlayer(i, "Anti-Hack", "Jetpack Hack", 0);
				continue;
			}
			//Anti-Spec Hack;
			int state=GetPlayerState(i);
			if(state == PLAYER_STATE_SPECTATING && za[i].Admin < 1)
			{
			    BanPlayer(i, "Anti-Hack", "Spec Hack", 0);
				continue;
			}
			//Anti-Vehicle Hack
			if(za[i].Admin == 0 && za[i].InEvent == false)
			{
				if((state == PLAYER_STATE_PASSENGER || state == PLAYER_STATE_DRIVER) && IsPlayerInAnyVehicle(i))
				{
					int vid=GetVehicleModel(GetPlayerVehicleID(i));
					if(vid && vid != 501)
					{
						Haxor[i].Vehicle++;
						
						if(Haxor[i].Vehicle > 1)
						{
						    BanPlayer(i, "Anti-Hack", "Vehicle Hack", 0);
							continue;
						}
					}
				}
				else
				{
					Haxor[i].Vehicle=0;
				}
			}
			//Anti-Money Hack;
			if(GetPlayerMoney(i) > 0)
			{
				BanPlayer(i, "Anti-Hack", "Money Hack", 0);
				continue;
			}
			//Anti-Hack Entry
			if(za[i].AFKhuman == 0 && za[i].AFKzombie == 0 && state == 1)
			{
				///////////////////////////////////////////////
				//Anti-Weapon Hack #1: Forbidden Weapons;
				int weap=GetPlayerWeapon(i);
				if((weap > 0 && weap < 8) || (weap > 10 && weap < 22) || weap == 35 || weap == 36 || weap == 44 || weap == 45)
				{
					char str[128], WeaponName[40];
					GetWeaponName(weap, WeaponName, 38);
					snprintf(str, sizeof(str), "Weapon Hack(%s) #1", WeaponName);
	                BanPlayer(i, "Anti-Hack", str, 0);
					continue;
				}
				///////////////////////////////////////////////
				////////////Anti-Ammo Hack/////////////////////
				#define GUN_STAND 1167
				#define GUNMOVE_FWD 1161
				#define GUNMOVE_BWD 1160
				#define GUNMOVE_L 1162
				#define GUNMOVE_R 1163
				if(weap == 23 || weap == 24)//2: Silinced Pistol, Desert Eagle;
				{
					if(Haxor[i].Ammo[0] >= 0)
					{
						#define PYTHON_FIRE 1333
						#define SILENCECROUCHFIRE 1453
						#define PYTHON_CROUCHFIRE 1331
						
						int index=GetPlayerAnimationIndex(i);
						int diff=abs(Haxor[i].Ammo[0]-GetPlayerAmmo(i));
						if(index == GUN_STAND || index == GUNMOVE_FWD || index == GUNMOVE_BWD || index == GUNMOVE_L || index == GUNMOVE_R || index == PYTHON_FIRE || index == SILENCECROUCHFIRE || index == PYTHON_CROUCHFIRE)
						{
							if(diff > 4)
							{
								Haxor[i].AmmoWarn[0]++;
							}
						}
						else if(diff != 0)
						{
							Haxor[i].AmmoWarn[0]++;
						}
						else
						{
							Haxor[i].AmmoWarn[0]=0;
						}
					}
					else
					{
						Haxor[i].AmmoWarn[0]=3;
					}
					
					if(Haxor[i].AmmoWarn[0] > 2)
					{
						char str[128];
						snprintf(str, sizeof(str), ">> %s(%d) is possible ammo-hacking..", za[i].Name, i);
						SendClientMessageToAdmins(str);
						Haxor[i].AmmoWarn[0]=0;
					}
				}
				else if(weap == 25)//3: Shotgun;
				{
					if(Haxor[i].Ammo[1] >= 0)
					{
						#define SHOTGUN_CROUCHFIRE 1449
						int index=GetPlayerAnimationIndex(i);
						int diff=abs(Haxor[i].Ammo[1]-GetPlayerAmmo(i));
						if(index == GUN_STAND || index == GUNMOVE_FWD || index == GUNMOVE_BWD || index == GUNMOVE_L || index == GUNMOVE_R || index == SHOTGUN_CROUCHFIRE)
						{
							if(diff > 3)
							{
								Haxor[i].AmmoWarn[1]++;
							}
						}
						else if(diff != 0)
						{
							Haxor[i].AmmoWarn[1]++;
						}
						else
						{
							Haxor[i].AmmoWarn[1]=0;
						}
					}
					else
					{
						Haxor[i].AmmoWarn[1]=3;
					}
					
					if(Haxor[i].AmmoWarn[1] > 2)
					{
						char str[128];
						snprintf(str, sizeof(str), ">> %s(%d) is possible ammo-hacking..", za[i].Name, i);
						SendClientMessageToAdmins(str);
						Haxor[i].AmmoWarn[1]=0;
					}
				}
				else if(weap == 30 || weap == 31)//5
				{//AK47, M4
					#define RIFLE_FIRE 1367
					#define RIFLE_CROUCHFIRE 1365					
					if(Haxor[i].Ammo[2] >= 0)
					{
						int index=GetPlayerAnimationIndex(i);
						int diff=abs(Haxor[i].Ammo[2]-GetPlayerAmmo(i));
						if(index == GUN_STAND || index == GUNMOVE_FWD || index == GUNMOVE_BWD || index == GUNMOVE_L || index == GUNMOVE_R || index == RIFLE_FIRE || index == RIFLE_CROUCHFIRE)
						{
							if(diff > 10)
							{
								Haxor[i].AmmoWarn[2]++;
							}
						}
						else if(diff != 0)
						{
							Haxor[i].AmmoWarn[2]++;
						}
						else
						{
							Haxor[i].AmmoWarn[2]=0;
						}
					}
					else
					{
						Haxor[i].AmmoWarn[2]=3;
					}
					
					if(Haxor[i].AmmoWarn[2] > 2)
					{
						char str[128];
						snprintf(str, sizeof(str), ">> %s(%d) is possible ammo-hacking..", za[i].Name, i);
						SendClientMessageToAdmins(str);
						Haxor[i].AmmoWarn[2]=0;
					}
				}
				
				//////////////////////////////////////////////
				//////////Anti-Health Hacks///////////////////
				if(za[i].AdminDuty == false && za[i].UCID != -15 && za[i].UCID != -47)
				{
					//no Boss Zombie;
					if(BossZombie == true && BZID == i)
					{
					}
					else//Anti-Hack Entry;
					{
						float health;
						GetPlayerHealth(i, &health);
						
						//Anti-Health Hack #1;
						if(health > 99.0)
						{						
							Haxor[i].HealthLag++;
	
							if(Haxor[i].HealthLag > 2)
							{
								Haxor[i].HealthLag=0;

								char str[128];
								snprintf(str, sizeof(str), ">> %s(%d) has been timeouted [Reason: Lagging]", za[i].Name, i);
								SendClientMessageToAdmins(str);

								SendClientMessage(i, COLOR_ANTIHACK, ">> You have been timeouted [Reason: Lagging]");
								BlockIpAddress(za[i].IP, 14000);
							}
						}//Anti-HH #1
						else 
						{
							Haxor[i].HealthLag=0;
						
							//Anti-Health Hack #2;
							if(health > Haxor[i].Health)
							{
								Haxor[i].HealthExceeded++;
				
								if(Haxor[i].HealthExceeded > 2)
								{
									SetPlayerHealth(i, Haxor[i].Health);
								}
								if(Haxor[i].HealthExceeded > 3)
								{
									Haxor[i].HealthExceeded=0;
									
									if(IsAdminOnline)
									{//Admins are online;
										char str[128];
										snprintf(str, sizeof(str), ">> %s(%d) is possible health-hacking..", za[i].Name, i);
										SendClientMessageToAdmins(str);
									}
									else
									{//No Admins;
										SendClientMessage(i, COLOR_ANTIHACK, ">> You have been kicked [Reason: Lagging]");
										SetTimerEx("PlayerKick", 500, i);
									}
								}
							}
							else
							{
								Haxor[i].Health=health;
								Haxor[i].HealthExceeded=0;	
							}
						}//Anti-HH #2
					}//Anti-Hack Entry;
				}//Anti-HH;
			}//Anti-Hack Entry
			
			int exp=GetPlayerScore(i);

			if(za[i].hOpenedClass < (MAX_CLASS_NUMBER-1) && humanclasses[za[i].hOpenedClass+1].UniqueID)
			{				
				if(humanclasses[za[i].hOpenedClass+1].EXP <= exp && humanclasses[za[i].hOpenedClass+1].VIP == 0)
				{
					char str[128];
					za[i].hOpenedClass++;
					snprintf(str, sizeof(str), ">> You have {FF0000}unlocked{99ccff} the \"{FFFFFF}%s{99ccff}\" class!", humanclasses[za[i].hOpenedClass].Name);
					SendClientMessage(i, 0x99ccffAA, str);
						
					for(int k=0; k < 4; k++)
					{
						if(za[i].hOpenedClass < (MAX_CLASS_NUMBER-1) && humanclasses[za[i].hOpenedClass+1].UniqueID)
						{
							if(humanclasses[za[i].hOpenedClass+1].EXP <= exp && humanclasses[za[i].hOpenedClass+1].VIP == 0)
							{
								za[i].hOpenedClass++;
								snprintf(str, sizeof(str), ">> You have {FF0000}unlocked{99ccff} the \"{FFFFFF}%s{99ccff}\" class!", humanclasses[za[i].hOpenedClass].Name);
								SendClientMessage(i, 0x99ccffAA, str);
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
						
					HumanGenerate(i);
					PlayerPlaySound(i, 4201);
				}
				else if(za[i].hOpenedClass != 0 && humanclasses[za[i].hOpenedClass-1].EXP > exp)
				{
					HumanGenerate(i);
				}
			}
			
			if(za[i].zOpenedClass < (MAX_CLASS_NUMBER-1) && zombieclasses[za[i].zOpenedClass+1].UniqueID)
			{				
				if(zombieclasses[za[i].zOpenedClass+1].EXP <= exp && zombieclasses[za[i].zOpenedClass+1].VIP == 0)
				{
					char str[128];
					za[i].zOpenedClass++;
					snprintf(str, sizeof(str), ">> You have {FF0000}unlocked{99ccff} the \"{FFFFFF}%s{99ccff}\" class!", zombieclasses[za[i].zOpenedClass].Name);
					SendClientMessage(i, 0x99ccffAA, str);
						
					for(int k=0; k < 4; k++)
					{
						if(za[i].zOpenedClass < (MAX_CLASS_NUMBER-1) && zombieclasses[za[i].zOpenedClass+1].UniqueID)
						{
							if(zombieclasses[za[i].zOpenedClass+1].EXP <= exp && zombieclasses[za[i].zOpenedClass+1].VIP == 0)
							{
									za[i].zOpenedClass++;
									snprintf(str, sizeof(str), ">> You have {FF0000}unlocked{99ccff} the \"{FFFFFF}%s{99ccff}\" class!", zombieclasses[za[i].zOpenedClass].Name);
									SendClientMessage(i, 0x99ccffAA, str);
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
						
					ZombieGenerate(i);
					PlayerPlaySound(i, 4201);
				}
				else if(za[i].zOpenedClass != 0 && zombieclasses[za[i].zOpenedClass-1].EXP > exp)
				{
					ZombieGenerate(i);
				}
			}		
			
			
			
			
		}
	}
	
		
return;
}


