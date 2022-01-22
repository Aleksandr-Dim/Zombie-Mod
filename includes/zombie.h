#pragma once

#define MAX_NATIVE_NAME 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#if defined(LINUX)
	#include <sys/mman.h>
	#define PLUGIN_CALL 
	#define EXPORTED __attribute__((visibility("default")))
#else
	#include "windows.h"
	#define PLUGIN_CALL __stdcall
	#define EXPORTED __declspec(dllexport)
#endif

void OnGameModeInit(void);
void OnGameModeExit(void);
void OnPlayerConnect(int playerid);
void OnPlayerDisconnect(int playerid, int reason);
int OnPlayerRequestSpawn(int playerid);
int OnPlayerRequestClass(int playerid, int classid);
void OnPlayerSpawn(int playerid);
void OnPlayerDeath(int playerid, int killerid, int reason);
void OnPlayerText(int playerid, char text[]);
int OnPlayerCommandText(int playerid, char cmdtext[]);
void OnPlayerEnterCheckpoint(int playerid);
void OnPlayerEnterRaceCheckpoint(int playerid);
void OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart);
void OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart);
void OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys);
void OnPlayerStateChange(int playerid, int newstate, int oldstate);
int OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ);
void OnPlayerPickUpPickup(int playerid, int pickupid);
int OnDialogResponse(int playerid, int dialogid, int response, int listitem, char inputtext[]);
void OnVehicleStreamIn(int vehicleid, int forplayerid);
int OnPlayerUpdate(int playerid);
void OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid);
void OnPlayerClickPlayer(int playerid, int clickedplayerid, int source);
void OnPlayerStreamIn(int playerid, int forplayerid);
void OnTime(void);
void OnInfect(void);
void Smoke(int value);
void Engines(int playerid);
void Black(int playerid);
void Space(int playerid);
void Slender(int playerid);
void Screamer(int playerid);
void Freeze(int playerid);
void KickPlayer(int playerid);
void Spores(int playerid);
void Stung(int playerid);
void cBug(int playerid);
void CheckUserBan(int playerid);
void UnbanPlayer(int playerid);
void SetVip();
void DayilyHighScores(int playerid);
void OnPlayerDuel(int playerid);
void KindaConnected(int playerid);
void Defender();
void Save(int playerid);
void OnPlayerEnterVehicle(int playerid, int vehicleid, int ispassenger);
void Tazer(int playerid);
void TazerObject(int objectid);
void GunJammer(int playerid);
void Unstuck(int playerid);
void HideAbility(int playerid);

typedef unsigned int ucell;
typedef int cell;

struct tagAMX;
typedef cell (*AMX_NATIVE)(struct tagAMX *amx, cell *params);
typedef int (*AMX_CALLBACK)(struct tagAMX *amx, cell index, cell *result, cell *params);
typedef int (*AMX_DEBUG)(struct tagAMX *amx);


#if defined __GNUC__
  #define PACKED        __attribute__((packed))
#else
  #define PACKED
#endif

typedef struct tagAMX_NATIVE_INFO {
  const char *name PACKED;
  AMX_NATIVE func       PACKED;
} PACKED AMX_NATIVE_INFO;

#define AMX_USERNUM     4
#define sEXPMAX         19      /* maximum name length for file version <= 6 */
#define sNAMEMAX        31      /* maximum name length of symbol name */

typedef struct tagAMX_FUNCSTUB {
  ucell address         PACKED;
  char name[sEXPMAX+1];
} PACKED AMX_FUNCSTUB;

typedef struct tagFUNCSTUBNT {
  ucell address         PACKED;
  uint32_t nameofs      PACKED;
} PACKED AMX_FUNCSTUBNT;

/* The AMX structure is the internal structure for many functions. Not all
 * fields are valid at all times; many fields are cached in local variables.
 */
typedef struct tagAMX {
  unsigned char *base PACKED; /* points to the AMX header plus the code, optionally also the data */
  unsigned char *data PACKED; /* points to separate data+stack+heap, may be NULL */
  AMX_CALLBACK callback PACKED;
  AMX_DEBUG debug       PACKED; /* debug callback */
  /* for external functions a few registers must be accessible from the outside */
  cell cip              PACKED; /* instruction pointer: relative to base + amxhdr->cod */
  cell frm              PACKED; /* stack frame base: relative to base + amxhdr->dat */
  cell hea              PACKED; /* top of the heap: relative to base + amxhdr->dat */
  cell hlw              PACKED; /* bottom of the heap: relative to base + amxhdr->dat */
  cell stk              PACKED; /* stack pointer: relative to base + amxhdr->dat */
  cell stp              PACKED; /* top of the stack: relative to base + amxhdr->dat */
  int flags             PACKED; /* current status, see amx_Flags() */
  /* user data */
  long usertags[AMX_USERNUM] PACKED;
  void *userdata[AMX_USERNUM] PACKED;
  /* native functions can raise an error */
  int error             PACKED;
  /* passing parameters requires a "count" field */
  int paramcount;
  /* the sleep opcode needs to store the full AMX status */
  cell pri              PACKED;
  cell alt              PACKED;
  cell reset_stk        PACKED;
  cell reset_hea        PACKED;
  cell sysreq_d         PACKED; /* relocated address/value for the SYSREQ.D opcode */
  #if defined JIT
    /* support variables for the JIT */
    int reloc_size      PACKED; /* required temporary buffer for relocations */
    long code_size      PACKED; /* estimated memory footprint of the native code */
  #endif
} PACKED AMX;

/* The AMX_HEADER structure is both the memory format as the file format. The
 * structure is used internaly.
 */
typedef struct tagAMX_HEADER {
  int32_t size          PACKED; /* size of the "file" */
  uint16_t magic        PACKED; /* signature */
  char    file_version;         /* file format version */
  char    amx_version;          /* required version of the AMX */
  int16_t flags         PACKED;
  int16_t defsize       PACKED; /* size of a definition record */
  int32_t cod           PACKED; /* initial value of COD - code block */
  int32_t dat           PACKED; /* initial value of DAT - data block */
  int32_t hea           PACKED; /* initial value of HEA - start of the heap */
  int32_t stp           PACKED; /* initial value of STP - stack top */
  int32_t cip           PACKED; /* initial value of CIP - the instruction pointer */
  int32_t publics       PACKED; /* offset to the "public functions" table */
  int32_t natives       PACKED; /* offset to the "native functions" table */
  int32_t libraries     PACKED; /* offset to the table of libraries */
  int32_t pubvars       PACKED; /* the "public variables" table */
  int32_t tags          PACKED; /* the "public tagnames" table */
  int32_t nametable     PACKED; /* name table */
} PACKED AMX_HEADER;

AMX *Machine;

typedef void (*logprintf_t)(char* format, ...);
logprintf_t logprintf;

