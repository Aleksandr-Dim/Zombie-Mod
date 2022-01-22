#include "GameVars.h"

void OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart)
{
    if(za[playerid].AdminDuty)
    {
        SetPlayerHealth(playerid, 700.0);
	}

	//no fire damage: firemaniac, anti-flamethrower zombie, pyromaniac
	if((za[playerid].UCID == 9 || za[playerid].UCID == -32 || za[playerid].UCID == 41) && weaponid == 37)
	{
		float armour;
		GetPlayerArmour(playerid, &armour);
		
		if(armour > 0.0 && armour <= 45.0)
		{
			SetPlayerArmour(playerid, armour+(2*amount));
		}
		else
		{
			float health;
			GetPlayerHealth(playerid, &health);
		
			if(health <= 93.0)
				SetPlayerHealth(playerid,health+(2*amount));	
  		}
	}
	
	if(issuerid != INVALID_PLAYER_ID)
	{
		if(issuerid > (AMOUNT-1) || issuerid < 0){Kick(playerid);return;}
		
		//Anti-Human TeamAttack
		if(za[playerid].Team == false && za[issuerid].Team == false)
		{
			temp[issuerid].AntiTA++;
			if(weaponid == 0)
			{
			   	ApplyAnimation(issuerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 1, 1, 0, 0, 0);
			   	GameTextForPlayer(issuerid, "~b~WTF ~r~Human?!", 1000, 5);
			}
			else if(weaponid == 10 && temp[issuerid].AntiTA > 1)
			{
				temp[issuerid].AntiTA=0;
			   	ApplyAnimation(issuerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 1, 1, 0, 0, 0);
			   	GameTextForPlayer(issuerid, "~b~WTF ~r~Human?!", 1000, 5);
			}
		}
		//Anti-Zombie TeamAttack
		else if(za[playerid].Team == true && za[issuerid].Team == true && weaponid == 9)
		{
		   	temp[issuerid].AntiTA++;
		   	if(temp[issuerid].AntiTA > 14)
		   	{
		   		temp[issuerid].AntiTA=0;
		  	   	ApplyAnimation(issuerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 1, 1, 0, 0, 0);
		  	  	GameTextForPlayer(issuerid, "~b~WTF ~g~Zombie?!", 1000, 5);
			}
		}
		
		if(za[playerid].UCID == -8 && za[issuerid].Team == true && temp[playerid].ZombieRogue)
		{
			GameTextForPlayer(issuerid, "~g~Fake ~r~Human", 1000, 5);
		}
		else if(za[playerid].UCID == 26 && za[issuerid].Team == false && temp[playerid].HumanRogue)
		{
			GameTextForPlayer(issuerid,"~g~Fake ~b~Zombie", 1000, 5);
		}
		//Copier
    	if(za[issuerid].UCID == 47 && weaponid == 10 && za[playerid].AdminDuty == false)
    	{
	    	if(temp[issuerid].Pause > time(NULL))
			{
				GameTextForPlayer(issuerid, "~g~STILL RECOVERING..", 500, 5);
			}
			else
			{
				temp[issuerid].Pause=time(NULL)+10;
				temp[playerid].AbilityNotReady=true;
				
				ResetPlayerWeapons(issuerid);
				GivePlayerWeapon(issuerid, 10, 1);
				int weapon, ammo;
				for(int i=1; i < 10; i++)
				{
					GetPlayerWeaponData(playerid, i, &weapon, &ammo);
					if(weapon && ammo)GivePlayerWeapon(issuerid, weapon, ammo/2);
				}
				float health, armour, armour2;
				GetPlayerArmour(playerid, &armour);
				GetPlayerArmour(issuerid, &armour2);				
				GetPlayerHealth(playerid, &health);
				if(armour > armour2 && armour < 51.0)SetPlayerArmour(issuerid, armour);
				//Boss, Tank or Juggernaut Zombies;
				if((playerid == BZID && BossZombie == true) || za[playerid].UCID == -15 || za[playerid].UCID == -47)
				{
					SetPlayerHealth(issuerid, 99.0);
				}
				else if(health < 100.0)SetPlayerHealth(issuerid, health);
				
				SetPlayerFightingStyle(issuerid, GetPlayerFightingStyle(playerid));

				ClearAnimations(issuerid, 1);
				SetPlayerSkin(issuerid, GetPlayerSkin(playerid));
				TogglePlayerControllable(issuerid, 1);
			}
		}
		//Helicopter Blades Fix;
		if(weaponid == 50 && za[playerid].Team == true)
		{
			ClearAnimations(playerid, 1);
		}
		
		if(za[issuerid].Team != za[playerid].Team)
		{
			//Sound
			if(za[issuerid].Team == false && !((za[issuerid].Settings)&0x2) && weaponid != 37)PlayerPlaySound(issuerid, 17802);
			//Assist Killing
			if(za[playerid].AdminDuty == false)temp[issuerid].Assist[playerid]+=amount;

			//firemaniac and pyromaniac
			if(((za[issuerid].UCID == 9 && weaponid == 25) || (za[issuerid].UCID == 41 && weaponid == 24)) && za[playerid].AdminDuty == false)
			{
				//anti-flamethrower, wizard, tank, juggernaut can't be burned.
				if(za[playerid].UCID == -32 || za[playerid].UCID == -31 || za[playerid].UCID == -15 || za[playerid].UCID == -47)return;
				float x, y, z;
				GetPlayerPos(playerid, &x, &y, &z);
				if(IsPlayerInRangeOfPoint(issuerid, 15.0, x, y, z))
				{
					temp[playerid].Burned=true;
					TextDrawShowForPlayer(playerid, t_Yellow);
					SetPlayerAttachedObject(playerid, 9, 18689, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0);
				}
			}
			//pyromaniac
			else if(za[issuerid].UCID == 41 && weaponid == 33 && za[playerid].AdminDuty == false)
			{
				//anti-flamethrower and wizard can't be exploded.
				if(za[playerid].UCID == -32 || za[playerid].UCID == -31)return;
				float x, y, z;
				GetPlayerPos(playerid, &x, &y, &z);
				CreateExplosion(x, y, z, 12, 4.0);
			}
			//samurai, boxer and pirate zombie
			else if(((za[issuerid].UCID == 14 || za[issuerid].UCID == -25) && weaponid == 8) || (za[issuerid].UCID == 18 && weaponid == 0))
			{
				if(za[playerid].AdminDuty == false)
				{
					if(za[playerid].UCID != -47 && za[playerid].UCID != -15)
					{
						float armour;
						GetPlayerArmour(playerid, &armour);
						
						if(armour > 0.0)
							SetPlayerArmour(playerid, 0.0);
						else
						{
							float health;
							GetPlayerHealth(playerid, &health);
					        SetPlayerHealth(playerid, health-50.0);
						}
					}
				}
			}
			//Assasin Or Sharpshooter
			else if((za[issuerid].UCID == 21 || za[issuerid].UCID == 32) && bodypart == 9 && za[playerid].AdminDuty == false && temp[playerid].Helmet == false && GetPlayerScore(playerid) > 1500)
	    	{
		       	if(za[playerid].UCID != -47 && za[playerid].UCID != -15)
				{
					if(BZID == playerid && BossZombie == true);
					else SetPlayerHealth(playerid, 0.0);
				}
	    	}
	   		//Advanced Boxer And Samurai
			else if((za[issuerid].UCID == 24 && weaponid == 0) || (za[issuerid].UCID == 25 && weaponid == 8))
			{
				if(za[playerid].UCID != -47 && za[playerid].UCID != -15)
				{
			    	if(za[playerid].AdminDuty == false)SetPlayerHealth(playerid, 0.0);
				}
			}
	    	//Poisoner
			else if(za[issuerid].UCID == 37)
			{
				if(za[playerid].Team == true && za[playerid].AdminDuty == false)
				{
					if(weaponid == 33)
					{
						temp[playerid].Poison=9;
						TextDrawShowForPlayer(playerid, t_Magenta);
					}
					else if(weaponid == 41)
					{
						TextDrawShowForPlayer(playerid, t_Magenta);
						temp[playerid].Poison=10;
					}
				}
			}
			//Reaper
			else if((za[issuerid].UCID == -4 || (issuerid == BZID && BossZombie == true)) && weaponid == 9)
			{
	            float armour;
				GetPlayerArmour(playerid, &armour);
					
				if(armour > 0.0)
					SetPlayerArmour(playerid, 0.0);
				else
				    SetPlayerHealth(playerid, 0.0);
			}
			//undead boxer zombie
			else if(za[issuerid].UCID == -33)
			{
				if(za[playerid].Team == false && weaponid == 0)
				{
					float x, y, fa, health;
					GetPlayerFacingAngle(issuerid, &fa);
					x=0.4*cos(((fa+90.0)/180)*3.14159265358979);
					y=0.4*sin(((fa+90.0)/180)*3.14159265358979);
					SetPlayerVelocity(playerid, x, y, 0.22);
	
					GetPlayerHealth(playerid, &health);
			        SetPlayerHealth(playerid, health-30.0);
				}
			}
	    	//Adv. Policeman
			else if(za[issuerid].UCID == 46 && za[playerid].Team == true && za[playerid].AdminDuty == false)
			{
				if(weaponid == 0 && temp[playerid].Tazered == false)
				{
					if((time(NULL)-temp[issuerid].TazerFist) > 5)
					{
						temp[issuerid].TazerFist=time(NULL);
						temp[playerid].Tazered=true;
						float x, y, z;
						GetPlayerPos(playerid, &x, &y, &z);
						int objectid=CreateObject(18717, x, y, z-2.3, 0.0, 0.0, 0.0, 100.0);
						ApplyAnimation(playerid, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 0, 1);	
						SetTimerEx("Tazer", 5000, playerid);
						SetTimerEx("TazerObject", 5000, objectid);
						GameTextForPlayer(playerid, "~n~~n~~n~~b~~h~~h~electro~g~~h~~h~shock", 5000, 3);
					}
					else
					{
						GameTextForPlayer(issuerid, "~g~STILL RECOVERING..", 800, 5);
					}
				}
				else if(weaponid == 24 && temp[issuerid].Tazer == true)
				{
					temp[issuerid].Tazer=false;
					RemovePlayerAttachedObject(issuerid, 9);
					float x, y, z;
					GetPlayerPos(playerid, &x, &y, &z);
					
					if(temp[playerid].Tazered == false)
					{
						temp[playerid].Tazered=true;
						int objectid=CreateObject(18717, x, y, z-2.3, 0.0, 0.0, 0.0, 100.0);
						ApplyAnimation(playerid, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 0, 1);
						SetTimerEx("Tazer", 5000, playerid);
						SetTimerEx("TazerObject", 5000, objectid);
						GameTextForPlayer(playerid, "~n~~n~~n~~b~~h~~h~electro~g~~h~~h~shock", 5000, 3);	
					}
					
					for(int i=0; i < AMOUNT; i++)
					{
						if(za[i].Log == true && za[i].Team == true && temp[i].Tazered == false && IsPlayerInRangeOfPoint(i, 7.0, x, y, z))
						{		
							temp[i].Tazered=true;
							float xi, yi, zi;
							GetPlayerPos(i, &xi, &yi, &zi);
							int oid=CreateObject(18717, xi, yi, zi-2.3, 0.0, 0.0, 0.0, 100.0);
							ApplyAnimation(i, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 0, 1);
							SetTimerEx("Tazer", 4000, i);
							SetTimerEx("TazerObject", 4000, oid);
							GameTextForPlayer(i, "~n~~n~~n~~b~~h~~h~electro~g~~h~~h~shock", 4000, 3);
						}
					}
				}
			}
	    	
			//Chainsaw-Proof: Hero, Adv.Zombiefan, Iron Man, Adv.Hero
			if((za[playerid].UCID == 22 || za[playerid].UCID == 28 || za[playerid].UCID == 31 || za[playerid].UCID == 39) && weaponid == 9)
	    	{
	        	float armour;
				GetPlayerArmour(playerid, &armour);
				if(armour == 0.0)
				{
					float health;
					GetPlayerHealth(playerid, &health);
					if((health+amount) < 95.0)
						SetPlayerHealth(playerid, health+amount-6.0);
					else
						SetPlayerHealth(playerid, 95.0);	
				}
			}	
			//Mirror Zombie
			else if(za[playerid].UCID == -48 && (bodypart == 3 || bodypart == 4 || bodypart == 5 || bodypart == 6))
			{
				float health;
				GetPlayerHealth(issuerid, &health);
				SetPlayerHealth(issuerid, health-8.0);
						
				if((health-((amount*3)/4)) < 1.0)
				{
					char str[32];
					snprintf(str, sizeof(str), "~n~~y~+%d", xpmult*10);
					GameTextForPlayer(playerid, str, 1000, 5);	
					SetPlayerScore(playerid, GetPlayerScore(playerid)+10*xpmult);						
				}
			}
			//Spore Zombie
			else if(za[playerid].UCID == -43 && temp[playerid].Spore == true && temp[issuerid].Infection == false)
			{
				Infect(issuerid, playerid);
			}
			
			
		}
	}//if(issuerid != INVALID_PLAYER_ID)
return;
}




