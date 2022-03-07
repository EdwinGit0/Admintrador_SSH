#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "insertar.c"
#define MAX 1024

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

bool probar(char *mensaje){
    int minUID = 1000;
    int maxUID = 1050;

    for (int i = minUID; i < maxUID; ++i){
        struct passwd * p = getpwuid(i);
        struct group * grp = getgrgid(i);

        if (p != NULL){
                if(strcmp(mensaje, p->pw_name) == 0){
                        return (true);
                }
        }
        if (grp != NULL){
                if(strcmp(mensaje, grp->gr_name) == 0){
                        return (true);
                }
        }
   }
    return (false);
}

char* buscar_cadena(char *str1,char *str2){
        int len1 = strlen(str1);
        int len2 = strlen(str2);
        int i;
        char c;
        if(len2 > len1){
                return NULL;
        }else {
                i = 0;
                c = str2[0];
                while(i<len1){
                        if(str1[i] == c){
                        if(strncmp(&str1[i],str2,len2)==0){
                                return &str1[i];
                        }
                }
                i++;
        }
        return NULL;
        }
}

void actualizarPagina(char *dUser, char *aUser, char *dGroup, char *aGroup){
	char cdenyuser[MAX]="DenyUsers	",cdenygroup[MAX]="DenyGroups	";
	int longDU = 0, longDG = 0;
	char *cadena1 = dUser, *cadena2 = dGroup;
	int minUID = 1000;
    	int maxUID = 1050;
	char *busqueda = NULL;

	cadena1[strlen(cadena1)-1] = '\0';
	cadena2[strlen(cadena2)-1] = '\0';
    for (int i = minUID; i < maxUID; ++i){
        struct passwd * p = getpwuid(i);
        struct group * grp = getgrgid(i);

        if (p != NULL){
	busqueda = buscar_cadena(dUser,p->pw_name);
        	if(busqueda == NULL){
			busqueda = buscar_cadena(aUser,p->pw_name);
                        if(busqueda == NULL){
				strcat(cadena1, ", ");
                                strcat(cadena1, p->pw_name);
				longDU++;
                        }		
		}
	}
	if (grp != NULL){
        busqueda = buscar_cadena(dGroup,grp->gr_name);
                if(busqueda == NULL){
                        busqueda = buscar_cadena(aGroup,grp->gr_name);
                        if(busqueda == NULL){
                                strcat(cadena2, ", ");
                                strcat(cadena2, grp->gr_name);
				longDG++;
                        }
                }
        }
   }
    	//if(longDU > 0)
      		strcat(cadena1, ".");cadena1[strlen(cadena1)-1] = '\n';
	//if(longDG > 0)
		strcat(cadena2, ".");cadena2[strlen(cadena2)-1] = '\n';

    insertar("DenyUsers", cadena1);
    insertar("DenyGroups", cadena2);

}

char*  buscar(FILE *archivo,char *mensaje, char *user, int comparacion, int i, char *allowusers, char *cadena){
	int longDU = 0;
            while (feof(archivo) == 0){
                 fgets(user,100,archivo);
                 for(i=0;i<strlen(allowusers);i++){
                        if(user[i]!=allowusers[i]){
                                comparacion = 1;
                                break;
                        }else
                                comparacion = 0;
                 }

            if (user[strlen(user)-1] == '\n')
                user[strlen(user)-1] = '\0';

                if(comparacion==0){
			separar(mensaje, user, '	');
            separar(mensaje, user, ',');    

			//if(probar(mensaje)){
			if(strcmp(allowusers, "AllowUsers") == 0 && strcmp(mensaje, "") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

				printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                        	printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","allowuser",mensaje);
                        	printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                		printf("</div>");
                		printf("<div class='form-check form-check-inline'>");
                        	printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","denyuser",mensaje);
                        	printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                		printf("</div></div>");
			}else if(strcmp(allowusers, "DenyUsers") == 0 && strcmp(mensaje, "") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

				printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","allowuser",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","denyuser",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);

                                printf("</div></div>");
			}

			if(strcmp(allowusers, "AllowGroups") == 0 && strcmp(mensaje,"") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

                                printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","allowgroup",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","denygroup",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
			}else if(strcmp(allowusers, "DenyGroups") == 0 && strcmp(mensaje,"") != 0){
                                strcat(cadena, mensaje);
		                strcat(cadena, ", ");longDU++;
				
				printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","allowgroup",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","denygroup",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
			}
			//}
                        while(*user != 0) {
                        separar(mensaje, user, ' ');
                        separar(mensaje, user, ',');
				if(probar(mensaje)){
                        	if(strcmp(allowusers, "AllowUsers") == 0 && strcmp(mensaje,"") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

                                printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","allowuser",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","denyuser",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
				}else if(strcmp(allowusers, "DenyUsers") == 0 && strcmp(mensaje, "") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

                                printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","allowuser",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","denyuser",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
				}

                        	if(strcmp(allowusers, "AllowGroups") == 0 && strcmp(mensaje, "") != 0){
				strcat(cadena, mensaje);
		                strcat(cadena, ", ");longDU++;

                                printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","allowgroup",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","denygroup",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disabled</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
				}else if(strcmp(allowusers, "DenyGroups") == 0 && strcmp(mensaje, "") != 0){
				strcat(cadena, mensaje);
                		strcat(cadena, ", ");longDU++;

                                printf("<div class='mb-3'><div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\">","allowgroup",mensaje);
                                printf("<label class='form-check-label text-success' for='inlineRadio'>enable</label>");
                                printf("</div>");
                                printf("<div class='form-check form-check-inline'>");
                                printf("<input class='form-check-input' type='radio' value=\"%s\" name=\"%s\" checked>","denygroup",mensaje);
                                printf("<label class='form-check-label text-primary' for='inlineRadio2'>disable</label>");
				 printf("<label class='form-check-label ml-4' for='inlineRadio2'> | %s</label>",mensaje);
                                printf("</div></div>");
				}
				}
                        }
                }
           }
	if(longDU > 0)
        	cadena[strlen(cadena)-1] = '\0';cadena[strlen(cadena)-1] = '\n';

	return cadena;
}

int main(void){
 	FILE *archivo; 	
 	char mensaje[100], user[100], group[100], allowusers[100]="AllowUsers", allowgroups[100]="AllowGroups";
	char denyusers[100]="DenyUsers", denygroups[100]="DenyGroups";
	int i, comparacion, comp;
	char callowuser[MAX]="AllowUsers	",cdenyuser[MAX]="DenyUsers	",callowgroup[MAX]="AllowGroups	",cdenygroup[MAX]="DenyGroups	";
	char *aUser,*dUser,*aGroup,*dGroup;

 	archivo = fopen("/etc/ssh/sshd.copia_config","r");
 	if (archivo == NULL)
 		exit(1);
 	else{

	printf ("Content-type:text/html\n\n");
	printf("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css' integrity='sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh' crossorigin='anonymous'>\n");
	printf("<link href='https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/css/bootstrap4-toggle.min.css' rel='stylesheet'>");
        printf("<TITLE>Response</TITLE>\n");
 	printf("<style>");
    		printf("body {  background: #034a70;background: -webkit-linear-gradient(to right, #960606, #034a70); background: linear-gradient(to right, #960606, #034a70); }");
  	printf("</style>");
	 printf("<div style='height: 30px;'></div>");
            printf("<div class='container-fluid'>");
                printf("<div class='row'>");
                    printf("<div class='col-lg-12'>");
                        printf("<div class='card shadow p-3 mb-5 bg-white'>");
                            printf("<div class='card-header text-center bg-primary text-white'><h4>SISTEMA DE ADMINISTRADOR DE PERMISOS</h4></div>");
                                printf("<div class='card-body'>");
                                    printf("<form id='formulario'  method='POST' ACTION='/cgi-bin/leerform' class='needs-validation formulario' novalidate>");
                                    printf("<div class='form-row'>");    
				    printf("<div class='col-md-8 order-md-1'>");
				        printf("<div class='form-row'>");	
                                            printf("<div class='col-md-6 order-md-1'>");
                                                printf("<div class='card mb-2 bg-white'>");
                                                    printf("<div class='card-header'>USUARIOS</div>");
                                                    printf("<div class='card-body'>");
                                                      printf("<div class='mb-3'>");
                                                      dUser = buscar(fopen("/etc/ssh/sshd.copia_config","r"),mensaje,user,comparacion,i,denyusers,cdenyuser);
                                                      aUser = buscar(fopen("/etc/ssh/sshd.copia_config","r"),mensaje,user,comparacion,i,allowusers,callowuser);
                                                      printf("</div>");
                                                    printf("</div>");
                                                printf("</div>");                 
                                            printf("</div>");

                                            printf("<div class='col-md-6 order-md-2'>");
                                                printf("<div class='card mb-2 bg-white'>");
                                                    printf("<div class='card-header'>GRUPOS</div>");
                                                    printf("<div class='card-body'>");
                                                     printf("<div class='mb-3'>");
                                                     dGroup= buscar(fopen("/etc/ssh/sshd.copia_config","r"),mensaje,user,comparacion,i,denygroups,cdenygroup);
                                                     aGroup =buscar(fopen("/etc/ssh/sshd.copia_config","r"),mensaje,user,comparacion,i,allowgroups,callowgroup);
                                                     printf("</div>");
                                                    printf("</div>");
                                                printf("</div>");  
                                            printf("</div>");
					printf("</div>");
					printf("<input class='btn btn-primary btn-lg btn-block' value='Guardar cambios' id='guardar' type='submit'>");
                                    printf("</div>");

				    	   printf("<div class='col-md-4 order-md-1'>");
                                                printf("<div class='card mb-2 bg-white'>");
                                                    printf("<div class='card-header'>ACCIONES DEL SERVIDOR</div>");
                                                    printf("<div class='card-body'>");
                                                      printf("<div class='mb-3'>");
                                                      	     
                                             		printf("<button type='button' class='btn btn-outline-success btn-lg btn-block' id='activar'>Habilitar</button>");
							printf("<button type='button' class='btn btn-outline-danger btn-lg btn-block' id='desactivar'>Deshabilitar</button>");
		
                                                      printf("</div>");
						      printf("<div class='card-footer'>ESTADO DEL SERVIDOR</div>");

                                                    printf("</div>");
                                                printf("</div>");
                                            printf("</div>");
								
					    printf("</div>");


				    printf("</form>");
                                printf("</div>");
                        printf("</div>");
                    printf("</div>");
                printf("</div>");
            printf("</div></body>");
	    printf("<script src='https://unpkg.com/sweetalert/dist/sweetalert.min.js'></script>");
	   printf("<script>");
	 	printf("document.getElementById('activar').onclick = function(){");
		system("service sshd start");
    		printf("swal('SERVIDOR ACTIVO','El servidor esta activo','success');");
		printf("}");
	   printf("</script>");

	 printf("<script>");
                printf("document.getElementById('desactivar').onclick = function(){");
		printf("swal('SERVIDOR DESACTIVADO','El servidor esta desactivo','warning');");
		system("service sshd stop");
                printf("}");
           printf("</script>");
   
	}
	actualizarPagina(dUser,aUser,dGroup,aGroup);
        fclose(archivo);
	return 0;
}
