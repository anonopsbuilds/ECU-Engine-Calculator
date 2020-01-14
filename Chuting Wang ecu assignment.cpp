// Chuting Wang ecu assignment.cpp : Defines the entry point for the console application.

/*-------------------------Chuting Wang ecu assignment--------------------------*/
/*																				*/
/*  This program uses funtions. Vehicle speed would be inputted into the txt. 	*/
/*  file called speed.txt and the program would make the relative calculations  */
/*  based on the inputted vehicle speed. Users can feel free to check them.     */
/*  When program is running, all the results of calculations would be printed   */
/*  out to a new txt file named speedOutput and like the speed file, users can  */
/*  check the results by getting acess to speedOutput.txt file.                 */
/*																				*/
/*  Introduction of the program would be printed to the screen with some		*/
/*  brief explaination of the functions of the relative sensors that the		*/
/*  calculations would be based on. They are vehicle speed sensor, crankshaft   */
/*  position sensor, engine coolant temperature sensor, throttle position		*/
/*  sensor, air-fuel ratio meter sensor and mass air flow sensor.				*/
/*  Among them, the calculations that would be made are vehicle accelerations	*/
/*  which would determine if the vehicle is accelerating, decelerating or		*/
/*  coasting, crankshaft rotations speed in rpm, engine temperature, throttle   */
/*  angle, throttle voltages, throttle percentage, rate of fuel injected in     */
/*  kg/h and rate of air injected in kg/h respectively. Based on the above		*/
/*  calculations result, the program then can determine if fan is required to	*/
/*  switch on/off, if air-fuel ratio stays in 14.7:1 and if the engine status	*/
/*  is normal,etc.																*/
/*																				*/	
/*  When displaying above information, a bar graph would be used to display		*/
/*  calculated results of vehicle acceleration, rotation speed and engine		*/
/*  temperature. Below them. a simple diagram of the four-stroke engine would	*/
/*  be printed indicating the location of air-fuel ratio meter sensor, mass air	*/
/*  flow sensor, as well as the throttle position sensor. Also, the realtive	*/
/*  results would be printed indicating how their calculated results are		*/
/*  affected by the change of vehicle speed.									*/
/*																				*/
/*  Below the diagram, all infomation that have been shown as well as some		*/
/*  details would be printed as the conclusion,									*/
/*																				*/
/*  Each vehicle speed is defined as one cycle, there would be 8 cycles in		*/
/*  total in this program.														*/
/*																				*/
/*	ps: please notice that in this program, specific speed is used in speed.txt	*/
/*		therefore if a different speed.txt is used, please comment some code	*/
/*		that has been labelled in the program.									*/
/*																				*/
/*	pps: in this program, all calculations are made based on the situation		*/
/*		 when air-fuel ratio is 14.7:1 and the engine is runnning normal.		*/																															
/*                                                                              */
/*  ppps: since some ASCII code has been used in this program and they are		*/
/*        really sensitive so make sure that the system language is in English	*/
/*		  otherwise some signs might not be printed properly.					*/
/*																				*/
/*                PROGRAM ID:Chuting Wang ecu assignment	                    */ 
/*                PROGRAMMER:Chuting Wang            	                        */
/*                RUN DATE:November 12, 2018                                    */ 
/*                                                   						    */
/*------------------------------------------------------------------------------*/

/********************************************************************************/
/* Notice: specific speed.txt is used, and therefore when using different		*/
/* speed.txt, please make sure the initial speed is 0, and there are 8 speed	*/
/* in total to be inputted, in order to allow the program to run properly.		*/
/********************************************************************************/

/*---------------------------PREPROCESSING DIRECTIVES------------------------------------*/

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <math.h>

/*-----------------------------------GOTOXY FUNCTION-------------------------------------*/

void gotoxy(int x, int y)
	{
	  static HANDLE hStdout = NULL;
	  COORD coord;
	
	  coord.X = x;
	  coord.Y = y;
	
	  if(!hStdout)
	  {
	    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	  }
	  
	  SetConsoleCursorPosition(hStdout,coord);
	}

/*---------------------------------MAINLINE CONTROL--------------------------------------*/
int main (void)
{
/*------------------------------------TITLE----------------------------------------------*/

	system("TITLE Chuting Wang ecu assignment");

/*------------------------------DECLARE FUNCTIONS PROTOTYPES-----------------------------*/

	/*calculations*/

	void vehicleSpeedSensor(int,int[9],int[9],char[9][30]);					//vehicle speed sensor
	void crankshaftPositionSensor(int,int[9],float[9]);						//crankshaft rotation sensor
	void engineCoolantTemperatureSensor(int,int[9],int[9]);					//engine coolant temperature sensor
	void airFuelRatioMeterSensor(int,int[9],float[9]);						//air-fuel ratio meter sensor
	void massAirFlowSensor(int,int[9],float[9]);							//mass air flow sensor
	void throttlePositionSensor(int,int[9],float[9],float[9],float[9]);		//throttle position sensor

	/*animation and displaying*/

	void display(int,int[9],int[9],char[9][30],float[9],int[9],float[9],float[9],float[9],float[9],float[9]); //display results
	void intro(void);																						  //intro program
	void readingAnimation(void);																			  //reading animation
	int  warning(int);																						  //users should know

/*-------------------------------DECLARE LOCAL VARIABLES---------------------------------*/

	/*loops*/

	int number=0;	
	int row=0;
	int flag=0;

	/*program*/

	int   cycleNumber[9];				//cycle number
	int   vehicleSpeed[9];				//vehicle speed
	int   acceleration[9];				//calculated acceleration 
	char  accelerationStatus[9][30];	//identify vehicle status
	float rotationSpeed[9];				//crankshaft rotation speed
	int   engineTemperature[9];			//engine temperature
	float injectedFuel[9];				//rate of mass of fuel injected 
	float injectedAir[9];				//rate of mass of air injected 
	float throttleAngle[9];				//throttle angle
	float throttlePercentage[9];		//throttle percentage
	float throttleVoltage[9];			//throttle Voltage
	char  choice;						//determine continue or return to previous page

	/*set text color*/

	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);   
	WORD wOldColorAttrs;                                                        
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;                     
	GetConsoleScreenBufferInfo(h, &csbiInfo);                     
	wOldColorAttrs = csbiInfo.wAttributes;

	/*set initial value for variables*/

	for(row=0;row<10;row++)
	{
		cycleNumber[row]=row+1; //cycle number starts from 1
		vehicleSpeed[row]=0;
		acceleration[row]=0;
		strcpy(accelerationStatus[row],"0");
		rotationSpeed[row]=0;
		engineTemperature[row]=0;
		throttleAngle[row]=0;
		throttleVoltage[row]=0;
		throttlePercentage[row]=0;
		injectedAir[row]=0;
		injectedFuel[row]=0;
	}
	
/*-------------------------------------FUNCTIONS CALLS-----------------------------------*/

	//read file 
	FILE *inputFile; 
	FILE *outputFile; //file pointers, first for input, second for output
	inputFile = fopen("d:speed.txt","r"); //open the input file to read
	outputFile=fopen("d:speedOutput.txt","w"); //open the output file to write 

	//print title of output file
	fprintf(outputFile,"\t\t\t\tECU Assignment");
	fprintf(outputFile,"\ncycle #  Speed(km/h)  acceleration(status)  rotation speed(rpm)  engine temperature(F)  fuel injected(kg/h)  air injected(kg/h)  throttle angle(degree)  throttle voltage(v) throttle percentage(percent)");

	if(inputFile == NULL) //check to see if file will open or can be found
	{
		gotoxy(0,1);
		printf("The file 'speed.txt' is not available.");
		gotoxy(0,2);
		printf("Press <ENTER> to exit");
	}
	else
	{	
		flag=warning(flag);
		if(flag==2) //if the speed.txt is correct
		{
			/*set screen size and position*/
			HWND foregroundWindow = GetForegroundWindow();
			MoveWindow(foregroundWindow,300,5,650,970,TRUE);

			/*introduction to the program and sensors*/
			intro();	

			MoveWindow(foregroundWindow,300,400,650,200,TRUE); //resize

			/*animation of reading and calculating*/
			readingAnimation();

			MoveWindow(foregroundWindow,300,5,650,970,TRUE);//resize 
			do
			{
				fscanf(inputFile,"%d",&vehicleSpeed[number]);//read in text from file and place in variables

				/*calculations*/

				vehicleSpeedSensor(number,vehicleSpeed,acceleration,accelerationStatus);  //acceleration - vehicle speed sensor					
				crankshaftPositionSensor(number,vehicleSpeed,rotationSpeed); //rotation speed - crankshaft position sensor
				engineCoolantTemperatureSensor(number,vehicleSpeed,engineTemperature); //engine temperature - engine coolant temperature sensor
				airFuelRatioMeterSensor(number,vehicleSpeed,injectedFuel);//rate of fuel injected - air-fuel ratio meter sensor
				massAirFlowSensor(number,vehicleSpeed,injectedAir); //rate of air injected - mass air flow sensor
				throttlePositionSensor(number,vehicleSpeed,throttleAngle,throttlePercentage,throttleVoltage); //throttle angle, voltages and percentage - throttle position sensor

				/*dsiplay above calculations result*/
				display(number,vehicleSpeed,acceleration,accelerationStatus,rotationSpeed,engineTemperature,injectedAir,injectedFuel,throttleAngle,throttleVoltage,throttlePercentage);
				
				//print results to output file			
				fprintf(outputFile,"\n%d       %d           %s                    %.0f                  %d                      %.0f                 %.0f                %.2f                    %.2f                  %.2f",number+1,vehicleSpeed[number],accelerationStatus[number],rotationSpeed[number],engineTemperature[number],injectedFuel[number],injectedAir[number],throttleAngle[number],throttleVoltage[number],throttlePercentage[number]);				
				
				number++; //to read the next speed
				
				getch();  //hold the screen			
				system("cls");  //clear the screen

			}while(!feof(inputFile));  //when it is not the end of the file
		}//end of if
		else
		{
			gotoxy(0,0);
			printf("Press <ENTER> to exit the program...");
			getch();
			system("cls");
		}
	}//end of else
	
	fclose(inputFile); //close the input file
	fclose(outputFile); //close the output file

	return 0;  
} //end main

/*----------------------------------FUNCTIONS DEFINITION---------------------------------*/

/*acceleration calculations*/
void vehicleSpeedSensor(int number,int vehicleSpeed[9],int acceleration[9],char accelerationStatus[9][30])
{
	if(number==0)  //when the vehicle at speed of 0 
		strcpy(accelerationStatus[number],"NULL"); //none would be printed to screen
	else
	{
		acceleration[number]=(vehicleSpeed[number]-vehicleSpeed[number-1])/0.5;
		if(acceleration[number]>0) //if the speed is higher than previous speed 
			strcpy(accelerationStatus[number],"accelerating");
		if(acceleration[number]==0) //if the vehicle speed is equal to the previous one
			strcpy(accelerationStatus[number],"coasting");
		if(acceleration[number]<0)  //if the vehicle speed is slower than the previous one 
			strcpy(accelerationStatus[number],"decelerating");
	}
	return;
} //end function 

/*crankshaft rotation speed calculations*/
void crankshaftPositionSensor(int number,int vehicleSpeed[9],float rotationSpeed[9])
{
	if(number==0) //if vehicle at the speed of 0
		rotationSpeed[0]=0;
	else
	rotationSpeed[number]=(vehicleSpeed[number]*2*4*800)/(3.14*65); //rotation speed calculations
	return;
} //end function

/*engine temperature calculations*/
void engineCoolantTemperatureSensor(int number,int vehicleSpeed[9],int engineTemperature[9])
{
	if(number==0) //if vehicle is at the speed of 0
		engineTemperature[0]=75; //the room temperature is assume to be 75 Fahrenheit 
	else
	engineTemperature[number]=fabs((vehicleSpeed[number]-vehicleSpeed[number-1])/10)+200; //calculate absolute value of change of speed
	return;
}//end function

/*rate of mass of fuel injected calculations*/
void airFuelRatioMeterSensor(int number,int vehicleSpeed[9],float injectedFuel[9])
{
	if(number==0) //if vehicle is at the speed of 0
		injectedFuel[0]=0; 
	else
	injectedFuel[number]=(((vehicleSpeed[number]-vehicleSpeed[number-1])/2)+300)/14.7; //rate of fuel injected calculations
	return;
}//end function 

/*rate of mass of air injected calculations*/
void massAirFlowSensor(int number,int vehicleSpeed[9],float injectedAir[9])
{
	if(number==0) //if vehicle speed starts at 0 
		injectedAir[0]=0;
	else
	injectedAir[number]=((vehicleSpeed[number]-vehicleSpeed[number-1])/2)+300; //rate of air injected calculations

	return;
}//end function 

/*throttle angle,throttle voltage and throttle percentage calculations*/
void throttlePositionSensor(int number,int vehicleSpeed[9],float throttleAngle[9],float throttlePercentage[9],float throttleVoltage[9])
{
	if(number==0) //when vehicle speed is at 0
	{
		throttleAngle[0]=0;
		throttleVoltage[0]=0;
		throttlePercentage[0]=0;
	}
	else
	{
		throttleAngle[number]=vehicleSpeed[number]/10;//throttle angle calculations 
		throttleVoltage[number]=((throttleAngle[number]/2)*0.1)+0.5;  //throttle voltage calculations 
		throttlePercentage[number]=((throttleAngle[number]*throttleVoltage[number])/90)*100; //throttle percentage calculations 
	}
	return;
}//end function 

/*display all calculated info*/
void display(int number,int vehicleSpeed[9],int acceleration[9],char accelerationStatus[9][30],float rotationSpeed[9],int engineTemperature[9],float injectedAir[9],float injectedFuel[9],float throttleAngle[9],float throttleVoltage[9],float throttlePercentage[9])
{
	/*color*/
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);   
	WORD wOldColorAttrs;                                                        
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;                     
	GetConsoleScreenBufferInfo(h, &csbiInfo);                     
	wOldColorAttrs = csbiInfo.wAttributes;   

	/*declare variables*/
	int times[9]; //determine how many times the space would be colored
	int row=0;    //loop
	
	for(row=0;row<10;row++) //set initial values for variables
	{
		times[row]=0;
	}

	/*print bar graph*/
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(30,0);
	printf("Cycle %d of 8 ",number+1);
	gotoxy(10,2);
	printf("Vehicle Speed");
	gotoxy(4,3);
	printf("Acceleration Status");
	gotoxy(9,4);
	printf("Rotation Speed");
	gotoxy(5,5);
	printf("Engine Temperature");
	gotoxy(24,6);
	printf("MIN");
	gotoxy(40,6);
	printf("---->");
	gotoxy(59,6);
	printf("MAX");
	SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_RED|BACKGROUND_BLUE );
	gotoxy(25,2);
	printf("                                   "); //35 space
	gotoxy(25,3);
	printf("                                   ");
	gotoxy(25,4);
	printf("                                   ");
	gotoxy(25,5);
	printf("                                   ");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(62,2);
	printf("%d km/h",vehicleSpeed[number]);	
	gotoxy(62,3);
	printf("%s",accelerationStatus[number]);
	gotoxy(62,4);
	printf("%.0f rpm",rotationSpeed[number]);
	gotoxy(62,5);
	printf("%d%cF",engineTemperature[number],248); //248-ASCII code for degree sign
	
	//print vehicle speed
	times[0] = vehicleSpeed[number]/5;
	for(row=0;row<2*times[0];row++)
	{
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_BLUE |BACKGROUND_INTENSITY );
		gotoxy(25+row,2);
		printf(" ");					
	}
	
	//print acceleration status
	times[1] = fabs(acceleration[number]/5);
	if((acceleration[number]==0)&&(number>0))
	{
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_RED |BACKGROUND_INTENSITY );	
		gotoxy(42,3);
		printf(" ");
	}

	if(acceleration[number]>0)
	{
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_RED |BACKGROUND_INTENSITY );	
		gotoxy(42,3);
		printf(" ");
		for(row=0;row<1.5*times[1];row++)
		{
			SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_INTENSITY );	
			gotoxy(43+row,3);
			printf(" ");
		}		
	}
	if(acceleration[number]<0)
	{
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN |BACKGROUND_RED |BACKGROUND_INTENSITY );	
		gotoxy(42,3);
		printf(" ");
		for(row=0;row<1.5*times[1];row++)
		{
			SetConsoleTextAttribute ( h, BACKGROUND_RED |BACKGROUND_INTENSITY );	
			gotoxy(41-row,3);
			printf(" ");
		}
	}
	
	//print engine rotation speed
	times[2] = rotationSpeed[number]/300;
	for(row=0;row<4*times[2];row++)
	{
		SetConsoleTextAttribute ( h, BACKGROUND_RED |BACKGROUND_BLUE |BACKGROUND_INTENSITY );
		gotoxy(25+row,4);
		printf(" ");					
	}
	
	//print engine temperature
	times[3]=engineTemperature[number]/10;
	for(row=0;row<times[3];row++)
	{
		SetConsoleTextAttribute ( h, BACKGROUND_RED |BACKGROUND_GREEN|BACKGROUND_INTENSITY );
		gotoxy(25+row,5);
		printf(" ");					
	}
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	
	//print air and fuel injected and throttle angle, voltage and percentage
	gotoxy(4,8);
	printf("AIR: %.2f kg/h",injectedAir[number]);
	gotoxy(22,27);
	printf("FUEL: %.2f kg/h",injectedFuel[number]);
	gotoxy(21,16);
	printf("%.2f %c",throttlePercentage[number],37); //37 - ASCII code for percentage sign 
	gotoxy(21,14);
	printf("%.2f v",throttleVoltage[number]);
	gotoxy(21,12);
	printf("%.2f %c",throttleAngle[number],248);

	//display the engine 
	for(row=0;row<3;row++)
	{
		gotoxy(10+row,9);
		printf("-"); 
		gotoxy(13+row,9);
		printf("-");
		gotoxy(10+row,11);
		printf("-");		
		gotoxy(14+row,22);
		printf("-");
		gotoxy(17+row,20);
		printf("-");
		gotoxy(17+row,22);
		printf("-");
		gotoxy(20,23+row);
		printf("|");
		gotoxy(23,23);
		printf("|");
		gotoxy(31+row,26);
		printf("-");
		gotoxy(30+row,20);
		printf("-");
		gotoxy(55+row,22);
		printf("-");
		gotoxy(55,19-row);
		printf("|");
		gotoxy(58,21-row);
		printf("|");
		gotoxy(56+row,16);
		printf("-");
		gotoxy(59+row,16);
		printf("-");
		gotoxy(59+row,18);
		printf("-");		
	}

	//print throttle 
	SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_INTENSITY );
	gotoxy(17,20);
	printf(",");
	gotoxy(19,22);
	printf("'");
	gotoxy(18,21);
	printf("\\");
	gotoxy(18,19);
	printf(",");
	for(row=0;row<6;row++)
	{
		gotoxy(19+row,19);
		printf("-");		
	}
	for(row=0;row<2;row++)
	{
		gotoxy(25,18-row);
		printf("|");
	}
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	for(row=0;row<10;row++)
	{
		gotoxy(16,10+row);
		printf("|");
		gotoxy(13,12+row);
		printf("|");
		gotoxy(21+row,26);
		printf("-");
		gotoxy(24+row,24);
		printf("-");
		gotoxy(34+row,26);
		printf("-");
		gotoxy(34+row,24);
		printf("-");
		gotoxy(20+row,20);
		printf("-");
		gotoxy(24+row,22);
		printf("-");
		gotoxy(45+row,29);
		printf("-");
		gotoxy(55,29);
		printf("-");
	}
	//print engine
	for(row=0;row<8;row++)
	{
		gotoxy(33,20-row);
		printf("|");
		gotoxy(46,20-row);
		printf("|");		
		gotoxy(47+row,22);
		printf("-");
		gotoxy(47+row,20);
		printf("-");		
	}
	for(row=0;row<12;row++)
	{
		gotoxy(34+row,12);
		printf("-");
		gotoxy(34+row,23);
		printf("-");		
		gotoxy(33,22);
		printf("|");
		gotoxy(46,22);
		printf("|");
		gotoxy(44+row,24);
		printf("-");
	}
	for(row=0;row<4;row++)
	{
		gotoxy(56,25+row);
		printf("|");
		gotoxy(44,27);
		printf("|");
		gotoxy(44,28);
		printf("|");
	}
	//print arrow
	SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_INTENSITY );	
	gotoxy(4,10);
	printf("IN --->");
	gotoxy(60,17);
	printf("---> OUT");
	SetConsoleTextAttribute ( h, BACKGROUND_GREEN | BACKGROUND_RED);
	gotoxy(45,27);
	printf("           ");
	gotoxy(45,28);
	printf("           ");
	SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY );
	gotoxy(51,26);
	printf("|");
	gotoxy(51,25);
	printf(",");
	for(row=0;row<6;row++)
	{
		gotoxy(50-row,25);
		printf("-");
	}
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(38,14);
	printf("FOUR");
	gotoxy(37,15);
	printf("STROKE");
	gotoxy(37,16);
	printf("ENGINE");

	//display general infomation
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(8,31);
	printf("CONCLUSION:");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE );	
	for(row=0;row<58;row++)
	{
		gotoxy(8+row,32);
		printf("=");
		gotoxy(8+row,46);
		printf("=");
	}
/***************************************************************************************/
	//if different speed.txt is used, please comment following code 
	//start of comment
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(33,34);
	printf(" 0  10  20  35  60  60  35  10");
	//end of comment
/***************************************************************************************/

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED|FOREGROUND_INTENSITY );	
	for(row=0;row<32;row++)
	{
		gotoxy(32+row,33);
		printf("-");
		gotoxy(32+row,35);
		printf("-");
	}
	
	//print speed
	if(number==0)
	{
		gotoxy(34,34);
		printf("%d",vehicleSpeed[number]);
	}
	else
	{
		gotoxy(37+(number-1)*4,34);
		printf("%d",vehicleSpeed[number]);
	}
	gotoxy(31,34);
	printf("|");
	gotoxy(64,34);
	printf("|");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(8,34);
	printf("Vehicle Speed (km/h): ");
	gotoxy(8,36);
	printf("Status: %10s",accelerationStatus[number]);
	gotoxy(39,36);
	printf("Throttle Angle: %8.2f%c",throttleAngle[number],248);
	gotoxy(8,38);
	printf("Throttle Voltage: %3.2fv",throttleVoltage[number]);
	gotoxy(34,38);
	printf("Throttle Percentage: %8.2f%c",throttlePercentage[number],37);
	gotoxy(8,40);
	printf("Engine Temperature: %2d%cF",engineTemperature[number],248);
	gotoxy(50,40);
	printf("Fan:");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE );
	gotoxy(58,40);
	printf("on/off");
	gotoxy(49,44);
	printf("rich/normal/lean");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED|FOREGROUND_INTENSITY );
	gotoxy(61,40);
	printf("off");
	gotoxy(54,44);
	printf("normal");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(8,42);
	printf("Air: %10.2f kg/h",injectedAir[number]);
	gotoxy(49,42);
	printf("Fuel: %.2f kg/h",injectedFuel[number]);
	gotoxy(8,44);
	printf("Air-Fuel Ratio: 14.7:1");
	gotoxy(34,44);
	printf("Engine Status: ");

	//print ps
	SetConsoleTextAttribute ( h, FOREGROUND_RED );
	gotoxy(8,47);
	printf("ps: in conclusion, text in    color means selected.");
	gotoxy(8,48);
	printf("pps: room temperature: 75%cF",248);
	SetConsoleTextAttribute ( h, BACKGROUND_GREEN | BACKGROUND_RED);
	gotoxy(35,47);
	printf("  ");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	if(number==7)
	{
		gotoxy(8,50);
		printf("Press <ENTER> to exit the program.");
	}
	else
	{
		gotoxy(8,50);
		printf("Press <ENTER> to go to cycle %d",number+2);
	}

	//animation 
	if(number>0)
	{
		for(row=0;row<6;row++)
		{
			SetConsoleTextAttribute ( h, BACKGROUND_RED | BACKGROUND_INTENSITY);
			gotoxy(14,11+row*2);
			printf(" ");
			gotoxy(15,11+row*2);
			printf(" ");
			SetConsoleTextAttribute ( h, BACKGROUND_RED | BACKGROUND_GREEN| BACKGROUND_INTENSITY);
			gotoxy(42-row*4,25);
			printf("  ");
			Sleep(300);
			SetConsoleTextAttribute ( h, FOREGROUND_INTENSITY ); //set background color back to black
			gotoxy(14,11+row*2);
			printf(" ");
			gotoxy(15,11+row*2);
			printf(" ");
			gotoxy(42-row*4,25);
			printf("  ");
		}
		SetConsoleTextAttribute ( h, BACKGROUND_RED | BACKGROUND_INTENSITY);
		gotoxy(14,21);
		printf("  ");
		SetConsoleTextAttribute ( h, BACKGROUND_RED | BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(21,24);
		printf("  ");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		gotoxy(14,21);
		printf("  ");
		gotoxy(21,24);
		printf("  ");
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(21,21);
		printf("  ");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN|FOREGROUND_INTENSITY );	
		gotoxy(21,21);
		printf("  ");	
		gotoxy(31,21);
		printf("--->");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN|FOREGROUND_INTENSITY );	
		gotoxy(36,18);
		printf("AIR:FUEL");
		gotoxy(35,19);
		printf("14.7:  1");	
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(38,21);
		printf("  ");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN|FOREGROUND_INTENSITY );
		gotoxy(38,21);
		printf("  ");
		gotoxy(45,21);
		printf("--->");
		Sleep(300);
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(52,21);
		printf("  ");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		gotoxy(52,21);
		printf("  ");
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(56,19);
		printf("  ");	
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		gotoxy(56,19);
		printf("  ");
		SetConsoleTextAttribute ( h, BACKGROUND_GREEN| BACKGROUND_INTENSITY);
		gotoxy(56,17);
		printf("  ");
		Sleep(300);
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		gotoxy(56,17);
		printf("  ");
	}
	
	return;

} //end function 

/*introduction of the program and types of sensors*/
void intro(void)
{
	/*color*/
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);  

	/*declare variables*/
	int row=0;  //loop	 
	
	/*animation*/
	for(row=0;row<60;row++)
	{
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		gotoxy(3+row,2);
		printf(".-'--`-._");
		gotoxy(3+row,3);
		printf("'-O---O--'");
		gotoxy(3,5);
		printf("==========");
		gotoxy(10+row,5);
		printf("=");
		gotoxy(3+row,52);
		printf(".-'--`-._");
		gotoxy(3+row,53);
		printf("'-O---O--'");
		gotoxy(3,55);
		printf("==========");
		gotoxy(10+row,55);
		printf("=");
		if((row>=0)&&(row<=14))
		{
			SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_INTENSITY);
			gotoxy(3,4);
			printf("==========");
			gotoxy(10+row,4);
			printf("=");
			gotoxy(3,6);
			printf("==========");
			gotoxy(10+row,6);
			printf("=");
			gotoxy(3,54);
			printf("==========");
			gotoxy(10+row,54);
			printf("=");
			gotoxy(3,56);
			printf("==========");
			gotoxy(10+row,56);
			printf("=");
		}
		if((row>14)&&(row<=37))
		{
			SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			gotoxy(10+row,4);
			printf("=");
			gotoxy(10+row,6);
			printf("=");
			gotoxy(10+row,54);
			printf("=");
			gotoxy(10+row,56);
			printf("=");
		}
		if((row>37)&&(row<=60))
		{
			SetConsoleTextAttribute ( h, FOREGROUND_RED|FOREGROUND_INTENSITY);
			gotoxy(10+row,4);
			printf("=");
			gotoxy(10+row,6);
			printf("=");
			gotoxy(10+row,54);
			printf("=");
			gotoxy(10+row,56);
			printf("=");
		}
		
		//print title
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		if(row==30)
		{
			gotoxy(32,3);
			printf("E");
		}
		if(row==31)
		{
			gotoxy(33,3);
			printf("C");
		}
		if(row==32)
		{
			gotoxy(34,3);
			printf("U");
		}
		if(row==34)
		{
			gotoxy(36,3);
			printf("S");
		}
		if(row==35)
		{
			gotoxy(37,3);
			printf("I");
		}
		if(row==36)
		{
			gotoxy(38,3);
			printf("M");
		}
		if(row==37)
		{
			gotoxy(39,3);
			printf("U");
		}
		if(row==38)
		{
			gotoxy(40,3);
			printf("L");
		}
		if(row==39)
		{
			gotoxy(41,3);
			printf("A");
		}
		if(row==40)
		{
			gotoxy(42,3);
			printf("T");
		}
		if(row==41)
		{
			gotoxy(42,3);
			printf("O");
		}
		if(row==42)
		{
			gotoxy(43,3);
			printf("R");
		}
		Sleep(50);
		if(row<59)
		{
			gotoxy(3+row,2);
			printf(" ");
			gotoxy(3+row,3);
			printf(" ");
			gotoxy(3+row,52);
			printf(" ");
			gotoxy(3+row,53);
			printf(" ");
		}		
	}
	
	//print sensor info
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,9);
	printf("Simulated calculations are made for following sensors in this program:");
	SetConsoleTextAttribute ( h,FOREGROUND_RED|FOREGROUND_INTENSITY );
	gotoxy(49,9);
	printf("sensors");
	
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,13);
	printf("Vehicle Speed Sensor: vehicle acceleration");
	gotoxy(3,15);
	printf("                      (acclerating/decelerating/coasting)");
	gotoxy(5,53);
	printf("Press <ENTER> to continue.");
	getch();

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(3,13);
	printf("Vehicle Speed Sensor: vehicle acceleration");
	gotoxy(3,15);
	printf("                      (acclerating/decelerating/coasting)");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,19);
	printf("Crankshaft Position Sensor: rotation speed of crankshaft");
	gotoxy(3,21);
	printf("                            Unit: RPM (revolution per minute)");
	getch();

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(3,19);
	printf("Crankshaft Position Sensor: rotation speed of crankshaft");
	gotoxy(3,21);
	printf("                            Unit: RPM (revolution per minute)");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,25);
	printf("Engine Coolant Temperature Sensor: engine temperature");
	gotoxy(3,27);
	printf("                                   Unit: %cF (fan switches on/off)",248);
	getch();

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(3,25);
	printf("Engine Coolant Temperature Sensor: engine temperature");
	gotoxy(3,27);
	printf("                                   Unit: %cF (fan switches on/off)",248);
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,31);
	printf("Throttle Position Sensor: throttle angle, voltage and percentage");
	gotoxy(3,33);
	printf("                          Unit: %c, v and %c respectively",248,37);
	getch();

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(3,31);
	printf("Throttle Position Sensor: throttle angle, voltage and percentage");
	gotoxy(3,33);
	printf("                          Unit: %c, v and %c respectively",248,37);
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,37);
	printf("Air-fuel Ratio Meter Sensor: mass of fuel used and air-fuel ratio");
	gotoxy(3,39);
	printf("                              Unit: kg/h");
	getch();

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE);
	gotoxy(3,37);
	printf("Air-fuel Ratio Meter Sensor: mass of fuel used and air-fuel ratio");
	gotoxy(3,39);
	printf("                              Unit: kg/h");
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,43);
	printf("Mass Air Flow Sensor: mass of air entering the engine to ECU");
	gotoxy(3,45);
	printf("                      Unit: kg/h");
	getch();


	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(3,13);
	printf("Vehicle Speed Sensor: vehicle acceleration");
	gotoxy(3,15);
	printf("                      (acclerating/decelerating/coasting)");
	gotoxy(3,19);
	printf("Crankshaft Position Sensor: rotation speed of crankshaft");
	gotoxy(3,21);
	printf("                            Unit: RPM (revolution per minute)");
	gotoxy(3,25);
	printf("Engine Coolant Temperature Sensor: engine temperature");
	gotoxy(3,27);
	printf("                                   Unit: %cF (fan switches on/off)",248);
	gotoxy(3,31);
	printf("Throttle Position Sensor: throttle angle, voltage and percentage");
	gotoxy(3,33);
	printf("                          Unit: %c, v and %c respectively",248,37);
	gotoxy(3,37);
	printf("Air-fuel Ratio Meter Sensor: mass of fuel used and air-fuel ratio");
	gotoxy(3,39);
	printf("                              Unit: kg/h");
	gotoxy(3,43);
	printf("Mass Air Flow Sensor: mass of air entering the engine to ECU");
	gotoxy(3,45);
	printf("                      Unit: kg/h");

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_BLUE|FOREGROUND_INTENSITY );
	gotoxy(5,53);
	printf("Press <ENTER> to start!      "); //prompt user to start the program
	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );

	getch(); //hold screen 
	system("cls"); //clear screen

	return;
}//end function 

/*animation of reading data and calculations*/
void readingAnimation(void)
{
	/*color*/
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);   
	
	/*declare variables*/
	int row=0;  //for loop
	int col=0;  //for loop
 
	/*animation of reading and calculating*/
	for(col=0;col<2;col++)
	{
		for(row=0;row<69;row++)
		{
			gotoxy(row,2);
			printf(".-'--`-._");
			gotoxy(row,3);
			printf("'-O---O--'");
			SetConsoleTextAttribute ( h, FOREGROUND_GREEN |FOREGROUND_INTENSITY );
			gotoxy(0,4);
			printf("==========");
			gotoxy(7+row,4);
			printf("=");
			SetConsoleTextAttribute ( h, FOREGROUND_BLUE|FOREGROUND_INTENSITY |BACKGROUND_RED |BACKGROUND_BLUE|BACKGROUND_GREEN );
			gotoxy(35,6);
			if(col==0)
				printf("READING");
			if(col==1)
				printf("CALCULATING");			
			Sleep(50);
			SetConsoleTextAttribute ( h,  FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );
			if(row<68)
			{
				gotoxy(row,2);
				printf(" ");
				gotoxy(row,3);
				printf(" ");
			}
		}//end of inner loop

		system("cls"); //clear screen 

	}//end of outer forloop

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );

	return;
} //end of function

/*users should know*/
int warning(int flag)
{
	/*color*/
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);  

	/*declare variables*/
	char choice; //user choice

	SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY );

	do
	{
		printf("Hello, user!\n");
		printf("Remember in this program, specific speed.txt file is used\n");
		printf("and therefore you might not input other speed!\n\n");
		printf("Press <ENTER> to continue...");
		getch();
		system("cls");
		printf("Alright, if different speeds are input...");
		printf("Please make the initial speed 0 and there are\n");
		printf("8 speed in total!!\n\n");
		printf("Because this would make the program run smoothly and properly!\n\n");
		printf("Press <ENTER> to continue...");
		getch();
		system("cls");
		printf("Following please select: \n");
		printf("a] The speed.txt is good to go!\n");
		printf("b] The speed.txt does not fulfill the requirements.\n\n");
		printf("Choice:");
		choice=_getch();
		fflush(stdin);
		printf("%c",choice);
		switch(choice)
		{
			case 'a':
			case 'A':
				printf("\n\nPress <ENTER> and START!!!!!!!!");
				flag=2;
				break;

			case 'b':
			case 'B':
				printf("\n\nSORRY...We cannnot allow the program to run...\n");
				printf("Please change your speed.txt if you want to run this program\n");
				printf("and we hope to see you again!!!");
				flag=3;
				break;

			default:
				printf("\n\nInvalid Input!\n");
				printf("Press <ENTER> to restart the program!");
				flag=1;
				getch();
				system("cls");
				break;
		}
	}while(flag<2);
	getch(); //hold the screen
	system("cls"); //clear the screen
	return(flag);
}//end function 
