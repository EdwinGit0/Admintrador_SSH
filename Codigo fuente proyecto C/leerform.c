#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include "insertar.c"
#define MAXLEN 1024
#define MAX 1024
#define NUM_STRINGS 10

void separar(char *cadena, char *linea, char separador){
    int x, y;
    x = 0; y = 0;
    while ((linea[x]) && (linea[x] != separador)){
	cadena[x] = linea[x];
	x = x + 1;
    }
    cadena[x] = '\0';
    if (linea[x]) ++x;
    y = 0;
    while (linea[y] = linea[x]){
	linea[y] = linea[x];
	y++;
	x++;
    }
}

int main(void){
    char *lenstr;
    char inputBuffer[MAXLEN];
    char buffer[MAXLEN];
    int contentLength;
    int i;
    char x;
    char mensaje[80];
    char shell[80];
    char callowuser[MAX]="AllowUsers	",cdenyuser[MAX]="DenyUsers	",callowgroup[MAX]="AllowGroups	",cdenygroup[MAX]="DenyGroups	";
    int longAU = 0, longDU = 0, longAG = 0, longDG = 0; 

    printf ("Content-type:text/html\n\n");
    printf("<TITLE>Response</TITLE>\n");
    lenstr = getenv("CONTENT_LENGTH");
    //lenstr = (char *)getenv("CONTENT_LENGTH");
    if (lenstr!= NULL){
	contentLength = atoi(lenstr);   
    }
    else contentLength = 0;

    if (contentLength > sizeof(inputBuffer)-1)
	contentLength = sizeof(inputBuffer)-1;

    i = 0;
    while (i < contentLength){
	x = fgetc(stdin);
	if (x==EOF) break;
	inputBuffer[i] = x;
	buffer[i] = x;
	i++;
    }
    inputBuffer[i] = '\0';
    buffer[i] = '\0';
    contentLength = i;

    printf("<br>Datos Formulario: %s\n", inputBuffer);

    while(*inputBuffer != 0){

    	separar(shell, inputBuffer, '=');
    	separar(mensaje, inputBuffer, '&');

	if(strcmp(mensaje, "allowuser") == 0){
                strcat(callowuser, shell);
		strcat(callowuser, ", ");
		longAU++;
	}
	if(strcmp(mensaje, "denyuser") == 0){
                strcat(cdenyuser, shell);
                strcat(cdenyuser, ", ");
		longDU++;
        }
	if(strcmp(mensaje, "allowgroup") == 0){
                strcat(callowgroup, shell);
                strcat(callowgroup, ", ");
		longAG++;
        }
	if(strcmp(mensaje, "denygroup") == 0){
                strcat(cdenygroup, shell);
                strcat(cdenygroup, ", ");
		longDG++;
        }
    }
    
    if(longAU > 0)
	callowuser[strlen(callowuser)-1] = '\0';callowuser[strlen(callowuser)-1] = '\n';
    
    if(longDU > 0)
	cdenyuser[strlen(cdenyuser)-1] = '\0';cdenyuser[strlen(cdenyuser)-1] = '\n';
    
    if(longAG > 0)
        callowgroup[strlen(callowgroup)-1] = '\0';callowgroup[strlen(callowgroup)-1] = '\n';
    
    if(longDG > 0)
    	cdenygroup[strlen(cdenygroup)-1] = '\0';cdenygroup[strlen(cdenygroup)-1] = '\n';
    
    insertar("AllowUsers", callowuser);
    insertar("DenyUsers", cdenyuser);

    insertar("AllowGroups", callowgroup);
    insertar("DenyGroups", cdenygroup);

    return 0;
}

