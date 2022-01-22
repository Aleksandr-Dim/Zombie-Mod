#include "GameVars.h"

//Variables
int label[5];

int EvacTime, map, xpmult, mysql, 
	lasthuman, BossZombie, BZID, CurrentDay, CurrentWeek;

Text t_human, t_zombie, t_forum, 
	t_site, t_evac, t_timer, t_welcome, t_za,
	t_server, t_help, t_rules, t_add, t_Red, 
	t_Black, t_Yellow, t_Blue, t_Magenta, t_Ability;
 
sqlite3 *db, *map_db, *class_db, *logs_db;

char LoginError[AMOUNT], MapName[MAX_MAP_NAME], MapList[MAP_LIST_LENGTH];
char LastSpamMessage[AMOUNT][128];

struct votekick Votekick;
struct ask Ask;
struct event Event;
struct undertaker Undertaker[7];
struct dhs DHS[AMOUNT];
struct aa AA[AMOUNT];
struct gang Gang[GANGS];
struct mapdata MapData;
struct AntiHack Haxor[AMOUNT];
struct classes humanclasses[MAX_CLASS_NUMBER], zombieclasses[MAX_CLASS_NUMBER];
struct ZA za[AMOUNT];
struct TEMP temp[AMOUNT];


