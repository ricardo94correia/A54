/******************************************************************************
    SINF A54
    Karollina Varis
    Francisco Vieira
    Guilherme Castro
    rooms
	1 - Gym
	2 - Entrance
	3 - Office
	4 - Nursery
	5 - Bathroom 1
	6 - Bathroom 2
	7 - Bathroom 3
	8 - Bathroom 4
	9 - 2/3 age room
	10 -4/5 age room
	11 - Study room
	12 - Canteen
	13 - Kitchen
	14 - Hall
	15- Out

	prevalues
	0 - temper
	1- humidity
	2 - light

	actuators
	0 - heater
	1 - air cooler
	2 - window 
	3 - blinds
	4 - door 
	5 - dryer
	6 - lamp
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOWBATTERY 12
#define LEVELRED 12 
#define MIDLIGHT 12
#define HIGHLIGHT 11
#define ACEPTEMP 5
#define ACEPHUM 5

long Mote;
float pre[15][3]={0}, atua[15][7], values[16][5];

void sensorread(void){
    char name[100];
	char *str, *str1;
	const char s[1] = " ";
	long Bat, Temp, Hum, Lig, Red;
	float battery, temper, humid, light, redlig;
	int ready=0, counter=0;
	
	while(1){
	sleep(1);
	fgets(name, 100, stdin);
	//FIND Begining of message
	str = strtok(name, s);
	while(ready!=1){
		while(strcmp(str, "7E")!=0){
			str = strtok(NULL, s);
		}
		str = strtok(NULL, s);
		if(strcmp(str, "45")==0)
			ready=1;
	}
	// READ MOTE ID
	while(counter!=4){
		str = strtok(NULL, s);
		counter++;
	}
	str1 = strtok(NULL, s);
	str= strcat( str, str1);
	Mote=strtol(str, NULL, 16);
	printf("Mote: %lu\n", Mote);
	//payload length - do we need this for anything?
	str = strtok(NULL, s);
	printf("Payload length: %s\n",str);
	//not needed group and handler ID
	counter=0;
	while(counter!=3){
		str = strtok(NULL, s);
		counter++;
	}
	//Voltage - Mote Batery
	str1 = strtok(NULL, s);
	str= strcat( str, str1);
	Bat=strtol(str, NULL, 16);
	battery=(float)(Bat/(float)(4096))*1.5*2;
	printf("Mote Battery: %.2f V\n", battery);
	//Visible Light
	str = strtok(NULL, s);
	str1 = strtok(NULL, s);
	str = strcat( str, str1);
	Lig =strtol(str, NULL, 16);
	light = (float) 0.625 * 1000000 * (((float) Lig)/4096)*(1.5 / 100000) * 1000;
	printf("Light: %.2f lux\n", light);
	//Infrared Light
	str = strtok(NULL, s);
	str1 = strtok(NULL, s);
	str = strcat( str, str1);
	Red =strtol(str, NULL, 16);
	redlig = 0.769 * 100000 * (((float) Red)/4096) * (1.5/100000) * 1000;
	printf("Red Light: %.2f lux\n", redlig);
	//Temperature
	str = strtok(NULL, s);
	str1 = strtok(NULL, s);
	str = strcat( str, str1);
	Temp =strtol(str, NULL, 16);
	temper = (float) -39.6 + (0.01*(float)Temp);
	printf("Temperature: %.2f C \n", temper);
	//Humidity
	str = strtok(NULL, s);
	str1 = strtok(NULL, s);
	str = strcat( str, str1);
	Hum =strtol(str, NULL, 16);
	humid = (float) -2.0468 + (0.0367 * (float) Hum) + ((-1.5955 * ((float) Hum) * ((float) Hum))/1000000) ;
	printf("Humidity: %.2f %%\n", humid);
	
	
	while(strcmp(str, "7E")!=0){
		str = strtok(NULL, s);
	}
	}
}

int definevalues(void){ //define values of temperature, humidity and light for each room
	int option=0, i=0;
	float dtemper, dhumid, dlight;
	while(option!=16){
		printf("\t-Choose the Room-\n \t1 - Gym \n \t2 - Entrance \n \t3 - Office \n \t4 - Nursery \n \t5 - Bathroom 1 \n \t6 - Bathroom 2 \n \t7 - Bathroom 3 \n \t8 - Bathroom 4 \n \t9 - 2/3 age Room \n \t10 - 4/5 age Room \n \t11 - Study Room \n \t12 - Canteen \n \t13 - Kitchen \n \t14 - Hall \n --Rooms with no values will not be controled-- \n 15 - See values \n 16 - Exit Menu \n");
		scanf("%i", &option);
		if(option<15 && option >0){
		printf("-Values-\nTemperature (C):");
		scanf("%f", &pre[option][0]);
		printf("Humidity (%%):");
		scanf("%f", &pre[option][1]);
		printf("Light (lux):");
		scanf("%f", &pre[option][2]);
		}else if(option == 15){
			printf("-Preset Values-\nRoom         Temp  Humi  Light\n");
			for(i=0; i<14; i++){
				switch(i+1){
					case 1: printf("Gym          "); break;
					case 2: printf("Entrance     "); break;
					case 3: printf("Office       "); break;
					case 4: printf("Nursery      "); break;
					case 5: printf("Bathroom 1   "); break;
					case 6: printf("Bathroom 2   "); break;
					case 7: printf("Bathroom 3   "); break;
					case 8: printf("Bathroom 4   "); break;
					case 9: printf("2/3 age Room "); break;
					case 10: printf("4/5 age Room "); break;
					case 11: printf("Study Room   "); break;
					case 12: printf("Canteen      "); break;
					case 13: printf("Kitchen      "); break;
					case 14: printf("Hall         "); break;
				}
				printf("%.2f %.2f %.2f\n", pre[i][0], pre[i][1], pre[i][2]);
			}
		}
	}
	return 1;
}

void control(int room){
	int val[5];
	if(values[room][0]<(pre[room][0]-ACEPTEMP)) val[0]=1; //temp val[0]=1 aquecer
	if(values[room][0]>pre[room][0]) val[0]=0; //val[0]=0 ta top
	if(values[room][0]>(pre[room][0]+ACEPTEMP)) val[0]=2; // val[0]=2 arrefecer
	if(values[room][1]<(pre[room][1]-ACEPHUM)) val[1]=1;//humi val[1]=1 dry
	if(values[room][1]>pre[room][1]) val[1]=0;
	if(values[room][1]>(pre[room][1]+ACEPHUM)) val[1]=2;//val[1]=1 + humidity
	if(values[room][2]< HIGHLIGHT){
		if(values[room][2]> MIDLIGHT)  val[2]=1; //just mid light
		else val[2]=2; //full light
	}
	if(values[room][3] < LOWBATTERY) val[3]=1; //Batte LOW
	if(values[room][4] > LEVELRED) val[4]=1; //red
	if(room == 1){ //GYM - blinds light heater and air cooler
		if((val[0]==1) && (values[15][0]>pre[1][0])){ // Aquecer e temperatura fora maior que preset
			atua[1][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[1][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[15][2]=1;//Window open
			}else{
				atua[15][0]=1; // heater ON
			}
		}
		if((val[0]==2) && (values[15][0]<pre[1][0])){ // Arrefecer e temperatura fora menor que preset
			atua[1][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[1][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[1][2]=1;//Window open
			}else{
				atua[1][1]=1; // air cooler ON
			}
		}
		atua[1][6]=val[2];		
	}
	if((room==5) || (room==7) || (room==8) || (room==4) || (room==2)){ //Bathroom 1 or 3 or 4 or Nursery or Entrance - windows, blinds, lights, heater and dryer
		if((val[0]==1) && (values[15][0]>pre[1][0])){ // Aquecer e temperatura fora maior que preset
			atua[room][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[room][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[room][2]=1;//Window open
			}else{
				atua[room][0]=1; // heater ON
			}
		}
		if((val[0]==2) && (values[15][0]<pre[1][0])){ // Arrefecer e temperatura fora menor que preset
			atua[room][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[room][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[room][2]=1;//Window open
			}else{
				atua[room][3]=2; //BLIND HALF OPEN
			}
		}
		atua[room][6]=val[2]; // LIGHT
		atua[room][5]=val[1]; // Dryer
	}
	if((room==3) || (room==9) || (room==10) || (room==11) || (room==12)){ // Office, 2/3 age room, 4/5 room, study room, canteen - windows, blinds, heater, light
		if((val[0]==1) && (values[15][0]>pre[1][0])){ // Aquecer e temperatura fora maior que preset
			atua[room][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[room][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[room][2]=1;//Window open
			}else{
				atua[room][0]=1; // heater ON
			}
		}
		if((val[0]==2) && (values[15][0]<pre[1][0])){ // Arrefecer e temperatura fora menor que preset
			atua[room][3]=1; //BLIND ALL OPEN 
			if((values[15][1]<pre[room][1]) && (val[4]==0)){ //humidade fora menor que preset e não ha pessoas
				atua[room][2]=1;//Window open
			}else{
				atua[room][3]=2; //BLIND HALF OPEN
			}
		}
		atua[room][6]=val[2]; // LIGHT
	}
	if((room==6) || (room==14)){ // bathroom 2, hall - heater, lights, dryer
		if(val[0]==1) //Aquecer
			atua[room][0]=1; //HEater on
		if(val[1]==1) //Humidity
			atua[room][5]=1; //Dryer on
		atua[room][6]=val[2]; // LIGHT
	}
	if(room==13){ //kitchen - heater, lights , air cooler,dryer
		if(val[0]==1) //Aquecer
			atua[room][0]=1; //HEater on
		if(val[0]==2) //Arrefecer
			atua[room][1]=1; //Air cooler
		if(val[1]==1) //Humidity
			atua[room][5]=1; //Dryer on
		atua[room][6]=val[2]; // LIGHT
	}
}

int main(){
	sensorread();
	return 0;
}
