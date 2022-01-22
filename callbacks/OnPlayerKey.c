#include "GameVars.h"

#define KEY_CROUCH				(2)
#define KEY_FIRE				(4)
#define KEY_SPRINT				(8)
#define KEY_JUMP				(32)
#define KEY_HANDBRAKE			(128)
#define KEY_WALK				(1024)

#define PRESSED(key) (((newkeys & (key)) == (key)) && ((oldkeys & (key)) != (key)))
static void DestroyBox(int playerid);

static float Speed(int playerid)
{
	float x, y, z;
	GetPlayerVelocity(playerid, &x, &y, &z);
	return sqrtf(powf(x, 2.0)+powf(y, 2.0));
}

void OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)return;
	if(EvacTime == -60 || EvacTime == -55 || EvacTime == 300)return;
	//Destroy engineer's box;
	if(newkeys & KEY_FIRE && za[playerid].Team == true)DestroyBox(playerid);

	if(newkeys & 65536)//class
	{
		ShowPlayerDialog(playerid, DIALOG_CLASS, 2,"Classes", "Humans\' Classes\nZombies\' Classes", "Ok", "Cancel");
		return;
	}
	if(newkeys & 131072)//shop
	{
		ShowPlayerDialog(playerid, DIALOG_SHOP, 2, "Shop", "Smoke Machine - 40 XP\nAnti-Seeker - 50 XP\nAnti-Freezer - 90 XP\nCamera - 5 XP\nArmour - 100 XP\nAnti-Burning - 80 XP\nMorphine - 90 EXP (No Leg Breaks)\nAnti-Screamer - 10 EXP\nAnti-Blind - 80 EXP\nHelmet - 10 EXP (No headshots)", "Buy", "Cancel");
		return;
	}

	//Anti-BunnyHop
   	if((newkeys & KEY_JUMP) == KEY_JUMP && za[playerid].UCID != 43 && Speed(playerid) > 0.098)
	{
		temp[playerid].BunnyHop++;
		if(temp[playerid].BunnyHop > 2)
		{
			temp[playerid].BunnyHop=0;
			ApplyAnimation(playerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 1, 1, 0, 0, 0);
			return;
		}
	}

	//General Check
	if(za[playerid].Team == false || za[playerid].UCID == -44 || za[playerid].UCID == -51 || za[playerid].UCID == -53)
	{
		int weap=GetPlayerWeapon(playerid);
		//Anti-Slide
		if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK && weap > 22 && weap < 34)
		{
			if((newkeys & KEY_SPRINT) && (newkeys & KEY_HANDBRAKE))
			{
				ApplyAnimation(playerid, "PED", "getup_front", 4.0, 0, 0, 1, 0, 0, 0);
				return;
			}
		}

		if(newkeys & KEY_FIRE)
		{
			//Anti-C
			if(temp[playerid].cBug == false && (weap == 23 || weap == 24 || weap == 25 || weap == 29 || weap == 31 || weap == 33 || weap == 34))
			{
				temp[playerid].cBugRepeat=false;
				temp[playerid].cBug=true;
				SetTimerEx("cBug", 800, playerid);
			}
		}
		
		//Anti-C
		if(newkeys & KEY_CROUCH)
		{
			if(temp[playerid].cBug == true && temp[playerid].cBugRepeat == false)
			{
				temp[playerid].cBugRepeat=true;
				ShowPlayerDialog(playerid, DIALOG_NOANSWER, 0, "Warning", "Do not use C+ bug!" , "Ok", "" );
				ApplyAnimation(playerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 1, 1, 0, 0, 0);
				return;
			}
		}
	}

	//Fly Zombie
	if(za[playerid].UCID == -49 && PRESSED(512))
	{
		if(temp[playerid].Fly != 0 && IsPlayerInAnyVehicle(playerid))
		{
			float x, y, z, fa;
			GetVehiclePos(temp[playerid].Fly, &x, &y, &z);
			GetVehicleZAngle(temp[playerid].Fly, &fa);
			SetPlayerPos(playerid, x, y, z);
			SetPlayerFacingAngle(playerid, fa);

			for(int i=0; i < 8; i++)
			{
				DestroyObject(temp[playerid].FlyObjects[i]);
			}
			DestroyVehicle(temp[playerid].Fly);
			temp[playerid].Fly=0;
			return;
		}
	}

	if(PRESSED(KEY_WALK))
	{
		//Engineers
		if(za[playerid].UCID == 10 || za[playerid].UCID == 11 || za[playerid].UCID == 38)
		{
			if(za[playerid].UCID == 10 && temp[playerid].Boxes >= 3)
			{
				GameTextForPlayer(playerid, "~r~ALL OBJECTS USED", 800, 5);
				return;
			}
			else if(za[playerid].UCID == 11 && temp[playerid].Boxes >= 5)
			{
				GameTextForPlayer(playerid, "~r~ALL OBJECTS USED", 800, 5);
				return;
			}
			else if(za[playerid].UCID == 38 && temp[playerid].Boxes > 5)
			{
				if(temp[playerid].Boxes > 17)
				{
					GameTextForPlayer(playerid, "~r~ALL OBJECTS USED", 800, 5);
					return;
				}
				
				if(time(NULL) < temp[playerid].BoxTime)
				{
					GameTextForPlayer(playerid, "~r~RECOVERING..", 800, 5);
					return;
				}
			}
			
			float x, y, z, fa;
			GetPlayerFacingAngle(playerid, &fa);
			GetPlayerPos(playerid, &x, &y, &z);
			x+=1.5*cos(((fa+90.0)/180)*3.1415926535);
			y+=1.5*sin(((fa+90.0)/180)*3.1415926535);
			temp[playerid].Box[temp[playerid].Boxes]=CreateObject(923, x, y, z, 0.0, 0.0, fa, 300.0);
			char str[80];
			snprintf(str, sizeof(str), "[BOX] {33CCFF}%s", za[playerid].Name);
			temp[playerid].BoxText[temp[playerid].Boxes]=Create3DTextLabel(str, 0x00FF00AA, x, y, z, 20.0, 0, 0);
			temp[playerid].Boxes++;
			
			if(za[playerid].UCID == 10)
				snprintf(str, sizeof(str), "~w~OBJECTS USED:~n~ ~r~ %d~w~/3", temp[playerid].Boxes);
			else if(za[playerid].UCID == 11)
				snprintf(str, sizeof(str), "~w~OBJECTS USED:~n~ ~r~ %d~w~/5", temp[playerid].Boxes);
			else
			{
				if(temp[playerid].Boxes > 5)temp[playerid].BoxTime=time(NULL)+30;
				snprintf(str, sizeof(str), "~w~OBJECTS USED:~n~ ~r~ %d~w~/18", temp[playerid].Boxes);
			}	
			
			GameTextForPlayer(playerid, str, 1000, 5);
		}
		//RocketMan
		else if(za[playerid].UCID == 19)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+3;
			temp[playerid].AbilityNotReady=true;
			
			float x, y, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x1, &y1, &z1);
			GetPlayerFacingAngle(playerid, &fa);
			x=0.28*cos(((fa+90.0)/180)*3.1415926535);
			y=0.28*sin(((fa+90.0)/180)*3.1415926535);
			SetPlayerVelocity(playerid, x-x1, y-y1, 0.28);

			SetPlayerAttachedObject(playerid, 5, 18702, 10, -0.033999,-0.076999, -1.58, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0, 0);
			SetPlayerAttachedObject(playerid, 6, 18702, 9, 0.0, -0.053, -1.614, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0, 0);
			SetTimerEx("Engines", 2200, playerid);
		}
		//Standart, Mutanted, Blind, Punk and Advanced Mutanted Zombies.
		else if(za[playerid].UCID == -1 || za[playerid].UCID == -2 || za[playerid].UCID == -3 || za[playerid].UCID == -26 || za[playerid].UCID == -22)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+2;
			temp[playerid].AbilityNotReady=true;
			ApplyAnimation(playerid, "BIKELEAP", "bk_jmp", 15.0, 0, 0, 0, 0, 0, 0);
				
			float x, y, z;	
			GetPlayerPos(playerid, &x, &y, &z);
				
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 1.5, x, y, z) && temp[i].Infection == false)
				{
					Infect(i, playerid);
					Pager(i);
					break;
				}
			}
		}
		//Spider-Man, Human Queen;
		else if(za[playerid].UCID == 35 || za[playerid].UCID == 36)
		{
			int CurrentTime=time(NULL);
			if(temp[playerid].Pause > CurrentTime)
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			int howmuch=(CurrentTime-temp[playerid].MegaJumperTime)/2;
			if(howmuch > 0 && howmuch <= temp[playerid].MegaJumper)
			{
				temp[playerid].MegaJumper-=howmuch;
			}	
			else if(howmuch > temp[playerid].MegaJumper)temp[playerid].MegaJumper=0;
					
			temp[playerid].MegaJumper++;
			temp[playerid].MegaJumperTime=CurrentTime;
			
			if(temp[playerid].MegaJumper == 5)
			{
				temp[playerid].Pause=CurrentTime+5;
				temp[playerid].AbilityNotReady=true;
				temp[playerid].MegaJumper=0;
			}
			
			float x, y, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x1, &y1, &z1);
			GetPlayerFacingAngle(playerid, &fa);
			x=0.7*cos(((fa+90.0)/180)*3.1415926535);
			y=0.7*sin(((fa+90.0)/180)*3.1415926535);
			SetPlayerVelocity(playerid, x-x1, y-y1, 0.15);
		}
		//Human Rogue
		else if(za[playerid].UCID == 26)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			
			temp[playerid].Pause=time(NULL)+50;	
			temp[playerid].AbilityNotReady=true;
			temp[playerid].HumanRogue=true;		
			SetPlayerColor(playerid, 0x99CC00AA);
			SetPlayerSkin(playerid, 181);
			TogglePlayerControllable(playerid, 1);
		}
		//fast zombie
		else if(za[playerid].UCID == -6)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+4;
			temp[playerid].AbilityNotReady=true;

			float x, y, z, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x, &y, &z);

			if(x == 0.0 && y == 0.0)
				SetPlayerVelocity(playerid, x, y, 0.2);
			else
			{
				GetPlayerVelocity(playerid, &x1, &y1, &z1);
				GetPlayerFacingAngle(playerid, &fa);
				x+=0.36*cos(((fa+90.0)/180)*3.14159265358979);
				y+=0.36*sin(((fa+90.0)/180)*3.14159265358979);
				SetPlayerVelocity(playerid, x-x1, y-y1, 0.22);
			}
		}
		//Mega Jumper;
		else if(za[playerid].UCID == -16)
		{
			int CurrentTime=time(NULL);
			if(temp[playerid].Pause > CurrentTime)
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			int howmuch=(CurrentTime-temp[playerid].MegaJumperTime)/2;
			if(howmuch > 0 && howmuch <= temp[playerid].MegaJumper)
			{
				temp[playerid].MegaJumper-=howmuch;
			}	
			else if(howmuch > temp[playerid].MegaJumper)temp[playerid].MegaJumper=0;
					
			temp[playerid].MegaJumper++;
			temp[playerid].MegaJumperTime=CurrentTime;
			
			if(temp[playerid].MegaJumper == 5)
			{
				temp[playerid].Pause=CurrentTime+10;
				temp[playerid].AbilityNotReady=true;
				temp[playerid].MegaJumper=0;
			}

			float x, y, z, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x, &y, &z);

			if(x == 0.0 && y == 0.0)
				SetPlayerVelocity(playerid, x, y, 0.2);
			else
			{
				GetPlayerVelocity(playerid, &x1, &y1, &z1);
				GetPlayerFacingAngle(playerid, &fa);
				x+=0.36*cos(((fa+90.0)/180)*3.14159265358979);
				y+=0.36*sin(((fa+90.0)/180)*3.14159265358979);
				SetPlayerVelocity(playerid, x-x1, y-y1, 0.22);
			}
		}
		//Bee Zombie
		else if(za[playerid].UCID == -51)
		{
			float x, y, z, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x, &y, &z);

			if(x == 0.0 && y == 0.0)
				SetPlayerVelocity(playerid, x, y, 0.2);
			else
			{
				GetPlayerVelocity(playerid, &x1, &y1, &z1);
				GetPlayerFacingAngle(playerid, &fa);
				x+=0.36*cos(((fa+90.0)/180)*3.14159265358979);
				y+=0.36*sin(((fa+90.0)/180)*3.14159265358979);
				SetPlayerVelocity(playerid, x-x1, y-y1, 0.22);
			}
		}
		//stomper
		else if(za[playerid].UCID == -7)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+10;
			temp[playerid].AbilityNotReady=true;
			
			float x, y, z, x1, y1, z1, AA, BB, CC;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05", 4.0, 0, 0, 0, 0, 0, 0);

			int IsStomped=false;
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Team == false && IsPlayerInRangeOfPoint(i, 5.0, x, y, z) && za[i].UCID != 16 && za[i].UCID != 31 && za[i].UCID != 39 && za[i].UCID != 36 && za[i].UCID != 17)
				{		
					IsStomped=true;
					
					GetPlayerPos(i, &x1, &y1, &z1);
					AA=x1-x;
					BB=y1-y;
					CC=sqrtf(powf(AA, 2.0)+powf(BB, 2.0));
					SetPlayerVelocity(i, (AA/CC)*0.8, (BB/CC)*0.8, 0.08);
				}
			}
			
			int ret=CreateObject(18675, x, y, z-1.5, 0, 0, 0, 30.0);
			SetTimerEx("Smoke", 500, ret);
			
			if(IsStomped == true)
			{
				GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
				SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			}				
		}
		//kamikadze
		else if(za[playerid].UCID == -11)
		{
			if(EvacTime < 10)
			{
				GameTextForPlayer(playerid, "~r~Time's ~g~up!", 500, 5);
				return;
			}
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+12;
			temp[playerid].AbilityNotReady=true;
		
			if(IsPlayerInRangeOfPoint(playerid, 7.0, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1)||
			IsPlayerInRangeOfPoint(playerid, 7.0, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2)||
			IsPlayerInRangeOfPoint(playerid, 7.0, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3)||
			IsPlayerInRangeOfPoint(playerid, 7.0, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4)||
			IsPlayerInRangeOfPoint(playerid, 7.0, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5))
			{
				SendClientMessage(playerid, -1, ">> You cannot use ability at this place.");
				return;
			}
		
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].UCID == 27 && i != playerid && IsPlayerInRangeOfPoint(i, 10.0, x, y, z))return;
			}
		
			CreateExplosion(x, y, z, 12, 10);
			
			if(BossZombie == true && BZID == playerid)
			{
				SetPlayerHealth(playerid, 0);
			}	
		
			int Numbers=0;
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 7.0, x, y, z))
				{
					if(Infect(i, playerid) == 1)Numbers++;
				}
			}
			
			if(Numbers > 3)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s the kamikaze has blown up and infected {FFFFFF}WHOPPING {FF0000}%d people!", za[playerid].Name, Numbers);
				SendClientMessageToAll(0x009900aa, str);
				GameTextForPlayer(playerid, "~n~~y~+10", 1000, 5);
				SetPlayerScore(playerid, GetPlayerScore(playerid)+10);
			}
			else if(Numbers != 0)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s the kamikaze has blown up and infected {FFFFFF}%d people!", za[playerid].Name, Numbers);
				SendClientMessageToAll(0x009900aa, str);
				GameTextForPlayer(playerid, "~n~~y~+5", 1000, 5);
				SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			}
		}
		//Seeker
		else if(za[playerid].UCID == -20)
		{
			if(EvacTime > 270)
			{
				GameTextForPlayer(playerid, "~g~wait some time", 500, 5);
				return;
			}
			
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+25;
			temp[playerid].AbilityNotReady=true;

			int Rand=0, Number[AMOUNT];
			int interior=GetPlayerInterior(playerid);
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && GetPlayerState(i) == 1 && temp[i].Evac == false && temp[i].AntiSeeker == false && za[i].Duel == false && interior == GetPlayerInterior(i))
				{
					Number[Rand]=i;
					Rand++;
				}
			}
					
			if(Rand == 0)return;
			
			float x, y, z, fa;
			int victim=Number[rand()%Rand];
			GetPlayerPos(victim, &x, &y, &z);			
			GetPlayerFacingAngle(victim, &fa);
			
			//Teleporting to victim's face.
			x+=0.8*cos(((fa+90.0)/180)*3.14159265358979);
			y+=0.8*sin(((fa+90.0)/180)*3.14159265358979);
			SetPlayerPos(playerid, x, y, z);
			SetPlayerFacingAngle(playerid, fa+180.0);
			SetCameraBehindPlayer(playerid);
			
			GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			
			char str[128];
			snprintf(str, sizeof(str), ">> %s has been found by seeker %s!", za[victim].Name, za[playerid].Name);
			SendClientMessageToAll(0x009900aa, str);
		}
		//Remover
		else if(za[playerid].UCID == -34)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+9;
			temp[playerid].AbilityNotReady=true;
			
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05",4.0,0,0,0,0,0,0);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 2.0, x, y, z) && za[i].UCID != 31)
				{
					float armour;
					GetPlayerArmour(i, &armour);
					if(armour == 0.0)
					{
						GameTextForPlayer(playerid, "~y~he/she has no armour", 1000, 5);
						return;
					}
	
					SetPlayerArmour(i, 0.0);
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
							
					char str[128];
					snprintf(str, sizeof(str), ">> %s the armour stealer has stolen {FFFFFF}%s's armour", za[playerid].Name, za[i].Name);
					SendClientMessageToAll(0x009900aa, str);
					break;
				}
			}//for
		}
		//wizard
		else if(za[playerid].UCID == -31)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+10;
			temp[playerid].AbilityNotReady=true;
				
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05",4.0,0,0,0,0,0,0);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && temp[i].Burned == false && IsPlayerInRangeOfPoint(i, 2.0, x, y, z))
				{
					if(za[i].UCID == 9 || za[i].UCID == 41 || za[i].UCID == 31 || za[i].UCID == 36 || za[i].UCID == 17 || temp[i].AntiBurning == true)
					{
						GameTextForPlayer(playerid, "~y~Wizard-Proof", 1000, 5);
						return;
					}
					
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);

					temp[i].Burned=true;
					TextDrawShowForPlayer(i, t_Yellow);
					SetPlayerAttachedObject(i, 9, 18689, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0);

					char str[128];
					snprintf(str, sizeof(str), ">> %s the wizard has burnt %s", za[playerid].Name, za[i].Name);
					SendClientMessageToAll(0xFFFF66aa, str);
					break;
				}
			}
		}
		//swapper
		else if(za[playerid].UCID == -42)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+10;
			temp[playerid].AbilityNotReady=true;		
			
			ApplyAnimation(playerid, "BIKELEAP", "bk_jmp",15.0,0,0,0,0,0,0);
			
			float x, y, z, health, health2;	
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerHealth(playerid, &health);
			
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 1.5, x, y, z))
				{
					GetPlayerHealth(i, &health2);
					if(health >= health2)return;

					SetPlayerHealth(playerid, health2);
					SetPlayerHealth(i, health);
						
					GameTextForPlayer(i, "~r~Swapper Attacked", 3500, 5);
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
				
					char str[128];
					snprintf(str, sizeof(str), ">> %s has been attacked by life swapper %s!", za[i].Name, za[playerid].Name);
					SendClientMessageToAll(0x009900aa,str);
					break;
		   		}
			}
		}
		//Ammo Stealer
		else if(za[playerid].UCID == -30)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+8;
			temp[playerid].AbilityNotReady=true;
			
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05",4.0,0,0,0,0,0,0);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 2.0, x, y, z))
				{
					int weapons[7][2];
					for(int k=2; k < 7; k++)
						GetPlayerWeaponData(i, k, &(weapons[k][0]), &(weapons[k][1]));
						
					if(weapons[2][1] == 0 && weapons[3][1] == 0 && weapons[4][1] == 0 && weapons[5][1] == 0 && weapons[6][1] == 0)
					{
						GameTextForPlayer(playerid, "He/She has no ammo!", 1000, 5);
						return;
					}
					
					for(int d=2; d < 7; d++)
					{
						if(weapons[d][1])
						{				
							SetPlayerAmmo(i, weapons[d][0], (weapons[d][1])/3);	
						}
					}
		
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			
					char str[128];
					snprintf(str, sizeof(str), ">> %s the stealer has stolen {FFFFFF}%s's ammo", za[playerid].Name, za[i].Name);
					SendClientMessageToAll(0x009900aa, str);
				}
			}
		}
		//Demolisher
		else if(za[playerid].UCID == -40)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+30;
			temp[playerid].AbilityNotReady=true;
			
			ApplyAnimation(playerid, "CARRY", "putdwn05",4.0,0,0,0,0,0,0);
		
			float x, y, z;
			int IsDemolished=0;
			for(int i=0; i < AMOUNT; i++)
			{
			    if(temp[i].Boxes)
				{
					for(int k=0; k < temp[i].Boxes; k++)
					{
						if(temp[i].Box[k] != 0)
						{
							GetObjectPos(temp[i].Box[k], &x, &y, &z);
							if(IsPlayerInRangeOfPoint(playerid, 20.0, x, y, z))
							{
							    DestroyObject(temp[i].Box[k]);
							    temp[i].Box[k]=0;
							    Delete3DTextLabel(temp[i].BoxText[k]);
							    IsDemolished++;
							}
						}
					}
				}
			}			
				
			if(IsDemolished)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s the demolisher destroyed some boxes", za[playerid].Name);
				SendClientMessageToAll(0xFFFF66aa, str);
				
				if(IsDemolished > 2)
				{
					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
				}
			}	
		}
		//Space Breaker
		else if(za[playerid].UCID == -39)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+8;
			temp[playerid].AbilityNotReady=true;
			
			if(GetPlayerVirtualWorld(playerid) == 0)
			{
				SetPlayerAttachedObject(playerid,9,18728,2,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0, 0, 0);
				SetPlayerAttachedObject(playerid,8,2780,1,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0, 0, 0);
				SetTimerEx("Space", 2000, playerid);
			}
			else
			{
				SetPlayerVirtualWorld(playerid, 0);
			}
		}
		//Earth Shaker
		else if(za[playerid].UCID == -28)
		{	
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+20;
			temp[playerid].AbilityNotReady=true;
			
			float x, y, z, x1, y1, z1, AA, BB, CC, armour, health; int EXP=0;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05", 4.0, 0, 0, 0, 0, 0, 0);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Team == false && IsPlayerInRangeOfPoint(i, 15.0, x, y, z) && za[i].UCID != 16 && za[i].UCID != 17 && za[i].UCID != 31 && za[i].UCID != 39 && za[i].UCID != 36)
				{
					GetPlayerArmour(i, &armour);
					if(armour)continue;
					
					EXP=1;
					
					GetPlayerHealth(i, &health);
					SetPlayerHealth(i, health-30.0);
					
					GetPlayerPos(i, &x1, &y1, &z1);
					AA=x1-x;
					BB=y1-y;
					CC=sqrtf(powf(AA, 2.0)+powf(BB, 2.0));
					SetPlayerVelocity(i, (AA/CC)*0.9, (BB/CC)*0.9, 0.12);
				}
			}
			
			int ret=CreateObject(18675, x, y, z-1.5, 0, 0, 0, 30.0);
			SetTimerEx("Smoke", 500, ret);
			
			if(EXP)
			{
				GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
				SetPlayerScore(playerid, GetPlayerScore(playerid)+5);
			}		
		}
		//slender man
		else if(za[playerid].UCID == -29)
		{
			if(EvacTime > 270)
			{
				GameTextForPlayer(playerid, "~g~wait some time", 500, 5);
				return;
			}
			
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+30;
			temp[playerid].AbilityNotReady=true;		
				
			int Rand=0, Number[AMOUNT];
			int interior=GetPlayerInterior(playerid);
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Team == false && GetPlayerState(i) == 1 && temp[i].Evac == false && za[i].UCID != 16 && temp[i].AntiSeeker == false && za[i].Duel == false && interior == GetPlayerInterior(i))
				{
					Number[Rand]=i;
					Rand++;
				}
			}
					
			if(Rand == 0)return;

			SetPlayerAttachedObject(playerid,8,18641,1,-0.1699,0.0,0.333,0.0,-21.599,0.0,1.0,1.0,3.947,0,0);
			SetPlayerAttachedObject(playerid,7,18641,1,-0.0109,0.032,-0.324,-172.899,0.0,0.0,1.0,1.0,4.0,0,0);
			SetPlayerAttachedObject(playerid,6,18641,1,0.0,0.0,0.326,0.0,0.0,0.0,1.0,1.0,3.5949,0,0);
			SetPlayerAttachedObject(playerid,5,18641,1,0.197,0.0,0.2739,0.0,24.00,0.0,1.0,1.0,4.9629,0,0);
			SetPlayerAttachedObject(playerid,4,18641,1,-0.2149,0.0,-0.3299,179.30,-29.199,0.0,1.0,1.0,3.589,0,0);
			SetPlayerAttachedObject(playerid,3,18641,1,0.166,0.0650,-0.3489,-174.89,30.2,0.0,1.0,1.0,4.5069,0,0);

			SetTimerEx("Slender", 4000, playerid);

			float x, y, z, x1, y1, fa, health;
			int victim=Number[rand()%Rand];
			GetPlayerPos(victim, &x, &y, &z);			
			GetPlayerFacingAngle(victim, &fa);
			
			//Teleporting to victim's face.
			x+=1.1*cos(((fa+90.0)/180)*3.14159265358979);
			y+=1.1*sin(((fa+90.0)/180)*3.14159265358979);
			SetPlayerPos(playerid, x, y, z);
			SetPlayerFacingAngle(playerid, fa+180.0);
			SetCameraBehindPlayer(playerid);

			GetPlayerHealth(victim, &health);
			SetPlayerHealth(victim, health-30.0);

			if(temp[victim].AntiBlind == false)
			{
				TextDrawShowForPlayer(victim, t_Black);
				SetTimerEx("Black", 4500, victim);
				SetPlayerWeather(victim, 214);
				SetPlayerTime(victim, 24);
			}

			x1=0.3*cos(((fa+90.0)/180)*3.14159265358979);
			y1=0.3*sin(((fa+90.0)/180)*3.14159265358979);
			SetPlayerVelocity(victim, -x1, -y1, 0.18);
		
			GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
			SetPlayerScore(playerid, GetPlayerScore(playerid)+5);

			char str[128];
			snprintf(str, sizeof(str), ">> %s has been frighted by %s!", za[victim].Name, za[playerid].Name);
			SendClientMessageToAll(0x009900aa, str);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log ==true && za[i].Team == false && i != victim && IsPlayerInRangeOfPoint(i, 2.5, x, y, z))
				{
					if(temp[i].AntiBlind == false)
					{
						TextDrawShowForPlayer(i, t_Black);
						SetTimerEx("Black", 3000, i);
						SetPlayerWeather(i, 214);
						SetPlayerTime(i, 24);
					}
					
					GetPlayerHealth(i, &health);
					SetPlayerHealth(i, health-10.0);
		
					snprintf(str, sizeof(str), ">> {FFFFFF}%s {009900}has been frighted by %s!", za[i].Name, za[playerid].Name);
					SendClientMessageToAll(0x009900aa, str);
				}
			}				
		}
		//Screamer
		else if(za[playerid].UCID == -9)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+9;
			temp[playerid].AbilityNotReady=true;			
			
			if(IsPlayerInRangeOfPoint(playerid, 8.0, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1)||
			IsPlayerInRangeOfPoint(playerid, 8.0, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2)||
			IsPlayerInRangeOfPoint(playerid, 8.0, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3)||
			IsPlayerInRangeOfPoint(playerid, 8.0, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4)||
			IsPlayerInRangeOfPoint(playerid, 8.0, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5))
			{
				SendClientMessage(playerid, -1, ">> You cannot use ability at this place.");
				return;
			}
			
			
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
				
			for(int i=0; i < AMOUNT; i++)
			{
				if(playerid != i && za[i].Log == true && temp[i].AntiScreamer == false && IsPlayerInRangeOfPoint(i, 5.0, x, y, z))
				{
					ApplyAnimation(i, "PED", "BIKE_fall_off",4.1,0,1,1,1,0,1);
					GameTextForPlayer(i, "~r~Screamer Attacked", 2500, 5);
					SetTimerEx("Screamer", 2200, i);
				}
			}	
		}
		//Rogue Zombie
		else if(za[playerid].UCID == -8)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+50;
			temp[playerid].AbilityNotReady=true;
			temp[playerid].ZombieRogue=true;
			
			int array[]={15,33,43,44,45,93,121,188,37,150,299,23,287};
			SetPlayerColor(playerid, 0x00CCCCAA);
			SetPlayerSkin(playerid, array[rand()%13]);
			TogglePlayerControllable(playerid, 1);	
		}
		//Freezer
		else if(za[playerid].UCID == -12)
		{
			if(EvacTime < 10)
			{
				GameTextForPlayer(playerid, "~r~Time's ~g~up!", 500, 5);
				return;
			}

			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+30;
			temp[playerid].AbilityNotReady=true;		
			
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			ApplyAnimation(playerid, "CARRY", "putdwn05",4.0,0,0,0,0,0,0);
			
			int Number=0;
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
				{
					if(za[i].UCID == 13 || za[i].UCID == 17 || za[i].UCID == 31 || za[i].UCID == 39 || za[i].UCID == 36 || temp[i].AntiFreezer == true || za[i].VIP > 0)continue;
					
					TogglePlayerControllable(i, 0);
					TextDrawShowForPlayer(i, t_Blue);
					SetTimerEx("Freeze", 2900, i);
					GameTextForPlayer(i, "~b~you have been frozen..", 1000, 5);
					Number++;
				}
			}
			
			if(Number > 3)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s the freezer has frozen up {FFFFFF}WHOPPING {FF0000}%d people!", za[playerid].Name, Number);
				SendClientMessageToAll(0x0055ffaa, str);
			}
			else if(Number != 0)
			{
				char str[128];
				snprintf(str, sizeof(str), ">> %s the freezer has frozen up {FFFFFF}%d people!", za[playerid].Name, Number);
				SendClientMessageToAll(0x0055ffaa, str);
			}
		}
		//Reincarnate
		else if(za[playerid].UCID == -19)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+2;
			temp[playerid].AbilityNotReady=true;	
			
			if(temp[playerid].rein)
			{
				temp[playerid].rein=false;
				GameTextForPlayer(playerid, "Respawn: ~r~OFF", 500, 5);
			}
			else
			{
				temp[playerid].rein=true;
				GameTextForPlayer(playerid, "Respawn: ~g~ON", 500, 5);	
			}
		}
		//Cockroach Zombie
		else if(za[playerid].UCID == -37)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+2;
			temp[playerid].AbilityNotReady=true;
			
			if(temp[playerid].Cockroach == false)
			{
				temp[playerid].Cockroach=true;
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllable(playerid, 0);
				GameTextForPlayer(playerid, "~b~Fly Mode: ~g~ON", 1500, 5);
			}
			else
			{
				temp[playerid].Cockroach=false;
				TogglePlayerControllable(playerid, 1);
				
				GameTextForPlayer(playerid, "~b~Fly Mode: ~r~OFF", 1500, 5);
			}

		}
		//Spore Zombie
		else if(za[playerid].UCID == -43)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+40;
			temp[playerid].AbilityNotReady=true;
			
			if(temp[playerid].Spore == false)
			{
				temp[playerid].Spore=true;
				GameTextForPlayer(playerid, "~g~activated!", 1500, 5);
				SetTimerEx("Spores", 20000, playerid);
			}
		}
		//Leg Breaker Zombie
		else if(za[playerid].UCID == -36)
		{
			if(EvacTime < 10 || EvacTime > 270)
			{
				GameTextForPlayer(playerid, "Now is not the time to use that!", 500, 5);
				return;
			}
			
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+30;
			temp[playerid].AbilityNotReady=true;
			
			ApplyAnimation(playerid, "CARRY", "putdwn05", 4.0,0,0,0,0,0,0);
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);

			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && playerid != i && IsPlayerInRangeOfPoint(i, 2.0, x, y, z) && za[i].UCID != 17 && za[i].UCID != 31 && za[i].UCID != 36)
				{
					if(temp[i].Morphine == true)continue;
					temp[i].Leg=true;	    
					
					char str[128];
					snprintf(str, sizeof(str), ">> Leg Breaker %s has broken %s's leg", za[playerid].Name, za[i].Name);
					SendClientMessageToAll(0x009900aa, str);

					GameTextForPlayer(playerid, "~n~~n~~n~~y~+10", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid)+10);
					break;
				}
			}
		}
		//Slow Zombie
		else if(za[playerid].UCID == -10)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+4;
			temp[playerid].AbilityNotReady=true;
			
			ApplyAnimation(playerid, "BIKELEAP", "bk_jmp", 15.0,0,0,0,0,0,0);
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
				
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && temp[i].Infection == false && IsPlayerInRangeOfPoint(i, 2.0, x, y, z))
				{
		    		Infect(i, playerid);
					break;
		   		}
			}
		}
		//Gun Jammer Zombie
		else if(za[playerid].UCID == -46)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+40;
			temp[playerid].AbilityNotReady=true;

			temp[playerid].JammerZombie=time(NULL)+20;
			SetTimerEx("GunJammer", 20000, playerid);
			GameTextForPlayer(playerid, "~g~activated!", 1500, 5);
		}
		//Black Eye Zombie
		else if(za[playerid].UCID == -45)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+15;
			temp[playerid].AbilityNotReady=true;
			
			ApplyAnimation(playerid, "BIKELEAP", "bk_jmp", 15.0,0,0,0,0,0,0);
			float x, y, z;
			GetPlayerPos(playerid, &x, &y, &z);
			
			for(int i=0; i < AMOUNT; i++)
			{
				if(za[i].Log == true && za[i].Team == false && IsPlayerInRangeOfPoint(i, 2.0, x, y, z))
				{
					if(temp[i].AntiBlind == true)
					{
						GameTextForPlayer(playerid, "~g~Anti-~y~Blind!", 1000, 5);
						return;
					}
					if(temp[i].IsBlinded == true)
					{
						GameTextForPlayer(playerid, "~g~Already ~y~Blinded..", 1000, 5);
						return;
					}
					temp[i].IsBlinded=true;

					char str[128];	
					snprintf(str, sizeof(str), ">> {ff0000}%s {009900}has been blinded by %s!", za[i].Name, za[playerid].Name);
					SendClientMessageToAll(0x009900aa, str);

					TextDrawShowForPlayer(i, t_Black);
					SetPlayerWeather(i, 214);
					SetPlayerTime(i, 24);

					GameTextForPlayer(playerid, "~n~~n~~n~~y~+5", 1000, 5);
					SetPlayerScore(playerid, GetPlayerScore(playerid) + 5);
					break;
		   		}
			}
		}
		//Iron Man
		else if(za[playerid].UCID == 31)
		{
			if(temp[playerid].Energy < 1)
			{
				GameTextForPlayer(playerid, "~r~OUT OF ENERGY..", 800, 5);
				return;
			}

			temp[playerid].Energy--;

			char energystr[128];
			if(temp[playerid].Energy > 40)snprintf(energystr, sizeof(energystr), "~g~Energy: %.1f%%", (((float)(temp[playerid].Energy))/150.0)*100.0);
			else if(temp[playerid].Energy > 20)snprintf(energystr, sizeof(energystr), "~g~Energy: ~y~%.1f%%", (((float)(temp[playerid].Energy))/150.0)*100.0);
			else snprintf(energystr, sizeof(energystr), "~g~Energy: ~r~%.1f%%", (((float)(temp[playerid].Energy))/150.0)*100.0);
			GameTextForPlayer(playerid, energystr, 500, 5);

			float x=0, y=0, x1, y1, z1, fa;
			GetPlayerVelocity(playerid, &x1, &y1, &z1);
			GetPlayerFacingAngle(playerid, &fa);
			x+=0.48*cos(((fa+90.0)/180.0)*3.14159265358979);
			y+=0.48*sin(((fa+90.0)/180.0)*3.14159265358979);
			SetPlayerVelocity(playerid, x-x1, y-y1, 0.28);

			SetPlayerAttachedObject(playerid,5,18702,10,-0.0339,-0.0769,-1.58,0.0,0.0,0.0,1.0,1.0,1.0,0,0);
			SetPlayerAttachedObject(playerid,6,18702,9,0.0,-0.053,-1.614,0.0,0.0,0.0,1.0,1.0,1.0,0,0);
			SetTimerEx("Engines", 3000, playerid);
		}
		//Fly Zombie
		else if(za[playerid].UCID == -49)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+40;
			temp[playerid].AbilityNotReady=true;
			
			if(temp[playerid].Fly != 0)
			{					
				for(int i=0; i < 8; i++)
				{
					DestroyObject(temp[playerid].FlyObjects[i]);
				}
				DestroyVehicle(temp[playerid].Fly);
				//temp[playerid].Fly=0;
			}
			SendClientMessage(playerid, -1, ">> Press {FFFF00}2 {FFFFFF}to get out!");
	
			float x, y, z, fa;
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &fa);
				
			temp[playerid].Fly=CreateVehicle(501, x, y, z+2.0, fa, -1, -1, -1, 0);
			LinkVehicleToInterior(temp[playerid].Fly, 90);
			PutPlayerInVehicle(playerid, temp[playerid].Fly, 0);

			temp[playerid].FlyObjects[0]=CreateObject(369, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[1]=CreateObject(1010, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[2]=CreateObject(2798, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[3]=CreateObject(2798, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[4]=CreateObject(1004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[5]=CreateObject(1004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[6]=CreateObject(2798, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);
			temp[playerid].FlyObjects[7]=CreateObject(2798, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 200.0);

			AttachObjectToVehicle(temp[playerid].FlyObjects[0], temp[playerid].Fly, 0.02640, 0.61480, 0.11470, 0.00000, 90.00000, 0.00000);
			AttachObjectToVehicle(temp[playerid].FlyObjects[1], temp[playerid].Fly, 0.02904, 0.38531, -0.03919, 0.00000, 0.00000, -90.09580);
			AttachObjectToVehicle(temp[playerid].FlyObjects[2], temp[playerid].Fly, -0.19340, 0.44470, -0.07410, 0.00000, 0.00000, 0.00000);
			AttachObjectToVehicle(temp[playerid].FlyObjects[3], temp[playerid].Fly, 0.25390, 0.43670, -0.09310, 0.00000, 0.00000, 180.00000);
			AttachObjectToVehicle(temp[playerid].FlyObjects[4], temp[playerid].Fly, -0.39830, 0.34390, 0.19060, 0.00000, -30.00000, 172.16251);
			AttachObjectToVehicle(temp[playerid].FlyObjects[5], temp[playerid].Fly, 0.45020, 0.26730, 0.15060, 0.00000, 30.00000, 180.00000);
			AttachObjectToVehicle(temp[playerid].FlyObjects[6], temp[playerid].Fly, -0.19340, 0.20470, -0.07410, 0.00000, 0.00000, 0.00000);
			AttachObjectToVehicle(temp[playerid].FlyObjects[7], temp[playerid].Fly, 0.25390, 0.20670, -0.09310, 0.00000, 0.00000, 180.00000);
		}
		//Adv. Policeman
		else if(za[playerid].UCID == 46)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
			temp[playerid].Pause=time(NULL)+20;
			temp[playerid].AbilityNotReady=true;

			temp[playerid].Tazer=true;
			temp[playerid].TazerCurrentWeapon=1;//Reset current weapon to update an attachment;
			GameTextForPlayer(playerid, "~n~~n~~n~~b~~h~~h~Taser is ~g~~h~~h~on!", 1500, 5);
		}
		//Undertaker Zombie
		else if(za[playerid].UCID == -50)
		{
			if(temp[playerid].Pause > time(NULL))
			{
				GameTextForPlayer(playerid, "~g~STILL RECOVERING..", 500, 5);
				return;
			}
		
			float x, y, z;
			GetPlayerVelocity(playerid, &x, &y, &z);
			if(x == 0.0 && y == 0.0 && z == 0.0)
			{
				if(MapData.ZombieSpawn < 7)
				{
					if(IsPlayerInRangeOfPoint(playerid, 110.0, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1)||
					IsPlayerInRangeOfPoint(playerid, 110.0, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2)||
					IsPlayerInRangeOfPoint(playerid, 110.0, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3)||
					(MapData.ZombieSpawn > 3 && Undertaker[3].Occupied == false && IsPlayerInRangeOfPoint(playerid, 110.0, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4))||
					(MapData.ZombieSpawn > 4 && Undertaker[4].Occupied == false && IsPlayerInRangeOfPoint(playerid, 110.0, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5)))
					{
						if(IsPlayerInRangeOfPoint(playerid, 15.0, MapData.ZombieX1, MapData.ZombieY1, MapData.ZombieZ1)||
						IsPlayerInRangeOfPoint(playerid, 15.0, MapData.ZombieX2, MapData.ZombieY2, MapData.ZombieZ2)||
						IsPlayerInRangeOfPoint(playerid, 15.0, MapData.ZombieX3, MapData.ZombieY3, MapData.ZombieZ3)||
						(MapData.ZombieSpawn > 3 && Undertaker[3].Occupied == false && IsPlayerInRangeOfPoint(playerid, 15.0, MapData.ZombieX4, MapData.ZombieY4, MapData.ZombieZ4))||
						(MapData.ZombieSpawn > 4 && Undertaker[4].Occupied == false && IsPlayerInRangeOfPoint(playerid, 15.0, MapData.ZombieX5, MapData.ZombieY5, MapData.ZombieZ5)))
						{
							SendClientMessage(playerid, -1, ">> You are {00FF00}too close {FFFFFF}to spawns to set a position!");
						}
						else
						{
							temp[playerid].Pause=time(NULL)+60;
							temp[playerid].AbilityNotReady=true;
							Undertaker[MapData.ZombieSpawn].Occupied=true;
							Undertaker[MapData.ZombieSpawn].PlayerID=playerid;
							Undertaker[MapData.ZombieSpawn].Time=time(NULL);
		
							float x, y, z, fa;
							GetPlayerPos(playerid, &x, &y, &z);
							GetPlayerFacingAngle(playerid, &fa);
							
							char str[64];
							snprintf(str, sizeof(str), "[Spawn Zone] {33CCFF}%s", za[playerid].Name);
							Undertaker[MapData.ZombieSpawn].Text3D=Create3DTextLabel(str, 0x00FF00AA, x, y, z, 25.0, 0, 1);
		
							*(float *)((&MapData.ZombieX1)+MapData.ZombieSpawn*4)=x;
							*(float *)((&MapData.ZombieY1)+MapData.ZombieSpawn*4)=y;
							*(float *)((&MapData.ZombieZ1)+MapData.ZombieSpawn*4)=z;
							*(float *)((&MapData.ZombieA1)+MapData.ZombieSpawn*4)=fa;
							MapData.ZombieSpawn++;
							SendClientMessage(playerid, -1, ">> You have successfully set a spawn position!");						
						}
					}
					else
					{
						SendClientMessage(playerid, -1, ">> You are {FF0000}too far {FFFFFF} from spawns to set a position!");
					}
				}
				else
				{
					SendClientMessage(playerid, -1, ">> All spawn positions have already been occupied by other undertakers!");		
				}
				
			}
			else
			{
				SendClientMessage(playerid, -1, ">> You should stand to set a spawn position!");
			}
		}
		
		
	}//if(PRESSED(KEY_WALK))
return;
}


static void DestroyBox(int playerid)
{
int objectid=GetPlayerCameraTargetObject(playerid);
if(objectid != INVALID_OBJECT_ID && GetObjectModel(objectid) == 923)
{
	float ox, oy, oz;
	GetObjectPos(objectid, &ox, &oy, &oz);
	
	if(IsPlayerInRangeOfPoint(playerid, 2.0, ox, oy, oz))
	{
		for(int i=0; i < AMOUNT; i++)
		{
			if(temp[i].Boxes > 0)
			{
				for(int k=0; k < temp[i].Boxes; k++)
				{
					if(temp[i].Box[k] == objectid)
					{
						temp[i].Box[k]=0;
						DestroyObject(objectid);
						Delete3DTextLabel(temp[i].BoxText[k]);
						return;
					}
				}
			}
		}
	}
}
return;
}

