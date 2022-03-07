#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#define MAXLEN 1024
#define MAX 1024
#define NUM_STRINGS 10

int insertar(char *allowusers, char *newln)
  {
        FILE *fptr1, *fptr2;
        int lno = 3, linectr = 0;
        char str[MAX],fname[] = "/etc/ssh/sshd.copia_config";
        char temp[] = "/etc/ssh/temp.txt";
        char mensaje[80], user[100];
        int i, comparacion, comp=0;

        fptr1 = fopen(fname, "r");
        if (!fptr1)
        {
                printf("Error al abrir archivo!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2)
        {
                printf("Error ala abrir archivo!!\n");
                fclose(fptr1);
                return 0;
        }

        while (feof(fptr1) == 0){
		fgets(user,100,fptr1);
                 for(i=0;i<strlen(allowusers);i++){
                        if(user[i]!=allowusers[i]){
                                comparacion = 1;
                                break;
                        }else
                                comparacion = 0;
                 }
                 comp++;
                 if(comparacion==0){
          
                 break;
                 }
        }
        fclose(fptr1);
        fptr1 = fopen(fname, "r");
         while (!feof(fptr1))
        {
            strcpy(str, "\0");
            fgets(str, MAX, fptr1);
            if (!feof(fptr1))
            {
                linectr++;
                if (linectr != comp)
                    {
 			fprintf(fptr2, "%s", str);
                    }
                    else
                    {
                        fprintf(fptr2, "%s", newln);
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(fname);
        rename(temp, fname);
        return 0;
 }


