/* Programmer: Jordan Shinespark
	Helper(s): 'otterpop!'- provided smarter RNG calls and removed redundant Die objects
	
	Purpose: to create a streamlined CC-numbers-generator to speed up play.
	Start Date: 3/31/20 yikes!
	Finish Date: 1/5/21
	
	Version 4.0
		-Release
		
	
	To-Do:
		-Add Events: continue at (first?)
		-rework dice for new random method
		-add allTrue to all Encounters
		
	Future:
		Create a turn simulator
		Add Caretaker action generator
		Universal statistic holder for calculations
		Create auto-filler feature for repeated stats
*/

#include <iostream.h>
#include <limits>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <windows.h>
#include <string>
using namespace std;

string version = "4.0";

//Menu navigation
bool Forfeit();
bool makeSure();
int  YesNo();
void Caretakers();
void eventHelper();
void HeadMenu();
void Traps();

//RNG calculators (numbers)
	//defined as input->output with no player prompting
int DieN(int N);
int IRTcalc(int penalty, int tier);
int Misfire(int Caring);
int pointCalc(int pTier);
int randomEvent();
int scroll();
int TrapArtist();
void CareGenerator(int Care[], bool Caring);
void drinking();
void eating();
void giveScroll();
void Playtime();
void TrapGenerator(int Trap[]);
void UseCheck(int UCpen, int UCtier);

//Major roller menus
	//defined as the major, biggest menus that are often used
int TrapRoll();
void Encounter();
void foodDrink();
void ForcedIR();
void IdleRolls();
void perTurn();
void UCprompt();

//Minor roller menus
	//defined as smaller, less commonly used menus
void BombPoof(int style);
void DiaperRoller(int type);
void HypnoCheck();
void ItemRoller(int type);
int Intensity(int base);
void uniPrint(); //not implemented until 4.1

//number to name translators
string CarePrint(int val);
string DiaperPrint(int val); //helper: only prints out diaper names
void FullDiaperPrint(int val); //prints out diaper names with trap diaper fake names
string EventPrint(int val);
string ItemPrint(int val); //helper: only prints out item names
void FullItemPrint(int val); //prints out item names with cursed item fake names
void jewelry(int jewel);
string MisfirePrint(int val);
void randomGR(int lock);
string ScrollPrint(int val);
string taRoomPrint(int val);
string TrapPrint(int val);

//Caretakers (in order of additions)
void Kangaskhan();
void Goodra();
void Wigglytuff();
void Serperior();
void Gardevoir();
void Flygon();
void Boss();
void Miltank();
void Primarina();
void PrimSiren();
void Leavanny();
void Togekiss();
void Ninetales();
void Chansey();
void Salazzle();
void Luxray();
void Nidoqueen();
void Vaporeon();
void Audino();
void Lucario();
void Ampharos();
void Ivysaur();
void Slowpoke();
void Snorlax();
void Mew();
void Galvantula();
void Ursaring();
void AlakazamPac();
void AlakazamSpo();
void Victini();
void Dragonite();
void Delphox();
void Smeargle();
void Shaymin();
void Lapras();
void Milotic();
void Sylveon();
void cDiapers();
void cImmature();
void cLazy();
void cLittle();
void cSissy();
void cSquish();


//Traps, in order of addition
/*
	1- critical failure
	2- heavy failure
	3- expected failure
	4- resisted
	5- partial success
	6- succeed
*/
void HypnoTV();
void MCL();
void Nursery();
void PowderRoom();
void BounceChair();
void HighChair();
void PowderPath();
void ChangingRoom(bool Caretaker);
void CareChangingRoom();
void PotionChallenge();
void Presentation();
void AlarmTrap();
void PacifierDoor();
void WaterGun();
void MusicRoom();
void SleepyRoom();
void ChangeMachine();
void OddMirror();
void DimWand();
void DrainDiaper();
void BabyVG();
void DiaperVG();
void DragonVG();
void LockDoor();
void TickleMachine();
void Energy();
void BossBomb();
void Ambush();
void PlushPile();
void PadSuit();
void CareSpecial();
void Ritual();
void ArtRoom();
void TagAlong();
void Storeroom();
void TrainPotty();
void ArmyHoard();
void SeekBottles();
void DivePool();
void SirenSong();
void foyGun();
void MCP();
void TimeDistort();

//Events, future addition
/*void DragonLair();
void GameShow();
void Greenhouse();
void KecleonShop();
void Gambling();
void MonsterHouse();
void PaddedPride();*/

//Global variables
string location = "ERROR"; //prints out whatever the type of generator it is for YesNo (ex. Encounter, IdleRoller)
bool debug = false; //if true, enables debug mode
bool looper = true; //makes the entire program persist until closed
bool check = true; //used by all text prompts to repeat if misinputted
bool perPlayer = true; //repeats Encounter for multiple player generations
bool mother = false; //if true, then ignore some prompts that are auto-filled
bool rollUntil = false; //used to make a roll repeat indefinitely until hitting threshold
bool bombArtist = false; //if true, ignore critical failures for BombPoof calculation
bool status = true; //if false, ignore status effect printouts
bool aura = false; //halves UC values when true
bool forfeit = false;
bool tdForfeit = true; //if true, ask if wanting to forfeit, else don't ask
bool certainty = false; //if true, player verifies to continue. If false, repeat inputs
bool first = true; //if true, re-iterating the function; silence some output accordingly
bool pass = false; //if true, this function was passed to by another function
bool allTrue = false; //debug value. If true, all per-Turn, per-Encounter, and per-Idle effects are forced to be true
bool uniAdd = false; //if true, do universal addition prompts
int rollCount = 0;
int intput;
int intput2;
int intput3;
char charput;
int Itotal;
int UCpen = 0;
int UCTauto = 0; //if -1, adds 1 to UC penalty on that roll VERIFY THIS
int IRauto = 0;
int FIRauto = 0; //boosts Forced Idle Roll's Tier
int HCauto = 0;
int foodAuto = 0;
int drinkAuto = 0;
int HPgain = 0;
int Pwet = 0;
int Pmess = 0;
int tRoll = 0;

//Item generator constants
const int foodItemMax = 6; //RNG max of food items
const int useItemMax = 18; //RNG max of use items
const int itemMax = 30; //RNG max of cursed items
const int useSpread = useItemMax - foodItemMax; //auto-calculate the use item spread
const int cursedSpread = itemMax - useItemMax; //auto-calculate the cursed item spread

//Diaper generator constants
const int diaperMax = 32; //RNG max of not-trap diapers
const int trapDMax = 46; //RNG max with cursed diapers
const int trapDSpread = trapDMax - diaperMax; //auto-calculate the trap diaper spread

//Universal totaling variables
int stackLv = 0; //if 1, not being passed. If not 1, is being passed to
int uniHP = 0;
int uniRB = 0;
int uniMP = 0;
int uniBP = 0;
int uniEP = 0;
int uniWHP = 0;
int uniMHP = 0;
bool hypnoState = false; //tells whether or not recent HC succeeded
string uniStat = ""; //underscores become spaces, spaces separate status effects


//Let's make the magic happen!
main()
{
	// Seed randomness once using the current time.
	srand(time(NULL));
	
	HeadMenu();

return 0;
}

										//basic menus

//ask for forfeit; last update: 4.0
bool Forfeit(){
	check = true;
   while(check == true)
   {
   	cout<<endl<<"Does the party forfeit to this Caretaker (y/n)?: ";
   	cin>>charput;

   	if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
   		cin.clear();
   		cin.ignore();
   		charput = 'q';
   		cout<<"Please enter a proper input. "<<endl;
   	} else {
   		check = false;
		tdForfeit = false;
   	}
   }

   if((charput == 'n')||(charput == 'N')) {
   	return false;
   } else {
      return true;
   }
}



//ensures player can clean misinputs; last update: 4.0
bool makeSure(){
	
	check = true;
	while(check == true){
		cout<<endl<<"Please review your inputs in this section. Would you like to redo (y/n)? ";
		cin>>charput;
		
		if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter a proper input. "<<endl;
		} else {
			check = false;
		}
	}
	
	if((charput == 'n')||(charput == 'N')) {
		return 1;
   } else {
		return 0;
   }
}



//Prompt to repeat generation; last update: 4.0
int YesNo(){
	check = true;
   while(check == true){
	   
		cout<<endl<<"Generate "<<location<<" again for another player (y/n)?: ";
		cin>>charput;

		if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter a proper input. "<<endl;
		} else {
			check = false;
		}
   }

   if((charput == 'n')||(charput == 'N')) {
   	return 0;
   } else {
      return 1;
   }
}



//Caretaker navigation menu; last update: 4.0 stackified
void Caretakers(){
	string input;
	location = "Caretaker";
	
	system("cls");
	
	stackLv++; //increment stack level early

   	check = true;
      while(check == true){
		cout<<"Welcome to the caretaker menu! Please enter your roll #."<<endl;
      	cout<<"Or, to go back to the main menu, enter 0. All bosses are on 1."<<endl;
		cout<<"Looking for Event Caretakers? See the Events section instead."<<endl;
		cout<<"Input: ";

      		cin>>intput;

            if(cin.fail()){
					cin.clear();
					cin.ignore();
               intput = -1;
               cout<<"Please enter a number."<<endl<<endl;
               getch();
               system("cls");
            } else {
             	check = false;
            }
         }
		 
		system("cls");

   	switch(intput){
         case 0:
         cout<<"Returning to main menu...";
         break;

		//Bosses
       	case 1:
         Boss();
         break;

		//Tier 0 Caretakers
         case 2:
         Chansey();
         break;

         case 3:
         cDiapers();
         break;

         case 4:
         cImmature();
         break;

         case 5:
         cLazy();
         break;

         case 6:
         cLittle();
         break;

         case 7:
         cSissy();
         break;

         case 8:
         cSquish();
         break;

         case 9:
         Gardevoir();
         break;
		 
		 case 10:
		 Ivysaur();
		 break;

         case 11:
         Kangaskhan();
         break;

         case 12:
         Leavanny();
         break;

         case 13:
         Mew();
         break;

         case 14:
         Ninetales();
         break;

         case 15:
         Serperior();
         break;

         case 16:
         Shaymin();
         break;

         case 17:
         Slowpoke();
         break;

         case 18:
         Smeargle();
         break;

         case 19:
         Togekiss();
         break;

         case 20:
         Victini();
         break;

		//Tier 1 Caretakers
         case 21:
         AlakazamPac();
         break;

         case 22:
         AlakazamSpo();
         break;

         case 23:
         Ampharos();
         break;

         case 24:
         Flygon();
         break;

         case 25:
         Galvantula();
         break;

         case 26:
         Goodra();
         break;

         case 27:
         Luxray();
         break;

         case 28:
         Milotic();
         break;

         case 29:
      	 Miltank();
         break;

         case 30:
         Primarina();
         break;

         case 31:
         PrimSiren();
         break;

         case 32:
         Snorlax();
         break;

         case 33:
         Ursaring();
         break;

         case 34:
         Wigglytuff();
         break;
		 
		 //Tier 2 Caretakers
		 case 35:
		 Audino();
		 break;
		 
		 case 36:
		 Delphox();
		 break;
		 
		 case 37:
		 Dragonite();
		 break;
		 
		 case 38:
		 Lapras();
		 break;
		 
		 case 39:
		 Lucario();
		 break;
		 
		 case 40:
		 Nidoqueen();
		 break;
		 
		 case 41:
		 Salazzle();
		 break;
		 
		 case 42:
		 Sylveon();
		 break;
		 
		 case 43:
		 Vaporeon();
		 break;

         default:
         cout<<"Please input a proper number from 1-43."<<endl;
         getch();
         system("cls");
         break;
      }
	  
	stackLv--; //decrement stackLv when done
}



//Event navigation menu; last update: 4.0 stackified
void eventHelper(){
/*	system("cls");
	location = "Event";
	
	stackLv++; //increment stackLv early

   	check = true;
      while(check == true)
      {
		cout<<"Welcome to the Event helper. Please enter your roll #."<<endl;
      	cout<<"Or, to go back to the main menu, enter 0."<<endl;
		cout<<"Input: ";

      		cin>>intput;

            if(cin.fail()){
					cin.clear();
					cin.ignore();
               intput = -1;
               cout<<"Please enter a number."<<endl<<endl;
               getch();
               system("cls");
            } else {
             	check = false;
            }
      }
	
	switch(intput){
		case 0:
		cout<<"Returning to main menu..."<<endl;
		break;
		
		case 1:
		DragonLair();
		break;
		
		case 2:
		GameShow();
		break;
		
		case 3:
		Greenhouse();
		break;
		
		case 4:
		KecleonShop();
		break;
		
		case 5:
		Gambling();
		break;
		
		case 6:
		MonsterHouse();
		break;
		
		case 7:
		PaddedPride();
		break;
		
		default:
		cout<<"Please enter a proper number between 1 and 7."<<endl; 
		getch();
        system("cls");
		break;
	}
	
	stackLv--; //decrement stack level when done*/
}



//Main menu; last update: 4.0
void HeadMenu(){
	int Care[3];
	int Trap[3];
	looper = true;

   while(looper == true) {
	   pass = false;
	   uniRB = 0;
	   uniMP = 0;
	   uniBP = 0;
	   uniEP = 0;
	   uniWHP = 0;
	   uniMHP = 0;
	   uniStat = "";
	   tdForfeit = true;
   	system("cls");

		if(!debug){ //hide useless info if debugging
			cout<<"Welcome to the v"<<version<<" CC defeat printout! How may I assist you today?"<<endl;
			cout<<"Whenever you are prompted with 2 or more inputs, you will be asked if you want to redo inputs for entire sequence."<<endl;	
			cout<<endl;
		} else { //give special info if debugging
			cout<<"Stack level: "<<stackLv<<endl<<endl;
		}
		
   	cout<<"Options:"<<endl;
   	cout<<"0- End program"<<endl;
	cout<<"1- Bomb effect calculator"<<endl;
	cout<<"2- Caretaker Party Wipes"<<endl;
	cout<<"3- Diaper print generator"<<endl;
	cout<<"4- Encounter effect caculator"<<endl;
	cout<<endl;
	cout<<"5- Event helper (currently unavailable)"<<endl;
	cout<<"6- Food calculations"<<endl;
	cout<<"7- Forced Idle Roll calculator"<<endl;
	cout<<"8- Ghost Regression generator"<<endl;
	cout<<"9- Hypnosis Check roller"<<endl;
	cout<<endl;
	cout<<"10- Idle Roll calculator"<<endl;
	cout<<"11- Item/Scroll roller"<<endl;
	cout<<"12- Misfire roller, including general, Wand, and Caretaker"<<endl;
	cout<<"13- Random Encounter generator"<<endl;
	cout<<"14- Room calculator"<<endl;
	cout<<"15- Trap Roll roller"<<endl;
	cout<<endl;
	cout<<"16- Turn effect calculator"<<endl;
	cout<<"17- Use Check calculator"<<endl;
	cout<<endl;
	if(status){ //if status is enabled, say disable
		cout<<"18- disable statuses for Idle Rolls"<<endl;
	} else { //if disabled, say enable
		cout<<"18- enable statuses for Idle Rolls"<<endl;
	}

	
	if(debug){
		cout<<endl<<"Debug:"<<endl;
		cout<<"20- toggle allTrue for chance effects"<<endl;
		//cout<<"21- "<<endl;
	}
	cout<<endl<<"Input: ";

      	do{
      		cin>>intput;

            if(cin.fail()){
					cin.clear();
					cin.ignore();
               intput = 0;
               cout<<"Please enter a number.";
               getch();
               system("cls");
			   check = true;
            } else {
             	check = false;
            }
         }while(check == true);

    switch(intput){
		case -1: //toggle debug, hidden and not shown in menu
			debug = !debug;
		break;
		
		case 0: //end program
         looper = false;
		 //ends program
		break;	
		 
		case 1: //BombPoof
			system("cls");
			BombPoof(0);
		break;
		
		case 2: //Caretaker party wipes
			Caretakers();
		break;
		 
		case 3: //Diaper print generator
			DiaperRoller(0);
		break;
		
		case 4: //Encounter effect calc
			Encounter();
		break;
		 
		/*case 5: //Event helper, future addition
			eventHelper();
        break;*/
		 
		case 6: //Food calc
			system("cls");
			foodDrink();

        break;
		
		case 7: //Forced IR calc
			system("cls");
			ForcedIR();
		break;
		
		case 8: //Ghost Regression randomizer
				perPlayer = true;
			while(perPlayer == true){
				location = "random GR";
				system("cls");
				randomGR(0);
				perPlayer = YesNo();
			}
		break;
		
		case 9: //Hypnosis Check calc
			HypnoCheck();
		break;
		 
        case 10: //Idle Roll calc
			IdleRolls();
        break;
		
		case 11: //Item roller
			ItemRoller(0);
		break;
		 
		case 12: //Misfire generator
			cout<<endl<<"Player-cast misfire: "<<MisfirePrint(Misfire(0))<<endl;
			cout<<"Weapon spell misfire: "<<MisfirePrint(Misfire(3))<<endl<<endl;
			cout<<"Caretaker-cast misfire of: "<<MisfirePrint(Misfire(1))<<endl;
			cout<<"Magic Caster misfire of: "<<MisfirePrint(Misfire(2))<<endl;
			cout<<endl<<"Press [Enter] to return to main menu.";
			getch();
		break;
		
		case 13: //Random Encounter generator
			CareGenerator(Care, false);
			TrapGenerator(Trap);
			
			system("cls");
			cout<<"Encounter Generator"<<endl;
			
		//Caretakers
			cout<<endl<<"Tier 0 Caretaker: #"<<Care[0]<<endl;
			cout<<"Tier 0-1 Caretaker: #"<<Care[1]<<endl;
			cout<<"Tier 0-2 Caretaker: #"<<Care[2]<<endl;
			if(Care[3] == 1){
				cout<<"SPECIAL: A non-Trap Artist Caretaker will appear in a Trap Artist room."<<endl;
			}
			cout<<"Trap Artist Room: "<<taRoomPrint(TrapArtist())<<endl;
		
		//Rooms
			cout<<endl<<"Tier 0 Room: #"<<Trap[0]<<endl;
			cout<<"Tier 0-1 Room: #"<<Trap[1]<<endl;
			cout<<"Tier 0-2 Room: #"<<Trap[2]<<endl;
			
		//Events	
			cout<<endl<<"Event #"<<randomEvent()<<endl;
			cout<<endl<<"Press [Enter] to return to main menu.";
			getch();
		break;
		 
		case 14: //Room effect calc
			Traps();
		break;
		
		case 15: //Trap Roller
			system("cls");
			cout<<"Trap Roll generator"<<endl;
			TrapRoll();
			cout<<"Press [Enter] to return to main menu.";
			getch();
		break;
		
		case 16: //Turn effect calc
			perTurn();
		break;
		
		case 17: //Use Check calc
				perPlayer = true;
			while(perPlayer == true){
				location = "Use Check";
				system("cls");
				UCprompt();
				perPlayer = YesNo();
			}
		break;
		
		case 18: //toggle status printouts for Idle Rolls
			status = !status;
		break;
		
		//debug menus
		case 20:
			if(debug){
				allTrue = !allTrue;
				cout<<"allTrue is now "<<allTrue<<endl;
				getch();
				break;
			}

        default:
			cout<<endl<<"Please enter a proper selection. Press [Enter] to retry.";
			getch();
        break;
    }
   }

}



//Trap navigation menu; last update: 4.0 stackified
void Traps(){
	system("cls");
	location = "Room";
	
	stackLv++; //increment stack level early

   	check = true;
      while(check == true){
		cout<<"Welcome to the Room Encounter calculator. Please enter your roll #."<<endl;
      	cout<<"Or, to go back to the main menu, enter 0."<<endl;
		cout<<"Input: ";

      		cin>>intput;

            if(cin.fail()){
					cin.clear();
					cin.ignore();
               intput = -1;
               cout<<"Please enter a number."<<endl<<endl;
               getch();
               system("cls");
            } else {
             	check = false;
            }
      }
	
	switch(intput){
		case 0:
		cout<<"Returning to main menu..."<<endl;
		break;

		//Tier 0 Rooms
		case 1:
		AlarmTrap();
		break;
		
		case 2:
		Ambush();
		break;
		
		case 3:
		CareChangingRoom();
		break;
		
		case 4:
		CareSpecial();
		break;
		
		case 5:
		ChangeMachine();
		break;
		
		case 6:
		ChangingRoom(false);
		break;
		
		case 7:
		Energy();
		break;
		
		case 8:
		HighChair();
		break;
		
		case 9:
		Storeroom();
		break;
		
		case 10:
		LockDoor();
		break;
		
		case 11:
		TagAlong();
		break;
		
		case 12:
		PacifierDoor();
		break;
		
		case 13:
		Nursery();
		break;
		
		case 14:
		PotionChallenge();
		break;
		
		case 15:
		Presentation();
		break;
		
		case 16:
		SeekBottles();
		break;
		
		case 17:
		ArtRoom();
		break;
		
		case 18:
		TickleMachine();
		break;
		
		case 19:
		TrainPotty();
		break;
		
		//Tier 1 Rooms
		case 20:
		BabyVG();
		break;
		
		case 21:
		BossBomb();
		break;
		
		case 22:
		BounceChair();
		break;
		
		case 23:
		DiaperVG();
		break;
		
		case 24:
		DivePool();
		break;
		
		case 25:
		DragonVG();
		break;
		
		case 26:
		MCL();
		break;
		
		case 27:
		MCP();
		break;
		
		case 28:
		OddMirror();
		break;
		
		case 29:
		PadSuit();
		break;
		
		case 30:
		PlushPile();
		break;
		
		case 31:
		PowderRoom();
		break;
		
		case 32:
		SirenSong();
		break;
		
		case 33:
		SleepyRoom();
		break;
		
		case 34:
		WaterGun();
		break;
		
		//Tier 2 Rooms
		case 35:
		Ritual();
		break;
		
		case 36:
		PowderPath();
		break;
		
		case 37:
		DimWand();
		break;
		
		case 38:
		DrainDiaper();
		break;
		
		case 39:
		foyGun();
		break;
		
		case 40:
		HypnoTV();
		break;
		
		case 41:
		MusicRoom();
		break;
		
		case 42:
		ArmyHoard();
		break;
		
		case 43:
		TimeDistort();
		break;
		
		default:
        cout<<"Please input a proper number from 1-43."<<endl;
        getch();
        system("cls");
		break;
	}

	first = true;
	stackLv--; //decrement stack level when done
}


										//RNG calculators
										
//Universal die roller; last update: 4.0
int DieN(int N){
	return rand() % N + 1;
}



//calculate Idle Roll value; last update: 4.0
int IRTcalc(int penalty, int tier){
	int bonus = 0;
	int HIGH;

	
	switch(tier){
		case -1:
			HIGH = 4;
		break;
		
		case 0:
			HIGH = 6;
		break;
		
		case 1:
			HIGH = 7;
			bonus = 2;
		break;
		
		case 2:
			HIGH = 11;
			bonus = 2;
		break;
		
		case 3:
			HIGH = 14;
			bonus = 3;
		break;
		
		case 4:
			HIGH = 16;
			bonus = 4;
		break;
		
		default:
			HIGH = 20 + (tier - 4) * 2;
			bonus = 4;
		break;
	}
	
	//generate roll
	return (DieN(HIGH) + bonus + penalty);
}



//Misfire RNG value; last update: 4.0
int Misfire(int Caring){
	int die;

	
	if(Caring == 1){ //if Caring, generate Caretaker Misfires
		die = DieN(31);
		
	} else if(Caring == 2){ //Magic Caster: no Magic Wind, Hyperbomb, or Powder Explosion
		die= DieN(23);
		
		if(die >= 20){ //if passed Powder Explosion
			die += 3;
		} else if(die >= 15){ //if passed Magical Wind
			die += 2;
		} else if(die >= 12){ //if passed Hyperbomb
			die++;
		}
		
	} else if (Caring == 3){ //if player Weapon Misfire
		die = DieN(14);
		
		switch(die){ //corrects weapon Misfire to Scroll Misfire values
			case 3: //bump 3/4 to scroll's 4/5
			case 4:
				die++;
			break;
			
			case 5:
				die = 7;
			break;
			
			case 6: //bump 6/7 to scroll's 10/11
			case 7:
				die += 4;
			break;
			
			case 8:
				die = 14;
			break;
			
			case 9:
				die = 18;
			break;
			
			case 10: //bump 10/11/12 to scroll's 21/22/23
			case 11:
			case 12:
				die += 11;
			break;
			
			case 13: //bump 13/14 to scroll's 25/26
			case 14:
				die += 12;
			break;
			
			default:
				cout<<"ERROR on Weapon Misfire"<<endl;
				return 0;
		}
		
	} else if(Caring == 3) { //if Scroll Misfire
		die = DieN(26);
	} else { //if Arch Delphox Party Wipe
		die = DieN(23);
		
		if(die >= 23){ //if passed Stupify
			die += 3;
		} else if(die >= 22){ //if passed Sting
			die += 2;
		}
	}
	
	return die;
}



//calculate a Point Idle value; last update: 4.0
int pointCalc(int pTier){
	int bonus = 4;
	int HIGH;
	
	if(pTier > 6){ pTier = 6; }

	
	switch(pTier){
		case 0:
			return 0;
		
		case 1:
			HIGH = 6;
		break;
		
		case 2:
			HIGH = 8;
		break;
		
		case 3:
			HIGH = 10;
			bonus = 5;
		break;
		
		case 4:
			HIGH = 12;
			bonus = 5;
		break;
		
		default:
			HIGH = 15 + (pTier - 4) * 3;
			bonus = 5;
		break;
	}
	
	//generate roll
	return (DieN(HIGH) +bonus);
}



//Event RNG value; last update: 4.0
int randomEvent(){	
	return DieN(7);
}



//Generate a random scroll value; last update: 4.0
int scroll(){
	return DieN(10);
}



//Trap Artist room RNG value; last update: 4.0
int TrapArtist(){
	return DieN(13);
}



//Caretaker RNG value; last update: 4.0
void CareGenerator(int Care[], bool Caring){
	int HIGH_T0 = 20; //if at Tier 0
	int HIGH_T1 = 34; //if at Tier 1
	int HIGH_T2 = 43; //if at Tier 2
	const int TrapArtistOdds = 5; //percentage
	
	
	//if only fighting Caretakers (remove Shaymin)
	if(Caring){
		HIGH_T0 = 19;
		HIGH_T1 = 33;
		HIGH_T2 = 42;
		}
	
	Care[0]=DieN(HIGH_T0);
	Care[1]=DieN(HIGH_T1);
	Care[2]=DieN(HIGH_T2);
	
	//non-Trap Artist in Trap Artist room
	if(DieN(100) <= TrapArtistOdds){
		Care[3] = 1;
	} else {
		Care[3] = 0;
	}
	
	//if non-hostiles removed
	if((Caring)&&(Care[0] >= 16)){Care[0]++;}
	if((Caring)&&(Care[1] >= 16)){Care[1]++;}
	if((Caring)&&(Care[2] >= 16)){Care[2]++;}
}



//drinking drink effects; last update: 4.0
void drinking(){
	HPgain += DieN(3) + 3;
	Pwet += DieN(5) + 1;
}



//eating food effects; last update: 4.0
void eating(){
	HPgain += DieN(4) + DieN(4);
	Pmess += DieN(6) + 2;
}



//gives a random Scroll after Room completion; last update: 4.0
void giveScroll(){
	if(first){
		cout<<"The party can find a "<<ScrollPrint(scroll())<<" scroll in this Room."<<endl;
		first = false;
	}
}


//Playtime Encounter results; last update: 4.0
void Playtime(){
	int food;
	int drink;
	int i;
	bool wetDiu;
	bool messLax;
	int bWHP = 0;
	
	cout<<endl<<endl<<"This Encounter has Playtime, meaning the party's health is not restored normally."<<endl;
	cout<<"But, they do get to have a snack break! Let's see what happens for this player."<<endl;
	
	//drink and food count
	food = DieN(10)/5 + 1;
	drink = DieN(10)/5 + 1;

	//drink effect
	for(i = 0; i < drink; i++){
		drinking();
	}
	
	//food effect
	for(i = 0; i < food; i++){
		eating();
	}
	
	//drink Diuretic
	if((allTrue)||(DieN(100) > 50)){
		wetDiu = true;
	} else {
		wetDiu = false;
	}
	
	//food Laxative
	if((allTrue)||(DieN(100) > 50)){
		messLax = true;
	} else {
		messLax = false;
	}
	
	//auto-bottle generation
	if((allTrue)||(DieN(100) <= 25)){
		bWHP = DieN(7) + 11;
	}
	
	cout<<endl<<"Player drinks "<<drink<<" bottles and "<<food<<" units of food."<<endl;
	cout<<"Player recovered "<<HPgain<<" HP."<<endl;
	cout<<"Player gains "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
	
	if(bWHP){cout<<"Player is inflicted Auto-Bottle with "<<bWHP<<" WHP inside."<<endl;}
	
	//Laxative/Diuretic printout
	if((wetDiu)&&(messLax)){
		cout<<"Food diaper: player gains Diuretic and Laxative."<<endl;
	} else if(wetDiu){
		cout<<"Food diaper: player gains Diuretic."<<endl;
	} else if(messLax){
		cout<<"Food diaper: player gains Laxative."<<endl;
	}
	
	cout<<endl;
	
	//scrub for future inputs
	HPgain = 0;
	Pwet = 0;
	Pmess = 0;
}



//Spoiled Caretaker results; last update: 4.0
void Spoiled(){
	cout<<"Spoiled: player gains 10 ";
	
	switch(DieN(3)){
		case 1:
			cout<<"MP";
		break;
		
		case 2:
			cout<<"BP";
		break;
		
		case 3:
			cout<<"EP";
		break;
		
		default:
			cout<<"ERROR";
	}
	
	cout<<"."<<endl;
}


//Room Encounter RNG value; last update: 4.0
void TrapGenerator(int Trap[]){
	int HIGH_T0 = 19; //if at Tier 0
	int HIGH_T1 = 34; //if at Tier 1
	int HIGH_T2 = 43; //if at Tier 2+
	
	Trap[0]=DieN(HIGH_T0);
	Trap[1]=DieN(HIGH_T1);
	Trap[2]=DieN(HIGH_T2);
}



//Processing Use Check; last update: 4.0
void UseCheck(int UCpen, int UCtier){
	int die [2] = {1, 1};
	int bonus = 3;
	int HIGH = 1;

	
	if(UCTauto > 0){
		UCtier += UCTauto;
	}
	
	for(int i = 0; i < 2; i++)
	{	
		switch(UCtier){
		case -1:
		case 0:
			HIGH = 3;
		break;
		case 1:
			HIGH = 5;
		break;
		case 2:
			HIGH = 7;
		break;
		case 3:
			HIGH = 8;
			bonus = 4;
		break;
		case 4:
			HIGH = 10;
			bonus = 4;
		break;
		default:
			 HIGH = 12 + (UCtier - 4) * 2;
			 bonus = 4;
		break;
		}
		
		if(UCTauto == -1){
			bonus++;
		}
	
		//generate roll
		die[i]=DieN(HIGH);
	}

	Pwet += die[0] + UCpen + bonus;

	Pmess += die[1] + UCpen + bonus;

	if(UCtier == -1){
		Pwet *= 0.5;
		Pmess *= 0.5;
	}
}


										//Major roller menus

//Trap roll generator; last update: 4.0
int TrapRoll(){
	/*returning values
	1- critical failure
	2- heavy failure
	3- expected failure
	4- resisted
	5- partial success
	6- succeed
	*/
	
	int playerGen = 0;
	int placeholder;
	int seesaw = 0;
	int mod = 0;
	bool disadvan;
	bool noPartial;
	bool noResist;
	int die;

	
	do{ //repeat until inputs correct
	
		//find Trap Roll (if pre-generated)
			check = true;
		while(check == true){
		cout<<endl<<"If generated separately, enter the Trap Roll with modifier (for random, enter 0): ";
		
		cin>>playerGen;

		if((cin.fail())||(playerGen < 0)||(playerGen > 20)){
			cin.clear();
			cin.ignore();
			playerGen = -1;
			cout<<"Please enter a proper input between 1 and 20. "<<endl;
		} else {
			check = false;
		}
	   }

		//ask if randomly calcing disadvantage
		if(playerGen == 0){
			check = true;
			while(check == true)
			{
			cout<<"Disadvantage on traps? (y/n): ";
			
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
			}
		   }
		   if((charput == 'n')||(charput == 'N')) {
			   disadvan = false;
		   } else {
			   disadvan = true;
		   }
		}
	   
		if((playerGen == 0)||((playerGen >= 16)&&(playerGen <= 19))){ //only ask if relevant, that being: if random (0), if within effect range (16-19)
			//ask if Partial Success has been removed
				check = true;
			while(check == true)
			{
			cout<<"Weapons diaper or Trap Magnet? (y/n): ";
			
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
			}
		   }

		   if((charput == 'n')||(charput == 'N')) {
			   noPartial = false;
		   } else {
			   noPartial = true;
		   }
		} else {
			noPartial = false;
		}
	   
		if((noPartial)||(playerGen == 0)||((playerGen >= 11)&&(playerGen <= 15))){ //only ask if relevant, that being: if random (0), if within effect range (11-19)
		   //ask if player has Advocate, which replaces Resist with Expected Failure
				check = true;
			while(check == true)
			{
			cout<<"Does the player have Advocate? (y/n): ";
			
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
			}
		   }

		   if((charput == 'n')||(charput == 'N')) {
			   noResist = false;
		   } else {
			   noResist = true;
		   }
		} else {
			noResist = false;
		}
	   
		if((playerGen == 0)||((playerGen >= 2)&&(playerGen <= 5))){ //only ask if relevant, that being: if random (0), if within effect range (2-5, 1 is always crit)
			//ask for Seesaw modifier
				check = true;
			while(check == true){
			cout<<"Critical seesaw bonus: ";
			
			cin>>seesaw;

			if((cin.fail())||(intput < 0)){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a positive integer. "<<endl;
			} else {
				check = false;
				if(seesaw > 4)
				{
					seesaw = 5; //caps at 5
				} else {
					seesaw++;
				}
			}
		   }
		} else {
			seesaw = 1;
		}
	   
		//ask for Trap roll modifier if random calc
		if(playerGen == 0){
				check = true;
			while(check == true)
			{
			cout<<"Trap Roll modifier (0 if none, else enter as printed): ";
			
			cin>>mod;

			if(cin.fail()){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter an integer."<<endl;
			} else {
				check = false;
			}
	   }
   
	//generate Trap Roll
		die=DieN(20)+mod;
		
		if(disadvan){
			placeholder=DieN(20)+mod;
			
			if(placeholder < die){
				die = placeholder;
			}
			
			//catch modifier oddness
			if(die < 0){
				die = 1;
			} else if(die > 20){
				die = 20;
			}
		}
		
	 } else { //if player generated roll, replace die with that roll
		 die = playerGen;
	 }
 
		certainty = makeSure();
	}while(!certainty);
 
	cout<<"-";
	if(die <= seesaw) {
		cout<<"Critical Failure."<<endl<<endl;
		return 1;
	} else if(die <= 5){
		cout<<"Heavy Failure."<<endl<<endl;
		return 2;
	} else if((die <= 10)||((die <= 15)&&(noResist))||((die <= 19)&&(noResist)&&(noPartial))) {
		cout<<"Expected Failure."<<endl<<endl;
		return 3;
	} else if((die <= 15)||((die <= 19)&&(noPartial))) {
		cout<<"Resist."<<endl<<endl;
		return 4;
	} else if(die <= 19) {
		cout<<"Partial Success."<<endl<<endl;
		return 5;
	} else {
		cout<<"Full Success."<<endl<<endl;
		return 6;
	}
}



//Roller for per-Encounter checks; last update: 4.0 stackified
void Encounter(){
	int hypnoPen;
	int duty = 0;
	int plush; //due to how Plushification works, bool does not work
	int mBottle;
	int bottle;
	char use;
	bool babble;
	bool stumble;
	bool sleepy;
	bool squisher;
	bool gameBab;
	bool visor;
	string bottleType;
	
	stackLv++; //increment stack level
	
	if(stackLv > 1){ //if passed to, set pass
		pass = true;
	} else {
		location = "Encounter effects";
	}
	
do{ //loop until generated for all players
	
	do{ //repeat until certain of inputs
		if(pass){ //if passed to by another function, such as IdleRoll or end of an encounter
			cout<<endl;
		} else { //if on its own
			system("cls");
		}
		
		cout<<"Welcome to the Encounter effect roller!"<<endl<<endl;
	
		if(!allTrue){ //if not allTrue, ask questions
				check = true;
			while(check == true)
			{
			cout<<"What is the Hypnosis Check penalty? (0 if no Hypno Visor): ";
			
			cin>>hypnoPen;

			if((cin.fail())||(hypnoPen < -5)||(hypnoPen > 20)){
				cin.clear();
				cin.ignore();
				hypnoPen = -1;
				cout<<"Please enter a proper input between -5 and 20. "<<endl;
			} else {
				check = false;
			}
		   }
		   
			check = true;
			while(check == true)
			{
			cout<<"And what is the Duty modifier (0 if no Plush ally): ";
			
			cin>>duty;

			if((cin.fail())||(duty < -20)||(duty > 10)){
				cin.clear();
				cin.ignore();
				duty = -1;
				cout<<"Please enter a proper input."<<endl;
			} else {
				check = false;
			}
		   }
		   
		   check = true;
			while(check == true)
			{
			cout<<"Did the player use their diaper this Encounter (y/n)?: ";
			
			cin>>use;

			if((cin.fail())||((use != 'y')&&(use != 'Y')&&(use != 'n')&&(use != 'N'))){
				cin.clear();
				cin.ignore();
				use = 'q';
				cout<<"Please enter only y or n."<<endl;
			} else {
				check = false;
			}
		   }
		   
		   certainty = makeSure();
		   
		} else { //if allTrue, no need to prompt- skip and fill in
			hypnoPen = 20;
			duty = -20;
			use = 'y';
			certainty = true;
		}
	}while(!certainty);
	
	if(pass){
		cout<<"Press [Enter] to clear the screen and receive Encounter effects.";
		getch();
	}
	
//calculations

	//squisher check
	if(DieN(20) > 10){
		squisher = true;
	} else {
		squisher = false;
	}
	
	
	//Magic Bottle generator
	mBottle = DieN(7);
	switch(mBottle){
		case 1:
			bottleType = "Vanilla";
		break;
		
		case 2:
			bottleType = "Powder";
		break;
			
		case 3:
			bottleType = "Fruity";
		break;
		
		case 4:
			bottleType = "Fill";
		break;
		
		case 5:
			bottleType = "Silly";
		break;
		
		case 6:
			bottleType = "Sleepy";
		break;
		
		case 7:
			bottleType = "Water";
		break;
		
		default:
			bottleType = "ERROR";
	}
	
	
	//Babbler check
	if(DieN(100) > 50){
		babble = true;
	} else {
		babble = false;
	}
	
	
	//Clumsy check
	if(DieN(100) <= 30){
		stumble = true;
	} else {
		stumble = false;
	}
	
	
	//Sleepy check
	if(DieN(20) > 10){
		sleepy = true;
	} else {
		sleepy = false;
	}
	
	//Plushification Duty check
	plush = DieN(20) + duty;
	
	
	//Bottle drink amount
	bottle = DieN(4);
	bottle+= 2;
	
	
	//Hypno visor roll
	if(DieN(20) + hypnoPen >= 16){
		visor = true;
	} else {
		visor = false;
	}
	
	
	//GameBab check
	if(DieN(20) > 10){
		gameBab = true;
	} else {
		gameBab = false;
	}
	
	if(allTrue){
		squisher = true;
		// mBottle has no need to change
		babble = true;
		stumble = true;
		sleepy = true;
		plush = 20;
		//bottle has no need to change
		visor = true;
		gameBab = true;
		
	}
	
	system("cls");
	
	//constants
	cout<<"Constants-"<<endl;
	cout<<"Auto-Bottle: player drinks "<<bottle<<" WHP, if applicable."<<endl; //last updated 4.0
	cout<<"Magic Bottle: rolled a "<<bottleType<<" Bomb."<<endl; //last updated 4.0
	BombPoof(mBottle);
	cout<<"Mantra: party gains 7 RB (activates once per player with Mantra)."<<endl<<endl; //last updated 4.0
	cout<<"Pleasure Pamps: 3 RB and also Distracted if diaper is used."<<endl; //last updated 4.0
	cout<<"Vocab Regression: 3 RB if player speaks."<<endl; //last updated 4.0
	cout<<"Yipper: inflicts 3 RB, or 5 RB if Partial/Full Doggo."<<endl; //last updated 4.0
	cout<<"Worrywart: +1 RB for every missed attack, spell, or ReAct failure."<<endl<<endl; //last updated 4.0
	
	
	//if diaper use
	cout<<"Diaper use-"<<endl;
	if((use == 'y')||(use == 'Y')){
		cout<<"Announcer: 5 RB to the party as player announces their use."<<endl; //last updated 4.0
		cout<<"Boss-theme (Use) diapers: inflicts UHP +5 for diaper use not triggered by this effect."<<endl; //last updated 4.0
		cout<<"Insulating Diaper: gain 3 RB from diaper warmth."<<endl<<endl; //last updated 4.0
		cout<<"Inspiration: 7 WHP to other players if they wet and/or 7 MHP if they mess."<<endl; //last updated 4.0
		cout<<"Pottypants: 5 WHP to other players if they wet and/or 5 MHP if they mess."<<endl; //last updated 4.0
		cout<<"A reminder that Pottypants and Inspiration do not stack, and do not trigger from these effects!"<<endl; //last updated 4.0
		cout<<"Rewarding Diapers: 10 RB as a reward for diaper use."<<endl; //last updated 4.0
		cout<<"Unaware: gain 5 RB from use surprise."<<endl; //last updated 4.0
	} else { //if no use
		cout<<"Unaware: gain 2 RB from checking."<<endl; //last updated 4.0
	}
	cout<<endl;
	
	//chance occurences
	cout<<"Chance-"<<endl;

	if(babble){ //last updated 4.0
		cout<<"Babbler: Player is compulsed to babble. 3 RB gained."<<endl;
	} else if(debug){
		cout<<"Babbler does not procc."<<endl;
	}
	
	if(stumble){ //last updated 4.0
		cout<<"Clumsy/Booties: Player stumbles and falls onto their butt. Inflicts ";
		
		if((use == 'y')||(use == 'Y')){
			cout<<"10 RB (diaper is used)."<<endl;
		} else {
			cout<<"5 RB, or 10 RB if their diaper is used or Crinkly."<<endl;
		}
	} else if(debug){
		cout<<"Clumsy/Booties does not procc."<<endl;
	}
	
	if(gameBab){ //last updated 4.0
		cout<<"GameBab: Player gains Distracted."<<endl;
	} else if(debug){
		cout<<"GameBab does not procc."<<endl;
	}
	
	if(visor){ //last updated 4.0
		cout<<"Hypno Visor/Suit: player fails the Hypnosis Check and gains Flimsy Logic."<<endl;
	} else if(debug){
		cout<<"Hypno Visor effects do not procc."<<endl;
	}
	
	if((babble+stumble+gameBab+visor > 1)||(debug)){ //if crowded, make space
		cout<<endl;
	}	
	
	if(plush < 14){ //last updated 4.0
		cout<<"Plushification ally: if effect triggered, player snuggles ally for 5 RB (or 10 if they have Luxray Suit)."<<endl;
	} else if(plush < 17) {
		cout<<"Plushification and Luxray Suit ally: if effect triggered, player snuggles ally for 10 RB."<<endl;
	} else {
		cout<<"Player ignores any impulse to snuggle a Plushification ally."<<endl;
	}
	
	if(sleepy){ //last updated 4.0
		cout<<"Sleepy Diaper: Player gains a stack of Sleepy."<<endl;
	} else if(debug){
		cout<<"Sleepy Diaper does not procc."<<endl;
	}

	if((squisher)&&(!mother)){ //last updated 4.0
		cout<<"Squisher: If undefeated and diaper is used, player admires their diaper for 10 RB/EP."<<endl;
	} else {
		cout<<"Squisher: if Pleasure Pamps, undefeated and diaper is used, player admires their diaper for 10 RB/EP."<<endl;
	}

	if((!sleepy)&&(!stumble)&&(!babble)&&((!squisher)||(mother))&&(!visor)&&(!gameBab)&&(!debug)){
		cout<<"No other chance effects occur. How lucky!"<<endl;
	}
	
	
	if(pass){ //if passed to by a function
		perPlayer = false;
		pass = false;
		
		if(uniAdd){ //if universal addition is on
			//add future universal add prompt here
		}
	} else { //if on its own
		perPlayer = YesNo();
	}
	
}while(perPlayer);
	stackLv--; //decrement stack level
}



//calculate food/drink rolls; last update: 4.0 stackified
void foodDrink(){
	bool bottle = false;
	bool wetDiu;
	bool messLax;
	int bWHP;
	int food;
	int drink;
	int i;
	int damage;
	int laxdiu;
	perPlayer = true;
	
	stackLv++;
	
	if((foodAuto > 0)||(drinkAuto > 0)||(stackLv > 1)){
		pass = true;
		
		if((!foodAuto)&&(!drinkAuto)){	//catch empty inputs
			drinkAuto = 1;
		}
	} else {
		location = "food & drink";
	}
	
	 while(perPlayer == true){	//loop while player wishes
		if(pass != true){	//if menu, ask questions
				check = true;
			while(check){
				cout<<"Welcome to the food and drink calculator. Please select your food/drink source."<<endl;
				cout<<"0- Return to main menu"<<endl;
				cout<<"1- Lunchbox"<<endl;
				cout<<"2- Found after an Encounter"<<endl;
				cout<<"3- Kitchen Room attack"<<endl;
				cout<<"Input: ";
				
				cin>>intput;
				
				if((cin.fail())||(intput > 4)||(intput < 0)){
					cin.clear();
					cin.ignore();
					intput = -1;
					cout<<endl<<"Please enter a number between 0 and 4."<<endl<<endl;
					getch();
					system("cls");
				} else {
					check = false;
				}
			}
			
			switch(intput){
				//return to main menu
				case 0:
					return;
				
				//Lunchbox
				case 1:
					//system("cls");
					
					drink = DieN(10) / 5 + 1;
					food = DieN(10) / 5 + 1;
					
					//drink effect
					for(i = 0; i < drink; i++){
						drinking();
					}
					
					//food effect
					for(i = 0; i < food; i++){
						eating();
					}
					
					cout<<"Lunchbox contained "<<drink<<" containers of juice and "<<food<<" cups of applesauce."<<endl;
					cout<<"Player recovered "<<HPgain<<" HP."<<endl;
					cout<<"Player gains "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
				break;
				
				//Found after Encounter, basic food/drink generation
				case 2:
					do{ //repeat until user is certain
							check = true;
						while(check){
							cout<<"How many container(s) is this player drinking?: ";
							
							cin>>drink;
							
							if((cin.fail())||(drink > 4)||(drink < 0)){
								cin.clear();
								cin.ignore();
								drink = 0;
								cout<<"Please enter a number between 0 and 4."<<endl<<endl;
								getch();
							} else {
								check = false;
							}
						}
						
							check = true;
						while(check){
							cout<<"How many container(s) is this player eating?: ";
							
							cin>>food;
							
							if((cin.fail())||(food > 4)||(food < 0)){
								cin.clear();
								cin.ignore();
								food = 0;
								cout<<"Please enter a number between 0 and 4."<<endl<<endl;
								getch();
							} else {
								check = false;
							}
						}
						
						certainty = makeSure();
					}while(!certainty);
					
					//drink effects
					for(i = 0; i < drink; i++){
						drinking();
					}
					
					//food effects
					for(i = 0; i < food; i++){
						eating();
					}
					
					//auto-bottle generation
					if((drink)&&((allTrue)||(DieN(100) <= 25))){
						bottle = true;
						bWHP = DieN(7) + 11;
					}
					
					//check if diuretic/laxative
					if((drink)&&((DieN(20) > 10)||(allTrue))){
						wetDiu = true;
					}
					if((food)&&((DieN(20) > 10)||(allTrue))){
						messLax = true;
					}
					
					//results
					cout<<"Player recovered "<<HPgain<<" HP."<<endl;
					cout<<"Player gains "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
					if(bottle){
						cout<<"Player is inflicted Auto-Bottle with "<<bWHP<<" WHP inside."<<endl;
					}
					
					//laxative/diuretic checks
					if((messLax > 10)&&(wetDiu > 10)){
						cout<<"Food diaper: player gains both Diuretic and Laxative."<<endl;
					}
					if((food)&&(messLax > 10)){
						cout<<"Food diaper: player gains Laxative."<<endl;
					}
					if((drink)&&(wetDiu > 10)){
						cout<<"Food diaper: player gains Diuretic."<<endl;
					}
				break;
				
				//Kitchen-unique attack
				case 3:
					damage = DieN(4);
				
						check = true;
					while(check){
						cout<<"Did the Caretaker throw food or drink? 1 for drink, 2 for food."<<endl;
						cout<<"Input: ";
						
						cin>>intput;
						
						if((cin.fail())||(intput > 2)||(intput < 1)){
							cin.clear();
							cin.ignore();
							intput = 0;
							cout<<"Please enter 1 or 2."<<endl<<endl;
							getch();
						} else {
							check = false;
						}
					}
					
					//drink WHP
					if(intput == 1){
						drinking();
						
					//food MHP
					} else {
						eating();
					}
					
					
					if((allTrue)||(DieN(100) <= 30)){
						laxdiu = true;
					} else {
						laxdiu = false;
					}
					
					
					//output
					cout<<"Player takes "<<damage<<" damage";
					
					//if drink was thrown
					if(intput == 1){
						if(laxdiu){
							cout<<", gains"<<Pwet<<" WHP, and suffers Diuretic."<<endl;
						} else {
							cout<<" and gains "<<Pwet<<" WHP."<<endl;
						}
					//if food was thrown
					} else {
						if(laxdiu){
							cout<<", gains"<<Pmess<<" MHP, and suffers Laxative."<<endl;
						} else {
							cout<<" and gains "<<Pmess<<" MHP."<<endl;
						}
					}
				break;
				
				//error catching
				default:
				cout<<"ERROR"<<endl;
				break;
			}
		
		//Caretaker food/drink generation
		} else {
			cout<<"Generating Caretaker food calculations..."<<endl<<endl;
			
			//drink effect
			for(int i = 0; i < drinkAuto; i++){
				drinking();
			}
			
			//food effect
			for(int i = 0; i < foodAuto; i++){
				eating();
			}
			
			cout<<"Player gains "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
			cout<<"Press [Enter] to continue.";
			getch();
		}
	
	//does foodDrink() loop?
	if(pass == true){
		perPlayer = false;
		pass = false;
		foodAuto = 0;
		drinkAuto = 0;
		
	} else {
		perPlayer = YesNo();
		cout<<endl<<endl;
	}
	
	Pwet = 0;
	Pmess = 0;
	HPgain = 0;
	
	 } //end of perPlayer loop
}



//Forced Idle Roll calculator; last update: 4.0 stackified
void ForcedIR(){
	int rolls = 0;
	int tier = 0;
	int total;
	bool repeat = true;
	
	stackLv++;

while(repeat){

	if(stackLv > 1){ //if passed to by another function
		pass = true;
		if(!rollCount){	//if passed with no specifications, default to 1 roll
			rollCount = 1;
		}
	} else {
		location = "Forced Idle Roll";
	}
	
if(pass == false){
	cout<<"Welcome to the Forced Idle Roll calculator!"<<endl;
	cout<<"Notice: diaper use no longer rolls FIRs as of 4.0."<<endl;
	cout<<"Remember! When FIRs stack, the extra rolls are turned into Tiers (the calculator will do this for you)."<<endl<<endl;
   
} else {
	cout<<"Encounter has Forced Idle Roll(s).";
}

	do{ //repeat all questions until certain
	   check = true;
	   while(check == true)
	   {
		   if(mother){
			   cout<<" Please input player's ";
		   }
			cout<<"Idle Roll Tier: ";

		  cin>>tier;

		  if((cin.fail())||(tier < -1)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer at or above -1."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }
	   
	   if(!rollCount){ //only if we don't already know roll amount
		   check = true;
		   while(check == true)
		   {
			cout<<"Number of FIRs being made: ";

			  cin>>rolls;

			  if((cin.fail())||(rolls < 1)){
				cin.clear();
				 cin.ignore();
				 cout<<"Please enter a positive integer."<<endl<<endl;
			  } else {
				check = false;
			  }
		   }
	   }
	   
	   certainty = makeSure();
	}while(!certainty);
	
//calculate FIR
	if(rolls){ //take player input if given
		rollCount = rolls;
	}
	tier += FIRauto + (rollCount - 1);
	total = IRTcalc(0,tier);
   
//Print results
		cout<<endl<<endl<<"The "<<rolls<<" FIR(s) result in a total of "<<total<<" RB."<<endl;

	if(pass){
		rollCount = 0;
		FIRauto = 0;
		repeat = false;
		pass = false;
	} else {
		repeat = YesNo();
	}
	
	//sanitize for next repeat, if any
	tier = 0;
	rolls = 0;
	
} //end of repeat
	stackLv--; //decrement stack level
}



//Idle Roll calculator; last update: 4.0 stackified
void IdleRolls(){
	int wet;
	int mess;
	int HP;
	int UCtier = 0;
	int UCpen = 0;
	int penalty;
	int tier;
	int hypno; //used to calculate Hypnosis Checks
	int mTier = 0; //Mental RTier count
	int mp = 0;
	int bTier = 0; //Body RTier count
	int bp = 0;
	int eTier = 0; //Equipment RTier count
	int ep = 0;
	int uTier = 0; //Universal RTier count
	int removal; //roll against pacifier removal DC
	int rolls;
	int total = 0;
	int mimikyu; //Forced Idle roll
	int leon; //what Little Leon effect
	int luxray; //what Luxray-Leon effect
	int shinx; //due to how Shinx's odds work, bool does not work
	char leon2; //character for point category
	char luxray2; //character for point category
	bool UDD;
	bool pacifier;
	bool magic;
	bool change;
	bool assistant;
	bool tiara;
	bool necklace;
	bool little;
	bool luxio; //whether Shinx or Luxray
	bool luxio2; //if Luxio set bonus
	bool repeat = true;
	
	stackLv++; //increment stack level
	
	if((rollCount > 0)||(stackLv > 1)){ //rolls = rollCount appears later
		pass = true;
	} else {
		location = "Idle Roll(s)";
	}

while(repeat){
	
if(pass == false){
//receive input   
	system("cls");
	cout<<"Welcome to the Idle Roll calculator! Please enter..."<<endl<<endl;
} else {
	cout<<endl<<"Encounter has Idle Roll(s). Please enter..."<<endl<<endl;
}

	do{ //repeat all prompts until correct
	//ask wetness
	   check = true;
	   while(check == true)
	   {
		cout<<"Wet level: ";

		  cin>>wet;

		  if((cin.fail())||(wet < 0)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer of at least 0."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }

	//ask messiness
	   check = true;
	   while(check == true)
	   {
		cout<<"Mess level: ";

		  cin>>mess;

		  if((cin.fail())||(mess < 0)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer of at least 0."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }


	//ask IR penalties
		  check = true;
	   while(check == true)
	   {
		cout<<"Idle Roll Penalties: ";

		  cin>>penalty;

		  if(cin.fail()){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter a number."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }


	//ask roll count
	if(pass == false)
	{
	   check = true;
	   while(check == true)
	   {
		cout<<"Number of Idle Rolls being made: ";

		  cin>>rolls;

		  if((cin.fail())||(rolls < 1)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter a positive integer of at least 1."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }
	} else {
		rolls = rollCount;
	}

	//ask UC penalties
	   check = true;
	   while(check == true)
	   {
		cout<<"Use Check Penalties: ";

		  cin>>UCpen;

		  if((cin.fail())||(UCpen < 0)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter a positive integer."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }

	   
	//ask UC Tiers
	   check = true;
	   while(check == true)
	   {
		cout<<"Enter -1 for half value base UC roll. Use Check Tiers: ";

		  cin>>UCtier;

		  if((cin.fail())||(UCtier < -1)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer at or above -1."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }


	//ask Mental RTiers
		check = true;
	   while(check == true)
	   {
		cout<<"Mental RTiers (0 if none, -1 to set all RTiers to 0): ";

		  cin>>mTier;

		  if((cin.fail())||(mTier < -1)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer at or above -1."<<endl<<endl;
		  } else {
			check = false;
		  }
	   }

	if(mTier != -1){ //if all RTiers are 0
		//ask Body RTiers
			check = true;
		   while(check == true)
		   {
			cout<<"Body RTiers (0 if none): ";

			  cin>>bTier;

			  if((cin.fail())||(bTier < 0)){
				cin.clear();
				 cin.ignore();
				 cout<<"Please enter an integer at or above 0."<<endl<<endl;
			  } else {
				check = false;
			  }
		   }
		
		//ask Equipment RTiers
			check = true;
		   while(check == true)
		   {
			cout<<"Equipment RTiers (0 if none): ";

			  cin>>eTier;

			  if((cin.fail())||(eTier < 0)){
				cin.clear();
				 cin.ignore();
				 cout<<"Please enter an integer at or above 0."<<endl<<endl;
			  } else {
				check = false;
			  }
		   }
		   
		//ask Universal RTiers
			check = true;
		   while(check == true)
		   {
			cout<<"Universal RTiers (0 if none): ";

			  cin>>uTier;

			  if((cin.fail())||(eTier < 0)){
				cin.clear();
				 cin.ignore();
				 cout<<"Please enter an integer at or above 0."<<endl<<endl;
			  } else {
				check = false;
			  }
		   }
	} else { //if -1, give all variables value of 0 to skip
		mTier = 0;
		bTier = 0;
		eTier = 0;
		uTier = 0;
	}
	
	certainty = makeSure();
	
	}while(!certainty);

	//warn encounter generators of screen wipe
	if(pass){
		cout<<"Press [Enter] to clear the screen and receive Idle Roll outputs.";
		getch();
	}
	
   
//Process diaper use
	if(wet <= 8){ //if not Infinite Diaper
		switch(wet){
		  case 0:
		  //no penalty boost
		  break;

		  case 1:
		  penalty++;
		  break;
		  
		  case 2:
		  case 3:
		  penalty += 2;
		  break;

		  case 4:
		  case 5:
		  penalty += 3;
		  break;

		  case 6:
		  case 7:
		  penalty += 4;
		  break;
		  
		  case 8:
		  penalty += 5;
		  break;

		  default:
		  penalty += 0;
		  cout<<"ERROR. Negative wetness?"<<endl;
		  break;
	   }
	   
	} else { //if Infinite Diaper
		penalty += 5 + (wet - 8);
	}

	if(mess <= 8){ //if not Infinite Diaper
		switch(mess){
		  case 0:
		  //no penalty boost
		  break;

		  case 1:
		  case 2:
		  penalty++;
		  break;

		  case 3:
		  penalty += 2;
		  break;

		  case 4:
		  case 5:
		  penalty += 3;
		  break;
		  
		  case 6:
		  penalty += 4;
		  break;

		  case 7:
		  case 8:
		  penalty += 5;

		  default:
		  penalty += 0;
		  cout<<"ERROR. Negative mess level?"<<endl;
		  break;
	   }
	   
	} else { //if Infinite Diaper
		penalty += 5 + (mess - 8);
	}

	
//calculate Idle Roll
	tier = IRauto + mTier + bTier + eTier + uTier;
for(int i = 0; i < rolls; i++){
	total += IRTcalc(penalty, tier);
   
//calculate Use Checks
	UseCheck(UCpen, UCtier);

//calculate Point Idles
	if(mTier > 0){ mp += pointCalc(mTier);}
	if(bTier > 0){ bp += pointCalc(bTier);}
	if(eTier > 0){ ep += pointCalc(eTier);}
}

	//roll against removal DC
	removal = DieN(20);
	
//Status calculations
if(status == true){
	system("cls");

	//Magic pacifier check
	if(DieN(20) > 10) {
		magic = true;
	} else {
		magic = false;
	}

	//Assistant check
	if(DieN(100) <= 50){
		assistant = true;
	} else {
		assistant = false;
	}
	   
	   
	//HP calc
	HP=DieN(20)/10 + 1;
	

	//Teehee Tiara check
	if(DieN(20) > 10) {
		tiara = true;
	} else {
		tiara = false;
	}
	

	//Pacifier check
	if(DieN(20) > 10) {
		pacifier = true;
	} else {
		pacifier = false;
	}

	
	//Changeless check
	if(DieN(100) <= 50) {
		change = true;
	} else {
		change = false;
	}
	
	
	//Pacifier Necklace check
	if(DieN(20) > 10) {
		necklace = true;
	} else {
		necklace = false;
	}
	
	
	//UDD roll
	if(DieN(100) <= 50) {
		UDD = true;
	} else {
		UDD = false;
	}


	//Littling diaper check
	if(DieN(100) > 50){
		little = true;
	} else {
		little = false;
	}


	//Little Leon generation
   	leon=DieN(8);

	//Little Leon secondary generation
	if(leon == 4){	
		
		switch(DieN(3)){
			case 1: leon2 = 'M'; break;
			case 2: leon2 = 'B'; break;
			default: leon2 = 'E'; break;
		}
	}
	
	//Luxray Suit effect
	luxray=DieN(8);

	//Luxray Suit secondary generation
	if(luxray == 4){	
		
		switch(DieN(3)){
			case 1: luxray2 = 'M'; break;
			case 2: luxray2 = 'B'; break;
			default: luxray2 = 'E'; break;
		}
	}
	
	
	//Luxio Suit type
	if(DieN(100) > 50){
		luxio = true;
	} else {
		luxio = false;
	}
	
	//Luxio Suit set bonus
	if(DieN(100) > 50){
		luxio2 = true;
	} else {
		luxio2 = false;
	}
	
	//Mimikyu Plush
	mimikyu = IRTcalc(0,tier);
	
	//Shinx Suit
	shinx = DieN(100);
	
	//set allTrue
	if(allTrue){
		magic = true;
		assistant = true;
		//HP has no need to change
		tiara = true;
		pacifier = true;
		change = true;
		necklace = true;
		UDD = true;
		little = true;
		//leon has no need to change
		//luxray has no need to change
		luxio = true;
		luxio2 = true;
	}
	
	
} //end of statuses
   
//Print results

	//consistent values
	cout<<rolls<<" Idle Roll(s) for a total of "<<total<<" point(s)."<<endl;
	cout<<"Use Check(s) rolled a total of "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
	cout<<"Player regains "<<HP<<" HP, or "<<(HP + 2)<<" if they used a pacifier this Idle Roll."<<endl;
	cout<<"Removal DC: player may remove their pacifier if their DC is "<<removal<<" or below."<<endl;
	
	if(status){
		cout<<"Mimikyu Plush: player rolls "<<mimikyu<<" RB on their Forced Idle roll."<<endl; //last updated 4.0
	}

	if((status)&&((mp||bp||ep))){ cout<<endl; } //make space if cluttered
	
		//print Point Idles, if applicable
	if(mp||bp||ep){
		cout<<"Point Idles for ";
		
		if(mp){
			cout<<mp<<" MP";
		
			if(bp&&ep){ cout<<", "; //if followed by both
			} else if(bp||ep){ cout<<" and "; //if followed by one
			} else{ cout<<"."<<endl;} //if alone				
		}
		
		if(bp){
			cout<<bp<<" BP";
			
		if(ep){ cout<<", and "; //if all three print
		} else { cout<<"."<<endl;} //if alone
		}
		
		if(ep){
			cout<<ep<<" EP."<<endl;
		}
	} else {
		cout<<"No Point Idles."<<endl;
	}
	cout<<endl;
	
	//Pacifier Check
	if((magic)&&(pacifier)){
		cout<<"Pacifier Check: up to 2 Pacifier levels (normal + Magic Pacifier)."<<endl;
	} else if(pacifier) {
		cout<<"Pacifier Check: 1 Pacifier level from normal Pacifier Checks, if using a pacifier."<<endl;
	} else if(magic) {
		cout<<"Pacifier Check: 1 Pacifier level if they have Magic Pacifier."<<endl;
	} else {
		cout<<"Pacifier Check: no Pacifier levels."<<endl;
	}
	cout<<endl;

//print statuses	
if(status == true){
	cout<<endl<<"Status/Trait effects (if applicable, or any):"<<endl<<endl;	
	
	if(assistant){ //last updated 4.0
		cout<<"Assistant/Big Pal/Little Leon: Player gains Advocate for 1 IR, if they hadn't last Idle Roll."<<endl;
	} else if(debug){
		cout<<"Assistant effects do not procc."<<endl;
	}
	
	if(change){ //last updated 4.0
		cout<<"Changeless on ally: this player forgets how to change diapers for an Idle Roll."<<endl;
	} else if(debug){
		cout<<"Changeless does not procc."<<endl;
	}
	
	cout<<"Cupcake diaper: player gains a stack of GR: Baby Fat. This refreshes GR: Obese's timer."<<endl; //last updated 4.0
	
	if(little){ //last updated 4.0
		cout<<"Littling Diaper: Player gains stack of GR: Teenified."<<endl;
	} else if(debug){
		cout<<"Littling Diaper does not procc."<<endl;
	}
	
	if(((assistant+change+little > 0)&&(necklace+tiara > 0))||(debug)){ //if crowded, make space
		cout<<endl;
	}
	
	if(necklace){ //last updated 4.0
		cout<<"Pacifier Necklace: Player must use pacifier during next Idle Roll period."<<endl;
	} else if(debug){
		cout<<"Pacifier Necklace does not procc."<<endl;
	}
	
		//Shinx Suit effect, last updated 4.0
	cout<<"Shinx Suit: ";
	if(shinx < 50){ //if normal
		cout<<"player impulsively snuggles an ally and inflicts GR: Absentminded."<<endl;
	} else if(shinx <= 75){ //if only set bonus
		cout<<"if Themed Suit or Footie Pajamas, snuggle an ally and inflict GR: Absentminded."<<endl;
	} else { //if always miss
		cout<<"no impulsive snuggling this time."<<endl;
	}
	
	if(tiara){ //last updated 4.0
		cout<<"Teehee Tiara: Player gains Giggly for 1 IR."<<endl;
	} else if(debug){
		cout<<"Teehee Tiara does not procc."<<endl;
	}
	
	if(UDD){ //UD Desire, last updated 4.0
		cout<<"UD Desire or UD Addict: player denies a diaper change."<<endl;
	} else { //UD Addict
		cout<<"UD Addict only: player denies a diaper change."<<endl;
	}
	
	//if no Idle effects occur
	if(!(assistant)&&!(change)&&!(little)&&!(necklace)&&!(tiara)&&!(debug)) {
		cout<<"Player suffers no other Idle Roll specific effects. How lucky!"<<endl;
	}
	
	cout<<endl<<"Little Leon: Player ";
	switch(leon){ //last updated 4.0
		case 1:
			cout<<"is hugged, and has a 3-IR timer status increased by 1 IR."<<endl;
		break;
		
		case 2:
			cout<<"sees his hypno eyes, and rolls a Hypnosis Check."<<endl;
			
			hypno = DieN(20);
			cout<<"Player fails the HC only if their HC Penalty is "<<cout<<(16-hypno);
			
			if(16 - hypno > 0){ //if below 16 and likely won't succeed
				cout<<" or higher."<<endl;
			} else { //if above 15 and will likely succeed
				cout<<" or lower."<<endl;
			}
		break;
		
		case 3:
			cout<<"hears 'sound' advice, and gains 10 additional RB."<<endl;
		break;
		
		case 4:
			cout<<"hears his tail rattle, and gains 5 RB/"<<leon2<<"P."<<endl;
		break;
		
		case 5:
			cout<<"feels his comforting presence, and gains a Pacifier Level."<<endl;
		break;
		
		case 7:
			cout<<"has Leon insist on them being little, and receives -1 to all CR/ReAct in the next Caretaker ENC."<<endl;
		break;
		
		case 8:
			cout<<"compulsively snuggles Leon, inflicting Distracted."<<endl;
		break;
		
		default:
			cout<<"has their diaper checked, and rolls 1 Use Check."<<endl;
			
			cout<<"Would you like to roll that Use Check now? Enter 'y' for yes, otherwise it's no: ";
			cin>>charput;
			
			if((charput == 'y')||(charput == 'Y')){
				UCprompt();
			} else {
				cout<<endl;
			}
		break;
	}

	//Luxio suit effects, last updated 4.0
	cout<<"Luxio Suit: if random, player swings towards ";
	if(luxio){
		cout<<"Luxray";
	} else {
		cout<<"Shinx";
	}
	cout<<" Suit's effect";
	if(luxio2){
		cout<<"..."<<endl<<"...and those who receive this effect gain the player's diaper curse (if any)";
	}
	
	//Luxray Suit effects, last updated 4.0
	cout<<"."<<endl<<"Luxray Suit: allies ";
	switch(luxray){
		case 1:
			cout<<"are hugged, and has a 3-IR timer status increased by 1 IR."<<endl;
		break;
		
		case 2:
			cout<<"see player's hypno eyes, and rolls a Hypnosis Check."<<endl;
			
			hypno = DieN(20);
			cout<<"Player fails the HC only if their HC Penalty is "<<cout<<(16-hypno);
			
			if(16 - hypno > 0){ //if below 16 and likely won't succeed
				cout<<" or higher."<<endl;
			} else { //if above 15 and will likely succeed
				cout<<" or lower."<<endl;
			}
		break;
		
		case 3:
			cout<<"are told 'sound' advice, and gains 10 additional RB."<<endl;
		break;
		
		case 4:
			cout<<"hear player's tail rattle, and gains 5 RB/"<<luxray2<<"P."<<endl;
		break;
		
		case 5:
			cout<<"feel player's comforting presence, and gains a Pacifier Level."<<endl;
		break;
		
		case 7:
			cout<<"has player insist on them being little, and receives -1 to all CR/ReAct in the next Caretaker ENC."<<endl;
		break;
		
		case 8:
			cout<<"compulsively snuggle player, inflicting Distracted."<<endl;
		break;
		
		default:
			cout<<"have their diaper checked, and roll 1 Use Check."<<endl;
			
			cout<<"Would you like to roll that Use Check now? (y/n)?: ";
			cin>>charput;
			
			if((charput == 'y')||(charput == 'Y')){
				UCprompt();
			} else {
				cout<<endl;
			}
		break;
	}

} //end of status print

	if(uniAdd){ //if universal addition is on, do the "free" stuff
		uniRB += total;
		uniMHP += Pmess;
		uniWHP += Pwet;
		uniMP += mp;
		uniBP += bp;
		uniEP += ep;
	}
	
	//sanitize variables for repeat, if any
	penalty = 0;
	Pwet = 0;
	Pmess = 0;
	UCtier = 0;
	total = 0;
	mp = 0;
	bp = 0;
	ep = 0;

	//ask if wishing to generate per-Encounter effects
	check = true;
   while(check == true){
	cout<<endl<<"Would you like to calculate per-Encounter effects next (y/n)?: ";

	  cin>>charput;

	  if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
		cin.clear();
		cin.ignore();
		charput = 'q';
		cout<<"Please enter a proper input. "<<endl;
	  } else {
		check = false;
	  }
   }
   
	if((charput == 'y')||(charput == 'Y')){
		Encounter();
	}
   

	if(pass){
		repeat = false;
		pass = false;
	} else {
		repeat = YesNo();
	}

} //end of per-player repeat

	//sanitize universals for future use
	IRauto = 0;
	rollCount = 0;
	stackLv--; //decrement stack level
}



//per Turn effects; last update: 4.0 stackified
void perTurn(){
//player-based turn effects
	bool stumble; //25% chance to stumble after attack
	int grip; // 25% chance to drop weapon, 50% if stacking Mittens and Bad Grip
	bool fiftyLoss; //50% chance of turn loss if Slowpoke Diaper or Carted Around
	bool twentyFiveLoss; //25% of missing this turn from pleasure, if Enjoyer/Pleasure Pamps and Vibrating Diaper or if Lazy
	
//mother-based turn effects
	bool paciCheck; //Pacifier Check for Comforting Prescence and Paci-Pop
	int HC;
	
//room-based effects
	int room; //determines trap room
	int potion; //determines potion effect in potions room
	int i;
	int duty; //base Duty player needs to avoid Lazy Aura
	
	location = "per-Turn effects";
	stackLv++; //increment stack level

	
	perPlayer = true;
while(perPlayer){
	system("cls");
	cout<<"Welcome to the per-turn results generator!"<<endl<<endl;
	
//ask for room #
	check = true;
	while(check == true){
	cout<<"Please enter Caretaker's Trap Artist room number (1 if empty, 0 if random, -1 to return to main menu): ";
	
	cin>>room;

	if((cin.fail())||(room < -1)){
		cin.clear();
		cin.ignore();
		cout<<"Please enter an integer between -1 and 12."<<endl;
	} else {
		check = false;
		
		if(room == -1){
			return;
		} else if(!room){
			room = DieN(13);
		}
	}
   }

//ask for Hypnosis Check
   check = true;
	while(check == true){
	cout<<"Please enter the combined Hypnosis Check penalty and Bonus: "<<endl;
	cin>>HC;

	if((cin.fail())||(HC < -5)||(HC > 20)){
		cin.clear();
		cin.ignore();
		cout<<"Please enter an integer between -5 and 20."<<endl;
	} else {
		check = false;
	}
   }
	
//calculate stuff
	//bad grip
	grip = DieN(100);
	
	//stumble
	if(DieN(100) <= 25){
		stumble = true;
	}
	
	//pacifier check
	if(DieN(20) > 10){
		paciCheck = true;
	}
	
	//base Duty roll
	duty = DieN(20);
	
	//potion room generator
	if(!debug){ //if normal setting, just generate
		potion = DieN(10);
	} else { //if debug, ask user
		cout<<"Please enter potion effect, from 0-11 (0 is random, no redos!): ";
		cin>>potion;
		
		if(cin.fail()){
			cin.clear();
			cin.ignore();
			cout<<"Incorrect input. Defaulting to random."<<endl;
			potion = 0;
		} else if((potion < 0)||(potion > 11)){
			cout<<"Defaulting to random."<<endl;
			potion = 0;
		}
	}
	
	if(allTrue){
		grip = 1;
		stumble = true;
		paciCheck = true;
		HC = 20;
		duty = 1;
	}
	
	
//print out results
	system("cls");
	cout<<"Per-turn results, from player sources:"<<endl<<endl;
	
	//Mittens/Bad Grip; last updated 4.0
	if(grip <= 50){ //if only with both
		cout<<"Mittens AND";
	} else if(grip <= 25){ //if with either active
		cout<<"Mittens OR";
	}
	if(grip <= 50){ //if either procc
		cout<<" Bad Grip: player drops weapon and gains 5 RB from embarrassment."<<endl;
	}
	
	if(stumble){ //last updated 4.0
		cout<<"Clumsy/Booties: player trips onto their butt after attacking, gaining 5 RB if clean or 10 RB if used/Crinkly."<<endl;
	} else if(debug){
		cout<<"Clumsy/Booties does not procc."<<endl;
	}
	
	if(fiftyLoss){ //last updated 4.0
		cout<<"Carted Around/Slowpoke diaper: player loses this turn."<<endl;
	} else if(debug){
		cout<<"Carted Around/Slowpoke diaper does not procc."<<endl;
	}
		
	if(twentyFiveLoss){ //last updated 4.0
		cout<<"Enjoyer/Pleasure Pamps with Vibrating diaper OR Lazy: player loses this turn."<<endl;
	} else if(debug){
		cout<<"Enjoyer/Pleasure Pamps with Vibrating diaper OR Lazy does not procc."<<endl;
	}
	
	if((!debug)&&(grip > 50)&&(!stumble)&&(!fiftyLoss)&&(!twentyFiveLoss)){
		cout<<"No player sourced effects occur. How lucky!"<<endl;
	}
	
cout<<endl<<endl<<"From Caretaker Auras:"<<endl<<endl;
	cout<<"Pollen: player takes 2 non-lethal damage."<<endl; //last updated 4.0
	cout<<"Authority: player gains 2 Buildup."<<endl;
	
	if(paciCheck){ //last updated 4.0
		cout<<"Comforting Presence/Paci-Pop: player gained a Pacifier Level."<<endl;
	} else if(debug){
		cout<<"Avoided Aroma/Paci-Pop."<<endl;
	}
	
	if((HC == 20)||(DieN(20)+HC > 15)){ //last updated 4.0
		cout<<"Aroma: Caretaker succeeds their Hypnosis Check."<<endl;
	} else {
		cout<<"Aroma: Player resists the Hypnosis Check and gains +1 Hypnosis Check bonus."<<endl;
	}
	
	if(duty == 20){ //last updated 4.0
		cout<<"Player avoids Lazy Aura's Hypnotized/Lazy (crit success!)."<<endl;
	}else if(duty >= 13){
		cout<<"Player must have at least "<<(duty-13)<<" Duty to avoid Lazy Aura's Hypnotized/Lazy."<<endl;
	}else if(duty == 1){
		cout<<"Player gains Lazy Aura's Hypnotized/Lazy (crit fail!)."<<endl;
	} else {
		cout<<"Player must have -"<<(13-duty)<<" or worse to gain Lazy Aura's Hypnotized/Lazy."<<endl;
	}
	
	
//Trap Artist room effects
cout<<endl;
	
	switch(room){
		//Trap Rooms with no per-turn effects; last updated 4.0
		case 1: //Empty Room
		case 6: //Girly Room
		case 8: //Kitchen Room
		case 9: //Plant Room
		case 12: //Timed Room
			cout<<"Trap Artist Room #"<<room<<" has no per-turn effects to calculate."<<endl<<endl;
		break;
		
		
		//Bouncy Room; last updated 4.0
		case 2:
			cout<<"In the Bouncy Room, if a player is trapped in a bouncy chair, they must roll DC14 with Adeptness or Skill to escape, else lose their turn."<<endl;
			
			if((allTrue)||(DieN(100) <= 25)){
				cout<<"Player gains GR: Used Diaper Desire if their diaper is used."<<endl;
			} else {
				cout<<"Player avoided the stack of GR: Used Diaper Desire even if their diaper was used."<<endl;
			}
			cout<<endl;
		break;
		
		
		//Dining Room; last updated 4.0
		case 3:
			cout<<"Dining Room: if the player is restrained in a chair, they roll a UC with +1 penalty (this penalty is automatically added to this calculation)."<<endl;
			cout<<"If this is a 3rd turn of being restrained in a row, player gains 1 stack of Enhancement."<<endl;
		   
			rollCount = 1;
			UCpen = 1;
			UCprompt();
		break;
		
		
		//Dusty Room; last updated 4.0
		case 4:
			cout<<"Dusty Room: forces a Use Check at half value."<<endl;
			cout<<"If diaper is used this turn, player loses their turn (cannot activate consecutively)."<<endl;
			
			rollCount = 1;
			aura = true;
			UCprompt();
		break;
		
		
		//Galaxy Room; last updated 4.0
		case 5:
			cout<<"In the Galaxy Room, the player must roll an HC with +3 bonus (auto-added) every turn,"<<endl;
			cout<<"which inflicts a stack of Hypnotized without its effects. On 3 stacks, player gains Empty-Headed rather than Brainwashed."<<endl;
			
			if((HC == 20)||(DieN(20)+HC+3 > 15)){
				cout<<"The room succeeds in inflicting phantom Hypnotized."<<endl;
			} else {
				cout<<"The room fails the Hypnosis Check."<<endl;
			}
			cout<<endl;
		break;
		
		
		//Potions Room; last updated 4.0
		case 10:
			cout<<"Potions Room: if the player rolls a 1-5 on their attack, they knock a potion over that"<<endl;
			
			switch(potion){
				case 1:
					cout<<"has player gain a stack of Enhancement."<<endl;
				break;
				
				case 2:
					cout<<"has player recover "<<DieN(4)<<" HP."<<endl;
				break;
				
				case 3:
					cout<<"triggers a Vanilla Bomb."<<endl;
					
					BombPoof(1);		
				break;
				
				case 4:
					cout<<"triggers a Powder Bomb."<<endl;
					
					BombPoof(2);
				break;
				
				case 5:
					cout<<"triggers a Fruity Bomb."<<endl;
					
					BombPoof(3);
				break;
				
				case 6:
					cout<<"triggers a Fill Bomb."<<endl;
					
					BombPoof(4);
				break;
				
				case 7:
					cout<<"triggers a Silly Bomb."<<endl;
					
					BombPoof(5);
				break;
				
				case 8:
					cout<<"triggers a Sleepy Bomb."<<endl;
					
					BombPoof(6);
				break;
				
				case 9:
					cout<<"inflicts Pacified."<<endl;
				break;
				
				case 10:
					cout<<"inflicts a stack of Helplessness as a glass shatters."<<endl;
				break;
				
				case 11:
					cout<<"has player's diaper alter into a Potion diaper."<<endl;
				break;
				
				default:
				cout<<"ERROR on potion room calculation"<<endl;
				break;
			}
			
			cout<<endl;
		break;
		
		//Sparkling Room; last updated 4.0
		case 11:
			cout<<"The Sparkling Room makes the player roll a Hypnosis Check with a +1 bonus gained if failing."<<endl;
			
			if((HC == 20)||(DieN(20)+HC > 15)){
				cout<<"The room succeeds in Hypnotizing the player."<<endl;
			} else {
				cout<<"The room fails but inflicts +1 bonus."<<endl;
			}
		break;
		
		//Tingly Room; last updated 4.0
		case 13:
			cout<<"In the Tingly Room, rolling 1-5 Attack Roll causes the player to drop their weapon."<<endl;
		break;
		
		
		default:
			cout<<"ERROR WITH TRAP ARTIST ROOMS."<<endl;
		break;
	}   
   
   //ask for Use Aura roll
		check = true;
   while(check == true){
   	cout<<endl<<"Do you want to roll for Use Aura values? Ex. Wet Aura. (y/n, h for Hyper Aura): ";
   	cin>>charput;

   	if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N')&&(charput != 'h')&&(charput != 'H'))){
   		cin.clear();
   		cin.ignore();
   		charput = 'q';
   		cout<<"Please enter a proper input. "<<endl;
   	} else {
   		check = false;
   	}
   }

   if((charput == 'y')||(charput == 'Y')) {
		rollCount = 1;
		aura = true;
		UCprompt();
   } else if((charput == 'h')||(charput == 'H')){
		rollCount = 1;
		UCprompt();
   }
	
	perPlayer = YesNo();
}
	stackLv--; //decrement stack level
}



//Prompts for UC input from player; last update: 4.0 stackified
void UCprompt(){
	int temp = 0;
	
	stackLv++;
	
	if((rollCount)||(rollUntil)||(stackLv > 1)){ //if given to, or stacking function, set pass
		pass = true;
	}
	
	if(!pass){ //if not passed to by another function
		location = "Use Check";
	} else if((!rollCount)&&(!rollUntil)){ //if passed to with no rollCount, default to 1
		rollCount = 1;
	}
	
if((rollCount != 0)||(rollUntil)){ //if an Encounter
	cout<<endl<<"Encounter has Use Check(s). Please enter..."<<endl;
} else { //if menu generated
	cout<<"Welcome to the Use Check roller! Please enter..."<<endl;
}

	do{ //repeat until inputs correct
		check = true;
		while(check == true)
		{
		cout<<"Use Check Penalties: ";

		  cin>>intput;

		  if((cin.fail())||(intput < 0)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter a positive integer."<<endl<<endl;
		  } else {
			check = false;
		  }
		}

		check = true;
		while(check == true)
		{
		cout<<"Enter -1 for half value base UC roll. Use Check Tiers: ";

		  cin>>intput2;

		  if((cin.fail())||(intput2 < -1)){
			cin.clear();
			 cin.ignore();
			 cout<<"Please enter an integer at or above -1."<<endl<<endl;
		  } else {
			check = false;
		  }
		}

		if((rollUntil != true)&&(rollCount == 0)){
			check = true;
			while(check == true)
			{
			cout<<"Number of Use Checks at once: ";

			  cin>>temp;

			  if((cin.fail())||(temp < 1)){
				cin.clear();
				 cin.ignore();
				 cout<<"Please enter an integer at or above 1."<<endl<<endl;
			  } else {
				check = false;
			  }
			}
		}

		certainty = makeSure();
	}while(!certainty);
	
	if(temp){ //if hand-given roll count, move variable
		rollCount = temp;
	}

if((rollUntil)||(rollCount > 1)){ //if multi-rolling, do special condition
	cout<<endl;
	
	while(rollUntil){ //if rollUntil, repeat rolling until user asks to stop
		
		if(rollCount){ //prevent running multi-roller after this
			rollCount = 0;
		}
		
		UseCheck(intput, intput2);	
		cout<<"Total so far: "<<Pwet<<" WHP, "<<Pmess<<" MHP."<<endl;
		
		check = true;
	   while(check == true) //ask for when to stop rollUntil
	   {
		cout<<"Continue rolling? (y/n): ";
		cin>>charput;

	if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter a proper input. "<<endl;
		} else {
			check = false;
		}
	   }

	   if((charput == 'n')||(charput == 'N')) { //if asked to stop rollUntil, stop
		rollUntil = false;
	   }	
	}
	
	int rollTemp = rollCount;
	while((rollTemp > 0)&&(rollCount > 1)){ //if multi-rolling, do so
		UseCheck(intput, intput2);
		rollTemp--;
	}
	
	if(rollCount > 1){
		cout<<rollCount<<" Use Checks for a total of "<<Pwet<<" WHP and "<<Pmess<<" MHP."<<endl;
		rollCount = 0;
	}
	
} else { //if only one roll, do this
	UseCheck(intput, intput2);
	
	if(aura){ //if inflicted by an aura
		Pwet *= 0.5;
		Pmess *= 0.5;
	}
	cout<<endl<<"Use Check for values of "<<Pwet<<" WHP and "<<Pmess<<" MHP.";
	
	if(aura){
		cout<<" This roll was halved.";
	}
	
	cout<<endl<<endl;
	if(!pass){
		cout<<endl;
	}
	
} //end of rollers

	stackLv--; //decrement stack level
	aura = false;
	UCTauto = 0;
	Pwet = 0;
	Pmess = 0;
	intput = 0;
	intput2 = 0;
	rollCount = 0;
	rollUntil = false;
}


										//Minor roller menus

//bomb effect generation; last update: 4.0 stackified
void BombPoof(int style){
	int hit = 0;
	int damage = 0;
	string name ="Error";
	string effect = "X";
	string status = "";
	int points = 0;
	int Caretaker = 0; //if 1 then Super Bomber, if 2 Prima(Siren)
	int count = 1;
	perPlayer = true;
	
	stackLv++; //increment stack level
	
	if((style)||(stackLv > 1)){
		pass = true;
	}
	
	if(!pass){	//if not passed to by another function
		location = "bomb effect(s)";
	} else { //if passed to...
		if(style == 8){ //Ivysaur
			Caretaker = 1;
			hit = 1;
			style = 3;
		} else if(style == 9){ // Primarina
			Caretaker = 2;
			hit = 6;
			style = DieN(3);
			count = DieN(20)/10 + 1 + 1;
		} else if(style == 10){ //Lati(a/o)s
			Caretaker = 1;
			hit = 1;
			style = 8;
			count = 2;
		} else { //default to hit of 10 otherwise
			hit = 10;
		}
	}
	if(debug) { //if debug, print the current stack Level
		cout<<"Stack level: "<<stackLv<<endl;
	}
	
do{ //repeat for every player if generated by menu
	
	while((style == 0)||(!certainty)){ //repeat questions until correct, or skip if passed into
			check = true;
		while(check == true){
			system("cls");
			cout<<"Welcome to the bomb calculator. Please enter bomb style."<<endl;
			cout<<"0- Return to main menu."<<endl;
			cout<<"1- Vanilla"<<endl<<"2- Powder"<<endl<<"3- Fruity"<<endl<<"4- Fill"<<endl<<"5- Silly"<<endl<<"6- Sleepy"<<endl<<"7- Water"<<endl;
			cout<<"Input: ";
			
			cin>>style;

			if((cin.fail())||(style < -1)||(style > 7)){
				cin.clear();
				cin.ignore();
				style = 0;
				cout<<"Please enter an integer between 0 and 7."<<endl<<endl;
				getch();
				system("cls");
			} else {
				check = false;
				
				if(style == 0){ //if back out to main menu, leave
					return;
				}
			}
		}
		
			check = true;
		while(check == true){
			cout<<"If the roll is random, enter 0. Else, enter the bomb roll: ";
			
			cin>>hit;

			if((cin.fail())||(hit < 0)||(hit > 20)){
				cin.clear();
				cin.ignore();
				hit = 0;
				cout<<"Please enter an integer between 0 and 20"<<endl<<endl;
				getch();
			} else {
				check = false;
			}
		}
		
			check = true;
		while((check == true)&&(debug)){ //debugger prompt for bombArtist
			cout<<"Toggle bombArtist (y/n)?: ";
			
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = ' ';
				cout<<"That is not a valid input."<<endl<<endl;
				getch();
				system("cls");
			} else {
				check = false;
			}
		}
		
	certainty = makeSure();
	}
	   
	if(hit == 0){ hit = DieN(20); }
	
	if(!pass){
		cout<<"-----"<<endl;
	}
	
	switch(style){ //give name of bomb based upon input
		//stat bombs
		case 1:
		name = "Vanilla";
		effect = "M";
		break;
		
		case 2:
		name = "Powder";
		effect = "E";
		break;
		
		case 3:
		name = "Fruity";
		effect = "B";
		break;
		
		//special case
		case 4:
		name = "Fill";
		effect = "UH";
		break;
		
		//effect bombs
		case 5:
		name = "Silly";
		break;
		
		case 6:
		name = "Sleepy";
		break;
		
		case 7:
		name = "Water";
		effect = "WH";
		break;
		
		//unique
		case 8:
		name = "Super Bomber Stat";
		effect = "MP/BP/E";
		style = 1;
		break;
		
		default:
		name = "Error";
		effect = "X";
		break;
	}
	
	//print out effect(s)
	cout<<name<<" bomb goes off for ";
	
	if((hit >= 16)||((hit >= 11)&&(style >= 5))){ //if a true miss, or miss on a status bomb
		cout<<"no effect (avoided)";
		
	} else if((hit >= 11)&&(style <= 3)){ //if a weak hit on stat bomb
		points = DieN(7) + 1;
		damage = DieN(3);
		
		if(debug){
			cout<<"weak hit on stat bomb with ";
		}
		
	} else if((hit >= 11)&&(style == 4)){ //if a weak hit on Fill bomb
		points = DieN(4) + 1;
		damage = DieN(4) + 2;
		status = "Holding Help";
		
		if(debug){
			cout<<"weak hit on Fill bomb with ";
		}
		
	} else if((hit >= 7)&&(style == 4)){ //if a medium hit on Fill bomb
		points = DieN(6) + 2;
		damage = DieN(4) + 2;
		status = "Holding Help";
		
		if(debug){
			cout<<"medium hit on Fill bomb with ";
		}
		
	} else if((hit >= 7)&&(style == 6)){ //if a weak hit on Silly bomb
		damage = DieN(3);
		status = "Sleepy";
		
		if(debug){
			cout<<"weak hit on Silly bomb with ";
		}
		
	} else if((hit < 11)&&(style <= 3)){ //if a strong hit on stat bomb
		for(int i = 0; i < count; i++){ //repeat multiple times if Primarina or Lati(a/os)
			points += DieN(8) + 4;
			damage += DieN(4) + 2;
		}
		
		if(debug){
			cout<<"stronger hit on stat bomb with ";
		}
		
	} else if((hit >= 4)||((bombArtist)&&(style != 4))){ //if weak hit on effect bomb, or strong hit on Fill Bomb, or if critical bombs not allowed
		damage = DieN(3);
		switch(style){
			case 4:
				points = DieN(8) + 4;
				damage = DieN(4) + 2;
				status = "Holding Help";
			break;
			case 5:
				status = "Silly";
			break;
			case 6:
				status = "Sleepier";
			break;
			case 7:
				points = DieN(5) + 5;
			break;
			default:
				cout<<"Error on weak hit of effect bomb!"<<endl;
			break;
		}
		
		if(debug){
			cout<<"weak hit on effect bomb, strong hit on Fill bomb, or bombArtist is active with ";
		}
		
	} else { //if a strong hit on effect bomb, or critical on Fill Bomb
		damage = DieN(3);
		switch(style){
			case 4:
				points = 15;
				status = "Holding Help";
			break;
			case 5:
				status = "Sillier";
			break;
			case 6:
				status = "Sleepiest";
			break;
			case 7:
				points = 0;
				status = "Bogged";
			break;
			default:
				cout<<"Error on strong hit of effect bomb!"<<endl;
			break;
		}
		
		if(debug){
			cout<<"Default: effect bomb with crit effects"<<endl;
		}
	}
	
	if(damage){ //if a player has taken damage, a bomb has hit- so print it out
		if(style <= 4){
			if(Caretaker == 1){ //if Lati(a/o)s or Ivysaur, double effects
				cout<<((points/2)*2)<<" RB and "<<(2*points)<<" "<<effect<<"P."<<endl;
				
			} else { //if not Lati(a/o)s
				cout<<(points/2)<<" RB, "<<points<<" "<<effect<<"P"; 
				
				if(Caretaker == 0){ //if not a Caretaker's roll
					cout<<", and "<<damage<<" damage."<<endl;
					cout<<"Super Bomber: "<<points<<" RB and "<<(points*2)<<" "<<effect<<"P instead."<<endl;
				} else {
					cout<<"."<<endl;
				}
			}
		} else if(style == 7){ //if Water bomb, give unique prompt
			cout<<points<<" "<<effect<<"P and "<<damage<<" damage ("<<(damage+2)<<" damage if Super Bomber).";
			
		} else { //if any effect bomb
			cout<<damage<<" damage ("<<(damage+2)<<" damage if Super Bomber).";
		}
		
		if(style == 2){ //if inflicting a Use Check, prompt for it
			cout<<"The player rolls ";
			
			if(count > 1){ //adjust if Prima(Siren) multiroll
				cout<<count<<" Use Checks."<<endl;
			} else {
				cout<<"a Use Check."<<endl;
			}
			
			rollCount = count;
			UCprompt();
			
		} else if(status != ""){
			cout<<" The player gains "<<status<<".";
		}
		
		cout<<endl;
	}
	
	if(pass){ //if passed to, ignore repeat prompt
		perPlayer = false;
		pass = false;
	} else {
		perPlayer = YesNo();
		damage = 0;
	}

	//purge variables
	hit = 0;
	points = 0;
	style = 0;	
	status = "";
	Caretaker = 0;
	count = 1;
	bombArtist = false;
	
}while(perPlayer);
	stackLv--; //decrement stack level
}



//Diaper Roller; last update: 4.0
void DiaperRoller(int type){
	stackLv++; //increment stack level
	
	if((stackLv > 1)||(type)){ //if called in another function, give post-Encounter item(s)
		pass = true;
	} else { //if at head menu, give normal
		location = "random diaper"; //just in case for now
	}
	
	if(!pass){ //if a main menu call
		cout<<endl<<"No-Trap diaper of: "; FullDiaperPrint(DieN(diaperMax)); cout<<endl;
		cout<<"Potential Trap diaper of: "; FullDiaperPrint(DieN(trapDMax)); cout<<endl;
		if(DieN(100) <= 5){
			cout<<"(Un)lucky! Diaper Bag generates a Trap Diaper of: "; FullDiaperPrint(DieN(trapDSpread + diaperMax)); cout<<endl;
		} else {
			cout<<"Diaper Bag generates a diaper of: "; FullDiaperPrint(DieN(diaperMax)); cout<<endl;
		}
		
	} else if(type == 1){ //if a random no-trap diaper
		FullDiaperPrint(DieN(diaperMax));
		
	} else if(type == 2){ //if a random trap diaper
		FullDiaperPrint(DieN(trapDSpread) + diaperMax);
		
	} else if(type == 3){ //if any random diaper
		FullDiaperPrint(DieN(trapDMax));
	
	} else {
		cout<<"Diaper roller ERROR"<<endl;
	}
	
	if(!pass){
		cout<<endl<<"Press [Enter] to return to main menu."<<endl;
		getch();
	}
	
	stackLv--;
}



//Calculate Hypnosis Check; last update: 4.0
void HypnoCheck(){
	int modifier;
	bool disadvan;
	int die;
	
	stackLv++; //increment stack level
	
	do{
	
	if(stackLv > 1){ //if passed to, set pass
		pass = true;
		cout<<endl<<"This Encounter has Hypnosis Check(s). Please enter..."<<endl;
		
		if(!HCauto){	//default to 1 roll if passed in
			HCauto = 1;
		}
	} else {
		cout<<"Welcome to the Hypnosis Check roller!"<<endl;
		location = "Hypnosis Check";
	}
	
	do{ //repeat prompts until certain
		
		if(!pass){ //if not passed to, ask for HC rolls
				check = true;
			while(check == true){
			cout<<"The number of HCs rolled in a row for one player: ";
			
			cin>>intput;

			if((cin.fail())||(intput < 1)){
				cin.clear();
				cin.ignore();
				intput = 0;
				cout<<"Please enter a positive integer."<<endl;
			} else {
				check = false;
				HCauto = intput;
			}
			}
		}
		
		check = true;
		while(check == true){
			cout<<"HC modifier: ";
			
			cin>>modifier;

			if(cin.fail()){
				cin.clear();
				cin.ignore();
				modifier = 0;
				cout<<"Please enter an integer."<<endl;
			} else {
				check = false;
			}
		}
		
		check = true;
	   while(check == true){
		cout<<"Does player roll HC at disadvantage (y/n)?: ";
		cin>>charput;

		if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter a proper input. "<<endl;
		} else {
			check = false;
		}
	   }

	   if((charput == 'n')||(charput == 'N')) {
			disadvan = false;
	   } else {
			disadvan = true;
	   }
   
		certainty = makeSure();
	}while(!certainty);
	
	//calculate
	die = DieN(20);
	
	if(disadvan){
		int temp = DieN(20);
		if(temp > die){
			die = temp;
		}
	}
	
	die += modifier;
	
	//print
	if(die > 15){
		if(!pass){ cout<<endl<<endl; }
		hypnoState = true;
		cout<<"Hypnosis Check failed, player gains a stack of Hypnotized."<<endl;
		cout<<"HC bonuses from previous HC rolls are reset."<<endl;
		
	} else {
		hypnoState = false;
		cout<<"Hypnosis Check succeeded, no effect.";
		
		if(die > 5){
			cout<<" HC bonus increased by 2, unless special roll (then +1).";
		}
		cout<<endl;
	}
	
	if(!pass){
		perPlayer = YesNo();
		cout<<endl;
	} else {
		perPlayer = false;
	}
	
	}while(perPlayer);
	
	stackLv--; //decrement stack level
	pass = false; //reset pass for future
	HCauto = 0;
}



//Item Roller; last update: 4.0
void ItemRoller(int type){
	stackLv++; //increment stack level
	
	if((stackLv > 1)||(type)){ //if called in another function, give post-Encounter item(s)
		pass = true;
	} else { //if at head menu, give normal
		location = "random item"; //just in case for now
	}
	
	if(!pass){ //if a menu call	
		cout<<endl<<"Item generator!"<<endl;
		cout<<"No-curse item of: "; FullItemPrint(DieN(useItemMax)); cout<<endl;
		cout<<"Possibly cursed item of: "; FullItemPrint(DieN(itemMax)); cout<<endl;
		cout<<"Magic scroll of: "<<ScrollPrint(scroll())<<endl;
		
		
	} else if(type == 1){ //if to resources found after an Encounter
		for(int i = i; i < 4; i++){ //see if a specific player found an item
			if(DieN(20) >= 14){
				cout<<"Player "<<i<<" found "; FullItemPrint(DieN(itemMax)); cout<<endl;
				//maybe add a counter for party item-find limit?
			} else {
				cout<<"Player "<<i<<" did not find an item."<<endl;
			}
		}

	} else if(type == 2){ //if any one item
		FullItemPrint(DieN(itemMax));
		
	} else if(type == 3){ //if one non-cursed item
		FullItemPrint(DieN(useItemMax));
		
	} else if(type == 4){ //if one cursed item
		FullItemPrint(DieN(cursedSpread)+useItemMax);
		
	} else {
		cout<<"ERROR on ItemRoller call"<<endl;
	}
	
	if(!pass){
		cout<<endl<<"Press [Enter] to return to main menu."<<endl;
		getch();
	}
	
	stackLv--; //decrement stack level
}



//Ask for IT and calc Intensity roll; last update: 4.0
int Intensity(int base){
	int input;
	int points;

   check = true;
	while(check == true)
   {
		cout<<endl<<"Please input Intensity Tier bonus: ";
		cin>>input;

   	if((cin.fail())||(input < 0)){
   		cin.clear();
      	cin.ignore();
      	input = 0;
      	cout<<"Please enter a positive integer."<<endl<<endl;
      	getch();
   	} else {
   		check = false;
		cout<<endl;
   	}
   }

   points = (base + input)*10 + 5;

   return points;
}



//print total points and statuses gained from full calculations
void uniPrint(){ //not written
}


										//Number to name translators 

//Caretaker name print, last update: 4.0
string CarePrint(int val){
 	switch(val){
		//Bosses
       	case 1:
         return "Selected boss";
         

		//Tier 0 Caretakers
         case 2:
         return "Chansey";
         

         case 3:
         return "Crystal of Diapers";
         

         case 4:
         return "Crystal of Immaturity";
         

         case 5:
         return "Crystal of Laziness";
         

         case 6:
         return "Crystal of Littleness";
         

         case 7:
         return "Crystal of Sissiness";
         

         case 8:
         return "Crystal of Squishiness";
         

         case 9:
         return "Gardevoir";
         
		 
		 case 10:
		 return "Ivysaur";

         case 11:
         return "Kangaskhan";
         

         case 12:
         return "Leavanny";
         

         case 13:
         return "Mew";
         

         case 14:
         return "Ninetales";
         

         case 15:
         return "Serperior";
         

         case 16:
         return "Shaymin";
         

         case 17:
         return "Slowpoke";
         

         case 18:
         return "Smeargle";
         

         case 19:
         return "Togekiss";
         

         case 20:
         return "Victini";
         

		//Tier 1 Caretakers
         case 21:
         return "Alakazam (Pacifier)";
         

         case 22:
         return "Alakazam (Spoon)";
         

         case 23:
         return "Ampharos";
         

         case 24:
         return "Flygon";
         

         case 25:
         return "Galvantula";
         

         case 26:
         return "Goodra";
         

         case 27:
         return "Luxray";
         

         case 28:
         return "Milotic";
         

         case 29:
      	 return "Miltank";
         

         case 30:
         return "Primarina (Mischief)";
         

         case 31:
         return "PrimSiren";
         

         case 32:
         return "Snorlax";
         

         case 33:
         return "Ursaring";
         

         case 34:
         return "Wigglytuff";
         
		 
		 //Tier 2 Caretakers
		 case 35:
		 return "Audino";
		 
		 
		 case 36:
		 return "Delphox";
		 
		 
		 case 37:
		 return "Dragonite";
		 
		 
		 case 38:
		 return "Lapras";
		 
		 
		 case 39:
		 return "Lucario";
		 
		 
		 case 40:
		 return "Nidoqueen";
		 
		 
		 case 41:
		 return "Salazzle";
		 
		 
		 case 42:
		 return "Sylveon";
		 
		 
		 case 43:
		 return "Vaporeon";
		 

         default:
         cout<<"Please input a proper number from 1-43."<<endl;
         getch();
         break;
      }
	  
	  return "ERROR";
}



//Helper Diaper print print (haha); last update: 4.0
string DiaperPrint(int val){
	switch(val){
		//Normal Diapers
		case 1:
		return "Barrel";
		
		case 2:
		return "Books";
		
		case 3:
		return "Cactus";
		
		case 4:
		return "Confetti";
		
		case 5:
		return "Cutesy";
		
		case 6:
		return "Digging";
		
		case 7:
		return "Energy";
		
		case 8:
		return "Farm";
		
		case 9:
		return "Flower";
		
		case 10:
		return "Food";
		
		case 11:
		return "Ghost";
		
		case 12:
		return "Great Ball";
		
		case 13:
		return "Level Ball";
		
		case 14:
		return "Math";
		
		case 15:
		return "Mew";
		
		case 16:
		return "Overnight";
		
		case 17:
		return "Pacifier";
		
		case 18:
		return "Paws";
		
		case 19:
		return "Pink Frills";
		
		case 20:
		return "Plain";
		
		case 21:
		return "Pokeball";
		
		case 22:
		return "Potions";
		
		case 23:
		return "Pullups";
		
		case 24:
		return "Safari";
		
		case 25:
		return "Sleepy";
		
		case 26:
		return "Slowpoke";
		
		case 27:
		return "Sponge";
		
		case 28:
		return "Stealth";
		
		case 29:
		return "Swim";
		
		case 30:
		return "Swirls";
		
		case 31:
		return "Ultra Ball";
		
		case 32:
		return "Weapons";
		
		//Trap Diapers
		case 33:
		return "Baby";
		
		case 34:
		return "Boss-theme";
		
		case 35:
		return "Brain Drainer";
		
		case 36:
		return "Cupcake";
		
		case 37:
		return "EXP Bar";
		
		case 38:
		return "Green Gem";
		
		case 39:
		return "Luxury Ball";
		
		case 40:
		return "Magic Wind";
		
		case 41:
		return "Master Ball";
		
		case 42:
		return "Nest Ball";
		
		case 43:
		return "Progress Bar";
		
		case 44:
		return "Toy Blocks";
		
		case 45:
		return "Vibrating";
		
		case 46:
		return "Whispering";
		
		default:
		return "diaper ERROR";
	}
	
	return "ERROR";
}



//Prints out diaper and what trap diaper feigns as; last update: 4.0
void FullDiaperPrint(int val){
	if(val > trapDMax){
		cout<<"ERROR on full diaper print";
		return;
	}
	
	cout<<DiaperPrint(val);
	
	if(val >= diaperMax){ //if a cursed diaper, give a fake name
		cout<<" feigning as "<<DiaperPrint(DieN(diaperMax));
	}
}



//Event name print, last update: 4.0
string EventPrint(int val){
	switch(val){
		case 1:
		return "Diaper Dragon's Lair";
		
		case 2:
		return "Game Show";
		
		case 3:
		return "Greenhouse";
		
		case 4:
		return "Kecleon Shop";
		
		case 5:
		return "Lotus Gambling Hall";
		
		case 6:
		return "Monster House!";
		
		case 7:
		return "Padded Pride";
		
		default:
		return "switch ERROR";
	}
	
	return "ERROR";
}
	


//Helper item print, last update: 4.0
string ItemPrint(int val){
	switch(val){
		//Food
		case 1:
		return "Lum Berry";
		
		case 2:
		return "Lunchbox";
		
		case 3:
		return "Moomoo Milk";
		
		case 4:
		return "Persim Berry";
		
		case 5:
		return "Reviver Seed";
		
		case 6:
		return "Bounceback Berry";
		
		//Use Item
		case 7:
		return "Bandana";
		
		case 8:
		return "Bright Powder";
		
		case 9:
		return "Escape Orb";
		
		case 10:
		return "Expert Belt";
		
		case 11:
		return "Fidget Spinner";
		
		case 12:
		return "Focus Band";
		
		case 13:
		return "Geo Pebble";
		
		case 14:
		return "Macho Brace";
		
		case 15:
		return "Perfume";
		
		case 16:
		return "Quick Claw";
		
		case 17:
		return "Stress Ball";
		
		case 18:
		return "X-Ray Specs";
		
		//Cursed item
		case 19:
		return "Baby Powder";
		
		case 20:
		return "Berry Pacifier";
		
		case 21:
		return "Decoy Orb";
		
		case 22:
		return "Droopeye Seed";
		
		case 23:
		return "Filler";
		
		case 24:
		return "Gone Pebble feigning as Geo Pebble";
		
		case 25:
		return "Mittens";
		
		case 26:
		return "Reviser Seed feigning as Reviver Seed";
		
		case 27:
		return "Silly Seed";
		
		case 28:
		return "Slow Claw";
		
		case 29:
		return "Weaken Powder";
		
		case 30:
		return "Y-Ray Specs feigning as X-Ray Specs";

		default:
			return "item ERROR";
	}
	
	return "ERROR";
}



//Prints out item and what a cursed item feigns as; last update: 4.0
void FullItemPrint(int val){
	
	if(val > itemMax){ //if over max, print error
		cout<<"ERROR on FullItemPrint";
		return;
	}
	
	cout<<ItemPrint(val); //print base name
	
	switch(val){	//if cursed item with random fake name, print its fake name
		//Use Items
		case 19: //Baby Powder
		case 21: //Decoy Orb
		case 25: //Mittens
		case 28: //Slow Claw
		case 29: //Weaken Powder
			cout<<" feigning as "<<ItemPrint(DieN(useSpread)+foodItemMax);
		break;
		
		//Food Items
		case 20: //Berry Pacifier
		case 22: //Droopeye Seed
		case 23: //Filler
		case 27: //Silly Seed
			cout<<" feigning as "<<ItemPrint(DieN(foodItemMax));
		break;
	}
}



//roll for a jewelry status; last update: 4.0
void jewelry(int jewel){
	int die;
	const int HIGH = 9; //count of jewelry statuses

	
	if(jewel == 0){
		//calculate the random jewelry status
		die=DieN(HIGH);
	} else {
		die = jewel;
	}
	
	
	//print that status
	switch(die){
		case 1:
			cout<<"Pink Gem Necklace";
		break;
		
		case 2:
			cout<<"Brown Gem Bracer";
		break;
		
		case 3:
			cout<<"Green Gem Diaper";
		break;
		
		case 4:
			cout<<"GR: KidCare Crown";
		break;
		
		case 5:
			cout<<"GR: Teehee Tiara";
		break;
		
		case 6:
			cout<<"GR: Fancy Accessories";
		break;
		
		case 7:
			cout<<"GR: Friendship Bracelet";
		break;
		
		case 8:
			cout<<"GR: Pacifier Necklace";
		break;
		
		case 9:
			cout<<"GR: Resonant Jewels";
		break;
		
		default:
		cout<<"ERROR ON JEWELRY"<<endl;
	}
}



//Misfire print; last update: 4.0
string MisfirePrint(int val){
	switch(val){
		//Normal
		case 1:
			return "Backblast";
		case 2:
			return "Bottle'd";
		case 3:
			return "Brain Drain";
		case 4:
			return "Changeless";
		case 5:
			return "Clumsify";
		case 6:
			return "Demotify";
		case 7:
			return "Diaper Filler";
		case 8:
			return "Diaper Lovers";
		case 9:
			return "Diaper Traps";
		case 10:
			return "Disarm";
		case 11:
			return "Foreboding";
		case 12:
			return "Hyperbomb";
		case 13:
			return "Infantify";
		case 14:
			return "Intense Backfire";
		case 15:
			return "Magical Wind";
		case 16:
			return "Mind Wipe";
		case 17:
			return "Pacified";
		case 18:
			return "Poofify";
		case 19:
			return "Pottypants";
		case 20:
			return "Powder Explosion";
		case 21:
			return "Sillify";
		case 22:
			return "Sissify";
		case 23:
			return "Sting";
		case 24:
			return "Stupify";
		case 25:
			return "Suiting Up";
		case 26:
			return "Super Filler";
	
	//Caretakers Only
		case 27:
			return "Aura Boost";
		case 28:
			return "Bab Burst";
		case 29:
			return "Bolster";
		case 30:
			return "Full Restore";
		case 31:
			return "Mega Evolve";
			
		default:
			return "Misfire ERROR";
	}
}



//Generate a random Ghost Regression; last update: 4.0
void randomGR(int lock) {
	/*lock values:
	0- empty, prompts for inputs
	1- any random
	2- mental exclusive
	3- body or equipment exclusive
	4- body exclusive
	5- equipment exclusive
	*/
	
	int die;
	int input = 0;
	int HIGH;
	
	//futureproofing math variables, enter what is the last value on sheet
	const int maxMental = 15;
	const int maxBody = 28;
	const int maxEquip = 45;
	const int maxUni = 53;

	
if(lock == 0){
	cout<<"Welcome to the random Ghost Regression generator!"<<endl<<endl;
}

	do{ //ask all questions and repeat until certain
		if(lock == 0){
				check = true;
			while(check == true)
			{
				cout<<"0- return to main menu"<<endl;
				cout<<"1- any random"<<endl;
				cout<<"2- mental exclusive"<<endl;
				cout<<"3- body or equipment exclusive"<<endl;
				cout<<"4- body exclusive"<<endl;
				cout<<"5- equipment exclusive "<<endl;
				cout<<"Input: ";
			
			cin>>input;

			if((cin.fail())||(input < 0)||(input > 5)){
				cin.clear();
				cin.ignore();
				cout<<"Please enter a proper input between 0 and 5."<<endl;
			} else {
				check = false;
				
				//if return to main menu
				if(input == 0){
					return;
				}
			}
		   }
		}
	
	if(!lock){
		certainty = makeSure();
	} else {
		certainty = true;
	}
	
	}while(!certainty);

	if(lock == 0){lock = input;}
   
   
   switch(lock) {
	   case 0:
	   cout<<"ERROR. Lock trapped at 0 value for unknown reason."<<endl;
	   HIGH = 2;
	   break;
	   
	   case 1: //any random
	   //Low end unchanged
	   HIGH = maxUni;
	   break;
	   
	   case 2: //mental only
	   //Low end unchanged
	   HIGH = maxMental;
	   break;
	   
	   case 3: //body or equipment only
		//Find number of GRs between maxEquip and maxMental, later add maxMental to 'bump'
	   HIGH = maxEquip - maxMental;
	   break;
	   
	   case 4: //body only
		//Find number of GRs between maxBody and maxMental, later add maxMental to 'bump'
	   HIGH = maxBody - maxMental;
	   break;
	   
	   case 5: //equipment only
		//Find number of GRs between maxEquip and maxBody, later add maxBody to 'bump'
	   HIGH = maxEquip - maxBody;
	   break;
	   
	   case 6: //universal only
		cout<<"We are not doing universal-only randomGRs right now."<<endl;
		cout<<"Actually, how did you get here??"<<endl;
		HIGH = maxUni - maxEquip;
	   break;
	   
	   default:
	   HIGH = 2;
	   cout<<"ERROR. Unknown Lock value."<<endl;
   }

//roll for Ghost Regressions according to specifications
	die=DieN(HIGH);
	
	//bump up for those not starting at 1 as lowest
	switch(lock){
		case 1:
		case 2:
			//catch empty changes
		break;
		
		case 3:
		case 4:
			//start at Body Traits
			die += maxMental;
		break;
		
		case 5:
			//start at Equipment Traits
			die += maxBody;
		break;
		
		case 6:
			//not used but made
			die += maxEquip;
		break;
		
		default:
			cout<<"ERROR ON DIE BUMPING"<<endl;
	}
	
	cout<<"Player receives Ghost Regression #"<<die<<": ";
	
	switch(die) {
		//Mental
		case 1:
		cout<<"Baby Logic";
		break;
		
		case 2:
		cout<<"Changeless";
		break;
		
		case 3:
		cout<<"Comfortable or Normalization (stacks)";
		break;
		
		case 4:
		cout<<"Diaper Trained";
		break;
		
		case 5:
		cout<<"Enjoyer";
		break;
		
		case 6:
		cout<<"Giggly";
		break;
		
		case 7:
		cout<<"Girly";
		break;
		
		case 8:
		cout<<"Immature";
		break;
		
		case 9:
		cout<<"Self-Distractor";
		break;
		
		case 10:
		cout<<"Snuggler";
		break;
		
		case 11:
		cout<<"Squisher";
		break;
		
		case 12:
		cout<<"Suckler";
		break;
		
		case 13:
		cout<<"Used Diaper [Desire or Addict] (stacks)";
		break;
		
		case 14:
		cout<<"Vocab Regression";
		break;
		
		case 15:
		cout<<"Babbler";
		break;
		
		//Body
		case 16:
		cout<<"Baby Fat, Obese, or Blobish (stacks)";
		break;
		
		case 17:
		cout<<"Bad Grip";
		break;
		
		case 18:
		cout<<"Butterpaws";
		break;
		
		case 19:
		cout<<"Cede to Authority";
		break;
		
		case 20:
		cout<<"Clumsy";
		break;
		
		case 21:
		cout<<"Depowered";
		break;
		
		case 22:
		cout<<"Drooling";
		break;
		
		case 23:
		cout<<"Super Soaker OR Mess Master (player's choice, stacks into Super User)";
		break;
		
		case 24:
		cout<<"Muscle Unmemory";
		break;
		
		case 25:
		cout<<"Pinkified OR Powdered (choice)";
		break;
		
		case 26:
		cout<<"Quick Filler";
		break;
		
		case 27:
		cout<<"Strong Filler";
		break;
		
		case 28:
		cout<<"Thumb Suckler";
		break;
		
		//Equipment
		case 29:
		cout<<"Baby Bib";
		break;
		
		case 30:
		cout<<"Baby Clothes";
		break;
		
		case 31:
		cout<<"Clumsifying Diaper";
		break;
		
		case 32:
		cout<<"Corrupting Diaper";
		break;
		
		case 33:
		cout<<"Crinkly Diaper";
		break;
		
		case 34:
		cout<<"Empowered Diaper";
		break;
		
		case 35:
		cout<<"Enchanted";
		break;
		
		case 36:
		cout<<"Energy Diaper";
		break;
		
		case 37:
		cout<<"Friendship Bracelet";
		break;
		
		case 38:
		cout<<"Frilly Clothes";
		break;
		
		case 39:
		cout<<"Kiddie Crown";
		break;
		
		case 40:
		cout<<"Teehee Tiara OR its alternate";
		break;
		
		case 41:
		cout<<"Themed Suit";
		break;
		
		case 42:
		cout<<"Pacifier Necklace";
		break;
		
		case 43:
		cout<<"Plastic Pants";
		break;
		
		case 44:
		cout<<"Poofy Diaper OR Extra Poof (stacks)";
		break;
		
		case 45:
		cout<<"Resonant Jewels";
		break;
		
		//Universal
		case 46:
		cout<<"Assistant";
		break;
		
		case 47:
		cout<<"Big Pal";
		break;
		
		case 48:
		cout<<"Little Leon";
		break;
		
		case 49:
		cout<<"Inspiration";
		break;
		
		case 50:
		cout<<"Permapamps";
		break;
		
		case 51:
		cout<<"Potty Rebel";
		break;
		
		case 52:
		cout<<"Trap Magnet";
		break;
		
		case 53:
		cout<<"Unlucky";
		break;
		
		default:
		cout<<"ERROR. GENERATION FAILED";
	}
	
	cout<<"."<<endl;
}



//Magic Scroll (item) print; last update: 4.0
string ScrollPrint(int val){
	switch(val){
		case 1:
			return "Cleanse";
		case 2:
			return "Mapping";
		case 3:
			return "Missile";
		case 4:
			return "Prophecy";
		case 5:
			return "Purge";
		case 6:
			return "Purify";
		case 7:
			return "Reflect";
		case 8:
			return "Reinforce";
		case 9:
			return "Shelter";
		case 10:
			return "Shield";
		
		default:
			return "scroll ERROR";
	}
}



//Trap Artist Room print; last update: 4.0
string taRoomPrint(int val){
	switch(val){
		case 1:
			return "Empty Room";
		case 2:
			return "Bouncy Room";
		case 3:
			return "Dining Room";
		case 4:
			return "Dusty Room";
		case 5:
			return "Galaxy Room";
		case 6:
			return "Girly Room";
		case 7:
			return "Island Room";
		case 8:
			return "Kitchen Room";
		case 9:
			return "Plant Room";
		case 10:
			return "Potions Room";
		case 11:
			return "Sparkling Room";
		case 12:
			return "Timed Room";
		case 13:
			return "Tingly Room";
			
		default:
		return "switch ERROR";
	}
}



//Room name print, last update: 4.0
string TrapPrint(int val){
	
	switch(val){
		//Tier 0 Rooms
		case 1:
		return "Alarm Trap";
		
		case 2:
		return "Ambush";
		
		case 3:
		return "Caretaker's Changing Room";
		
		case 4:
		return "Caretaker's Special Room";
		
		case 5:
		return "Change Machine";
		
		case 6:
		return "Changing Room";
		
		case 7:
		return "Energy Room";
		
		case 8:
		return "High Chair Dining Room";
		
		case 9:
		return "Item Room";
		
		case 10:
		return "Lock Door";
		
		case 11:
		return "Mimikyu's Tag-Along";
		
		case 12:
		return "Pacifier Door";
		
		case 13:
		return "Playtime Nursery";
		
		case 14:
		return "Potion Challenge";
		
		case 15:
		return "Presentation";
		
		case 16:
		return "Seeking Bottles";
		
		case 17:
		return "Smeargle's Art Room";
		
		case 18:
		return "Tickling Machine";
		
		case 19:
		return "Training Potties";
		
		//Tier 1 Rooms
		case 20:
		return "Baby Video Game Room";
		
		case 21:
		return "Boss Bomb";
		
		case 22:
		return "Bouncing Chair Lock";
		
		case 23:
		return "Diaper Video Game Room";
		
		case 24:
		return "Diving FoY Pool";
		
		case 25:
		return "Dragon Video Game Room";
		
		case 26:
		return "(Super) Massaging Chair Lock";
		
		case 27:
		return "Multicolor Crystal Path";
		
		case 28:
		return "Odd Mirror";
		
		case 29:
		return "Themed Suit Ambush";
		
		case 30:
		return "Plushie Pile";
		
		case 31:
		return "Powder Dust Room";
		
		case 32:
		return "Siren's Ballad";
		
		case 33:
		return "Sleepy Room";
		
		case 34:
		return "Water Gun Turret";
		
		//Tier 2 Rooms
		case 35:
		return "Baby Ritual";
		
		case 36:
		return "Concentrated Powder Path";
		
		case 37:
		return "Dimensional Wand";
		
		case 38:
		return "Draining Diaper Trap";
		
		case 39:
		return "FoY Water Gun Turret";
		
		case 40:
		return "Hypno TV";
		
		case 41:
		return "Music Room";
		
		case 42:
		return "Plush Army Hoard";
		
		case 43:
		return "Time Distortion";
		
		default:
		return "switch ERROR";
	}
	
	return "ERROR";
}



//Caretaker Encounters////////////////////////////////////////////////////////////////////
void Kangaskhan(){                                    //Kangaskhan, last update: 4.0

   perPlayer = true;
	while(perPlayer == true) {
		system("cls");
		cout<<"KANGASKHAN"<<endl;
		
		do{
		
			if(tdForfeit){forfeit = Forfeit();}

			Itotal = Intensity(1);
			
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

	if(forfeit){
		cout<<"Intensity roll of "<<(Itotal/2)<<" RB/BP."<<endl;
		cout<<"Pouched/Finished Intensity roll of "<<(Itotal/2 + 5)<<" RB/BP."<<endl<<endl;
	} else {
		cout<<"Intensity roll of "<<Itotal<<" RB/BP."<<endl;
		cout<<"Pouched/Finished Intensity roll of "<<(Itotal + 10)<<" RB/BP."<<endl<<endl;
	}
	
	Spoiled();

   	perPlayer = YesNo();
   }
}



void Goodra(){                                    		//Goodra, last update: 4.0
	int fifty1 = 51;

	perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"GOODRA"<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
   
			Itotal = Intensity(2);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

		if(forfeit){
			Itotal *= 0.5;
			fifty1 = DieN(100);
		}
		
		if(fifty1 > 50){
			cout<<"Player gains a stack of Obedience, GR: Mittens, or Pacifier Gag."<<endl;
		} else {
			cout<<"Goodra's status infliction was resisted."<<endl;
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/MP and "<<(Itotal/2)<<" BP."<<endl;
		cout<<"Hogtied? That player gains Magic Pacifier too."<<endl;
		
		Spoiled();

      perPlayer = YesNo();
   }
}



void Wigglytuff(){                                    //Wigglytuff, last update: 4.0
	
	perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"WIGGLYTUFF"<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
	
			Itotal = Intensity(2);

			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				cout<<"Player gains GR: Used Diaper Desire."<<endl;
			} else {
				cout<<"Player resisted the infliction of GR: Used Diaper Desire."<<endl;
			}
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/EP."<<endl;
		cout<<"If a player has no Use Level in a category, they gain 1 level in that category."<<endl;
	
		Spoiled();
		
      perPlayer = YesNo();
   }
}



void Serperior(){                                    //Serperior, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"SERPERIOR"<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
	
			Itotal = Intensity(0);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

		if(forfeit){Itotal *= 0.5;}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/EP."<<endl;
		
		cout<<"Player gains 1 UL in both categories."<<endl;
		cout<<"This Caretaker will only change Max Capacity diapers."<<endl;

		Spoiled();

      perPlayer = YesNo();
   }
}



void Gardevoir(){                                    //Gardevoir, last update: 4.0
	
	perPlayer = true;
   while(perPlayer == true) {
	system("cls");
   	cout<<"GARDEVOIR"<<endl;
	
		do{
			if(tdForfeit){forfeit = Forfeit();}
	
			Itotal = Intensity(2);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				cout<<"Player gains GR: Changeless."<<endl;
			} else{
				cout<<"Player resisted GR: Changeless."<<endl;
			}
		} else {
			cout<<"Player gains GR: Changeless."<<endl;
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
		
		Spoiled();

      perPlayer = YesNo();
   }
}



void Flygon(){                                    //Flygon, last update: 4.0
	perPlayer = true;
	int Care;

	
   while(perPlayer == true) {
	system("cls");
   	cout<<"FLYGON"<<endl;
	
	do{
		if(tdForfeit){forfeit = Forfeit();}
		
		Itotal = Intensity(1);
	
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);
	cout<<endl<<endl;
	
	if(forfeit){
		Itotal *= 0.5;
		
		if(DieN(100) > 50){
			cout<<"Players suffer 2 stacks of Holding Help."<<endl;
		} else {
			cout<<"Player was not inflicted with 2 stacks of Holding Help."<<endl;
		}
	} else {
		cout<<"Players suffer 2 stacks of Holding Help."<<endl;
	}
	
		//Auto-Bottle check	
	if(DieN(100) <= 25){
		Care= DieN(7) + 11;
		cout<<"Player also gains Auto-Bottle with "<<Care<<" WHP contained."<<endl;
	}
	  
	cout<<"Intensity roll of "<<Itotal<<" RB/BP."<<endl;
	
	cout<<"Flygon changes players into Food diapers."<<endl;
	
	Spoiled();

      perPlayer = YesNo();
   }
}



void Boss(){                                    //Bosses
	
	do{ //loop until certain of boss and if forfeit
		system("cls");
		
		//ask for boss
		check = true;
	   while(check == true){
			cout<<"Which Boss are you encountering?"<<endl;
			cout<<"1- Arch Delphox"<<endl<<"2- Flora"<<endl<<"3- Gemcutter"<<endl<<"4- Lati(a/o)s"<<endl<<"5- Lead Flygon"<<endl;
			cout<<"6- Lugia"<<endl<<"7- Luxray Alpha"<<endl<<"8- Mewtwo"<<endl;
			cout<<endl<<"Input: ";
			
		cin>>intput;

		if((cin.fail())||(intput > 8)||(intput < 1)){
			cin.clear();
			cin.ignore();
			intput = -1;
			cout<<"Please enter a number between 1 and 8."<<endl<<endl;
		} else {
			check = false;
		}
	   }
   
		//ask for forfeit
		if(tdForfeit){
			forfeit = Forfeit();
			cout<<endl;
		}
   
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);


if(intput == 4){												//Latias/Latios, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
		cout<<"LATIAS/LATIOS"<<endl<<endl;
			
		if((forfeit)&&(DieN(100) > 50)){
			cout<<"Player avoided the random Ghost Regression."<<endl;
		} else {
			randomGR(1);
		}
		
		BombPoof(10);
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;
		
		Spoiled();

    perPlayer = YesNo();
   }


} else if(intput == 8){                                		//Mewtwo, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
		cout<<"MEWTWO"<<endl;

		do{
			Itotal = Intensity(3);
			
			certainty = makeSure();
		}while(!certainty);
			cout<<endl;

		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				randomGR(4);
			} else {
				cout<<"Player avoided the random Mental GR."<<endl;
			}
			
			if(DieN(100) > 50){
				cout<<"Player suffers GR: Changeless."<<endl;
			} else {
				cout<<"Player avoided GR: Changeless."<<endl;
			}
		} else {
			randomGR(2);
			cout<<"Player suffers GR: Changeless."<<endl;
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;

		Spoiled();

    perPlayer = YesNo();
   }


} else if(intput == 6){                                     //Lugia, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
		cout<<"LUGIA"<<endl;
		
		do{
			Itotal = Intensity(2);
		
			certainty = makeSure();
		}while(!certainty);
			cout<<endl;
		  
		//Roll 2 UC at 5 higher Tiers
		UCTauto = 5;
		rollCount = 2;
		UCprompt();
		
		cout<<"If either Hold Point category does not procc diaper use, add enough UHP to use diaper in that category."<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				randomGR(3);
			} else {
				cout<<"Player avoided the random Body or Equipment Ghost Regression."<<endl;
			}
		} else {
			randomGR(3);
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/BP/EP."<<endl;  
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;

		Spoiled();

    perPlayer = YesNo();
   }
   
   
} else if(intput == 7){										//Alpha, last update: 4.0
	 perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
		cout<<"LUXRAY PRIDE ALPHA"<<endl;
		
		do{
			Itotal = Intensity(2);
		
			certainty = makeSure();
		}while(!certainty);
			cout<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				cout<<"Player gains a Ghost Regression of a Trait they've yet to earn from their Resonance."<<endl;
			} else {
				cout<<"Player avoided the Ghost Regression from their Resonance."<<endl;
			}
			
			if(DieN(100) > 50){
				cout<<"Player gains Submissive."<<endl;
			} else {
				cout<<"Player avoids Submissive."<<endl;
			}
			
			if(DieN(100) > 50){
				cout<<"Player gains their choice of GR: Pride Suit (Luxray/Luxio/Shinx)."<<endl;
			} else {
				cout<<"Player avoids the GR: Pride Suit."<<endl;
			}
			
		} else {
			cout<<"Player gains a Ghost Regression of a Trait they've yet to earn from their Resonance."<<endl;
			cout<<"Player gains Submissive."<<endl;
			cout<<"Player gains their choice of GR: Pride Suit (Luxray/Luxio/Shinx)."<<endl;
		}
		
		cout<<endl<<"Player gains "<<Itotal<<" RB/MP/EP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;

		Spoiled();
	
	perPlayer = YesNo();
   }


} else if(intput == 2){										//Flora, last update: 4.0
	int rando [2] = {0, 0};
	int Care [2] = {0, 0};
	
	
	 perPlayer = true;
	while(perPlayer == true){
		system("cls");
		cout<<"FLORA (best Caretaker don't @ me -Jordan)"<<endl;
		
		do{
			Itotal = Intensity(1);
			
			certainty = makeSure();
		}while(!certainty);
			cout<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			Care[0] = DieN(100);
			Care[1] = DieN(100);
		} else {
			Care[0] = 51;
			Care[1] = 51;
		}
		
		//if forfeit and both, or defeated
		if((Care[0] > 50)&&(Care[1] > 50)){
			rando[0] = DieN(5);
			rando[1] = DieN(4);
			
			//ensure GRs don't stack
			if(rando[1] >= rando[0]){
				rando[1]++;
			}
		
		//if forfeit and get one
		} else if((Care[0] > 50)||(Care[1] > 50)){
			rando[0] = DieN(5);
			rando[1] = 0;
			
		//if forfeit and neither
		} else {
			rando[0] = 0;
		}
		
		//GR printouts
		if(rando[0] != 0){
			cout<<"Player gains GR: ";
				
			for(int i = 0; i < 2; i++){
				switch(rando[i]){
					case 1:
						cout<<"Vocab Regression";
					break;
					case 2:
						cout<<"Little'd";
					break;
					case 3:
						cout<<"Themed Suit (of a joey)";
					break;
					case 4:
						cout<<"Suckler";
					break;
					default:
						cout<<"Cede to Authority";
				}
				if((rando[1] != 0)&&(i == 0)){
					cout<<" and GR: ";
				}
			};
			cout<<"."<<endl;
			
		} else {
			cout<<"Player avoids the GR effects."<<endl;
		}
		
		if((forfeit)&&(DieN(100) > 50)){
			cout<<"Player gains an Auto-Bottle with "<<(DieN(6)+7)<<" WHP."<<endl;
		} else {
			cout<<"Player avoided the Auto-Bottle."<<endl;
		}
		
		cout<<"Player gains "<<Itotal<<" RB/MP/BP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;

		Spoiled();
		
		perPlayer = YesNo();
	}


}else if(intput == 3){										//Gemcutter, last update: 4.0
	
	 perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"GEMCUTTER"<<endl;
		
		do{
			Itotal = Intensity(2);
			
			certainty = makeSure();
		}while(!certainty);
			cout<<endl;
		
		if(forfeit){ //if forfeiting
			Itotal *= 0.5;
			
			if(DieN(100) > 50){ //random jewelry effect
				cout<<"Player avoided the random jewelry status."<<endl;
			} else {
				cout<<"Player gains jewelry status of ";
				jewelry(0);
				cout<<"."<<endl;
			}
			
			if(DieN(100) > 50){ //random Equipment GR
				cout<<"Player avoided the random Equipment GR."<<endl;
			} else {
				randomGR(5);
			}
			
		} else { //if not forfeiting
			cout<<"Player gains jewelry status of ";
				jewelry(0);
				cout<<"."<<endl;
				randomGR(5);
		}
		
		cout<<endl<<"Player gains "<<Itotal<<" RB/EP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;
		cout<<"The Gemcutter can also change players into Green Gem Diapers."<<endl;

		Spoiled();
		
		perPlayer = YesNo();
	}


} else if(intput == 5){										//Lead Flygon, last update: 4.0
	int magicBottle;

	 perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"LEAD FLYGON"<<endl;
		
		do{
			Itotal = Intensity(2);
			certainty = makeSure();
		}while(!certainty);
		
		if(forfeit){ Itotal *= 0.5; }
		
		magicBottle = (DieN(20)/10) + 1 + 2;
		
		cout<<endl<<"Player gains a Magic Bottle with "<<magicBottle<<" bomb effects inside."<<endl;
		cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;
		cout<<"Lead Flygon can also change players into Food Diapers."<<endl;

		Spoiled();
		
		perPlayer = YesNo();
	}
	
} else if(intput == 1){										//Arch Delphox, last update: 4.0
	int magic[2] = {0, 0};
	magic[0] = Misfire(4);
		do{ //prevent identical Misfires
			magic[1] = Misfire(4);
	}while(magic[1] == magic[0]);

	 perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"ARCH DELPHOX"<<endl;
		
		do{
			Itotal = Intensity(0);
			certainty = makeSure();
		}while(!certainty);
		
		if(forfeit){ Itotal *= 0.5; }
		
		cout<<"The party is hit by Potent "<<MisfirePrint(magic[0])<<" and "<<MisfirePrint(magic[1])<<" Misfires."<<endl;
		cout<<"Player gains "<<Itotal<<" RB/MP/BP/EP."<<endl;
		cout<<"All bosses can change players into Boss-themed Diapers."<<endl;

		Spoiled();
		
		perPlayer = YesNo();
	}

}	// end of potential bosses

}



void Miltank(){                                     //Miltank, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"MILTANK"<<endl;

	do{
		if(tdForfeit){forfeit = Forfeit();}
		
		Itotal = Intensity(1);
	
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
	if(forfeit){Itotal *= 0.5;}
		
	cout<<"Intensity roll of "<<Itotal<<" RB/BP/EP."<<endl;
	
	//roll 1 UC at 4 higher Tiers
	UCTauto = 4;
	rollCount = 1;
	UCprompt();
	
	cout<<"Miltank changes players into Farm diapers."<<endl;

	Spoiled();

	perPlayer = YesNo();
   }
}



void Primarina(){                                  //Primarina (Mischief), last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"PRIMARINA, Mischief"<<endl<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
		
			Itotal = Intensity(0);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

	if(forfeit){
		Itotal *= 0.5;
	}
	
	BombPoof(9);
	
	Spoiled();

	perPlayer = YesNo();
   }
}



void PrimSiren(){									//Primarina (Siren), last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
		system("cls");
		cout<<"PRIMARINA (SIREN)"<<endl<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
			
			Itotal = Intensity(2);
			
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100)>50){
				cout<<"Player gains Enlightened."<<endl;
			} else {
				cout<<"Player avoided Enlightened."<<endl;
			}
		} else {
			cout<<"Player gains Enlightened."<<endl;
		}
		
		cout<<"If player is not wearing a Swim diaper, they gain Bogged."<<endl;
		cout<<"Player gains "<<Itotal<<" RB/MP/EP."<<endl;

		Spoiled();
	
		perPlayer = YesNo();
   }
}



void Leavanny(){                                   //Leavanny, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"LEAVANNY"<<endl;
	   
	do{
		if(tdForfeit){forfeit = Forfeit();}
		   
		Itotal = Intensity(0);
	
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);
	cout<<endl<<endl;
	   
	if(forfeit){
		Itotal *= 0.5;
		if(DieN(100) > 50){
			cout<<"Player gains GR: Frilly Clothes."<<endl;
		} else {
			cout<<"Player avoided GR: Frilly Clothes."<<endl;
		}
	} else {
		cout<<"Player gains GR: Frilly Clothes."<<endl;
	}
	
	cout<<"Intensity roll of "<<Itotal<<" for RB/EP."<<endl;

	Spoiled();

	perPlayer = YesNo();
   }
}



void Togekiss(){                                   //Togekiss, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"TOGEKISS"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(1);
		   
		   	certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){Itotal *= 0.5;}
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/MP. Any player with Silly is cured for 10 RB per level."<<endl;
	   
		cout<<"Togekiss changes players into Confetti diapers."<<endl;
		
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Ninetales(){                                  //Ninetales, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"NINETALES"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
		  	certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){
			Itotal *= 0.5;
		   
			if(DieN(100)>50){
				cout<<"Player resisted the random Ghost Regression."<<endl;
			} else {
				randomGR(1);
			}
	   } else {
		   randomGR(1);
	   }
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
	   
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Chansey(){                                    //Chansey, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"CHANSEY"<<endl;
	   
	do{
		if(tdForfeit){forfeit = Forfeit();}
		
		Itotal = Intensity(0);
		
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);
	cout<<endl<<endl;
	  
	if(forfeit){
		Itotal *= 0.5;
	}
	
	cout<<"Intensity roll of "<<Itotal<<" RB/MP/BP."<<endl;
	
	IRauto = 1;
	rollCount = 1;
	IdleRolls();
	
	Spoiled();
	
	perPlayer = YesNo();
   }
}



void Salazzle(){									//Salazzle, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"SALAZZLE"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(4);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){
		   Itotal *= 0.5;
		   
		   if(DieN(100)>50){
			   cout<<"Player gains GR: Hypno Suit (Salandit theme)."<<endl;
		   } else {
			   cout<<"Player avoided the GR: Themed Suit."<<endl;
		   }
	   } else {
		   cout<<"Player gains GR: Hypno Suit (Salandit theme)."<<endl;
	   }
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
	   
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Luxray(){										//Luxray, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"LUXRAY"<<endl;
	   
	do{
		if(tdForfeit){forfeit = Forfeit();}
		   
		check = true;
	   while(check == true)
	   {
			cout<<"How many times has the party lost to Luxray (including this time)?"<<endl;
		cin>>intput;

		if((cin.fail())&&(intput < 1)){
			cin.clear();
			cin.ignore();
			intput = 0;
			cout<<"Please enter an integer greater than 0."<<endl<<endl;
			getch();
		} else {
			check = false;
			if(intput > 3)
			{intput = 3;}
		}
	   }
	   
		Itotal = Intensity(intput * 2);
	
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);
	cout<<endl<<endl;
	
	if(forfeit){
		Itotal *= 0.5;
		
		if(DieN(100) > 50){
			cout<<"Player gains GR: Themed Suit."<<endl;
		} else {
			cout<<"Player manages to avoid GR: Themed Suit."<<endl;
		}
	} else {
		cout<<"Player gains GR: Themed Suit."<<endl;
	}
	
	cout<<"Intensity roll of "<<Itotal<<" RB/MP/EP."<<endl;
	cout<<"If the party encounters Luxray again, the party starts with"<<intput<<" stack(s) of Obedience."<<endl;
	cout<<endl;
	cout<<"Luxray changes players into Safari diapers."<<endl;

	Spoiled();
	
	perPlayer = YesNo();
   }
}



void Nidoqueen(){									//Nidoqueen, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"NIDOQUEEN"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if((forfeit)&&(DieN(100)>50)){
		    cout<<"Player avoids the 1 stack of Enhancement."<<endl;
	   } else {
			cout<<"Player suffers 1 stack of Enhancement."<<endl;
	   }
	   
	   cout<<"NOTE: Player only suffers MHP in the following generation."<<endl;
	   cout<<"If the points do not result in an MUL, they gain enough points for 1 MUL anyway."<<endl;
	   
	   rollCount = 5;
	   UCprompt();
	   
	   cout<<endl<<endl;
	   
	   	check = true;
   while(check == true)
   {
		cout<<"How many MUL did the player gain?: ";
   	cin>>intput;

   	if((cin.fail())||(intput < 0)){
   		cin.clear();
   		cin.ignore();
   		intput = 0;
   		cout<<"Please enter a positive integer."<<endl<<endl;
   		getch();
   	} else {
   		check = false;
		Itotal += (intput * 10);
   	}
   }
   
   if(forfeit){ Itotal *= 0.5; }
   
   cout<<endl<<"Intensity roll of "<<(Itotal)<<" RB/EP."<<endl;
   cout<<"This Caretaker only changes Max Capacity diapers."<<endl;
   
	Spoiled();
	
	perPlayer = YesNo();
   }
}



void Vaporeon(){									//Vaporeon, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"VAPOREON"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if((forfeit)&&(DieN(100)>50)){
			cout<<"Player avoids the 1 stack of Enhancement."<<endl;
	   } else {
		   cout<<"Player suffers 1 stack of Enhancement."<<endl;
	   }
	   
	   cout<<"NOTE: Player only suffers WHP in the following generation."<<endl;
	   cout<<"If the points do not result in an WUL, they gain enough points for 1 WUL anyway."<<endl<<endl;
	   
	   rollCount = 5;
	   UCprompt();
	   
	   cout<<endl;
	   
	   	check = true;
   while(check == true)
   {
		cout<<"How many WUL did the player gain?: ";
   	cin>>intput;

   	if((cin.fail())||(intput < 0)){
   		cin.clear();
   		cin.ignore();
   		intput = 0;
   		cout<<"Please enter a positive integer."<<endl<<endl;
   		getch();
   	} else {
   		check = false;
		Itotal += (intput * 10);
   	}
   }
   
   if(forfeit){Itotal *= 0.5;}
   
   cout<<"Intensity roll of "<<(Itotal)<<" RB/EP."<<endl;
   cout<<"This Caretaker only changes Max Capacity diapers."<<endl;
   
   Spoiled();
	
	perPlayer = YesNo();
   }
}



void Audino(){										//Audino, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"AUDINO"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(1);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   
	   cout<<"The following calculations are done at minimum Potty Limit."<<endl;
	   
	   rollCount = 3;
	   UCprompt();
	   
	   cout<<endl;
	   
	   	check = true;
   while(check == true)
   {
		cout<<"How many Use Levels did the player gain?"<<endl;
   	cin>>intput;

   	if((cin.fail())&&(intput < 0)){
   		cin.clear();
   		cin.ignore();
   		intput = 0;
   		cout<<"Please enter a positive integer."<<endl<<endl;
   		getch();
   	} else {
   		check = false;
		Itotal += 10 * intput;
   	}
   }
   
   
   if(forfeit){
	   Itotal *= 0.5;
	   
	   if(DieN(100)>50){
		   cout<<"Player gains 2 stacks of Holding Help."<<endl;
	   } else {
		   cout<<"Player avoided the 2 stacks of Holding Help."<<endl;
	   }
   } else {
	   cout<<"Player gains 2 stacks of Holding Help."<<endl;
   }
   
   cout<<"Intensity roll of "<<Itotal<<" RB/BP."<<endl;
   cout<<"This Caretaker only changes Max Capacity diapers."<<endl;
   
   Spoiled();
	
	perPlayer = YesNo();
   }
}



void Lucario(){										//Lucario, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   bool skew = false;
	   system("cls");
	   cout<<"LUCARIO"<<endl;
	
	do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
			check = true;
		while(check == true)
		{
			cout<<"Does the player have Skewed Aura? (y/n): "<<endl;
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				if((charput == 'y')||(charput == 'Y')){
					skew = true;
				} else {
					skew = false;
				}
			}
		}
		
		Itotal = Intensity(3);
	
		certainty = makeSure();
		if(!certainty){ tdForfeit = true;}
	}while(!certainty);
	cout<<endl<<endl;
	
	if(forfeit){Itotal *= 0.5;}
	
	if(skew == true){
		randomGR(1);
	} else {
		cout<<"The player gains Skewed Aura."<<endl;
	}
	
	cout<<"Intensity roll of "<<Itotal<<" RB/MP/BP."<<endl;
	
	Spoiled();
	
	perPlayer = YesNo();
   }
}



void Ampharos(){									//Ampharos, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"AMPHAROS"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(1);
	   
	   		certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){Itotal *= 0.5;}
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/MP. Player suffers 2 stacks of Obedience during next Caretaker Encounter."<<endl;
	
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Ivysaur(){										//Ivysaur, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"IVYSAUR"<<endl<<endl;
	   
		do{
			if(tdForfeit){forfeit = Forfeit();}
	   
			cout<<"Player is changed into Overnight diaper before any further effects."<<endl;
		   
			Itotal = Intensity(0);
	   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   BombPoof(8);
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/BP."<<endl;
		
		rollCount = 1;
		IdleRolls();

		Spoiled();

	perPlayer = YesNo();
   }
}



void Slowpoke(){									//Slowpoke, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"SLOWPOKE"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){
		   Itotal *= 0.5;
		   
		 if(DieN(100)>50){
			cout<<"Player gains GR: Changeless."<<endl;
		 } else {
			cout<<"Player avoids GR: Changeless."<<endl;
		 }
	   } else {
		   cout<<"Player gains GR: Changeless."<<endl;
	   }
	   
	   if(DieN(20) > 10){
		   cout<<"Player gains GR: Lazy."<<endl;
	   } else {
		   cout<<"Player avoids GR: Lazy."<<endl;
	   }
	   
	   cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
	   cout<<"Slowpoke changes diapers into Slowpoke diapers."<<endl;
	   Spoiled();
	   
	perPlayer = YesNo();
   }
}



void Snorlax(){										//Snorlax, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"SNORLAX"<<endl;
	   
	   do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
		if((forfeit)&&(DieN(20) > 10)){
		   cout<<" Player gains GR: Lazy. ";
		}
		
		cout<<"Reminder to also apply the Idle Roll's RB to BP."<<endl;
		
	   IRauto = 2;
	   rollCount = 1;
	   IdleRolls();
	   
	   //food generation
	   foodAuto = DieN(4);
	   drinkAuto = DieN(4);
	   
	   //calculate food/drink
	   foodDrink();
	   
	   cout<<"Snorlax changes diapers into Sleepy diapers."<<endl;
	   Spoiled();
	
	perPlayer = YesNo();
   }
}



void Mew(){											//Mew, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"MEW"<<endl;
	   
	   cout<<"Because all of Mew's effects ignore forfeit's effects, this Caretaker will generate without the prompt."<<endl<<endl;
	   
	   //chance of random GR
	   if(DieN(20) > 10){
		   randomGR(1);
	   }
	   
	   cout<<"Apply the RB value from this Idle Roll to all other Points (MP/BP/EP) as well."<<endl;
	   
	   rollCount = 1;
	   IdleRolls();

	   Playtime();
	   
	   cout<<"Mew changes diapers into Mew diapers."<<endl;
	   Spoiled();
	
	perPlayer = YesNo();
   }
}



void Galvantula(){									//Galvantula, last update: 4.0
	bool pink = false;
	bool drool = false;
	int wear;

	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"GALVANTULA"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(1);
	   
		//check if pink
			check = true;
			while(check == true)
			{
				cout<<"Does this player suffer Pinkified or Girly (y/n)?: ";
				cin>>charput;

				if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
					cin.clear();
					cin.ignore();
					charput = 'q';
					cout<<"Please enter a proper input. "<<endl;
				} else {
					check = false;
					if((charput == 'y')||(charput == 'Y')){
						pink = true;
					} else {
						pink = false;
					}
				}
			}
			
		//check if drooling
			check = true;
			while(check == true)
			{
				cout<<"Does the player suffer Drooling (y/n)?: ";
				cin>>charput;

				if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
					cin.clear();
					cin.ignore();
					charput = 'q';
					cout<<"Please enter a proper input. "<<endl;
				} else {
					check = false;
					if((charput == 'y')||(charput == 'Y')){
						drool = true;
					} else {
						drool = false;
					}
				}
			}
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
	//generate GR
	if((forfeit == false)||(DieN(100)>50)){
		randomGR(2);
	} else {
		cout<<"Player avoided the random Mental GR."<<endl;
	}
		
		//find second GR possibility
		if(pink&&drool){
			wear = DieN(3);
		} else if(pink){
			wear = DieN(20);
			if(wear > 10)
			{
				wear = 2;
			} else {
				wear = 1;
			}
		} else if(drool){
			wear = DieN(20);
			if(wear > 10)
			{
				wear = 3;
			} else {
				wear = 1;
			}
		} else {
			wear = 1;
		}
		
		//print clothing results
		if(((forfeit)&&(DieN(100)>50))||(forfeit == false)){
			cout<<"Player gains GR: ";
			
			switch(wear){
				case 1:
				cout<<"Baby Clothes."<<endl;
				break;
				
				case 2:
				cout<<"Frilly Clothes."<<endl;
				break;
				
				case 3:
				cout<<"Baby Bib."<<endl;
				break;
				
				default:
				cout<<"Baby Clothes (ERROR IN NUM GEN)."<<endl;
				break;
			}
		} else {
			cout<<"Player managed to avoid the clothing GR."<<endl;
		}
		
		cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Ursaring(){									//Ursaring, last update: 4.0
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"URSARING"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(2);
	   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){
		   Itotal *= 0.5;
		   
		   if(DieN(100)>50){
				cout<<"Player gains GR: Snuggler."<<endl;
		   } else {
			   cout<<"Player avoids GR: Snuggler."<<endl;
		   }
	   } else {
		   cout<<"Player gains GR: Snuggler."<<endl;
	   }
	   
	   cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
	   
	   Playtime();
	   
	   Spoiled();
	
	perPlayer = YesNo();
   }
}



void AlakazamPac(){									//Alakazam (Pacifier), last update: 4.0
	int fifty;
	perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"ALAKAZAM- PACIFIERS"<<endl<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   cout<<"Player is Pacified before any further effects."<<endl;
		   
		   Itotal = Intensity(2);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(DieN(100) > 50){
		   cout<<"Player gains Magic Pacifier."<<endl;
	   } else {
		   cout<<"Player avoided Magic Pacifier."<<endl;
	   }
	   
	   
	   if(forfeit){Itotal *= 0.5;}
	   
	   cout<<"Player gains "<<Itotal<<" RB/MP."<<endl;
	   Spoiled();   
	
	perPlayer = YesNo();
   }
}



void AlakazamSpo(){									//Alakazam (Spoon), last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"ALAKAZAM- SPOONS"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}

		   Itotal = Intensity(2);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
	   
	   if(forfeit){
		   Itotal *= 0.5;
		   
		   if(DieN(100)>50){
			   cout<<"Player gains Soft Foods."<<endl;
		   } else {
			   cout<<"Player avoids soft foods."<<endl;
		   }
	   } else {
		   cout<<"Player gains Soft Foods."<<endl;
	   }
	   
	   cout<<"Player gains "<<Itotal<<" RB/MP."<<endl;
	   
		foodAuto = DieN(20)/10 + 1;
		drinkAuto = DieN(20)/10 + 1;
		foodDrink();
		
		Spoiled();
	
	perPlayer = YesNo();
   }
}



void Victini(){										//Victini, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"VICTINI"<<endl;
	   
		cout<<"Because all of Victini's effects ignore forfeit's effects, this Caretaker will generate without the prompt."<<endl<<endl;
	   
	   if(DieN(100)>50){
		   cout<<"Player gains ";
		   switch(DieN(3))
		   {
			   case 1:
			   cout<<"GR: Quick Filler";
			   break;
			   
			   case 2:
			   cout<<"GR: Strong Filler";
			   break;
			   
			   case 3:
			   cout<<"GR: Hyper Use";
			   break;
			   
			   default:
			   cout<<"ERROR. Just go with GR: Quick Filler";
			   break;
		   }
		   cout<<"."<<endl;
	   }
	   
	   UCTauto = 1;
	   rollCount = 1;
	   UCprompt();
	   
	   Playtime();
	   Spoiled();
	   
	perPlayer = YesNo();
   }
}



void Dragonite(){									//Dragonite, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"DRAGONITE"<<endl;
	   
	   cout<<"Because all of Dragonite's effects ignore forfeit's effects, this Caretaker will generate without the prompt."<<endl<<endl;
	   
	   cout<<"Player changes into Overnight Diaper before any further effect."<<endl;
	   
	   IRauto = 4;
	   rollCount = 1;
	   IdleRolls();
	   
	   Spoiled();
	   
	perPlayer = YesNo();
   }
}



void Delphox(){										//Delphox, last update: 4.0
	int mis = Misfire(false);
	
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"DELPHOX"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   cout<<"The Misfire is not rerolled when generating for several players."<<endl<<endl;
		   
		   cout<<"Delphox casts a random spell on the party!"<<endl;
		   cout<<"Delphox casts a Misfire of "<<MisfirePrint(mis)<<"! Do not apply this effect until the end."<<endl;
		   
		   Itotal = Intensity(1);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

	   if(forfeit){Itotal *= 0.5;}
	   
	   cout<<"This player gains "<<Itotal<<" RB/MP/BP/EP."<<endl;
	   Spoiled();
	   
	perPlayer = YesNo();
   }
}



void Smeargle(){									//Smeargle, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"SMEARGLE"<<endl;
	   
		do{
		   if(tdForfeit){forfeit = Forfeit();}
		   
		   Itotal = Intensity(0);
		   
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;

	   if(forfeit){
		   Itotal *= 0.5;

		   if(DieN(100)>50){
				cout<<"Player suffers Pinkified or Powdered (their preference)."<<endl;
				cout<<"If they have either, then instead find a random GR after this calculation."<<endl;
				cout<<"Suggestion: "; randomGR(1); cout<<endl;
		   } else {
			   cout<<"Player avoids Pinkified/Powdered."<<endl;
		   }	   
	   } else {
		    cout<<"Player suffers Pinkified or Powdered (their preference)."<<endl;
			cout<<"If they have either, then instead find a random GR after this calculation."<<endl;
			cout<<"Suggestion: "; randomGR(1); cout<<endl;
	   }
	   
	   cout<<"Player gains "<<Itotal<<" RB/BP/EP."<<endl;
	   Spoiled();

	perPlayer = YesNo();
   }
}



void Shaymin(){										//Shaymin, last update: 4.0
		perPlayer = true;
   while(perPlayer == true) {
	   system("cls");
	   cout<<"SHAYMIN"<<endl;
	   
	   cout<<"Shaymin is non-hostile, so they do not have a forfeit. You also cannot redo on these inputs."<<endl;

	check = true;
	while(check == true)
	{
		cout<<"Does the player wear a Flower Diaper (y/n)?: ";
		cin>>charput;

		if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter y or n. "<<endl;
		} else {
			check = false;
			cout<<endl<<endl;
			if((charput == 'N')||(charput == 'n')){
				cout<<"Player rolls 1 IR and rolls Tiny IT, but are fully healed and can acquire/change into Flower Diapers."<<endl;
				
				Itotal = Intensity(0);

				rollCount = 1;
				IdleRolls();
				
				cout<<"Player gains "<<Itotal<<" RB/MP/BP/EP from Intensity roll."<<endl;
			} else {
				
				rollCount = 1;
				IdleRolls();
				
				cout<<"Player is fully healed, and may acquire/change into Flower Diapers."<<endl;
			}
		}
	}
	   
	perPlayer = YesNo();
   }
}



void Lapras(){										//Lapras, last update: 4.0
	int Care;
  perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"LAPRAS"<<endl<<endl;
		
		Care = DieN(8);
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
			
			Itotal = Intensity(2);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
		if(forfeit){
			Itotal *= 0.5;
			
			if(DieN(100) > 50){
				cout<<"Player avoided gaining GR: Life Jacket."<<endl;
			} else {
				cout<<"Player gained GR: Life Jacket."<<endl;
			}
			
		} else {
			cout<<"Player gained GR: Life Jacket."<<endl;
		}
		
		cout<<"Player gained "<<Itotal<<" RB/MP and "<<(Care + 12)<<" WHP."<<endl;
		Spoiled();
		
		perPlayer = YesNo();
	}
}



void Milotic(){										//Milotic, last update: 4.0
  perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"MILOTIC"<<endl<<endl;
		
		do{
			Itotal = Intensity(2);
			
			if(tdForfeit){forfeit = Forfeit();}
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		
		if(forfeit){Itotal *= 0.5;}
		
		cout<<"Player gains "<<Itotal<<" RB/EP and 2 Wet Use Levels from water."<<endl;
		cout<<"This Caretaker only changes Max Capacity diapers."<<endl;
		Spoiled();
		
		perPlayer = YesNo();
	}
}



void Sylveon(){										//Sylveon, last update: 4.0
	int clothes;
	int accessory;
  perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"SYLVEON"<<endl;
		
		do{
			if(tdForfeit){forfeit = Forfeit();}
			
			Itotal = Intensity(1);
		
			certainty = makeSure();
			if(!certainty){ tdForfeit = true;}
		}while(!certainty);
		cout<<endl<<endl;
		
		//forfeit effects
		if(forfeit){
			Itotal *= 0.5;
			
			//clothes effect roll
			if(DieN(100) > 50){
				clothes = 4;
			} else {
				clothes = DieN(3);
			}
			
			if(DieN(100) > 50){
				accessory = 6;
			} else {
				accessory = DieN(5);
			}
		} else {
			clothes = DieN(3);
			accessory = DieN(5);
		}
				
			if(clothes != 4){
				cout<<"Player gains ";
			}				
			
			switch(clothes){
				case 1:
				cout<<"Baby Clothes";
				break;
				
				case 2:
				cout<<"Frilly Clothes";
				break;
				
				case 3:
				cout<<"Royal Garb";
				break;
				
				case 4:
				cout<<"Player resisted the clothes GR";
				break;
			}
			cout<<"."<<endl;
			
			
			//accessories
			if(accessory != 6){
				cout<<"Player gains ";
			}	
			
			switch(accessory){
				case 1:
				cout<<"GR: Fancy Accessories";
				break;
				
				case 2:
				cout<<"GR: Plastic Pants";
				break;
				
				case 3:
				cout<<"GR: Royal Garb";
				break;
				
				case 4:
				cout<<"GR: Mittens";
				break;
				
				case 5:
				cout<<"GR: Baby Bib";
				break;
				
				case 6:
				cout<<"Player resisted the accessory effect";
				break;
			}
			cout<<"."<<endl;
		
		cout<<"Player gains "<<Itotal<<" RB/EP."<<endl;
		Spoiled();
		
		perPlayer = YesNo();
	}
}



void cDiapers(){									//Diaper Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF DIAPERS"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(1);
		
		cout<<"Player gains "<<Itotal<<" RB/EP."<<endl;
		Spoiled();
		
	perPlayer = YesNo();
	}
}



void cImmature(){									//Immature Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF IMMATURITY"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(1);
		
		cout<<"Player gains "<<Itotal<<" RB/MP."<<endl;
		Spoiled();
		
	perPlayer = YesNo();
	}
}



void cLazy(){										//Lazy Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF LAZINESS"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(0);
		
		cout<<"Player gains GR: Lazy for 1 Idle Roll, or gains 10 RB/MP if they have Lazy."<<endl;
		cout<<"Player gains "<<Itotal<<" RB/MP."<<endl;
		Spoiled();
		
	perPlayer = YesNo();
	}
}



void cLittle(){										//Little Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF LITTLENESS"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(1);
		
		cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
		Spoiled();
		
	perPlayer = YesNo();
	}
}



void cSquish(){										//Squish Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF SQUISHINESS"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(0);
		
		cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
		
		rollUntil = true;
		UCprompt();
		
		Spoiled();
		
	perPlayer = YesNo();
	}
}



void cSissy(){										//Sissy Crystal, last update: 4.0
		perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"CRYSTAL OF SISSINESS"<<endl;
		cout<<"This Caretaker cannot be forfeited to."<<endl;
		
		Itotal = Intensity(1);
		
		cout<<"Player gains "<<Itotal<<" RB/EP."<<endl;
		Spoiled();
		
	perPlayer = YesNo();
	}
}



//Trap Encounters/////////////////////////////////////////////////////////////////////////
void HypnoTV(){										//Hypno TV, last update: 4.0
	system("cls");
	cout<<"HYPNO TV"<<endl;
	tRoll = TrapRoll();
	
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"HYPNO TV"<<endl;
	
	switch(tRoll){
		case 1:
		cout<<"Brainwashed. Forces 2 Idle Rolls (gain MP equal to the RB rolled)."<<endl;
		rollCount = 2;
		break;
		
		case 2:
		cout<<"Brainwashed. Forces 1 Idle Roll (gain MP equal to the RB rolled)."<<endl;
		rollCount = 1;
		break;
		
		case 3:
		cout<<"2 stack of Hypnotized. Forces 1 Idle Roll (gain MP equal to the RB rolled)."<<endl;
		rollCount = 1;
		break;
		
		case 4:
		cout<<"Hypnotized. Forces 1 Idle Roll (gain MP equal to the RB rolled)."<<endl;
		rollCount = 1;
		break;
		
		case 5:
		cout<<"Roll 1 Hypnosis Check. If it succeeds, Hypnotized for 1 IR (gain MP equal to the RB rolled)."<<endl;
		
		HCauto = 1;
		HypnoCheck();
		cout<<endl;
		
		if(hypnoState){
			rollCount = 1;
			IdleRolls();
		}
		break;
		
		case 6:
		cout<<"No effect, ignored."<<endl;
		break;
		
		default:
		cout<<"ERROR. Invalid Trap Roll."<<endl;
		break;	
	}
	
	if(tRoll < 5){
		IdleRolls();
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
}



void MCL(){											//(Super) Massaging Chair Lock, last update: 4.0
	bool super = false;
	
	cout<<"(SUPER) MASSAGE CHAIR LOCK"<<endl;
	
	check = true;
	while(check == true)
	{
		cout<<"Is this the Super version?"<<endl;
		cin>>charput;

if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
			cin.clear();
			cin.ignore();
			charput = 'q';
			cout<<"Please enter a proper input. "<<endl;
		} else {
			check = false;
		}
	}
	
	if((charput == 'y')||(charput == 'Y')){
		super = true;
	}	
	
	
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"(SUPER) MASSAGE CHAIR LOCK"<<endl<<endl;
	
	tRoll = TrapRoll();
	
	Itotal = Intensity(1);
	
	switch(tRoll){
		case 1:
		cout<<"Intensity roll of "<<Itotal<<" RB/BP. Inflicts 2 stacks Holding Help. Potty Limit to minimum until exit."<<endl;
		break;
		
		case 2:
		cout<<"Intensity roll of "<<Itotal<<" RB/BP. Inflicts Holding Help. Potty Limit to minimum until exit."<<endl;
		break;
		
		case 3:
		cout<<"Intensity roll of "<<Itotal<<" RB/BP. Reduces Potty Limit to minimum until exit."<<endl;
		break;
		
		case 4:
		cout<<"Intensity roll of "<<Itotal<<" RB/BP. Reduces Potty Limit by 5 until exit."<<endl;
		break;
		
		case 5:
		cout<<"Intensity roll of "<<(Itotal-10)<<" RB/BP. Reduces Potty Limit by 3 until exiting."<<endl;
		break;
		
		case 6:
		cout<<"Intensity roll of "<<((Itotal-10) / 2)<<" RB/BP."<<endl;
		break;
		
		default:
		cout<<"ERROR. Invalid Trap Roll."<<endl;
		break;	
	}
	
if(super)
{
	switch(tRoll){
		case 1:
		case 2:
		cout<<"Vibrating Diaper with Diaper Lock."<<endl;
		break;
		
		case 3:
		case 4:
		case 5:
		cout<<"Vibrating Diaper with Diaper Lock for ";
		break;
		
		case 6:
		cout<<"No additional effect."<<endl;
		break;
		
		default:
		cout<<"STILL INVALID"<<endl;
		break;
	}
	
	if(tRoll == 3)
	{
		cout<<"2 Idle Rolls"<<endl;
	} else if((tRoll < 6)&&(tRoll > 3))
	{
		cout<<"1 Idle Roll"<<endl;
	}
}
	giveScroll();

	perPlayer = YesNo();
}
 }
 
 
 
void Nursery(){										//Playtime Nursery, last update: 4.0
	int dCount = 0;

	perPlayer = true;
	while(perPlayer)
{
	system("cls");
	cout<<"PLAYTIME NURSERY"<<endl;
	
	//Loop for roll generation
	for(int i = 0; i < 4; i++){
		if(DieN(20) > 10){
			dCount++;
		}
	}
	
	//Generate points gained
	cout<<endl;
	Itotal = Intensity(dCount);
	
	//Roll Idle Roll
	rollCount = 1;
	status = true;
	
	IdleRolls();
	
	cout<<endl;
	
	
	//Roll for Mittens and Pacifier Gag
	switch(DieN(20)){
		case 4:
		case 8:
		case 12:
		case 16:
		cout<<"Player gains GR: Mittens."<<endl;
		break;
		
		case 5:
		case 10:
		case 15:
		cout<<"Player gains Pacifier Gag."<<endl;
		break;
		
		case 20:
		cout<<"Player gains both GR: Mittens and Pacifier Gag."<<endl;
		
		default:
		cout<<"No statuses gained from Trap."<<endl;
		break;
	}
	
	cout<<"Intensity roll of "<<Itotal<<" RB and "<<(Itotal / 2)<<" MP/BP/EP.";
	
	if(dCount >= 3){
		cout<<"Briefly inflict Potty Limit -5 for being jolted by alarm."<<endl;
	}
	
	dCount = 0;
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void PowderRoom(){									//Powder Dust Room, last update: 4.0
	int trapSize;
	int curSize;
	int Ptotal;
	bool waddle;
	bool crawl;
	bool powder;
	
	trapSize = DieN(4);

	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"POWDER DUST ROOM"<<endl<<endl;
	
	do{
		check = true;
		while(check){
			cout<<"Is the player waddling? (y/n): ";
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				
				if((charput == 'y')||(charput == 'Y')){
					waddle = true;
				} else {
					waddle = false;
				}
			}	
		}
		
		check = true;
		while(check)
		{
			cout<<"Is the player crawling? (y/n): ";
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				
				if((charput == 'y')||(charput == 'Y')){
					crawl = true;
				} else {
					crawl = false;
				}
			}	
		}
	
		certainty = makeSure();
	}while(!certainty);
	
	if(crawl){
		curSize = 3 * trapSize;
	} else if(waddle){
		curSize = 2 * trapSize;
	} else {
		curSize = trapSize;
	}
	
	if(trapSize > 3){
		powder = true;
	} else {
		powder = false;
	}
	
	//calculate points
	for(int i = 0; i < curSize; i++){
		Ptotal += DieN(4) + 2;
	}
	
	if(powder){
		cout<<"This player gains GR: Powdered."<<endl;
	}

	cout<<"This trap has a size of "<<curSize<<" for this player."<<endl;
	cout<<"This player gains "<<Ptotal<<" RB/EP, and "<<(Ptotal / 2)<<" BP."<<endl;
	
	Ptotal = 0;
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void BounceChair(){									//Bouncing Chair Lock, last update: 4.0
	perPlayer = true;
 while(perPlayer){
	
	do{
		tRoll = TrapRoll();
		Itotal = Intensity(0);
		
		certainty = makeSure();
	}while(!certainty);
	
	
	//Generate intensity roll
	if(tRoll == 1){
		Itotal += 10;
	} else if(tRoll == 5){
		Itotal /= 2;
	} else if(tRoll == 6){
		Itotal /= 4;
	}
	
	
	//Generate Use Checks (if any)
	if(tRoll < 4){
		rollUntil = true;
		UCprompt();
	} else if(tRoll == 4){
		rollCount = 2;
		UCprompt();
	} else if(tRoll == 5){
		rollCount = 1;
		UCprompt();
	}
	

	//Output
	cout<<"Intensity roll of "<<Itotal<<" RB/EP."<<endl;
	
	if((tRoll < 3)||((tRoll == 4)&&(DieN(100) > 50))){
		cout<<"Player gains GR: Used Diaper Desire."<<endl;
	} else if(tRoll == 4){
		cout<<"Player avoids GR: Used Diaper Desire."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
 }
}



void HighChair(){									//High Chair Dining Room; last updated: 4.0
	int total;
	
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"HIGH CHAIR DINING ROOM"<<endl;
	
	drinkAuto = DieN(4);
	foodAuto = DieN(4);
	total = DieN(10);
	
//Calculate food points
	foodDrink();
	
	
//bonus RB/BP output
	cout<<"Player gains "<<total<<" RB/BP";
	
//status check
	if(DieN(20) > 10)
	{
		cout<<" and a stack of Soft Foods";
	}
	cout<<"."<<endl;
	
	giveScroll();

	perPlayer = YesNo();
}
 }
 
 
 
void PowderPath(){									//Concentrated Powder Path; last updated: 4.0
	system("cls");
	cout<<"Please wait...";
	int trapSize = DieN(3) + DieN(3);
	
	int curSize;
	int counter;
	int Ptotal;
	bool waddle;
	bool crawl;

	perPlayer = true;
while(perPlayer)
{
	counter = 0;
	Ptotal = 0;
	
	system("cls");
	cout<<"CONCENTRATED POWDER PATH"<<endl<<endl;
	
	do{		
		check = true;
		while(check)
		{
			cout<<"Is the player waddling? (y/n): ";
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				
				if((charput == 'y')||(charput == 'Y')){
					waddle = true;
				} else {
					waddle = false;
				}
			}
		}
		
		
		check = true;
		while(check)
		{
			cout<<"Is the player crawling? (y/n): ";
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				
				if((charput == 'y')||(charput == 'Y')){
					crawl = true;
				} else {
					crawl = false;
				}
			}
		}
	
		certainty = makeSure();
	}while(!certainty);
	
	
	if(crawl){
		curSize = 3 * trapSize;
	} else if(waddle){
		curSize = 2 * trapSize;
	} else {
		curSize = trapSize;
	}
	
	//generate trap triggers
	for(int i = 0; i < curSize; i++)
	{
		if(DieN(20) > 10)
		{
			counter++;
		}
	}
	
	Ptotal = 4 * counter;
	
	cout<<endl<<endl;
	
	//printouts
	if(counter >= 4)
	{
		cout<<"Player gains GR: Powdered."<<endl;
	}
	
	cout<<"Player tripped "<<counter<<" traps."<<endl;
	cout<<"Player gains "<<Ptotal<<" RB/MP/EP."<<endl;
	
	
	//Use Check(s)
	check = true;
		while(check)
		{
			cout<<"Is the player \"biological\" (y/n)?: ";
			cin>>charput;

			if((cin.fail())||((charput != 'y')&&(charput != 'Y')&&(charput != 'n')&&(charput != 'N'))){
				cin.clear();
				cin.ignore();
				charput = 'q';
				cout<<"Please enter a proper input. "<<endl;
			} else {
				check = false;
				
				if((charput == 'y')||(charput == 'Y')){
					rollCount = counter;
					UCprompt();
				} else {
					cout<<"Ignoring Use Check(s), non-biological transformed players ignore them."<<endl;
				}
			}
		}
		
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void ChangingRoom(bool Caretaker){					//Changing Room; last updated: 4.0
	system("cls");
	if(Caretaker){
		cout<<"CARETAKER'S ";
	}
	cout<<"CHANGING ROOM"<<endl;
	
	int diapers = DieN(4) + 2;
	int list[6];
	
	for(int i = 0; i < diapers; i++){
		list[i] = DieN(trapDMax);
		
		for(int j = 0; j < i; j++){
			if(list[j] == list[i]){
				i--;
			}
		}
	}
	
	cout<<endl<<"Diaper prints are:"<<endl;
	
	for(int i = 0; i < diapers; i++){
		FullDiaperPrint(list[i]); cout<<endl;
	}
	
	cout<<endl;
	
	cout<<"This Room generates no Scrolls."<<endl<<endl;
	
	if(Caretaker == false){
	cout<<"Press [Enter] to return to main menu.";
	getch();
	}
	
 }
 
 
 
void CareChangingRoom(){							//Caretaker's Changing Room; last updated: 4.0
	int Care[3];
	CareGenerator(Care, true);

	ChangingRoom(true);
	
	cout<<"This changing room is occupied by..."<<endl;
	cout<<"Tier 0 Caretaker: "<<CarePrint(Care[0])<<endl;
	cout<<"Tiers 0-1 Caretaker: "<<CarePrint(Care[1])<<endl;
	cout<<"Tiers 0-2 Caretaker: "<<CarePrint(Care[2])<<endl;
	
	cout<<"This Room generates no Scrolls."<<endl<<endl;
	
	cout<<endl<<"Press [Enter] to return to main menu.";
	getch();
 }
 
 
 
void PotionChallenge(){								//Potion Challenge; last updated: 4.0
	int dice;
	int points;
	int extra;
	int repeat;
	int i;

	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"POTION CHALLENGE"<<endl<<"Warning: this may load slowly."<<endl<<endl;
	
	points = 0;
	extra = 0;
	repeat = 0;
	

do{	
	if(repeat == 0)
	{
		dice = DieN(7);
	} else {
		dice = DieN(6);
	}

	points = 0;
	
	switch(dice)
	{
		case 1:
			for(i = 0; i < 3; i++)
			{
				points+= DieN(4);
			}
			cout<<"Player gains "<<points<<" RB/MP and GR: Drooling."<<endl;
			break;
			
		case 2:
			for(i = 0; i < 3; i++)
			{
				points+= DieN(4);
			}
			cout<<"Player gains "<<points<<" RB/BP and GR: Baby Fat."<<endl;
		break;
		
		case 3:
			for(i = 0; i < 3; i++)
			{
				points+= DieN(4);
			}
			cout<<"Player gains "<<points<<" RB/EP";
			
			extra = DieN(100);
			
			if(extra % 2 == 0){
				cout<<" and 1 WUL";
			}
			if(extra % 3 == 0){
				cout<<" and 1 MUL.";
			}
			if((extra % 2 != 0)&&(extra % 3 != 0)){
				cout<<" and 2 UC."<<endl;
				
				rollCount = 2;
				UCprompt();
			} else {
			cout<<endl;
			}
		break;
		
		case 4:
			points = DieN(6) + 4;
			
			cout<<"Player gains "<<points<<" RB and diaper alters into a Potions Diaper."<<endl;
		break;
		
		case 5:
			for(i = 0; i < 3; i++)
			{
				points+= DieN(4);
			}
			cout<<"Player gains "<<points<<" RB/BP and GR: Teenified."<<endl;
		break;
		
		case 6:
			points += DieN(5) + 5;
			
			cout<<"Player gains "<<points<<" RB and gains a random Ghost Regression from their Resonance."<<endl;
		break;
		
		case 7:
			for(i = 0; i < 3; i++)
			{
				points+= DieN(4);
			}
			cout<<"Player gains "<<points<<" RB/BP and Sleepier."<<endl;
		break;
		
		case 8:
			cout<<"Player gains brainwashed. If this is stacked, gain 20 RB instead."<<endl;
		break;
		
		case 9:
			cout<<"Rolled 9! Generating two effects. Player gains 1 stack of Enhancement."<<endl;
			repeat = 3;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		
	}
	
	if(repeat > 0){
		repeat -= 1;
	}
	
}while(repeat > 0);

	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void Presentation(){								//Presentation; last updated: 4.0
	int inten;
	int total;
	
	perPlayer = true;
	system("cls");

	cout<<"PRESENTATION"<<endl;
	tRoll = TrapRoll();
	
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"PRESENTATION"<<endl;
	
	inten = -1;
	
	switch(tRoll){
		case 1:
		cout<<"Inflicts Pacified and GR: Diaper Lover. Roll Small IT for RB/MP."<<endl;
		inten = 1;
		break;
		
		case 2:
		cout<<"Inflicts Pacified and GR: Diaper Lover. Tiny IT for RB/MP."<<endl;
		inten = 0;
		break;
		
		case 3:
		cout<<"Inflicts Pacified and GR: Diaper Lover."<<endl;
		break;
		
		case 4:
		cout<<"Inflicts GR: Diaper Lover."<<endl;
		break;
		
		case 5:
		cout<<"Inflicts GR: Diaper Trained."<<endl;
		break;
		
		case 6:
		cout<<"No effect (ignored successfully)."<<endl;
		break;
		
		default:
		cout<<"ERROR. Invalid Trap Roll."<<endl;
		break;	
	}
	
	if(inten >= 0){
		Itotal = Intensity(inten);
		cout<<"Intensity roll of "<<Itotal<<" RB/MP."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 
 }
 
 
 
void AlarmTrap(){									//Alarm Trap; last updated: 4.0
	int Care[3];
	CareGenerator(Care, true);
	system("cls");
	cout<<"ALARM TRAP"<<endl;
	cout<<"This room is occupied by..."<<endl;
	cout<<"Tier 0 Caretaker: #"<<Care[0]<<endl;
	cout<<"Tier 0-1 Caretaker: #"<<Care[1]<<endl;
	cout<<"Tier 0-2 Caretaker: #"<<Care[2]<<endl<<endl;
	
	cout<<"This Caretaker has a bonus of +1 to Attack Rolls, Attack Damage, and Armor Class for 5 turns."<<endl;
	
	cout<<"This Room generates no Scrolls."<<endl<<endl;
	
	cout<<"Press [Enter] to return to main menu.";
	getch();
 }
 
 
 
void PacifierDoor(){								//Pacifier Door; last updated: 4.0
	int doubler = 0;
	int dice = 0; //post-modification die roll
	int oldDie;
	int die = 1; //the pre-modified die roll
	bool idle = false;
		
	
	system("cls");
	perPlayer = true;
while(perPlayer){
	cout<<"PACIFIER DOOR"<<endl;
do{
	if(doubler > 0){ //if multi-rolling from repeat roll
		oldDie = dice;
		
		if(oldDie != 5){ //if the second reroll of the 2 effects
		
			die = DieN(4);
		
			switch(oldDie){ //prevent repeat of same effect
				case 1: //if first was 1, increase by 1 since can't be 1
				dice = die + 1;
				break;
				
				case 2: //if first was 2 and second is not 1, bump up one
				if(die != 1){
					dice = die + 1;
				}
				break;
				
				case 3: //if first was 3 and second is 3, bump up one
				if(die == 3){
					dice = 4;
				}
				break;
				
				case 4: //if first was 4, no change necessary so pass on
				dice = die;
				break;
				
				default:
				cout<<"ERROR with generating reroll"<<endl;
			}
		
		} else { //if the first reroll from rolling the 2 effects
			dice = DieN(4);
		}
		
	} else { //doing the first roll
		dice = DieN(5);
	}
	
	switch(dice)
	{
		case 1:
			cout<<"Tiny IT for RB. Player gains Pacifier Gag."<<endl;
			Itotal = Intensity(0);
			cout<<"Player gains "<<Itotal<<" RB."<<endl;
		break;
		
		case 2:
			cout<<"Tiny IT for RB. Player rolls Use Checks at minimum Potty Limit until diaper is used."<<endl;
			Itotal = Intensity(0);
			cout<<"Player gains "<<Itotal<<" RB."<<endl;
			rollUntil = true;
			UCprompt();
		break;
		
		case 3:
			cout<<"GR: Drooling. 2 Idle Rolls for all players (does not stack)."<<endl;
			if(!idle){
				rollCount = 2;
				IdleRolls();
			} else {
				cout<<"Idle Rolls do not repeat, can only roll them once."<<endl;
			}
		break;
		
		case 4:
			cout<<"Small IT for RB and diaper alters into a Pacifier Diaper."<<endl;
			Itotal = Intensity(0);
			cout<<"Player gains "<<Itotal<<" RB."<<endl;
		break;
		
		case 5:
			doubler = 3;
			cout<<"Rolled a 5! Generating two effects.";
			if(DieN(100) > 50)
			{
				cout<<" Additionally, this player is Stupefied.";
			}
			cout<<endl<<endl<<endl;
		break;
		
		default:
			cout<<"ERROR IN POTIONS"<<endl;
		
	}
	
	if(doubler > 0)
	{
		doubler -= 1;
		
	}
	if(doubler == 1)
	{
		cout<<endl;
	}
}while(doubler > 0);

	giveScroll();
	
	perPlayer = YesNo();
	system("cls");
}
 }
 
 
 
void WaterGun(){									//Water Gun Turret; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"WATER GUN TURRET"<<endl;
	
	tRoll = TrapRoll();
	
	if(tRoll <= 2){
		cout<<"Player's diaper becomes Bogged."<<endl;
	} else if(tRoll == 3){
		if(DieN(100)>50){
			cout<<"Player diaper becomes Bogged."<<endl;
		} else {
			cout<<"Player avoided Bogged."<<endl;
		}
	} else if(tRoll == 4){
		if(DieN(100)>75){
			cout<<"Player diaper becomes Bogged."<<endl;
		} else {
			cout<<"Player avoided Bogged."<<endl;
		}
	}
	
	switch(tRoll)
	{
		case 1:
		cout<<"Large Intensity for EP. 1 Use Check for WHP at 2 higher Tiers."<<endl;
		Itotal = Intensity(3);
		rollCount = 1;
		UCTauto = 2;
		break;
		
		case 2:
		cout<<"Normal Intensity for EP. 1 Use Check for WHP at 1 higher Tier."<<endl;
		Itotal = Intensity(2);
		rollCount = 1;
		UCTauto = 1;
		break;
		
		case 3:
		cout<<"Small Intensity for EP. 1 Use Check for only WHP."<<endl;
		Itotal = Intensity(1);
		rollCount = 1;
		break;
		
		case 4:
		cout<<"Tiny Intensity for EP. 1 Use Check for only WHP."<<endl;
		Itotal = Intensity(0);
		rollCount = 1;
		break;
		
		case 5:
		cout<<"1 Use Check for WHP, gain half total. Tiny Intensity for EP at half total."<<endl;
		Itotal = Intensity(0) / 2;
		rollCount = 1;
		break;
		
		case 6:
		cout<<"No effect, dodged."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
	}
	
	if(rollCount)
	{
		cout<<"Player gains "<<Itotal<<" EP."<<endl;
		UCprompt();
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void MusicRoom(){									//Music Room; last updated: 4.0
	perPlayer = true;
	
	system("cls");

	cout<<"MUSIC ROOM"<<endl;
	tRoll = TrapRoll();
	
while(perPlayer)
{
	cout<<"MUSIC ROOM"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"Minimum Potty Limit until exiting room. Large Intensity for RB/BP. Stupefied."<<endl;
		Itotal = Intensity(3);
		break;
		
		case 2:
		cout<<"Potty Limit -10 until exiting room. Normal Intensity for RB/BP. GR: Absentminded."<<endl;
		Itotal = Intensity(2);
		break;
		
		case 3:
		cout<<"Potty Limit -7 until exiting room. Normal Intensity for RB/BP. GR: Absentminded."<<endl;
		Itotal = Intensity(2);
		break;
		
		case 4:
		cout<<"Potty Limit -5 until exiting room. Small Intensity for RB/BP. GR: Absentminded."<<endl;
		Itotal = Intensity(1);
		break;
		
		case 5:
		cout<<"Potty Limit -3 until exiting room. Tiny Intensity for RB/BP. GR: Absentminded."<<endl;
		Itotal = Intensity(0);
		break;
		
		case 6:
		cout<<"Potty Limit -2 until exiting room. Tiny Intensity for RB/BP at 1/2 total."<<endl;
		Itotal = Intensity(0) / 2;
		break;
	}
	
	cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
	
	giveScroll();
	
	perPlayer = YesNo();
	system("cls");
}
 }
 
 
 
void SleepyRoom(){									//Sleepy Room; last updated: 4.0
	perPlayer = true;
	
	system("cls");
	cout<<"SLEEPY ROOM"<<endl;
	tRoll = TrapRoll();
while(perPlayer)
{
	system("cls");
	cout<<"SLEEPY ROOM"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"Diaper alters into Overnight before further effect. Roll 3 Idle Rolls."<<endl;
		rollCount = 3;
		break;
		
		case 2:
		cout<<"Diaper alters into Overnight before further effect. Roll 2 Idle Rolls."<<endl;
		rollCount = 2;
		break;
		
		case 3:
		cout<<"Diaper alters into Overnight before further effect. Roll 1 Idle Roll."<<endl;
		rollCount = 1;
		break;
		
		case 4:
		cout<<"Leave room with Sleepier. 1 Idle Roll."<<endl;
		rollCount = 1;
		break;
		
		case 5:
		cout<<"Leave room with Sleepy."<<endl;
		break;
		
		case 6:
		cout<<"No effect."<<endl;
		break;
	}
	
	if(rollCount)
	{
		IdleRolls();
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void ChangeMachine(){								//Change Machine; last updated: 4.0
	perPlayer = true;
	int odds;
while(perPlayer)
{
	system("cls");
	cout<<"CHANGE MACHINE"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Player is inflicted with Pacifier Gag with Magic Pacifier. ";
		
		case 2:
		if(tRoll == 2){
			cout<<"Player is inflicted with Pacifier Gag. ";
		}
		case 3:
		cout<<"Player is changed into any random diaper."<<endl;
		break;
		
		case 4:
		cout<<"Player is changed into any random diaper."<<endl;
		break;
		
		case 5:
		cout<<"Player is changed into any random diaper."<<endl;
		break;
		
		case 6:
		cout<<"No effect, avoided."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		break;	
	}

	odds = DieN(100);
	
	if(((odds <= 50)&&(tRoll == 4))||((odds <= 25)&&(tRoll == 5))){
		cout<<"Player is changed into a: "; DiaperRoller(1); cout<<" diaper."<<endl;
	} else if (tRoll < 4){
		cout<<"Player is changed into a: "; DiaperRoller(3); cout<<" diaper."<<endl;
	} else {
		cout<<"Player avoids a diaper change."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void OddMirror(){									//Odd Mirror; last updated: 4.0
	perPlayer = true;
	
	system("cls");
	cout<<"ODD MIRROR"<<endl;
	
	tRoll = TrapRoll();
	
while(perPlayer)
{
	system("cls");
	cout<<"ODD MIRROR"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"2 Use Checks. 20 RB. 2 random Ghost Regressions from Resonance. Stupefied."<<endl;
		rollCount = 2;
		break;
		
		case 2:
		cout<<"2 Use Checks. 10 RB. 1 random Ghost Regression."<<endl;
		rollCount = 2;
		break;
		
		case 3:
		cout<<"1 Use Check. 10 RB. 1 random Ghost Regression."<<endl;
		rollCount = 1;
		break;
		
		case 4:
		cout<<"50% chance of 1 Use Check and/or Ghost Regression from Resonance. 5 RB."<<endl;
		
		if(DieN(100) > 50)
		{
			cout<<"Player rolls 1 UC. ";
			rollCount = 1;
		}
		if(DieN(100) > 50)
		{
			cout<<"Player rolls for 1 Ghost Regression from Resonance.";
		}
		cout<<endl;
		
		break;
		
		case 5:
		cout<<"5 RB."<<endl;
		break;
		
		case 6:
		cout<<"No effect, ignored."<<endl;
	}
	
	if(rollCount){
		UCprompt();
	}
	
	giveScroll();
	
	if(tRoll < 5){
		perPlayer = YesNo();
	} else {
		cout<<"Press [Enter] to return to main menu."<<endl;
		getch();
	}
}
 }
 
 
 
void DimWand(){										//Dimensional Wand; last updated: 4.0
	perPlayer = true;
	int GRs;
	cout<<"DIMENSIONAL WAND"<<endl;
	tRoll = TrapRoll();
while(perPlayer)
{
	GRs = 0;
	system("cls");
	cout<<"DIMENSIONAL WAND"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"5 Idle Rolls. Random non-Trap diaper change and Ghost Regression. GR: Infantile. Stupefied."<<endl;
		cout<<"Player is changed into a "; DiaperRoller(1); cout<<" diaper."<<endl;
		rollCount = 5;
		GRs = 1;
		break;
		
		case 2:
		cout<<"3 Idle Rolls. Random non-Trap diaper change and Ghost Regression. GR: Infantile. Stupefied."<<endl;
		cout<<"Player is changed into a "; DiaperRoller(1); cout<<" diaper."<<endl;
		rollCount = 3;
		GRs = 1;
		break;
		
		case 3:
		cout<<"3 Idle Rolls. Random diaper change. GR: Immature. Stupefied."<<endl;
		cout<<"Player is changed into a "; DiaperRoller(1); cout<<" diaper."<<endl;
		rollCount = 3;
		break;
		
		case 4:
		cout<<"2 Idle Rolls. Random diaper change. GR: Immature."<<endl;
		cout<<"Player is changed into a "; DiaperRoller(1); cout<<" diaper."<<endl;
		rollCount = 2;
		break;
		
		case 5:
		cout<<"1 Idle Roll. GR: Drooling."<<endl;
		rollCount = 1;
		break;
		
		case 6:
		cout<<"No effect, dodged."<<endl;
		break;
	}
	
	if(rollCount)
	{
		IdleRolls();
	}
	if(GRs)
	{
		for(int i = 0; i < GRs; i++)
		{
			randomGR(1);
		}
	}
	
	giveScroll();
	
	if(tRoll != 6){
		perPlayer = YesNo();
	} else {
		cout<<"Press [Enter] to return to main menu."<<endl;
		getch();
	}
}
 }
 
 
 
void DrainDiaper(){									//Draining Diaper Trap; last updated: 4.0
	perPlayer = true;
	system("cls");
	cout<<"DRAINING DIAPER TRAP"<<endl;
	
	tRoll = TrapRoll();

while(perPlayer)
{
	system("cls");
	cout<<"DRAINING DIAPER TRAP"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"Changed into a random Trap Diaper with a 5 Idle Roll Diaper Lock."<<endl;
		break;
		
		case 2:
		cout<<"Changed into a random Trap Diaper with a 4 Idle Roll Diaper Lock."<<endl;
		break;
		
		case 3:
		cout<<"Changed into a random Trap Diaper."<<endl;
		break;
		
		case 4:
		cout<<"Changed into a random Trap Diaper."<<endl;
		break;
		
		case 5:
		cout<<"Changed into a random Trap Diaper with a 2 Idle Roll Diaper Lock."<<endl;
		break;
		
		case 6:
		cout<<"Changed into a random Trap Diaper with a 1 Idle Roll Diaper Lock."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void BabyVG(){										//Baby Video Game Room; last updated: 4.0
	perPlayer = true;
	int rolls;
	int penalty;
	int pValue;
	int dice;

	
	system("cls");
	cout<<"BABY VIDEO GAME ROOM"<<endl;
	tRoll = TrapRoll();
	
while(perPlayer)
{
	rolls = 0;
	penalty = 0;
	pValue = 0;

	system("cls");
	cout<<"BABY VIDEO GAME ROOM"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"3 Idle Rolls. RB/MP +5 per hit, roll 1d4 + 4 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 4;
		pValue = 5;
		break;
		
		case 2:
		cout<<"3 Idle Rolls. RB/MP +5 per hit, roll 1d4 + 2 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 2;
		pValue = 5;
		break;
		
		case 3:
		cout<<"2 Idle Rolls. RB/MP + 4 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 4;
		break;
		
		case 4:
		cout<<"2 Idle Rolls. RB/MP +3 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 3;
		break;
		
		case 5:
		cout<<"1 Idle Roll. RB/MP +4 per hit, 1d2 for hits."<<endl;
		rollCount = 1;
		rolls = 2;
		penalty = 0;
		pValue = 4;
		break;
		
		case 6:
		cout<<"1 Idle Roll. No hits taken."<<endl;
		rollCount = 1;
		rolls = 0;
		break;
	}
	
	if(rollCount)
	{
		status = true;
		IdleRolls();
	}
	
	if(rolls){
		dice = DieN(rolls) + penalty;
		
		cout<<"Player gains "<<(dice * pValue)<<" RB/MP."<<endl;
	}
	
	if(tRoll < 4){
		cout<<"Diaper altered into Progress Bar Diaper."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void DiaperVG(){									//Diaper Video Game Room; last updated: 4.0
	perPlayer = true;
	int rolls;
	int penalty;
	int pValue;
	int Upoints;
	int dice;

	
	system("cls");
	cout<<"DIAPER VIDEO GAME ROOM"<<endl;
	tRoll = TrapRoll();
	
while(perPlayer)
{
	rolls = 0;
	penalty = 0;
	pValue = 0;
	Upoints = 3;

	system("cls");
	cout<<"DIAPER VIDEO GAME ROOM"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"3 Idle Rolls. RB/EP +5 and UHP +3 per hit, roll 1d4 + 4 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 4;
		pValue = 5;
		break;
		
		case 2:
		cout<<"3 Idle Rolls. RB/EP +5 and UHP +3 per hit, roll 1d4 + 2 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 2;
		pValue = 5;
		break;
		
		case 3:
		cout<<"2 Idle Rolls. RB/EP + 4 and UHP +3 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 4;
		break;
		
		case 4:
		cout<<"2 Idle Rolls. RB/EP +3 and UHP +2 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 3;
		Upoints = 2;
		break;
		
		case 5:
		cout<<"1 Idle Roll. RB/EP +4 and UHP +3 per hit, 1d2 for hits."<<endl;
		rollCount = 1;
		rolls = 2;
		penalty = 0;
		pValue = 4;
		break;
		
		case 6:
		cout<<"1 Idle Roll. No hits taken."<<endl;
		rollCount = 1;
		rolls = 0;
		break;
	}
	
	if(rollCount)
	{
		status = true;
		IdleRolls();
		cout<<endl;
	}
	
	if(rolls){
		dice = DieN(rolls) + penalty;
		
		cout<<"Player gains "<<(dice * pValue)<<" RB/EP and "<<(dice * Upoints)<<" UHP."<<endl;
	}
	
	if(tRoll < 4)
	{
		cout<<"Diaper altered into Progress Bar Diaper."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void DragonVG(){									//Dragon Video Game Room; last updated: 4.0
	perPlayer = true;
	int rolls;
	int penalty;
	int pValue;
	int dice = 1;
	const int LOW = 1;

	
	system("cls");
	cout<<"BABY VIDEO GAME ROOM"<<endl;
	tRoll = TrapRoll();
	
while(perPlayer)
{
	rolls = 0;
	penalty = 0;
	pValue = 0;

	system("cls");
	cout<<"BABY VIDEO GAME ROOM"<<endl;
	
	switch(tRoll)
	{
		case 1:
		cout<<"3 Idle Rolls. RB/BP +5 per hit, roll 1d4 + 4 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 4;
		pValue = 5;
		break;
		
		case 2:
		cout<<"3 Idle Rolls. RB/BP +5 per hit, roll 1d4 + 2 for hits."<<endl;
		rollCount = 3;
		rolls = 4;
		penalty = 2;
		pValue = 5;
		break;
		
		case 3:
		cout<<"2 Idle Rolls. RB/BP + 4 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 4;
		break;
		
		case 4:
		cout<<"2 Idle Rolls. RB/BP +3 per hit, 1d2 + 2 for hits."<<endl;
		rollCount = 2;
		rolls = 2;
		penalty = 2;
		pValue = 3;
		break;
		
		case 5:
		cout<<"1 Idle Roll. RB/BP +4 per hit, 1d2 for hits."<<endl;
		rollCount = 1;
		rolls = 2;
		penalty = 0;
		pValue = 4;
		break;
		
		case 6:
		cout<<"1 Idle Roll. No hits taken."<<endl;
		rollCount = 1;
		rolls = 0;
		break;
	}
	
	if(rollCount)
	{
		status = true;
		cout<<"Press [Enter] to begin Idle Roll."<<endl;
		IdleRolls();
	}
	
	if(rolls){	
		dice = DieN(rolls) + penalty;
		
		cout<<"Player gains "<<(dice * pValue)<<" RB/BP."<<endl;
	}
	if(tRoll < 4){
		cout<<"Diaper altered into Progress Bar Diaper."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void LockDoor(){									//Lock Door; last updated: 4.0
	perPlayer = true;
	int counter;

	int die;
	int dice;
	int rounding;
	system("cls");
	cout<<"Lock Door"<<endl;
	
	tRoll = TrapRoll();
	
while(perPlayer)
{
	counter = 0;
	rounding = 0;
	system("cls");
	cout<<"Lock Door"<<endl;
	
	if(tRoll < 6){
		cout<<"Diaper Lock";
	}
	if(tRoll < 4){
		cout<<". ";
	}
	
	switch(tRoll)
	{
		case 1:
		counter = 4;
		break;
		
		case 2:
		counter = 3;
		break;
		
		case 3:
		counter = 2;
		break;
		
		case 4:
		cout<<" for 2 Idle Rolls. ";
		counter = 1;
		break;
		
		case 5:
		cout<<" for 1 Idle Roll. ";
		counter = -1;
		break;
		
		case 6:
		cout<<"No effect, first key was the right one."<<endl;
		break;
	}
	
if(tRoll != 6)
{
	if((counter % 2 != 0)&&(counter > 0))
	{
		rounding = 1;
	}
		
	dice = DieN(3);
	
	if(counter == 0)
	{
		dice = 3;
		rounding = 1;
	} else if(counter == -1){
		
		if(DieN(100) > 50)
		{
			dice = 3;
			rounding = 1;
		} else {
			dice = 4;
		}
	}
	
	switch(dice)
	{
		case 1:
		cout<<"Player rolls "<<counter<<" Wet Use Checks."<<endl;
		rollCount = counter;
		break;
		
		case 2:
		cout<<"Player rolls "<<counter<<" Mess Use Checks."<<endl;
		rollCount = counter;
		break;
		
		case 3:
		rollCount = counter / 2 + rounding;
		cout<<"Player rolls "<<rollCount<<" Use Checks."<<endl;
		break;
		
		case 4:
		cout<<"Player does not make any additional rolls."<<endl;
		break;
		
		default:
		cout<<"ERROR. Dice generated wrong."<<endl;
		break;
	}
	
	if(rollCount)
	{
		UCprompt();
	}
}

	giveScroll();

	perPlayer = YesNo();
}
 }
 
 
 
void TickleMachine(){								//Tickling Machine; last updated: 4.0
	perPlayer = true;
	int inten;
while(perPlayer)
{
	inten = 0;
	system("cls");
	cout<<"TICKLING MACHINE"<<endl;
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Sillier and Normal IT for RB. Minimum Potty Limit until exiting trap."<<endl;
		inten = 2;
		break;
		
		case 2:
		cout<<"Sillier and Small IT for RB. Minimum Potty Limit until exiting trap."<<endl;
		inten = 1;
		break;
		
		case 3:
		cout<<"Silly and Small IT for RB. Potty Limit -10 until exiting trap."<<endl;
		inten = 1;
		break;
		
		case 4:
		cout<<"Silly and Tiny IT for RB. Potty Limit -8 until exiting trap."<<endl;
		inten = 0;
		break;
		
		case 5:
		cout<<"Tiny IT for RB. Potty Limit -5 until exiting trap."<<endl;
		inten = 0;
		break;
		
		case 6:
		cout<<"No effect, avoided."<<endl;
		break;
		
		default:
		cout<<"ERROR, trap roll broken."<<endl;
		break;
	}
	
	if(tRoll != 6){
		Itotal = Intensity(inten);
		
		cout<<"Player gains "<<Itotal<<" RB."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void Energy(){										//Energy Room; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"ENERGY ROOM"<<endl;
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Player gains Skewed Aura and rolls 1 Forced Idle Roll at 2 higher Tiers (auto-calculated)."<<endl;
		FIRauto = 2;
		ForcedIR();
		break;
		
		case 2:
		Itotal = Intensity(0);
		cout<<"Player gains Skewed Aura and rolls 1 Forced Idle Roll."<<endl;
		ForcedIR();
		break;
		
		case 3:
		cout<<"Player gains Skewed Aura."<<endl;
		break;
		
		case 4:
		cout<<"Player gains GR: Depowered."<<endl;
		break;
		
		case 5:
		if(DieN(100) > 50)
		{
			cout<<"Player gains GR: Depowered."<<endl;
		} else {
			cout<<"No effect, resisted."<<endl;
		}
		break;
		
		case 6:
		cout<<"No effect, resisted."<<endl;
		break;	
		
		default:
		cout<<"ERROR"<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void BossBomb(){									//Boss Bomb; last updated: 4.0
	perPlayer = true;
	
	int die;
	int dTotal;
	int maxDie;
	
	system("cls");
	cout<<"BOSS BOMB"<<endl;
	
	tRoll = TrapRoll();
	
while(perPlayer)
{
	dTotal = 10;
	system("cls");
	cout<<"BOSS BOMB"<<endl;
	
	switch(tRoll)
	{
		case 1:
		maxDie = 20;
		cout<<"Critical failure"<<endl;
		break;
		
		case 2:
		maxDie = 15;
		cout<<"Heavy failure"<<endl;
		break;
		
		case 3:
		maxDie = 10;
		cout<<"Expected failure"<<endl;
		break;
		
		case 4:
		maxDie = 10;
		cout<<"Resisted"<<endl;
		break;
		
		case 5:
		maxDie = 5;
		cout<<"Partial success."<<endl;
		break;	
		
		case 6:
		cout<<"Full success. No effect, realized fake."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
	}
	
	if(tRoll != 6)
	{
		for(int i=0; i < 2; i++){
			dTotal += DieN(maxDie);
		}
		
		switch(tRoll){
			case 1:
			cout<<"Player gains 2 Use Levels in both categories."<<endl;
			break;
			
			case 2:
			cout<<"Player gains 1 Use Level in both categories."<<endl;
			break;
			
			case 3:
			if(DieN(4) <= 2)
			{
				cout<<"Player gains 1 Wet Use Level."<<endl;
			} else {
				cout<<"Player gains 1 Mess Use Level."<<endl;
			}
			break;
			
			case 4:
			case 5:
			rollCount = 1;
			UCprompt();
			break;
			
			default:
			cout<<"USE ERROR"<<endl;
			break;
		}
		
		if((tRoll < 4)||((DieN(100) <= 50)&&(tRoll == 4))||((DieN(100) <= 25)&&(tRoll == 5))){
			cout<<"Party is Stupefied (please reference only the first generation for a party)."<<endl;
		} else {
			cout<<"No Stupefied status to party."<<endl;
		}
		
		cout<<"Player gains "<<dTotal<<" RB."<<endl;	
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void Ambush(){										//Ambush; last updated: 4.0
	int Care[3];
	CareGenerator(Care, true);
	
	system("cls");
	cout<<"AMBUSH"<<endl<<endl;
	
	cout<<"The party is ambushed by..."<<endl;
	cout<<"Tier 0 Caretaker: #"<<Care[0]<<endl;
	cout<<"Tier 1 Caretaker: #"<<Care[1]<<endl;
	cout<<"Tier 2 Caretaker: #"<<Care[2]<<endl<<endl;
	
	cout<<"Roll to attack up to two players as if Caretaker is attacking both. Caretaker will be the last to move when combat begins."<<endl;
	cout<<"Combat MUST be engaged in this Encounter. Forfeits are allowed on turn 1."<<endl;
	cout<<"This Room generates no Scrolls."<<endl<<endl;
	
	cout<<"Press [Enter] to return to main menu."<<endl;
	getch();
 }
 
 
 
void PlushPile(){									//Plushie Pile; last updated: 4.0
	int inten;

	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"PLUSHIE PILE"<<endl;
	
	tRoll = TrapRoll();
	
	inten = -1;
	
	switch(tRoll)
	{
		case 1:
		cout<<"Advocate for 2 Idle Rolls and GR: Snuggler. 1 Idle Roll (also inflicts EP equal to RB)."<<endl;
		rollCount = 1;
		IdleRolls();
		break;
		
		case 2:
		cout<<"Advocate for 1 Idle Roll and GR: Snuggler. Small Intensity for RB/BP."<<endl;
		inten = 1;
		break;
		
		case 3:
		cout<<"Advocate for 1 Idle Roll and GR: Snuggler. Small Intensity for RB/BP."<<endl;
		inten = 1;
		break;
		
		case 4:
		cout<<"GR: Snuggler for 2 Idle Rolls. Small Intensity for RB/EP."<<endl;
		inten = 1;
		break;
		
		case 5:
		cout<<"GR: Snuggler for 1 Idle Roll. Tiny Intensity for RB/EP."<<endl;
		inten = 0;
		break;
		
		case 6:
		cout<<"No effect, resisted."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		break;
	}
	
	if(inten >= 0){
		Itotal = Intensity(inten);
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void PadSuit(){									//Themed Suit Ambush; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"PADDED SUIT AMBUSH"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Hypno Suit AND Bondage Suit. Diaper Lock."<<endl;
		break;
		
		case 2:
		cout<<"Hypno Suit AND Bondage Suit."<<endl;
		break;
		
		case 3:
		if(DieN(100) > 50)
		{
			cout<<"Gain Hypno Suit."<<endl;
		} else {
			cout<<"Gain Bondage Suit."<<endl;
		}
		break;
		
		case 4:
		if(DieN(100) > 50)
		{
			cout<<"Gain Hypno Suit ";
		} else {
			cout<<"Gain Bondage Suit ";
		}
		cout<<" for 2 Idle Rolls."<<endl;
		break;
		
		case 5:
		if(DieN(100) > 50)
		{
			cout<<"Gain Hypno Suit ";
		} else {
			cout<<"Gain Bondage Suit ";
		}
		cout<<" for 1 Idle Roll."<<endl;
		break;
		
		case 6:
		cout<<"No effect, dodged."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
	
	
	
void CareSpecial(){									//Caretaker's Special Room; last updated: 4.0
	int dice[3];
	int TrapRoom;

	system("cls");
	cout<<"CARETAKER'S SPECIAL ROOM"<<endl<<endl;
	
	do{
		CareGenerator(dice, true);
		
	}while((dice[0] == 7)||(dice[1] == 7)||(dice[2] == 7));
	
	cout<<"This room is occupied by..."<<endl;
	cout<<"Tier 0 Caretaker: #"<<dice[0]<<endl;
	cout<<"Tier 0-1 Caretaker: #"<<dice[1]<<endl;
	cout<<"Tier 0-2 Caretaker: #"<<dice[2]<<endl;
	
	do{
		//Trap Artist roll
		TrapRoom = TrapArtist();
	}while(TrapRoom == 1);
	
	cout<<"...in a Trap Artist room of #"<<TrapRoom<<endl;
	cout<<"This Room generates no Scrolls."<<endl<<endl;
	
	cout<<endl<<"Press [Enter] to return to main menu."<<endl;
	getch();
 }
 
 
 
void Ritual(){										//Baby Ritual; last updated: 4.0
	int ro[3];
	ro[0] = 0;
	ro[1] = 0;
	ro[2] = 0;
	int dice;
	int i;

	system("cls");
	cout<<"BABY RITUAL"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		for(i = 0; i < 3; i++)
		{
			ro[i] = Misfire(false);
			
			if(((i == 1)&&(ro[1] == ro[0]))||((i == 2)&&(ro[2] == ro[1])))
			{
				i--;
			}
		}
		cout<<"Magical misfire rolls of: #"<<ro[0]<<", #"<<ro[1]<<", and #"<<ro[2]<<endl;
		cout<<"These are "<<MisfirePrint(ro[0])<<", "<<MisfirePrint(ro[1])<<", and "<<MisfirePrint(ro[2])<<endl;
		break;
		
		
		case 2:
		for(i = 0; i < 2; i++)
		{
			ro[i] = Misfire(false);
			
			if((i == 1)&&(ro[1] == ro[0]))
			{
				i--;
			}
		}
		cout<<"Magical misfire rolls of: #"<<ro[0]<<"and #"<<ro[1]<<endl;
		cout<<"These are "<<MisfirePrint(ro[0])<<" and "<<MisfirePrint(ro[1])<<endl;
		break;
		
		
		case 3:
		ro[0] = Misfire(false);
		cout<<"Magical misfire roll of: #"<<ro[0]<<endl;
		cout<<"This is "<<MisfirePrint(ro[0])<<"."<<endl;
		break;
		
		
		case 4:
		dice = DieN(20) + 3;
		ro[0] = Misfire(false);
		
		cout<<"Calculate hit/miss on players with an AR of "<<dice<<" and magical misfire roll of #"<<ro[0]<<endl;
		cout<<"This is "<<MisfirePrint(ro[0])<<"."<<endl;
		break;
		
		
		case 5:
		cout<<"No effect, symbol disrupted."<<endl;
		break;
		
		
		case 6:
		cout<<"Ritual hijacked, party gain 2 scrolls of choice!"<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		break;
	}
	
	cout<<"Press [Enter] to return to main menu."<<endl;
	getch();
	
 }
 
 
 
void ArtRoom(){										//Smeargle's Art Room; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"SMEARGLE'S ART ROOM"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		Itotal = Intensity(1);
		
		cout<<"Inflicts Pinkified or Powdered for 4 Idle Rolls (player choice)."<<endl;
		cout<<"Small Intensity roll of "<<Itotal<<" RB/BP/EP."<<endl;
		break;
		
		case 2:
		Itotal = Intensity(0);
		
		cout<<"Inflicts Pinkified or Powdered (player choice)."<<endl;
		cout<<"Tiny Intensity roll of "<<Itotal<<" RB/BP/EP."<<endl;
		break;
		
		case 3:
		cout<<"Inflicts Pinkified or Powdered (player choice)."<<endl;
		break;
		
		case 4:
		cout<<"Inflicts Pinkified or Powdered for 2 IR (player choice)."<<endl;
		break;
		
		case 5:
		cout<<"Inflicts Pinkified or Powdered for 1 IR(player choice)."<<endl;
		break;
		
		case 6:
		cout<<"Players are inspired by the art for "<<(DieN(3) + DieN(3))<<" RB/BP/EP."<<endl;
		break;
		
		default:
		cout<<"ERROR."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void TagAlong(){									//Mimikyu's Tag-Along; last updated: 4.0
	system("cls");
	cout<<"MIMIKYU'S TAG-ALONG"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Lowest RB player suffers Mimikyu Plush for 4 IR. Small Intensity for RB to all players (use worst IT bonus)."<<endl;
		Itotal = Intensity(1);
		cout<<"Players rolled "<<Itotal<<" RB."<<endl;
		break;
		
		case 2:
		cout<<"Lowest RB player suffers Mimikyu Plush. Tiny IT for RB to all players (use worst IT bonus)."<<endl;
		Itotal = Intensity(0);
		cout<<"Players rolled "<<Itotal<<" RB."<<endl;
		break;
		
		case 3:
		cout<<"Lowest RB player suffers Mimikyu Plush."<<endl;
		break;
		
		case 4:
		cout<<"Random player suffers Mimikyu Plush for 2 Idle Rolls."<<endl;
		break;
		
		case 5:
		cout<<"Random player suffers Mimikyu Plush for 1 Idle Rolls."<<endl;
		break;
		
		case 6:
		cout<<"No effect, ignored."<<endl;
		break;
		
		default:
		cout<<"ERROR"<<endl;
		break;
	}
	
	giveScroll();
	
	cout<<endl<<"Press [Enter] to return to main menu.";
	getch();
	
 }
 
 
 
void Storeroom(){									//Item Room; last updated: 4.0
	int curses = DieN(3) - 1;
	bool isCursed = false;
	bool isDupe = false;
	int itemCount = DieN(3) + 1;
	int items[4];
	items[2] = 0;
	items[3] = 0;
	
	system("cls");
	cout<<"ITEM ROOM"<<endl<<endl;
	
	/*const int foodItemMax = 6; //RNG max of food items
	const int useItemMax = 18; //RNG max of use items
	const int itemMax = 30; //RNG max of cursed items
	const int useSpread = useItemMax - foodItemMax; //auto-calculate the use item spread
	const int cursedSpread = itemMax - useItemMax; //auto-calculate the cursed item spread*/

	cout<<"This item room contains..."<<endl;

	for(int i = 0; i < itemCount; i++){ //assign items
		if(curses){
			items[i] = DieN(itemMax);
			if(items[i] > useItemMax){
				isCursed = true;
			} else {
				isCursed = false;
			}
		} else {
			items[i] = DieN(useItemMax);
		}
		
		for(int j = 0; j < i; j++){ //check for duplicate items
			if(items[j] == items[i]){
				i--;
				isDupe = true;
				break;
			} else {
				isDupe = false;
			}
		}
		if(!isDupe){
			FullItemPrint(items[i]);
			cout<<endl;
			
			if(isCursed){
				curses--;
			}
		}
	}
	
	cout<<endl<<"This Room generates no Scrolls."<<endl;
	
	cout<<endl<<"Press [Enter] to return to main menu.";
	getch();
 }
 
 
 
void TrainPotty(){									//Training Potties; last updated: 4.0
	int dice;
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"TRAINING POTTIES"<<endl;
	cout<<"Warning: program may load in spurts."<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Player gains GR: Quick Filler & Diaper Trained, and 1 Use Level in both categories."<<endl;
		break;
		
		case 2:
		cout<<"Player gains ";
		if(DieN(100) > 50)
		{
			cout<<"GR: Quick Filler";
		} else {
			cout<<"GR: Diaper Trained";
		}
		cout<<" and 1 Use Level in both categories."<<endl;
		break;
		
		case 3:
		cout<<"Player gains ";
		if(DieN(100) > 50)
		{
			cout<<"GR: Quick Filler ";
		} else {
			cout<<"GR: Diaper Trained ";
		}
		cout<<"and ";
		
		if(DieN(100) > 50)
		{
			cout<<"1 Wet Use Level."<<endl;
		} else {
			cout<<"1 Mess Use Level."<<endl;
		}
		break;
		
		case 4:
		cout<<"Player gains GR: Diaper Trained and rolls 1 Use Check."<<endl;
		rollCount = 1;
		UCprompt();
		break;
		
		case 5:
		cout<<"Player gains GR: Diaper Trained for 2 Idle Rolls and rolls 1 Use Check."<<endl;
		rollCount = 1;
		UCprompt();
		break;
		
		case 6:
		cout<<"Player's diaper is mysteriously cleaned to 0 Use Levels."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void ArmyHoard(){									//Plush Army Hoard; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"PLUSH ARMY HOARD"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Player gains Help Hoard, rolls Normal IT for RB/BP/EP, and gains GR: Baby Clothes."<<endl;
		Itotal = Intensity(2);
		break;
		
		case 2:
		cout<<"Player gains Help Hoard, rolls Small IT for RB/BP/EP, and gains GR: Baby Clothes."<<endl;
		Itotal = Intensity(1);
		break;
		
		case 3:
		cout<<"Player gains 2 Helpers, rolls Small IT for RB/BP/EP, and gains GR: Baby Clothes."<<endl;
		Itotal = Intensity(1);
		break;
		
		case 4:
		cout<<"Player gains 2 Helpers, and rolls Tiny IT for RB/BP/EP."<<endl;
		Itotal = Intensity(0);
		break;
		
		case 5:
		cout<<"Player gains 1 Helper, and rolls Tiny IT for RB/BP/EP."<<endl;
		Itotal = Intensity(0);
		break;
		
		case 6:
		cout<<"Player gains 1 Helper, despite their best attempts."<<endl;
		break;
	}
	
	if(tRoll != 6)
	{
		cout<<"Player rolled "<<Itotal<<" for RB/BP/EP."<<endl;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
}
 }
 
 
 
void SeekBottles(){									//Seeking Bottles; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"SEEKING BOTTLES"<<endl;
	
	tRoll = TrapRoll();
	
	switch(tRoll)
	{
		case 1:
		cout<<"Player gains Auto-Bottle with 24 WHP. Inflicts Pacified for the next encounter."<<endl;
		break;
		
		case 2:
		cout<<"Player gains Auto-Bottle with 18 WHP. Inflicts Pacified for the next Encounter."<<endl;
		break;
		
		case 3:
		cout<<"Player gains Auto-Bottle with 18 WHP."<<endl;
		break;
		
		case 4:
		cout<<"Player gains Auto-Bottle with 12 WHP."<<endl;
		break;
		
		case 5:
		cout<<"Player gains Auto-Bottle with ";
		
		cout<<(DieN(8) + 2)<<" WHP."<<endl;
		break;
		
		case 6:
		cout<<"No effect, avoided."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
 }
}



void DivePool(){									//Diving FoY Pool; last updated: 4.0
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"DIVING FOY POOL"<<endl<<endl;
	
	cout<<"Encounter starts with Pool Prep, players may gain Floaties, GR: Life Jacket, or change into a Swim diaper beforehand."<<endl;
	
	do{
		Itotal = Intensity(0);
		
		tRoll = TrapRoll();
		
		certainty = makeSure();
	}while(!certainty);
	
	cout<<"If player is not wearing a Swim diaper, they gain "<<(DieN(20)/10+1)<<" WULs."<<endl;
	
	switch(tRoll)
	{
		case 1:
			cout<<"Bogged."<<endl;
			
			if(DieN(100)>50){
				cout<<"Little'd."<<endl;
			} else {
				cout<<"Teenified."<<endl;
			}
			
			cout<<"Player gains "<<(Itotal + 10)<<" RB/BP."<<endl;
		break;
		
		case 2:
			cout<<"Bogged."<<endl;
		
			if(DieN(100)>75){
				cout<<"Little'd."<<endl;
			} else {
				cout<<"Teenified."<<endl;
			}
			
			cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
		break;
		
		case 3:
			if(DieN(100)>50){
				cout<<"Bogged."<<endl;
			} else {
				cout<<"Player avoided Bogged."<<endl;
			}
			
			cout<<"Teenified."<<endl;
			cout<<"Player gains "<<Itotal<<" RB/BP."<<endl;
		break;
		
		case 4:			
			if(DieN(100)>75){
				cout<<"Bogged."<<endl;
			} else {
				cout<<"Player avoided Bogged."<<endl;
			}
			
			if(DieN(100)>50){
				cout<<"Teenified."<<endl;
			} else {
				cout<<"Player avoided Teenified but instead gains "<<(Itotal + 10)<<" RB/BP."<<endl;
			}			
		break;
		
		case 5:
			if(DieN(100)>50){
				cout<<"Teenified."<<endl;
			} else {
				cout<<"Player avoided Teenified but instead gains "<<Itotal<<" RB/BP."<<endl;
			}
		break;
		
		case 6:
		cout<<"Player "<<Itotal<<" RB/BP."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
 }
}



void SirenSong(){									//Siren's Ballad; last updated: 4.0
	perPlayer = true;
	
while(perPlayer)
{
	system("cls");
	cout<<"SIREN'S BALLAD"<<endl;
	
	cout<<"Encounter starts with Pool Prep, players may inflict Floaties, GR: Life Jacket, or change into a Swim diaper beforehand."<<endl;
	
	do{
		tRoll = TrapRoll();
		Itotal = Intensity(0);
		
		certainty = makeSure();
	}while(!certainty);
	
	cout<<"If player is not wearing a Swim diaper, they gain "<<(DieN(20)/10+1)<<" WULs."<<endl;
	
	if(tRoll < 5){cout<<"Player gains Enlightened."<<endl;}
	
	switch(tRoll)
	{
		case 1:
			randomGR(2);
			cout<<"Player gains "<<(Itotal + 20)<<" RB/MP."<<endl;
		break;
		
		case 2:
			randomGR(2);
			cout<<"Player gains "<<(Itotal + 10)<<" RB/MP."<<endl;
		break;
		
		case 3:
			if(DieN(100)>50){
				randomGR(2);
			} else {
				cout<<"Player avoided the random Mental GR."<<endl;
			}
			cout<<"Player gains "<<(Itotal + 10)<<" RB/MP."<<endl;
		break;
		
		case 4:
		case 5:
			cout<<"Player gains "<<(Itotal + 10)<<" RB/MP."<<endl;
		break;
		
		case 6:
			cout<<"Player gains "<<Itotal<<" RB/MP."<<endl;
		break;
	}
	
	giveScroll();
	
	perPlayer = YesNo();
 }
}



void foyGun(){										//FoY Water Gun Turret; last updated: 4.0
	int die;
	int die2;
	perPlayer = true;
while(perPlayer)
{
	system("cls");
	cout<<"FOY WATER GUN TURRET"<<endl;
	
	tRoll = TrapRoll();
	
	if(tRoll <= 3){
		cout<<"Player gains Little'd."<<endl;
		
	} else if(tRoll == 4){
		if(DieN(100)>50){
			cout<<"Player gains Little'd."<<endl;
		} else {
			cout<<"Player gains Teenified."<<endl;
		}
	} else if(tRoll == 5){
		cout<<"Player gains Teenified."<<endl;
	}
	
	switch(tRoll)
	{
		case 1:
			cout<<"Player gains GR: ";
			
			die = DieN(4);
			//generate first GR
			switch(die){
				case 1:
					cout<<"Immature";
				break;
				case 2:
					cout<<"Baby Logic";
				break;
				case 3:
					cout<<"Vocab Regression";
				break;
				
				case 4:
					cout<<"Self-Distractor";
				break;
			}
			cout<<" and ";
			
			//ensure GRs don't stack for second
			do{
				die2 = DieN(4);
			}while(die == die2);
			
			//generate second GR
			switch(die2){
				case 1:
					cout<<"Immature.";
				break;
				case 2:
					cout<<"Baby Logic.";
				break;
				case 3:
					cout<<"Vocab Regression.";
				break;
				
				case 4:
					cout<<"Self-Distractor.";
				break;
			}
		cout<<endl;
		break;
		
		case 2:
			cout<<"Player gains GR: ";
			
			die = DieN(4);
			switch(die){
				case 1:
					cout<<"Immature ";
				break;
				case 2:
					cout<<"Baby Logic ";
				break;
				case 3:
					cout<<"Vocab Regression ";
				break;
				
				case 4:
					cout<<"Self-Distractor ";
				break;
			}
		break;
		
		case 3:
			if(DieN(100)>50){
				cout<<"Player gains GR: ";
				
				die = DieN(4);
				switch(die){
					case 1:
						cout<<"Immature ";
					break;
					case 2:
						cout<<"Baby Logic ";
					break;
					case 3:
						cout<<"Vocab Regression ";
					break;
					
					case 4:
						cout<<"Self-Distractor ";
					break;
				}
			} else {
				cout<<"Player avoided the random effect."<<endl;
			}
		break;
		
		case 6:
		cout<<"No effect, avoided."<<endl;
		break;
	}
	
	perPlayer = YesNo();
 }
}



void MCP(){											//Multicolor Crystal Path; last updated: 4.0
	int jewels [2];
	jewels[1] = 0;
	
	perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"MULTICOLOR CRYSTAL PATH (what a mouthful)"<<endl;
		
		tRoll = TrapRoll();

		for(int i = 0; i < 2; i++){
			jewels[i]= DieN(9);
			
			if(jewels[0] == jewels[1]){ //dupe protection
				i--;
			}
		}
		
		switch(tRoll){
			case 1: //critical failure
				Itotal = Intensity(2);
				
				cout<<"Player gains";
				jewelry(jewels[0]);
				cout<<" and ";
				jewelry(jewels[1]);
				cout<<"."<<endl<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			case 2: //heavy failure
				Itotal = Intensity(2);
				cout<<"Player gains";
				jewelry(jewels[0]);
				cout<<"."<<endl<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			case 3: //expected failure
				Itotal = Intensity(1);
				cout<<"Player gains";
				jewelry(jewels[0]);
				cout<<"."<<endl<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			case 4: //resisted
				Itotal = Intensity(1);
				
				if(DieN(100) > 50){
					cout<<"Player gains";
					jewelry(jewels[0]);
				} else {
					cout<<"Player avoided the random Jewelry status.";
				}
				cout<<"."<<endl<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			case 5: //partial success
				Itotal = Intensity(1);
				cout<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			case 6: //full success
				Itotal = Intensity(0);
				cout<<"Player gains "<<Itotal<<" RB/EP."<<endl;
			break;
			
			default:
				cout<<"ERROR"<<endl;
		}

		giveScroll();
		
		perPlayer = YesNo();
	}
}



void TimeDistort(){									//Time Distortion; last updated: 4.0
	int GRs [2];
	system("cls");
	cout<<"TIME DISTORTION"<<endl;

	tRoll = TrapRoll();

	perPlayer = true;
	while(perPlayer){
		system("cls");
		cout<<"TIME DISTORTION"<<endl;
		
		Itotal = 0;
		GRs[0] = DieN(3);
		
		switch(tRoll){
			case 1:
				cout<<"Critical failure."<<endl;
				
				Itotal = Intensity(1);
				
				cout<<"Player gains the following GRS: Little'd, Immature, and Baby Clothes."<<endl;
			break;
			
			case 2:
			case 3:
				if(tRoll == 2){
					cout<<"Heavy failure."<<endl;
				} else {
					cout<<"Expected failure."<<endl;
				}
				
				if(tRoll == 2){
					Itotal = Intensity(1);
				} else {
					Itotal = Intensity(0);
				}
				
				GRs[1] = (DieN(20)/10) + 1;
				
				cout<<"Player gains GRs: ";
				
				switch(GRs[0]){
					case 1: //if first is 1, second is bumped by 1
						GRs[1]++;
						
						cout<<"Little'd and ";
					break;
					
					case 2: //if first is 2, bump up to 3 if 2
						if(GRs[1] == 2){
							GRs[1]++;
						}
						
						cout<<"Immature and ";
					break;
					
					case 3: //if first is 3, no change to second GR
						cout<<"Baby Clothes and ";
					break;
					
					default:
						cout<<"ERROR on first GR calc and ";
				}
				
				switch(GRs[1]){
					case 1:
						cout<<"Little'd."<<endl;
					break;
					
					case 2:
						cout<<"Immature."<<endl;
					break;
					
					case 3:
						cout<<"Baby Clothes."<<endl;
					break;
					
					default:
						cout<<"ERROR on second GR calc."<<endl;
				}			
			break;
			
			case 4:
				cout<<"Resisted."<<endl;
				
				Itotal = Intensity(0);
				
				cout<<"Player gains GR: ";
				switch(GRs[0]){
					case 1:
						cout<<"Little'd."<<endl;
					break;
					
					case 2:
						cout<<"Immature."<<endl;
					break;
					
					case 3:
						cout<<"Baby Clothes."<<endl;
					break;
					
					default:
						cout<<"ERROR on second GR calc"<<endl;
				}
			break;
			
			case 5:
				cout<<"Partial success."<<endl;
				
				cout<<"Player gains GR: ";
				switch(GRs[0]){
					case 1:
						cout<<"Little'd."<<endl;
					break;
					
					case 2:
						cout<<"Immature."<<endl;
					break;
					
					case 3:
						cout<<"Baby Clothes."<<endl;
					break;
					
					default:
						cout<<"ERROR on second GR calc"<<endl;
				}
			break;
			
			case 6:
				cout<<"Full success."<<endl;
				
				Itotal = Intensity(0);
			break;
			
			default:
				cout<<"ERROR"<<endl;
		}
		
		if(Itotal != 0){
			cout<<"Player gains "<<Itotal<<" RB/MP/BP/EP."<<endl;
		}
		
		giveScroll();
		
		perPlayer = YesNo();
	}
}