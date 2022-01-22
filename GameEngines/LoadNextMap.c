#include <GameVars.h>

static int PrevMap;

static void InvokeMap(int mapid)
{
char str[32];
snprintf(str, sizeof(str), "unloadfs Maps/Map%d", PrevMap);
SendRconCommand(str);

snprintf(str, sizeof(str), "loadfs Maps/Map%d", mapid);
SendRconCommand(str);
PrevMap=mapid;
}

void LoadNextMap()
{
sqlite3_stmt *stmt;
if(sqlite3_prepare_v2(map_db, "SELECT * FROM MapData WHERE MapID=?", 35, &stmt, NULL) != SQLITE_OK)
{
	Log("map_pre", sqlite3_errmsg(map_db));
}
sqlite3_bind_int(stmt, 1, map);
	
int rc=sqlite3_step(stmt);
if(rc == SQLITE_ROW)
{
	if(sqlite3_column_int(stmt, 1) == 0)
	{
		MapData.Time=sqlite3_column_int(stmt, 2);
		MapData.Weather=sqlite3_column_int(stmt, 3);
		MapData.Interior=sqlite3_column_int(stmt, 4);
		MapData.CameraX=sqlite3_column_double(stmt, 5);
		MapData.CameraY=sqlite3_column_double(stmt, 6);
		MapData.CameraZ=sqlite3_column_double(stmt, 7);
		MapData.CameraToX=sqlite3_column_double(stmt, 8);
		MapData.CameraToY=sqlite3_column_double(stmt, 9);
		MapData.CameraToZ=sqlite3_column_double(stmt, 10);
		MapData.CPx=sqlite3_column_double(stmt, 11);
		MapData.CPy=sqlite3_column_double(stmt, 12);
		MapData.CPz=sqlite3_column_double(stmt, 13);
		MapData.CPSize=sqlite3_column_double(stmt, 14);
		MapData.CPRaceType=sqlite3_column_int(stmt, 15);
		MapData.CPRaceNextX=sqlite3_column_double(stmt, 16);
		MapData.CPRaceNextY=sqlite3_column_double(stmt, 17);
		MapData.CPRaceNextZ=sqlite3_column_double(stmt, 18);
		MapData.GateModel1=sqlite3_column_int(stmt, 19);
		MapData.GateX1=sqlite3_column_double(stmt, 20);
		MapData.GateY1=sqlite3_column_double(stmt, 21);
		MapData.GateZ1=sqlite3_column_double(stmt, 22);
		MapData.GateRotX1=sqlite3_column_double(stmt, 23);
		MapData.GateRotY1=sqlite3_column_double(stmt, 24);
		MapData.GateRotZ1=sqlite3_column_double(stmt, 25);
		MapData.GateMoveX1=sqlite3_column_double(stmt, 26);
		MapData.GateMoveY1=sqlite3_column_double(stmt, 27);
		MapData.GateMoveZ1=sqlite3_column_double(stmt, 28);
		MapData.GateSpeed1=sqlite3_column_double(stmt, 29);
		MapData.GateModel2=sqlite3_column_int(stmt, 30);
		MapData.GateX2=sqlite3_column_double(stmt, 31);
		MapData.GateY2=sqlite3_column_double(stmt, 32);
		MapData.GateZ2=sqlite3_column_double(stmt, 33);
		MapData.GateRotX2=sqlite3_column_double(stmt, 34);
		MapData.GateRotY2=sqlite3_column_double(stmt, 35);
		MapData.GateRotZ2=sqlite3_column_double(stmt, 36);
		MapData.GateMoveX2=sqlite3_column_double(stmt, 37);
		MapData.GateMoveY2=sqlite3_column_double(stmt, 38);
		MapData.GateMoveZ2=sqlite3_column_double(stmt, 39);
		MapData.GateSpeed2=sqlite3_column_double(stmt, 40);
		MapData.HumanSpawn=sqlite3_column_int(stmt, 41);
		MapData.HumanX1=sqlite3_column_double(stmt, 42);
		MapData.HumanY1=sqlite3_column_double(stmt, 43);
		MapData.HumanZ1=sqlite3_column_double(stmt, 44);
		MapData.HumanA1=sqlite3_column_double(stmt, 45);
		MapData.HumanX2=sqlite3_column_double(stmt, 46);
		MapData.HumanY2=sqlite3_column_double(stmt, 47);
		MapData.HumanZ2=sqlite3_column_double(stmt, 48);
		MapData.HumanA2=sqlite3_column_double(stmt, 49);
		MapData.HumanX3=sqlite3_column_double(stmt, 50);
		MapData.HumanY3=sqlite3_column_double(stmt, 51);
		MapData.HumanZ3=sqlite3_column_double(stmt, 52);
		MapData.HumanA3=sqlite3_column_double(stmt, 53);
		MapData.HumanX4=sqlite3_column_double(stmt, 54);
		MapData.HumanY4=sqlite3_column_double(stmt, 55);
		MapData.HumanZ4=sqlite3_column_double(stmt, 56);
		MapData.HumanA4=sqlite3_column_double(stmt, 57);
		MapData.HumanX5=sqlite3_column_double(stmt, 58);
		MapData.HumanY5=sqlite3_column_double(stmt, 59);
		MapData.HumanZ5=sqlite3_column_double(stmt, 60);
		MapData.HumanA5=sqlite3_column_double(stmt, 61);
		MapData.ZombieSpawn=sqlite3_column_int(stmt, 62);
		MapData.ZombieX1=sqlite3_column_double(stmt, 63);
		MapData.ZombieY1=sqlite3_column_double(stmt, 64);
		MapData.ZombieZ1=sqlite3_column_double(stmt, 65);
		MapData.ZombieA1=sqlite3_column_double(stmt, 66);
		MapData.ZombieX2=sqlite3_column_double(stmt, 67);
		MapData.ZombieY2=sqlite3_column_double(stmt, 68);
		MapData.ZombieZ2=sqlite3_column_double(stmt, 69);
		MapData.ZombieA2=sqlite3_column_double(stmt, 70);
		MapData.ZombieX3=sqlite3_column_double(stmt, 71);
		MapData.ZombieY3=sqlite3_column_double(stmt, 72);
		MapData.ZombieZ3=sqlite3_column_double(stmt, 73);
		MapData.ZombieA3=sqlite3_column_double(stmt, 74);
		MapData.ZombieX4=sqlite3_column_double(stmt, 75);
		MapData.ZombieY4=sqlite3_column_double(stmt, 76);
		MapData.ZombieZ4=sqlite3_column_double(stmt, 77);
		MapData.ZombieA4=sqlite3_column_double(stmt, 78);
		MapData.ZombieX5=sqlite3_column_double(stmt, 79);
		MapData.ZombieY5=sqlite3_column_double(stmt, 80);
		MapData.ZombieZ5=sqlite3_column_double(stmt, 81);
		MapData.ZombieA5=sqlite3_column_double(stmt, 82);
		
		DestroyAllObjects();
		InvokeMap(map);
	}
	else
	{
		map++;
		LoadNextMap();
	}
}
else if(rc == SQLITE_DONE)
{
	map=1;
	LoadNextMap();
}

sqlite3_finalize(stmt);
}


void LoadMapNames()
{
sqlite3_stmt *stmt;
if(sqlite3_prepare_v2(map_db, "SELECT MapName FROM MapData", 27, &stmt, NULL) !=  SQLITE_OK)
{
	Log("mapnames_pre", sqlite3_errmsg(map_db));
}

memset(MapList, 0, sizeof(MapList));
memset(MapName, 0, sizeof(MapName));
int i=1;
while(sqlite3_step(stmt) == SQLITE_ROW)
{
	if(map == i)
	{
		snprintf(MapName, MAX_MAP_NAME, "%s", (char *)sqlite3_column_text(stmt, 0));
	}
		
	if(i == (MAX_MAPS+1))
	{
		Log("Map Memory Exceeded.", "");
		return;
	}
	
	//add some colors.
	if(map == i)strcat(MapList, "{FF0000}");
	else if((i%2) == 0)strcat(MapList, "{FFFFFF}");
	else strcat(MapList, "{B0B0B0}");
	
	char buff[6];
	snprintf(buff, sizeof(buff), "%d. ", i);
	strncat(MapList, buff, sizeof(buff)-1);
	strncat(MapList, (char *)sqlite3_column_text(stmt, 0), MAX_MAP_NAME);
	strcat(MapList, "\n");
	
	i++;
}

sqlite3_finalize(stmt);	
}
