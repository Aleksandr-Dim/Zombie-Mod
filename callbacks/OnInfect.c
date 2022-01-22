#include "GameVars.h"

static int CanBeInfected(int playerid);

void OnInfect()
{
	for(int i=0; i < AMOUNT; i++)
	{	
		if(za[i].Log == true)
		{
			//Infected
			if(temp[i].Infection)
			{
			   	GameTextForPlayer(i, "~r~INFECTED", 1000, 5);
				float health;
				GetPlayerHealth(i, &health);
				
				if(temp[i].Infection == 2)SetPlayerHealth(i, health-10.0);
				else SetPlayerHealth(i, health-3.0);
		
				if(temp[i].TransferInfection)
				{
				    float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
							
					for(int k=0; k < AMOUNT; k++)
					{
						if(za[k].Log && za[k].Team == false && temp[k].Infection == false && IsPlayerInRangeOfPoint(k, 2.5, x, y, z) && GetPlayerState(k) != PLAYER_STATE_SPECTATING)
						{	
							if(Infect(k, i))
							{
								temp[k].TransferInfection=temp[i].TransferInfection;
	
								char str[128];
								snprintf(str, sizeof(str), ">> %s has been infected by %s! {FFFFFF}(by air)", za[k].Name, za[i].Name);
								SendClientMessageToAll(0x009900aa, str);
							}
						}
					}
				}
			}
			//Burned
			if(temp[i].Burned == true)
			{
				GameTextForPlayer(i, "~y~You're burning", 1000, 5);
				float health;
				GetPlayerHealth(i, &health);
				SetPlayerHealth(i, health-10.0);
			}
			//Poisoned
			if(temp[i].Poison > 0)
			{
				temp[i].Poison--;
				float health;
				GetPlayerHealth(i, &health);
				SetPlayerHealth(i, health-15.0);
				GameTextForPlayer(i, "~p~POISON", 1000, 5);
			}
			
			//Regenerator Zombie
			if(za[i].UCID == -14)
			{
				float health;
				GetPlayerHealth(i, &health);
				
				if(health <= 90.0)
				{
					SetPlayerHealth(i, health+9.0);
				}
				else if(health != 99.0)
				{
					SetPlayerHealth(i, 99.0);
				}
			}
			//Zombie Support
			else if(za[i].UCID == -52)
			{
				if(za[i].AFKzombie < 2)
				{
					float x, y, z, health; int healed=0;
					GetPlayerPos(i, &x, &y, &z);
					
					for(int k=0; k < AMOUNT; k++)
					{
						if(za[k].Team == true && i != k && IsPlayerInRangeOfPoint(k, 9.0, x, y, z))
						{
							//Anti-Abusing;
							if(GetPlayerState(k) == PLAYER_STATE_ONFOOT && za[k].AFKzombie < 2)
							{
								//No Boss Zombie, Tank, Juggernaut;
								if((k == BZID && BossZombie == true) || za[k].UCID == -15 || za[k].UCID == -47)
								{
									continue;
								}
								else
								{
									float MaxHealth=zombieclasses[za[k].CDP].health;
									GetPlayerHealth(k, &health);
		
									float r_health=roundf(health);
									if(r_health <= roundf(MaxHealth-9.0))
									{
										SetPlayerHealth(k, r_health+9.0);
										healed=1;
										GameTextForPlayer(k, "~y~Support", 800, 5);
									}
									else if(r_health < MaxHealth)
									{
										SetPlayerHealth(k, MaxHealth);
									}
								}
							}
						}
					}
					
					if(healed == 1)
					{
						GameTextForPlayer(i, "~n~~n~~n~~y~+1", 1000, 5);
						SetPlayerScore(i, GetPlayerScore(i)+1);	
					}
				}
			}
			//Radioactive Zombie
			else if(za[i].UCID == -13)
			{
				if(za[i].AFKzombie < 2)
				{
					float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
					
					int Nurse=0;
					for(int k=0; k < AMOUNT; k++)
					{
						if(za[k].UCID == 42 && IsPlayerInRangeOfPoint(k, 10.0, x, y, z))
						{
							GameTextForPlayer(i, "~r~Anti-Radioactive Field", 800, 5);
							Nurse=1;
							break;
						}
					}
	
					if(Nurse == 0)
					{
						for(int k=0; k < AMOUNT; k++)
						{//No: Vip Human, Iron Man, Human Queen;
							if(za[k].Log == true && za[k].Team == false && za[k].UCID != 17 && za[k].UCID != 31 && za[k].UCID != 36 && IsPlayerInRangeOfPoint(k, 9.0, x, y, z))
							{
								//Anti-Abusing;
								if(GetPlayerState(k) == PLAYER_STATE_ONFOOT)
								{
									float armour, health;
									GetPlayerArmour(k, &armour);
									if(armour)continue;
										
									GetPlayerHealth(k, &health);
									if(health > 89.9)continue;
									SetPlayerHealth(k, health-6.0);				
									GameTextForPlayer(k, "~r~RADIONACTIVE ~g~FIELD", 1000, 5);
						
									GameTextForPlayer(i, "~n~~n~~n~~y~+2", 1000, 5);
									SetPlayerScore(i, GetPlayerScore(i)+2);
								}
							}
						}
					}
				}
			}
			//Gun Jammer Zombie
			else if(za[i].UCID == -46)
			{
				if(temp[i].JammerZombie > time(NULL))
				{
					float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
					
					for(int k=0; k < AMOUNT; k++)
					{
						if(za[k].Log == true && za[k].Team == false && temp[k].IsPlayerJammed == false && IsPlayerInRangeOfPoint(k, 12.0, x, y, z))
					    {
							GameTextForPlayer(i, "~n~~n~~n~~y~+3", 1000, 5);
							SetPlayerScore(i, GetPlayerScore(i)+3);
							
							temp[k].IsPlayerJammed=true;
							temp[k].IsPlayerJammedTime=time(NULL)+20;
							GameTextForPlayer(k, "~r~GUN JAMMER ATTACK", 4000, 5);
						}
					}
				}
			}
			//Adv.Hero
			else if(za[i].UCID == 39)
			{
				float x, y, z;
				GetPlayerPos(i, &x, &y, &z);
				
		  		for(int k=0; k < AMOUNT; k++)
		    	{
		    		if(za[k].Team == true && IsPlayerInRangeOfPoint(k, 13.0, x, y, z))
					{
						GameTextForPlayer(k, "~g~HERO FIELD", 1000, 5);
					
						temp[k].IsPlayerJammed=true;
						temp[k].IsPlayerJammedTime=time(NULL)+4;
					}
			  	}		
			}
			
			//Anti-Swimming
		    if(za[i].Team == false)
			{
				char animlib[32], animname[32];
		   		GetAnimationName(GetPlayerAnimationIndex(i), animlib, 31, animname, 31);			
				
				if(!strcmp(animlib, "SWIM"))
				{
				    float health;
				    GetPlayerHealth(i, &health);
				    SetPlayerHealth(i, health-11.0);
				    GameTextForPlayer(i, "~r~ACID WATER~r~", 1000, 5);
		    	}
			}	

			if(za[i].InEvent == false && za[i].Duel == false && za[i].AFKzombie == 0)
			{
				//Maps: Ship, The Bridge;
				if(map == 6 || map == 30)
				{
					float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
					if(z < 1.5)
					{
						za[i].Team=true;
						SpawnPlayer(i);
						za[i].Killstreaks=0;
					}				
				}
				//Map: Lights, Camera, Action;
				else if(map == 68)
				{
					float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
					if(z < 70.0 && GetPlayerState(i) == 1)
					{
						za[i].Team=true;
						SpawnPlayer(i);
						za[i].Killstreaks=0;
					}				
				}
				//Map: Lost Island;
				else if(map == 70)
				{
					float x, y, z;
					GetPlayerPos(i, &x, &y, &z);
					if(z < 20.0)
					{
						za[i].Team=true;
						SpawnPlayer(i);
						za[i].Killstreaks=0;
					}				
				}	
			}
		}//if log is true;
	}
}

void Pager(int infectedid)
{
char str[128];
snprintf(str, sizeof(str), ">> PAGER : %s has been infected, go and cure him/his if possible!", za[infectedid].Name);

for(int i=0; i < AMOUNT; i++)
	if(za[i].UCID == 3 || za[i].UCID == 4 || za[i].UCID == 23 || za[i].UCID == 33)
		SendClientMessage(i, 0xFC6C85AA, str);
return;
}

int Infect(int playerid, int fromplayerid)
{
	if(!CanBeInfected(playerid))
	{
		if(za[fromplayerid].UCID != -5 && za[fromplayerid].UCID != -21 && za[fromplayerid].UCID != -11 && za[fromplayerid].UCID != -24)
			GameTextForPlayer(fromplayerid, "~r~Cannot infect", 500, 5);
		return 0;
	}
	if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)return 0;
	
	float x, y, z;
	
	if(za[playerid].UCID == 42)//Is Nurse;
	{
		GetPlayerPos(fromplayerid, &x, &y, &z);
					
		if(IsPlayerInRangeOfPoint(playerid, 10.0, x, y, z))
		{
			GameTextForPlayer(fromplayerid, "~r~Anti-Infection Field", 800, 5);
			return 0;
		}
	}
	else
	{
		GetPlayerPos(playerid, &x, &y, &z);
		
		for(int i=0; i < AMOUNT; i++)
		{
			if(za[i].UCID == 42 && IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
			{
				GameTextForPlayer(fromplayerid, "~r~Anti-Infection Field", 800, 5);
				return 0;
			}
		}
	}
	
	GameTextForPlayer(playerid, "~r~YOU HAVE BEEN INFECTED!", 500, 5);	
	if(temp[playerid].HumanRogue == false)
		SetPlayerColor(playerid, 0xFF8080AA);
		
	if(za[fromplayerid].UCID == -26)temp[playerid].Infection=2;
	else temp[playerid].Infection=true;
	
	TextDrawShowForPlayer(playerid, t_Red);
	if(za[fromplayerid].UCID == -2)
	{
		SetPlayerDrunkLevel(playerid, 50000);
	}
	else if(za[fromplayerid].UCID == -3)
	{
		if(temp[playerid].AntiBlind == false)
		{
			TextDrawShowForPlayer(playerid, t_Black);
			SetTimerEx("Black", 6000, playerid);
			SetPlayerWeather(playerid, 214);
			SetPlayerTime(playerid, 24);
		}
		else
		{
			GameTextForPlayer(playerid, "~g~Anti-~y~Blind!", 1000, 5);
		}
	}
	else if(za[fromplayerid].UCID == -22)
	{
		temp[playerid].TransferInfection=fromplayerid+1;
	}
	
	if(za[fromplayerid].UCID != -5 && za[fromplayerid].UCID != -21 && za[fromplayerid].UCID != -11 && za[fromplayerid].Team == true)
	{
		char str[128];
		
		if(za[fromplayerid].UCID == -24)snprintf(str, sizeof(str), ">> %s has been infected by %s! {FFFFFF}(picking up*)", za[playerid].Name, za[fromplayerid].Name);
		else if(za[fromplayerid].UCID == -44)snprintf(str, sizeof(str), ">> %s has been infected by spitter zombie %s!", za[playerid].Name, za[fromplayerid].Name);
		else if(za[fromplayerid].UCID == -43)snprintf(str, sizeof(str), ">> %s has been infected by spore zombie %s!", za[playerid].Name, za[fromplayerid].Name);
		else if(za[fromplayerid].UCID == -10)
		{
			snprintf(str, sizeof(str), ">> %s has been infected by slow zombie %s!", za[playerid].Name, za[fromplayerid].Name);
			if(temp[playerid].Morphine == false)temp[playerid].Slow=true;
		}
		else if(za[fromplayerid].UCID == -22)snprintf(str, sizeof(str), ">> %s has been infected by advanced mutanted zombie %s!", za[playerid].Name, za[fromplayerid].Name);	
		else snprintf(str, sizeof(str), ">> %s has been infected by %s!", za[playerid].Name, za[fromplayerid].Name);

		SendClientMessageToAll(0x009900aa, str);

		GameTextForPlayer(fromplayerid, "~n~~n~~n~~y~+5", 1000, 5);
		SetPlayerScore(fromplayerid, GetPlayerScore(fromplayerid)+5);
	}
	
	return 1;
}

static int CanBeInfected(int playerid)
{
if(za[playerid].UCID == 7//S.W.A.T.
|| za[playerid].UCID == 12 //Zombiefan
|| za[playerid].UCID == 28//adv. zombiefan
|| za[playerid].UCID == 17//VIP Human
|| za[playerid].UCID == 39//Adv. Hero
|| za[playerid].UCID == 36//Human Queen
|| za[playerid].UCID == 38//Adv. Engineer #2
)return 0;


return 1;
}
