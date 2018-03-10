/******************************************************************************
    SINF A54
    Karollina Varis
    Francisco Vieira
    Guilherme Castro
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long Mote;
float battery, temper, humid, light, redlig;

int sensorread(void){
    char name[100]="12 13 7E 15 16 17 7E 45 00 FF FF 00 05 0A 3F 0C 0F CC 1D 3F 02 27 00 BD 01 6D 48 81 7E", valor[40];
	char *str, *str1;
	const char s[1] = " ";
	long Bat, Temp, Hum, Lig, Red;
	int ready=0, counter=0;
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
	humid = (float) -2.0468 + (0.0367 * (float) Hum) - ((1.5955 * ((float) Hum) * ((float) Hum))/1000000) ;
	humid = (float) (temper - 25) * (0.01 + (0.00008 * ((float) Hum))) + humid;
	printf("Humidity: %.2f %%\n", humid);
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
	while(strcmp(str, "7E")!=0){
		str = strtok(NULL, s);
	}
	return 1;
}

int main(){
		
	
	return 0;
}
