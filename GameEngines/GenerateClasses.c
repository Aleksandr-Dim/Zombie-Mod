#include "GameVars.h"

void HumanGenerate(int playerid)
{
	int exp=GetPlayerScore(playerid);
	
	memset(&(za[playerid].HumanString), 0, CLASS_LENGTH);
	
	for(int i=0; i < (MAX_CLASS_NUMBER-1); i++)
	{
		if(humanclasses[i].UniqueID)
		{
			char buffer[136];
			
			if(humanclasses[i].VIP)
			{
				snprintf(buffer, sizeof(buffer), "{FFFA66}%.40s - %.55s - VIP\n", humanclasses[i].Name, humanclasses[i].Desc);
			}
			else
			{
				if(exp >= humanclasses[i].EXP)//opened classes
				{
					za[playerid].hOpenedClass=i;
	
					if((i%2) == 0)
					{
						snprintf(buffer, sizeof(buffer), "{00cc44}%.40s {FFFFFF}- %.55s - {009933}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
//						snprintf(buffer, sizeof(buffer), "{FFFFFF}%.40s - %.55s - {009933}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
					}
					else
					{
						snprintf(buffer, sizeof(buffer), "{66ff99}%.40s {B0B0B0}- %.55s - {009933}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
//						snprintf(buffer, sizeof(buffer), "{B0B0B0}%.40s - %.55s - {009933}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
					}
				}
				else//closed classes
				{
					if((i%2) == 0)
					{
						snprintf(buffer, sizeof(buffer), "{ff3333}%.40s {FFFFFF}- %.55s - {ff0000}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
					}
					else
					{
						snprintf(buffer, sizeof(buffer), "{ff6666}%.40s {B0B0B0}- %.55s - {ff0000}%d EXP\n", humanclasses[i].Name, humanclasses[i].Desc, humanclasses[i].EXP);
					}
				}
			}
			strcat(za[playerid].HumanString, buffer);
		}
		else break;
	}

	return;
}


void ZombieGenerate(int playerid)
{
	int exp=GetPlayerScore(playerid);
	
	memset(&(za[playerid].ZombieString), 0, CLASS_LENGTH);
	
	for(int i=0; i < (MAX_CLASS_NUMBER-1); i++)
	{
		if(zombieclasses[i].UniqueID)
		{
			char buffer[136];
			
			if(zombieclasses[i].VIP)
			{
				snprintf(buffer, sizeof(buffer), "{FFFA66}%.40s - %.55s - VIP\n", zombieclasses[i].Name, zombieclasses[i].Desc);
			}
			else
			{
				if(exp >= zombieclasses[i].EXP)//opened classes
				{
					za[playerid].zOpenedClass=i;
					
					if((i%2) == 0)
					{
//						snprintf(buffer, sizeof(buffer), "{FFFFFF}%.40s - %.55s - {009933}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
						snprintf(buffer, sizeof(buffer), "{00cc44}%.40s {FFFFFF}- %.55s - {009933}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
					}
					else
					{
//						snprintf(buffer, sizeof(buffer), "{B0B0B0}%.40s - %.55s - {009933}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
						snprintf(buffer, sizeof(buffer), "{66ff99}%.40s {B0B0B0}- %.55s - {009933}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
					}
				}
				else//closed classes
				{
					if((i%2) == 0)
					{
						snprintf(buffer, sizeof(buffer), "{ff3333}%.40s {FFFFFF}- %.55s - {ff0000}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
					}
					else
					{
						snprintf(buffer, sizeof(buffer), "{ff6666}%.40s {B0B0B0}- %.55s - {ff0000}%d EXP\n", zombieclasses[i].Name, zombieclasses[i].Desc, zombieclasses[i].EXP);
					}
				}
			}
			strcat(za[playerid].ZombieString, buffer);
		}
		else break;
	}

	return;
}



void HumanClasses()
{
sqlite3_stmt *stmt;
if(sqlite3_prepare_v2(class_db, "SELECT * FROM Human ORDER BY VIP ASC, EXP ASC", 45, &stmt, NULL) !=  SQLITE_OK)
{
	Log("class_h_pre", sqlite3_errmsg(class_db));
	return;
}

int i=0;	
while(sqlite3_step(stmt) == SQLITE_ROW)
{
	if(i == (MAX_CLASS_NUMBER-1))
	{
		Log("Class Array Memory Exceeded.", "");
		return;
	}
	
	humanclasses[i].UniqueID=sqlite3_column_int(stmt, 0);
	strncpy(humanclasses[i].Name, (char *)sqlite3_column_text(stmt, 1), 39);
	strncpy(humanclasses[i].Desc, (char *)sqlite3_column_text(stmt, 2), 55);
	humanclasses[i].EXP=sqlite3_column_int(stmt, 3);
	humanclasses[i].VIP=sqlite3_column_int(stmt, 4);	
	humanclasses[i].health=sqlite3_column_double(stmt, 5);
	humanclasses[i].armour=sqlite3_column_double(stmt, 6);
	humanclasses[i].Skins=sqlite3_column_int(stmt, 7);
	for(int k=0; k < 10; k++)humanclasses[i].Skin[k]=sqlite3_column_int(stmt, 8+k);
	for(int k=0; k < 5; k++)humanclasses[i].Weapon[k]=sqlite3_column_int(stmt, 18+k*2);
	for(int k=0; k < 5; k++)humanclasses[i].Ammo[k]=sqlite3_column_int(stmt, 19+k*2);
	humanclasses[i].FightStyle=sqlite3_column_int(stmt, 28);

	i++;
}

sqlite3_finalize(stmt);
}


void ZombieClasses()
{
sqlite3_stmt *stmt;
if(sqlite3_prepare_v2(class_db, "SELECT * FROM Zombie ORDER BY VIP ASC, EXP ASC", 46, &stmt, NULL) !=  SQLITE_OK)
{
	Log("class_z_pre", sqlite3_errmsg(class_db));
	return;
}

int i=0;	
while(sqlite3_step(stmt) == SQLITE_ROW)
{
	if(i == (MAX_CLASS_NUMBER-1))
	{
		Log("Class Array Memory Exceeded.", "");
		return;
	}
	
	zombieclasses[i].UniqueID=sqlite3_column_int(stmt, 0);
	strncpy(zombieclasses[i].Name, (char *)sqlite3_column_text(stmt, 1), 39);
	strncpy(zombieclasses[i].Desc, (char *)sqlite3_column_text(stmt, 2), 55);
	zombieclasses[i].EXP=sqlite3_column_int(stmt, 3);
	zombieclasses[i].VIP=sqlite3_column_int(stmt, 4);	
	zombieclasses[i].health=sqlite3_column_double(stmt, 5);
	zombieclasses[i].armour=sqlite3_column_double(stmt, 6);
	zombieclasses[i].Skins=sqlite3_column_int(stmt, 7);
	for(int k=0; k < 10; k++)zombieclasses[i].Skin[k]=sqlite3_column_int(stmt, 8+k);
	for(int k=0; k < 5; k++)zombieclasses[i].Weapon[k]=sqlite3_column_int(stmt, 18+k*2);
	for(int k=0; k < 5; k++)zombieclasses[i].Ammo[k]=sqlite3_column_int(stmt, 19+k*2);
	zombieclasses[i].FightStyle=sqlite3_column_int(stmt, 28);

	i++;
}

sqlite3_finalize(stmt);
}
