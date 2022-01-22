/*
R11-2:
Fix Anti-Ammo;

R11-1:
Fix Anti-Ammo;
Timeout on lagging  #1; Done!
boss zombie and kamikadze.
Y/N - to press keys;
Ability is ready sound;
Black Eye Pause;
bee pause;
Black screen - weather spawn bug;
/time, /weather
undertaker abusing 3D Text.

Release 11:
Update: zombie.amx, class.db
/spec and getting evacuated.
/ostats admin level.
Ability Is Ready!
Anti-Ammo Hack;
Anti-Health Hack;
anti-ta and zombies.
long to int!
/getinfo packetloss.
tougie and earth shaker bug;
temp is memset`ed on Disconnect.
Blind Zombie; Black-Eye Zombie; Slender Zombie;
Anti-Blind;
Mega Jumper;
Spider-Man;
bee zombie;
bee and mirror zombie EXP;
undertaker should be improved. <=============================
Nurse Bug and spores! <=============================
//////////////////////////////////////////////////
attachment slot id.
map 65 unbalanced place when zombies outcoming..
http://sampdm.net/showthread.php?tid=9709293
explosive abusing. Nerf VIP.
//////////////////////////////////////////////////
remove zombie business.
detect no team-class sync and disconnect!
blind zombie is useless. + add weather.
//////////////////////////////////////////////////
Set bee zombie exp requirement to 400k, make the high jump ability infinite, because, it's, literally, a, bee.
Explode radious reduce to avoid team-damage.
http://sampdm.net/showthread.php?tid=9709575
verbal warn in PM cmd;
http://sampdm.net/showthread.php?tid=9709594
Forum Multi-Language Problem.
SetPlayerAttachedObject(playerid,9,19054,1,0.092999,-0.246000,-0.011000,0.000000,86.099998,0.000000,0.341999,0.321000,0.385000); //- Gift on the back
SetPlayerAttachedObject(playerid,8,19064,2,0.150999,0.020000,-0.003000,81.499984,119.300010,4.699996,1.000000,1.112001,1.000000); //- Xmas Hat
SetPlayerAttachedObject(playerid,7,18634,5,0.098999,0.027999,0.029000,-8.199998,-92.500068,95.099868,1.000000,1.000000,1.000000, 0xFFFF5100);// - Lollipop
///////////////////////////////
Mysql lost connection problem;
map 103 gravity
event skin reserved;
Remove Zombie Businessman;
Sewer Madness move to center.
Anti-Abusing. Using admin-functions only in aduty.
Add maps.
Team-Class Sync; PlayerSpawn instead of SetPlayerPos again?!
Slapper Zombie;
http://sampdm.net/showthread.php?tid=9708972
Lol_Noob is planning something!?!?

http://sampdm.net/showthread.php?tid=9709114
http://sampdm.net/showthread.php?tid=9709081
http://sampdm.net/showthread.php?tid=9709028
http://sampdm.net/showthread.php?tid=9709005
http://sampdm.net/showthread.php?tid=9709183
http://sampdm.net/showthread.php?tid=9709138
http://sampdm.net/showthread.php?tid=9709074

PlayerPlaySound(playerid, 6001, 0.0, 0.0, 0.0); -- suggested sound about evacuation;
This class will be just like the Anti-Flamethrower Zombie, nobody will use it. And if they do, they gonna camp on the water.
http://sampdm.net/showthread.php?tid=9709183
http://sampdm.net/showthread.php?tid=9709138
http://sampdm.net/showthread.php?pid=162791
http://sampdm.net/showthread.php?tid=9709225&pid=163541#pid163541
New Year, and Hallowyein Auto-Events.
http://sampdm.net/showthread.php?tid=9709052
Black Hole Zombie;
Slow and Jammer Zombie EXP reqs;
Somewhen it will be added, xD:
Return Event Maps;
http://sampdm.net/showthread.php?tid=9706891 - Magician
http://sampdm.net/showthread.php?tid=9708733 -- ?
Trophies/Achievements: http://sampdm.net/showthread.php?tid=9708439
////////////////////////////
Here it's. Needs some Indentations fixing and fixing what happenes after the Animation. (Just use this in game, After pressing LALT. Look at your characters arms) :"D pff...
change RCON!
AFK and resetplayerweapons;

stock IsFalling(playerid)
{
new animlib[32];
new animname[32];
new msg[128];
GetAnimationName(GetPlayerAnimationIndex(playerid),animlib,32,animname,32);
if(!strcmp(animname, "FALL_back", true) || !strcmp(animname, "FALL_collapse", true)
|| !strcmp(animname, "FALL_fall", true) || !strcmp(animname, "FALL_front", true)
|| !strcmp(animname, "FALL_glide", true) || !strcmp(animname, "FALL_land", true)
|| !strcmp(animname, "FALL_skyDive", true)) return 1;
return 0;
}

Adrenaline Human??
SetPlayerAttachedObject(playerid,0,18683,6);
SetPlayerAttachedObject(playerid,1,18728,6);
enum aname
{
    SuperPower
}
new Abilitys[MAX_PLAYERS][aname];

stock Float:GetDistanceBetweenPlayers(p1, p2)
{
    if(!IsPlayerConnected(p1) || !IsPlayerConnected(p2)) return -1.00;
    new Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2;
    GetPlayerPos(p1, x1, y1, z1); GetPlayerPos(p2, x2, y2, z2);
    return floatsqroot(floatpower(floatabs(floatsub(x2, x1)), 2) + floatpower(floatabs(floatsub(y2, y1)), 2) + floatpower(floatabs(floatsub(z2, z1)), 2));
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(newkeys & KEY_WALK))
    {
        if(gettime() - 6 < Abilitys[playerid][SuperPower])return GameTextForPlayer(playerid,"~w~Still Recovering",1000,5);
        ApplyAnimation(playerid,"ped","Shove_Partial",3.9,0,1,1,1,1,1);
        SetPlayerAttachedObject(playerid,0,18683,6);
        SetPlayerAttachedObject(playerid,1,18728,6);
        SetPlayerAttachedObject(playerid,2,338,6);
        new Float:x,Float:y,Float:z,Float:Angle;
        Abilitys[playerid][SuperPower] = gettime();
      for(new i = 0; i < MAX_PLAYERS; i++)
{
    if(i==playerid) continue;
    if(GetDistanceBetweenPlayers(playerid,i) < 6.0)
    {
        new Float:vec[4];
        GetPlayerFacingAngle(i,Float:Angle);
        GetPlayerVelocity(i,Float:x,Float:y,Float:z);
        SetPlayerVelocity(i,Float:x+0.3,Float:y+0.3,Float:z+0.2);
        SetPlayerFacingAngle(i,Float:Angle);
        GetPlayerVelocity(i,vec[0],vec[1],vec[2]); GetPlayerFacingAngle(i,vec[3]);
        return 1;
    }
}
}
} 
////////////////////////////
Add Heri human. Other color than usual? Lighter? High-KS + bounty???
///////////////////////////////////////////
pyro maniac-abuse;
Maps:
Skotoboinya
adv.scout should have green color for wall-rengen;
Plane Crash - A pretty BIG Map with a lot of places for Humans to Spawn Camp.
Canyon
Sewer Station
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "sqlite3.h"

#define AMOUNT 70
#define GANGS 25
#define MAX_MAP_NAME 32
#define MAX_MAPS 150
#define MAP_LIST_LENGTH (41*MAX_MAPS+1+5)
#define MAX_BOXES 19

#define MAX_CLASS_NUMBER 54//Max number of classes allowed to load to local memory;
#define CLASS_LENGTH (MAX_CLASS_NUMBER*136)

#define INVALID_PLAYER_ID	(0xFFFF)
#define INVALID_CLASS_ID	(0xFFFF)
#define INVALID_OBJECT_ID	(0xFFFF)

#define COLOR_ANTIHACK 0x33CCFFAA
#define COLOR_BROWN 0xFF9900AA
#define COLOR_GRAYWHITE 0xEEEEFFC4
#define COLOR_BLUE 0x5D8AFFFF
#define COLOR_ORANGE 0xFF9900AA
#define COLOR_INDIGO 0x4B00B0AA

#define PLAYER_STATE_NONE						(0)
#define PLAYER_STATE_ONFOOT						(1)
#define PLAYER_STATE_DRIVER						(2)
#define PLAYER_STATE_PASSENGER					(3)
#define PLAYER_STATE_EXIT_VEHICLE				(4)
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		(5)
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	(6)
#define PLAYER_STATE_WASTED						(7)
#define PLAYER_STATE_SPAWNED					(8)
#define PLAYER_STATE_SPECTATING					(9)

#define SPECIAL_ACTION_NONE				0
#define SPECIAL_ACTION_DUCK				1
#define SPECIAL_ACTION_USEJETPACK		2
#define SPECIAL_ACTION_ENTER_VEHICLE	3
#define SPECIAL_ACTION_EXIT_VEHICLE     4
#define SPECIAL_ACTION_DANCE1			5
#define SPECIAL_ACTION_DANCE2			6
#define SPECIAL_ACTION_DANCE3			7
#define SPECIAL_ACTION_DANCE4			8
#define SPECIAL_ACTION_HANDSUP			10
#define SPECIAL_ACTION_USECELLPHONE		11
#define SPECIAL_ACTION_SITTING			12
#define SPECIAL_ACTION_STOPUSECELLPHONE 13
#define SPECIAL_ACTION_DRINK_BEER		20
#define SPECIAL_ACTION_SMOKE_CIGGY		21
#define SPECIAL_ACTION_DRINK_WINE		22
#define SPECIAL_ACTION_DRINK_SPRUNK		23
#define SPECIAL_ACTION_CUFFED			24
#define SPECIAL_ACTION_CARRY			25

#define DIALOG_REGISTER 0
#define DIALOG_LOGIN 1
#define DIALOG_CLASS 2
#define DIALOG_HUMAN 3
#define DIALOG_ZOMBIE 4
#define DIALOG_MAP 5
#define DIALOG_NOANSWER 6
#define DIALOG_SHOP 7
#define DIALOG_SETTINGS 8

#define FIGHT_STYLE_NORMAL 4
#define FIGHT_STYLE_BOXING 5
#define FIGHT_STYLE_KUNGFU 6
#define FIGHT_STYLE_KNEEHEAD 7

#define VIRTUALWORLD_SPACEBREAKER 1
#define VIRTUALWORLD_AFK 2
#define VIRTUALWORLD_EVENT 3
#define VIRTUALWORLD_CONNECTED 4

typedef unsigned long Text;
typedef unsigned int Text3D;
typedef unsigned int PlayerText;
enum{false, true};

//Variables
extern int label[5];

extern int EvacTime, map, xpmult, mysql, lasthuman,
	BossZombie, BZID, CurrentDay, CurrentWeek;
	
extern Text t_human, t_zombie, t_forum, 
	t_site, t_evac, t_timer, t_welcome, t_za,
	t_server, t_help, t_rules, t_add, t_Red, 
	t_Black, t_Yellow, t_Blue, t_Magenta, t_Ability;
	
extern sqlite3 *db, *map_db, *class_db, *logs_db;

extern char LoginError[AMOUNT], MapName[MAX_MAP_NAME], MapList[MAP_LIST_LENGTH];
extern char LastSpamMessage[AMOUNT][128];

//Structures
struct undertaker
{
	int Occupied;
	int PlayerID;
	int Time;
	int Text3D;
};

struct dhs
{
	int Kills;
	int Wins;
	int Time;
	int AFKtime;
	int HumanKills;
	int ZombieKills;
	int Bans;
	int Admin;
	char name[25];
};

struct aa
{
	int Time;
	int AFK;
};

struct gang
{
	char Name[32];
	char Rank1[32];
	char Rank2[32];
	char Rank3[32];
	char Rank4[32];
	char Rank5[32];
	char Rank6[32];
	char Tag[5];
	int Score;
	int Wins;
	int Losses;
	int IsCreated;
};

struct votekick
{
	int VoteOpen;
	int Vote[AMOUNT];
	int TimeLimit;
	int ToBeKicked;
	int VoteTime;
	int Counted;
	int VoteNO;
};

struct ask
{
	int EXP;
	int Opened;
	int CaseSensitive;
	char Answer[110];
};

struct event
{
	int IsEvent;
	int Closed;
	float Spawn[4];
};

struct ZA
{
	int AccountID;
	char Name[24];
	int Kills;
	int Wins;
	int Admin;
	int VIP;
	int VipTime;
	int Rank;
	int Clan;
	int DuelWins;
	int DuelLoses;
	int personalKS;
	char IP[16];
	int Settings;
	int NameChange;
	int Team;
	int Log;
	int Hclass;
	int Zclass;
	int UCID;//Unique Class ID
	int CDP;//Class Data Pointer
	char *ClassDesc;
	int Text;
	int CMD;
	int Mute;
	int AdminDuty;
	int Killstreaks;
	int Vehicle;
	int Warning;
	int Duel;
	int DuelFalsePositive;
	int DuelRequestPause;
	int DuelInviteID;
	int DuelInviteWID;
	int DuelInviteTime;
	int DuelVictimID;
	int PMSPY;
	int vSkin;
	int NameIsChanged;
	struct
	{
		int t_Ping;
		int t_Health;
		int t_Team;
		int t_Weapons;
	}TDs;
	int Spec;
	int KnockBack;
	int AFKhuman;
	int AFKzombie;
	int Ping, PingCounter;
	int Unconnected;
	int UnconnectedID;
	int GangRequest, GangRequestTime;
	int Fakekill;
	int Hacker;
	int HackerTime;
	int Kicked;
	int Hide;
	int Timer;
	int PlayedAsZombie;
	int FakekillSpawn;
	int ReportTime;
	int ReplyID;
	char HumanString[CLASS_LENGTH], ZombieString[CLASS_LENGTH];
	int hOpenedClass, zOpenedClass;
	int InEvent;
	int LastSpamMessageTime;
	int LastSpamMessageCount;
	int BanCheck;
	unsigned int uAFK;
	int JustConnected;
	int SpawnedAsHuman;
	int IsDead;
};

struct TEMP
{
	int Infection;
	int Antidote;
	int CureTime;
	int Boxes;
	int Box[MAX_BOXES];
	Text3D BoxText[MAX_BOXES];
	int BoxTime;
	int AntiTA;
	int Burned;
	int Pause;
	int MegaJumper;
	int MegaJumperTime;
	int HumanRogue;
	int ZombieRogue;
	int Poison;
	int Evac;
	int Flesher;//Flesher's Pickup ID.
	float reinPos[3];
	int rein;
	int Cockroach;
	int Spore;
	int Leg;
	int Slow;
	int TransferInfection;
	int JammerZombie;
	int IsPlayerJammed;
	int IsPlayerJammedTime;
	int IsBlinded;
	int Energy;
	int AntiSeeker, AntiFreezer, AntiBurning, Morphine, AntiScreamer, AntiBlind;
	int BunnyHop;
	int cBug;
	int cBugRepeat;
	int Helmet;
	float Assist[AMOUNT];
	int Tazer;
	int TazerFist;
	int TazerCurrentWeapon;
	int Tazered;
	int Unstucked;
	int Fly;
	int FlyObjects[8];
	float DamageAFK;
	int AbilityNotReady;
	int AbilityIdleTick;
};

struct mapdata
{
	int Time;
	int Weather;
	int Interior;
	float CameraX;
	float CameraY;
	float CameraZ;
	float CameraToX;
	float CameraToY;
	float CameraToZ;
	float CPx;
	float CPy;
	float CPz;
	float CPSize;
	int CPRaceType;
	float CPRaceNextX;
	float CPRaceNextY;
	float CPRaceNextZ;
	int GateModel1;
	float GateX1;
	float GateY1;
	float GateZ1;
	float GateRotX1;
	float GateRotY1;
	float GateRotZ1;
	float GateMoveX1;
	float GateMoveY1;
	float GateMoveZ1;
	float GateSpeed1;
	int GateModel2;
	float GateX2;
	float GateY2;
	float GateZ2;
	float GateRotX2;
	float GateRotY2;
	float GateRotZ2;
	float GateMoveX2;
	float GateMoveY2;
	float GateMoveZ2;
	float GateSpeed2;
	int HumanSpawn;
	float HumanX1;
	float HumanY1;
	float HumanZ1;
	float HumanA1;
	float HumanX2;
	float HumanY2;
	float HumanZ2;
	float HumanA2;
	float HumanX3;
	float HumanY3;
	float HumanZ3;
	float HumanA3;
	float HumanX4;
	float HumanY4;
	float HumanZ4;
	float HumanA4;
	float HumanX5;
	float HumanY5;
	float HumanZ5;
	float HumanA5;
	int ZombieSpawn;
	float ZombieX1;
	float ZombieY1;
	float ZombieZ1;
	float ZombieA1;
	float ZombieX2;
	float ZombieY2;
	float ZombieZ2;
	float ZombieA2;
	float ZombieX3;
	float ZombieY3;
	float ZombieZ3;
	float ZombieA3;
	float ZombieX4;
	float ZombieY4;
	float ZombieZ4;
	float ZombieA4;
	float ZombieX5;
	float ZombieY5;
	float ZombieZ5;
	float ZombieA5;
	float ZombieX6;
	float ZombieY6;
	float ZombieZ6;
	float ZombieA6;
	float ZombieX7;
	float ZombieY7;
	float ZombieZ7;
	float ZombieA7;
	int Gate1;
	int Gate2;
};

struct classes
{
	int UniqueID;
	int EXP;
	int VIP;
	float health;
	float armour;
	int Skins;
	int Skin[10];
	int Weapon[5];
	int Ammo[5];
	int FightStyle;
	char Name[40];
	char Desc[56];
};

struct AntiHack
{
	int HealthLag;//Anti-Health Hack #1;
	int Vehicle;//Anti-Vehicle Hack;
	int AntiRapidFire;
	int AntiRapidFireShots;
	int Ammo[3];
	int AmmoWarn[3];
	float Health;//Player's Health;
	int HealthExceeded;//Anti-Health Hack #2;
};

extern struct votekick Votekick;
extern struct ask Ask;
extern struct event Event;
extern struct undertaker Undertaker[7];
extern struct dhs DHS[AMOUNT];
extern struct aa AA[AMOUNT];
extern struct gang Gang[GANGS];
extern struct mapdata MapData;
extern struct AntiHack Haxor[AMOUNT];
extern struct classes humanclasses[MAX_CLASS_NUMBER], zombieclasses[MAX_CLASS_NUMBER];
extern struct ZA za[AMOUNT];
extern struct TEMP temp[AMOUNT];


//Internal Functions;
extern void Log(char message[], const char message2[]);
extern void RegisterOrLogin(int playerid);
extern void Register(int playerid, char password[]);
extern void Login(int playerid, char password[]);
extern void Save(int playerid);
extern void LoadNextMap(void);
extern void LoadMapNames(void);
extern void HumanClasses();
extern void ZombieClasses();
extern void HumanGenerate(int playerid);
extern void ZombieGenerate(int playerid);
extern int TextContainsIP(const char text[]);
extern int Infect(int playerid, int fromplayerid);
extern void Pager(int infectedid);
extern void SendClientMessageToAdmins(char str[]);
extern void DayilyHighScores(int playerid);
extern unsigned IsPlayerPaused(int playerid);
extern void OnPlayerDeath(int playerid, int killerid, int reason);
extern void BanPlayer(int banid, char admin[], char reason[], int duration);

//SAMP Natives;
extern int SendClientMessageToAll(unsigned long color, const char message[]);
extern int SendClientMessage(unsigned short playerid, unsigned long color, const char message[]);
extern int GameTextForAll(const char string[], unsigned long time, unsigned short style);
extern int GameTextForPlayer(unsigned int playerid, const char string[], unsigned long time, unsigned short style);
extern int SetGameModeText(const char string[]);
extern int SetTeamCount(unsigned int count);
extern int AddPlayerClass(unsigned int modelid, float x, float y, float z, float angle, unsigned short weapon1, unsigned short ammo1, unsigned short weapon2, unsigned short ammo2, unsigned short weapon3, unsigned short ammo3);
extern int CreatePickup(unsigned int model, unsigned int type, float x, float y, float z, unsigned int virtualworld);
extern int DestroyPickup(unsigned int pickup);
extern int SetWorldTime(unsigned short hour);
extern int SetWeather(int weatherid);
extern int CreateExplosion(float x, float y, float z, unsigned short type, float Radius);
extern int DisableInteriorEnterExits(void);
extern int LimitPlayerMarkerRadius(float Radius);
extern int Kick(unsigned int playerid);
extern int SendRconCommand(const char command[]);
extern int BlockIpAddress(const char ipaddress[], unsigned int timems);
extern int ShowPlayerDialog(int playerid, int dialogid, int style, char caption[], char info[], char button1[], char button2[]);
extern Text TextDrawCreate(float x, float y, const char text[]);
extern int TextDrawDestroy(Text text);
extern int TextDrawLetterSize(Text text, float x, float y);
extern int TextDrawTextSize(Text text, float x, float y);
extern int TextDrawAlignment(Text text, unsigned int alignment);
extern int TextDrawColor(Text text, unsigned long color);
extern int TextDrawUseBox(Text text, unsigned short use);
extern int TextDrawBoxColor(Text text, unsigned long color);
extern int TextDrawSetShadow(Text text, unsigned short size);
extern int TextDrawBackgroundColor(Text text, unsigned long color);
extern int TextDrawFont(Text text, unsigned short font);
extern int TextDrawSetProportional(Text text, unsigned short set);
extern int TextDrawShowForPlayer(unsigned int playerid, Text text);
extern int TextDrawHideForPlayer(unsigned int playerid, Text text);
extern int TextDrawShowForAll(Text text);
extern int TextDrawHideForAll(Text text);
extern int TextDrawSetString(Text text, const char string[]);
extern Text3D Create3DTextLabel(const char text[], unsigned long color, float x, float y, float z, float DrawDistance, unsigned virtualworld, unsigned LOS);
extern int Delete3DTextLabel(Text3D ID);
extern int Attach3DTextLabelToPlayer(Text3D ID, unsigned playerid, float x, float y, float z);
extern int Update3DTextLabelText(Text3D ID, unsigned long color, const char text[]);
extern int SpawnPlayer(unsigned playerid);
extern int SetPlayerPos(unsigned playerid, float x, float y, float z);
extern int GetPlayerPos(unsigned playerid, float *x, float *y, float *z);
extern int SetPlayerFacingAngle(unsigned playerid, float angle);
extern int GetPlayerFacingAngle(int playerid, float *angle);
extern int SetPlayerInterior(unsigned playerid, unsigned interiorid);
extern int GetPlayerInterior(unsigned playerid);
extern int SetPlayerHealth(unsigned playerid, float health);
extern int GetPlayerHealth(int playerid, float *health);
extern int SetPlayerArmour(unsigned playerid, float armour);
extern int GetPlayerArmour(int playerid, float *armour);
extern int SetPlayerAmmo(int playerid, int weaponslot, int ammo);
extern int SetPlayerTeam(unsigned playerid, unsigned teamid);
extern int SetPlayerScore(unsigned playerid, int score);
extern int GetPlayerScore(unsigned playerid);
extern int SetPlayerDrunkLevel(unsigned playerid, unsigned level);
extern int SetPlayerColor(unsigned playerid, unsigned long color);
extern int GetPlayerColor(unsigned playerid);
extern int SetPlayerSkin(unsigned playerid, unsigned skin);
extern int GivePlayerWeapon(unsigned playerid, unsigned weaponid, unsigned ammo);
extern int ResetPlayerWeapons(unsigned playerid);
extern int SetPlayerArmedWeapon(unsigned playerid, unsigned weaponid);
extern int GetPlayerWeaponData(int playerid, int slot, int *weapons, int *ammo);
extern int SetPlayerName(int playerid, char name[]);
extern int GetPlayerState(unsigned playerid);
extern int GetPlayerIp(int playerid, char IP[]);
extern int GetPlayerPing(unsigned playerid);
extern int GetPlayerWeapon(unsigned playerid);
extern int GetPlayerKeys(int playerid, int *keys, int *updown, int *leftright);
extern int GetPlayerName(int playerid, char *name);
extern int SetPlayerFightingStyle(int playerid, int style);
extern int SetPlayerVelocity(int playerid, float X, float Y, float Z);
extern int GetPlayerVelocity(int playerid, float *x, float *y, float *z);
extern int PlayAudioStreamForPlayer(int playerid, char url[], float posX, float posY, float posZ, float distance, int usepos);
extern int StopAudioStreamForPlayer(int playerid);
extern int SendDeathMessage(int killerid, int playerid, int reason);
extern int GetPlayerLastShotVectors(int playerid, float *fOriginX, float *fOriginY, float *fOriginZ, float *fHitPosX, float *fHitPosY, float *fHitPosZ);
extern int SetPlayerAttachedObject(int playerid, int index, int modelid, int bone, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ, int materialcolor1, int materialcolor2);
extern int RemovePlayerAttachedObject(int playerid, int index);
extern int IsPlayerAttachedObjectSlotUsed(int playerid, int index);
extern int SHA256_PassHash(char password[], char hash[]);
extern int PutPlayerInVehicle(int playerid, int vehicleid, int seatid);
extern int GetPlayerVehicleID(int playerid);
extern int RemovePlayerFromVehicle(int playerid);
extern int TogglePlayerControllable(int playerid, int toggle);
extern int ApplyAnimation(int playerid, char animlib[], char animname[], float fDelta, int loop, int lockx, int locky, int freeze, int time, int forcesync);
extern int ClearAnimations(int playerid, int forcesync);
extern int GetPlayerAnimationIndex(int playerid);
extern int GetAnimationName(int index, char animlib[], int len1, char animname[], int len2);
extern int GetPlayerSpecialAction(int playerid);
extern int SetPlayerSpecialAction(int playerid, int actionid);
extern int SetPlayerCheckpoint(int playerid, float x, float y, float z, float size);
extern int DisablePlayerCheckpoint(int playerid);
extern int SetPlayerRaceCheckpoint(int playerid, int type, float x, float y, float z, float nextx, float nexty, float nextz, float size);
extern int DisablePlayerRaceCheckpoint(int playerid);
extern int PlayerPlaySound(int playerid, int soundid);
extern int SetPlayerMarkerForPlayer(int playerid, int forplayerid, int color);
extern int ShowPlayerNameTagForPlayer(int playerid, int showplayerid, int show);
extern int SetPlayerCameraPos(int playerid, float x, float y, float z);
extern int SetPlayerCameraLookAt(int playerid, float x, float y, float z);
extern int SetCameraBehindPlayer(int playerid);
extern int GetPlayerCameraTargetObject(int playerid);
extern int IsPlayerConnected(int playerid);
extern int IsPlayerInVehicle(int playerid, int vehicleid);
extern int IsPlayerInAnyVehicle(int playerid);
extern int SetPlayerVirtualWorld(int playerid, int worldid);
extern int GetPlayerVirtualWorld(int playerid);
extern int EnableStuntBonusForAll(unsigned enable);
extern int TogglePlayerSpectating(int playerid, int toggle);
extern int PlayerSpectatePlayer(int playerid, int targetplayerid, int mode);
extern int PlayerSpectateVehicle(int playerid, int targetvehicleid, int mode);
extern int CreateVehicle(int vehicletype, float x, float y, float z, float rotation, int color1, int color2, int respawn_delay, int addsiren);
extern int DestroyVehicle(int vehicleid);
extern int SetVehicleParamsForPlayer(int vehicleid, int playerid, int objective, int doorslocked);
extern int SetVehicleToRespawn(int vehicleid);
extern int LinkVehicleToInterior(int vehicleid, int interiorid);
extern int GetVehicleModel(int vehicleid);
extern int SetPlayerWeather(int playerid, int weatherid);
extern int SetPlayerTime(int playerid, int time);
extern int CreateObject(int modelid, float X, float Y, float Z, float rX, float rY, float rZ, float DrawDistance);
extern int GetObjectPos(int objectid, float *x, float *y, float *z);
extern int IsValidObject(int objectid);
extern int DestroyObject(int objectid);
extern int DestroyAllObjects(void);
extern int GetObjectModel(int objectid);
extern int MoveObject(int objectid, float X, float Y, float Z, float Speed);
extern int IsPlayerInRangeOfPoint(int playerid, float range, float x, float y, float z);
extern int SetTimerEx(char funcname[], int interval, int value);
extern int EnablePlayerCameraTarget(int playerid, int toggle);
extern float VectorSize(float x, float y, float z);
extern int mysql_connect(const char host[], const char user[], const char database[], const char password[]);
extern int mysql_close(int connectionHandle);
extern int CreatePlayerObject(int playerid, int modelid, float X, float Y, float Z, float rX, float rY, float rZ, float DrawDistance);
extern int GetWeaponName(int weaponid, char weaponname[], int len);
extern int mysql_escape_string(const char source[], char destination[], int len, int mysql);
extern int mysql_tquery(int mysql, const char query[], const char callback[], int value);
extern PlayerText CreatePlayerTextDraw(int playerid, float x, float y, const char text[]);
extern int PlayerTextDrawLetterSize(int playerid, PlayerText text, float x, float y);
extern int PlayerTextDrawTextSize(int playerid, PlayerText text, float x, float y);
extern int PlayerTextDrawAlignment(int playerid, PlayerText text, unsigned int alignment);
extern int PlayerTextDrawFont(int playerid, PlayerText text, unsigned long color);
extern int PlayerTextDrawSetProportional(int playerid, PlayerText text, unsigned short set);
extern int PlayerTextDrawColor(int playerid, PlayerText textdraw, int color);
extern int PlayerTextDrawShow(int playerid, PlayerText textdraw);
extern int PlayerTextDrawHide(int playerid, PlayerText textdraw);
extern int PlayerTextDrawSetString(int playerid, PlayerText textdraw, char str[]);
extern int PlayerTextDrawUseBox(int playerid, PlayerText textdraw, int use);
extern int PlayerTextDrawBoxColor(int playerid, PlayerText textdraw, int color);
extern int cache_num_rows(int mysql);
extern int cache_affected_rows(int mysql);
extern int cache_get_field_content(int row, const char field_name[], char destination[], int mysql, int len);
extern int cache_get_field_content_int(int row, const char field_name[], int mysql);
extern int GetPlayerCameraMode(int playerid);
extern int GetPlayerCameraPos(unsigned playerid, float *x, float *y, float *z);
extern int GetPlayerMoney(int playerid);
extern int TimerKill(int timerid);
extern int SetTimerExForPlayer(int value);
extern int AttachObjectToVehicle(int objectid, int vehicleid, float OffsetX, float OffsetY, float OffsetZ, float RotX, float RotY, float RotZ);
extern int GetVehiclePos(int vehicleid, float *x, float *y, float *z);
extern int GetVehicleZAngle(int vehicleid, float *fa);
extern int GetPlayerSkin(int playerid);
extern int GetPlayerFightingStyle(int playerid);
extern int SetVehicleVirtualWorld(int vehicleid, int worldid);
extern unsigned int GetCount();
extern int NetStats_PacketLossPercent(int playerid);
extern int TextDrawSetOutline(Text text, int size);
extern int GetPlayerAmmo(int playerid);





