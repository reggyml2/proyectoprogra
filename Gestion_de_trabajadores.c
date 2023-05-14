#include <stdio.h>
#include <string.h>

typedef enum {
	RRHH=1,
	CONSULTORIA=2,
	DISENO=3,
	PRODUCCION=4,//departamento de trabajadores
	CALIDAD=5,
	DISTRIBUCION=6 
}departamento;

typedef enum cargos{
	GERENTE=1,
	SUPERVISOR=2,
	ANALISTA=3,
	DISENADOR=4,//cargos de trabajadores
	DESARROLLADOR=5,
	AUDITOR=6 
}cargos_t;

typedef struct 
{
	int dia;
	int mes; //fecha de ingreso
	int ano; 
}FECHA;

typedef struct {
	
	int CI;
	char nombre[20];
	char apellido[20];	//datos empleados   
	float sueldo;
	FECHA fecha;	
}empleado;

//prototipo; 1.modulo empresa
void ingresar(empleado empleados[], int empl, FILE* ARCH); //funcion que ingresa los empleados
void departamentos(FILE* deparARCH); // funcion que llena el campo departamento
void cargos (FILE* cargARCH); // funcion que llena el campo cargo
int validarFecha(int dia,int mes,int ano); //funcion que valida las fechas

//prototipo; 2.modulo consulta
void menuDeOpciones();
void consultaPorCi();
void CONSULTAporDEPA();
void CONSULTAporCARGO();
void contarDepa(int* emple, float* sueldo, char depa[20]);
void contarCargo(int* eEMPLE, float* SUELDOS, char cargo[20]);
//void sueldoMayorMenor();
// prototipo: 3. modulo modificar
void modificar(FILE* arch);
//prototipo: 4.modulo eliminar
void eliminar(FILE* arch);
//void eliminarEMPLEADO(FILE* arch);
void ingresarEMPLE(empleado empleados, FILE* ARCH,char depa[20],char cargo[20]);
//programa principal
int main(){
	empleado empleados[2000];
	FILE* trabajadores;
	int lim,e;
	char op;
	//verificar que el documento abrio perfectamente
	if((trabajadores=fopen("trabajadores.in","a+"))==NULL){ 
		printf("\nError: no se pudo abrir el archivo");
		
	}else{
		//imprimir menu
		do{
			printf("\n===============================================\n\n");
			printf("\n  |     BIENVENIDO AL GESTOR DE EMPLEADOS    |\n");
			printf("  |           [1] INGRESAR                      |\n");
			printf("  |           [2] CONSULTAR                     |\n");
			printf("  |           [3] MODIFICAR                     |\n");
			printf("  |           [4] ELIMINAR                      |\n");
			printf("  |           [5] SALIR                         |\n");
			printf("\n===============================================\n\n");
			printf("OPCION: ");
			scanf("%s",&op); // se ha puesto las opciones tipo caracter para que no hayan errores en caso que se ingrese un caracter en vez de un entero
			switch (op){
				case '1':
					//este caso ingresa los empleados a un archivo .in 
					do{
						printf("\nIngrese la cantidad de personas que desea ingresar: ");
						scanf("%d", &lim);	    
					}while(lim>2000);
					ingresar(empleados,lim,trabajadores);
					fclose(trabajadores);
				break;
				case '2':
					//consulta a los trabajadores
					fclose(trabajadores);
					menuDeOpciones();
				break;
				case '3':
					//modificacion de los trabajadores
					modificar(trabajadores);
					fclose(trabajadores);
				break;
				case '4':
					trabajadores=fopen("trabajadores.in","r");
					//eliminar trabajadores
					eliminar(trabajadores);
					fclose(trabajadores);
				break;
				case '5':
					printf("\n\n============================\n\n");
					printf("    |   ADIOS, VUELVA PRONTO :) |  ");
					printf("\n\n============================\n\n");
				break; 
			default:
					printf("\n\n\n===============================================\n\n");
					printf("     |   ESTOY SEGURO QUE ESTO %d NO ES UNA OPCION |  ",op);
					printf("\n\n=============================================\n\n\n");
				break;
			}

		}while(op!='5');
		fclose(trabajadores);
	}
	
	return 0;
}
/*
1. modulo ingresar
Permite registrar la informacion de nuevos trabajadores, de acuerdo a los datos indicados 
previamente, a partir de un numero n indicado por el usuario. Toda la informacion se almacena en un archivo 
llamado trabajadores.in, cada nuevo registro se ubica al final de este archivo
*/
void ingresar(empleado empleados[],int empl, FILE* ARCH){
	int i,valFecha=0;
	for (i=1;i<=empl;i++){
		//guardamos la cedula del empleado en el archivo
		printf("\nIngresa CI del empleado: ");
		scanf("%d", &empleados[i].CI);
		fprintf(ARCH,"\n%d\t\t", empleados[i].CI); //ingresando en el archivo 

		//guardamos el nombre del empleado en el archivo
		printf("\nIngresa Nombre del empleado: ");
		scanf("%s", empleados[i].nombre);
		fprintf(ARCH,"%s", empleados[i].nombre);

		//guardamos el apellido del empleado en el archivo
		printf("\nIngresa Apellido del empleado: ");
		scanf("%s", empleados[i].apellido);
		fprintf(ARCH," %s\t\t", empleados[i].apellido);
		
		departamentos(ARCH); // funcion que llena el campo departamento
		cargos(ARCH); // funcion que llena el campo cargos

		//pedimos al usuario su fecha de ingreso y validamos que sea correcta, si es asi la guardamos en el archivo trabajadores.in
		do{
			printf("\nIngresa la fecha que ingreso a la empresa en formato DD/MM/AA: \n");
			printf("Dia: "); scanf("%d",&empleados[i].fecha.dia);
			printf("\nMes: "); scanf("%d",&empleados[i].fecha.mes);
			printf("\nAno: "); scanf("%d",&empleados[i].fecha.ano);
			valFecha=validarFecha(empleados[i].fecha.dia,empleados[i].fecha.mes,empleados[i].fecha.ano);
		}while(valFecha !=1);
		if (valFecha == 1){
			fprintf(ARCH,"%d/%d/%d\t\t", empleados[i].fecha.dia,empleados[i].fecha.mes,empleados[i].fecha.ano);
		}
		//pedimos el sueldo del trabajador para ingresarlo en trabajadores.in y sera formateada con 3 decimales
		printf("\nSalario: ");scanf("%f",&empleados[i].sueldo);
		fprintf(ARCH," %.3f\t\t \n\n", empleados[i].sueldo);
	}
}

//funcion que llena el campo de partamento
void departamentos(FILE* deparARCH){
	
	int depa;
	//menu para llenar los datos del departamento del empleado
	do{
		printf("\nIngrese el departamento que pertenece: ");
		printf("\n1.RRHH");
		printf("\n2.CONSULTORIA");
		printf("\n3.DISENO");
		printf("\n4.PRODUCCION");
		printf("\n5.CALIDAD");
		printf("\n6.DISTRIBUCION\n");
		scanf("%d", &depa);
		switch(depa){
			case RRHH:
				fprintf(deparARCH,"%s\t\t", "RRHH");
			break;
			case CONSULTORIA:
				fprintf(deparARCH,"%s\t\t", "CONSULTORIA");
			break;
			case DISENO:
				fprintf(deparARCH,"%s\t\t", "DISENO");
			break;
			case PRODUCCION:
				fprintf(deparARCH,"%s\t\t", "PRODUCCION");
			break;
			case CALIDAD:
				fprintf(deparARCH,"%s\t\t", "CALIDAD");
			break;
			case DISTRIBUCION:
				fprintf(deparARCH,"%s\t\t", "DISTRIBUCION");
			break;
				
		}
			
	}while ((depa!=1) && (depa!=2) && (depa!=3) && (depa!=4) && (depa!=5) && (depa!=6));
}
//funcion que llena el campo cargo
void cargos(FILE* cargARCH){
	int CARGOS;
	//menu para llenar los datos del cargo del empleado
	do{
		printf("\nIngrese el cargo que pertene: ");
		printf("\n1.GERENTE");
		printf("\n2.SUPERVISOR");
		printf("\n3.ANALISTA");
		printf("\n4.DISENADOR");
		printf("\n5.DESARROLLADOR");
		printf("\n6.AUDITOR\n");
		scanf("%d",&CARGOS);
		switch(CARGOS){
			case GERENTE:
				fprintf(cargARCH,"%s\t\t","GERENTE");
			break;
			case SUPERVISOR:
					fprintf(cargARCH,"%s\t\t","SUPERVISOR");
			break;
			case ANALISTA:
					fprintf(cargARCH,"%s\t\t","ANALISTA");
			break;
			case DISENADOR:
				fprintf(cargARCH,"%s\t\t","DISENADOR");
			break;
			case DESARROLLADOR:
				fprintf(cargARCH,"%s\t\t","DESARROLLADOR");
			break;
			case AUDITOR:
				fprintf(cargARCH,"%s\t\t","AUDITOR");
			break;
			
		}
	}while ((CARGOS!=1) && (CARGOS!=2) && (CARGOS!=3) && (CARGOS!=4) && (CARGOS!=5) && (CARGOS!=6));
}

int validarFecha(int dia,int mes,int ano){
	if((mes>=1) && (mes<=12)){
		switch (mes){
			case  1 :
            case  3 :
            case  5 :
            case  7 :
            case  8 :
            case 10 :
            case 12 :
				if((dia>=1) && (dia<=31)){
					return 1;
				}
			break;
			case  4 :
            case  6 :
            case  9 :
            case 11 :
				if((dia>=1) && (dia<=31)){
					return 1;
				}
			break;
			case 2:
				if((ano % 4 == 0) && (ano % 100 !=0)){
					if((dia>=1) && (dia<=29)){
						return 1;
					}
				}else {
					if((dia>=1) && (dia<=28)){
						return 1;
					}
				}
			break;
		}
	}

		return 0;
}


//menu de opciones del modulo dos
void menuDeOpciones(){
	char op;
	//imprimimos menu para preguntar al usuario que tipo de busqueda quiere hacer
	do{
		printf("\n===============================================================\n\n");
		printf("\n  |     	BIENVENIDO AL GESTOR DE EMPLEADOS          |\n");
		printf("  |           [a] CONSULTA POR CI                      		|\n");	
		printf("  |           [b] CONSULTA POR DEPARTAMENTO                    	|\n");
		printf("  |           [c] CONSULTA POR CARGO                     	|\n");
		printf("  |           [d] MENOR Y MAYOR SUELDO                    	|\n");
		printf("  |           [f] SALIR                         	        |\n");
		printf("\n===============================================================\n\n");
		printf("OPCION: ");
		scanf("%s",&op);
		switch (op){
		case 'a':
			//opcion para consultar por la cedula del empleado
			consultaPorCi();
		break;
		case 'b':
			//opcion para consultar por el departamento del empleado
			CONSULTAporDEPA();
		break;
		case 'c':
			//opcion para consultar por el cargo del empleado
			CONSULTAporCARGO();
		break;
		case 'd':
			//consultar empleado con mayor y menor sueldo 
		break;
		case 'f':
			//ARREGLAR ESTO (APRENDETE EL ABECEDARIO)
		break;
		default:
			printf("\n\n\n===============================================\n\n");
			printf("     |   ESTOY SEGURO QUE ESTO %d NO ES UNA OPCION |  ",op);
			printf("\n\n=============================================\n\n\n");
		}

	}while(op!='f');
	
}
/*consulta por CI, muestra al empleado que esta asociado con esa CI*/
void consultaPorCi(){
	//abrimos el archivo y la estructura empleados y variables para los demas datos de dicha estructura
	FILE* ARCHConsulta;
	empleado empleados;
	int ci;
	char cargo[20], depa[20],fecha[20];
	ARCHConsulta=fopen("trabajadores.in","a+"); //se tuvo que abrir y cerrar el archivo en una nueva funcion porque me daba conflicto pasandolo por parametro
	if(ARCHConsulta == NULL){
		printf("NO SE ENCONTRO ARCHIVO");
		return ;
	}
	//pedimos al usuario la cedula
	printf("|   BUSCAR EMPLEADO\nINGRESE LA CI:   ");
	scanf("%d",&ci);
	//iniciamos el ciclo de busqueda mientras no sea el final del archivo y no haya una ocurrencia en la cedula
	while((!(feof(ARCHConsulta))) && (empleados.CI!=ci)){
		fscanf(ARCHConsulta,"%d", &empleados.CI);
		fscanf(ARCHConsulta,"%s",empleados.nombre);
		fscanf(ARCHConsulta,"%s",empleados.apellido);
		fscanf(ARCHConsulta,"%s",cargo);
		fscanf(ARCHConsulta,"%s",depa);
		fscanf(ARCHConsulta,"%s",fecha);
		fscanf(ARCHConsulta,"%f",&empleados.sueldo);
	}
	if(empleados.CI == ci){
		//si encuentra la cedula la imprimimos por pantalla
		printf("El nombre es: %s \n",empleados.nombre);
		printf("El apellido es: %s \n",empleados.apellido);
		printf("El cargo es: %s \n",cargo);
		printf("El departamento es: %s \n",depa);
		printf("La fecha de ingreso es: %s \n",fecha);
		printf("El sueldo es: %.2f \n",empleados.sueldo);
	}else{
		printf("\nLA CI INDICADA NO ESTA ASOCIADA A NINGUN TRABAJADOR DE LA EMPRESA\n");
	}

	fclose(ARCHConsulta);

}

/*consulta por numero de departamento y cuenta cuantos hay ahí,
genera un archivo con el total de trabajadores y sueldo que hay por todos los trabajadores*/

void CONSULTAporDEPA(){
	//abrimos el archivo y las variables que vamos a utilizar
	FILE* ARCHdepa;
	int depa;
	int conTrabajadores=0;
	float sueldos=0;
	
	ARCHdepa=fopen("departamentos.txt","a+");
	if(ARCHdepa == NULL){
		printf("NO SE ENCONTRO ARCHIVO");
		return;
	}
	//menu para preguntar en cual departamento quiere buscar
	do{	
		printf("\nIngrese el departamento que pertenece: ");
		printf("\n1.RRHH");
		printf("\n2.CONSULTORIA");
		printf("\n3.DISENO");
		printf("\n4.PRODUCCION");
		printf("\n5.CALIDAD");
		printf("\n6.DISTRIBUCION");
		printf("\n7.SALIR\n");
		scanf("%d",&depa);
		switch (depa){
			case RRHH:
				//caso para contar los empleados de RRHH y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"RRHH");		
				printf("\n\nPersonal RRHH: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento RRHH: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case CONSULTORIA:
				//caso para contar los empleados de CONSULTORIA y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"CONSULTORIA");		
				printf("\n\nPersonal CONSULTORIA: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento CONSULTORIA: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case DISENO:
				//caso para contar los empleados de DISENO y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"DISENO");
				printf("\n\nPersonal DISENO: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento DISENO: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case PRODUCCION:
				//caso para contar los empleados de PRODUCCION y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"PRODUCCION");
				printf("\n\nPersonal PRODUCCION: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento PRODUCCION: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case CALIDAD:
				//caso para contar los empleados de CALIDAD y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"CALIDAD");
				printf("\n\nPersonal CALIDAD: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento CALIDAD: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case DISTRIBUCION:
				//caso para contar los empleados de DISTRIBUCION y sus sueldos
				contarDepa(&conTrabajadores,&sueldos,"DISTRIBUCION");
				printf("\n\nPersonal DISTRIBUCION: %d\nTotal sueldos: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"departamento DISTRIBUCION: %d \nsueldos %.2f",conTrabajadores,sueldos);
				fprintf(ARCHdepa,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			default:
				printf("\nOPCION INVALIDA");
		}

	}while (depa!=7);

	fclose(ARCHdepa);

}
void contarDepa(int* emple, float* sueldo,char depa[20]){
	//abrimos el archivo de consulta y la estructura empleados

	FILE* ARCHConsulta;
	empleado empleados;
	int sueldos=0;
	char cargo[20],fecha[20],departa[20];
	ARCHConsulta=fopen("trabajadores.in","a+");
	if(ARCHConsulta==NULL){
		printf("\nNO SE ENCONTRO ARCHIVO\n");
		return ;
	}
	//iniciamos el ciclo de lectura del archivo
	while(!(feof(ARCHConsulta))){
			fscanf(ARCHConsulta,"%d", &empleados.CI);
			fscanf(ARCHConsulta,"%s",empleados.nombre);
			fscanf(ARCHConsulta,"%s",empleados.apellido);
			fscanf(ARCHConsulta,"%s",departa);	
			fscanf(ARCHConsulta,"%s",cargo);
			fscanf(ARCHConsulta,"%s",fecha);
			fscanf(ARCHConsulta,"%f",&empleados.sueldo);
			if(strcmp(depa,departa)==0){ //comparamos cadenas para saber hacer el conteo
				//cada vez que encuentre un empleado en este departamento incrementamos el acumulador de empleados y de sueldos
				*emple+=1;
				sueldos=empleados.sueldo;
				*sueldo+=sueldos;
			}
	}
	fclose(ARCHConsulta);
}
void CONSULTAporCARGO(){
	//abrimos el archivo de consulta
	FILE* ARCHcargo;
	int CARGOS;
	int conTrabajadores=0;
	float sueldos=0;
	ARCHcargo=fopen("cargo.txt","a+");
	if(ARCHcargo == NULL){
		printf("NO SE ENCONTRO ARCHIVO");
		return;
	}
	//menu para preguntar por cual cargo quiere buscar
	do{
		printf("\nIngrese el cargo que pertene: ");
		printf("\n1.GERENTE");
		printf("\n2.SUPERVISOR");
		printf("\n3.ANALISTA");
		printf("\n4.DISENADOR");
		printf("\n5.DESARROLLADOR");
		printf("\n6.AUDITOR");
		printf("\n7.SALIR\n");
		scanf("%d",&CARGOS);
		switch(CARGOS){
			case GERENTE:
				//caso para contar los empleados en el cargo GERENTE
				contarCargo(&conTrabajadores,&sueldos,"GERENTE");
				printf("\nPersonal GERENTE: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo GERENTE: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case SUPERVISOR:
				//caso para contar los empleados en el cargo SUPERVISOR
				contarCargo(&conTrabajadores,&sueldos,"SUPERVISOR");
				printf("\nPersonal SUPERVISOR: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo SUPERVISOR: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case ANALISTA:
				//caso para contar los empleados en el cargo ANALISTA
				contarCargo(&conTrabajadores,&sueldos,"ANALISTA");
				printf("\nPersonal ANALISTA: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo ANALISTA: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case DISENADOR:
				//caso para contar los empleados en el cargo DISENADOR
				contarCargo(&conTrabajadores,&sueldos,"DISENADOR");
				printf("\nPersonal DISENADOR: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo DISENADOR: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case DESARROLLADOR:
				//caso para contar los empleados en el cargo DESARROLLADOR
				contarCargo(&conTrabajadores,&sueldos,"DESARROLLADOR");
				printf("\nPersonal DESARROLLADOR: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo DESARROLLADOR: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			case AUDITOR:
				//caso para contar los empleados en el cargo AUDITOR
				contarCargo(&conTrabajadores,&sueldos,"AUDITOR");
				printf("\nPersonal AUDITOR: %d\nTotal de sueldo es: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"\nCargo AUDITOR: %d\nSUELDOS: %.2f",conTrabajadores,sueldos);
				fprintf(ARCHcargo,"%s","\n==================================\n");
				conTrabajadores=0;
				sueldos=0;
			break;
			default:
				printf("\nOPCION INVALIDA");
		}
	}while (CARGOS!=7);

	fclose(ARCHcargo);
}

void contarCargo(int* EMPLE, float* sueldos, char cargo[20]){
	//abrimos el archivo y la estructura empleados
	FILE* ARCHcargo;
	empleado empleados;
	int sueldo=0;
	char E_cargo[20],fecha[20],departa[20];
	ARCHcargo=fopen("trabajadores.in","a+");
	if(ARCHcargo == NULL){
		printf("\nNO SE ENCONTRO ARCHIVO\n");
	}
	fread(&empleados,sizeof(empleado)+1,1,ARCHcargo);
	//iniciamos el ciclo de lectura del archivo
	while(!(feof(ARCHcargo))){
		fscanf(ARCHcargo,"%d", &empleados.CI);
		fscanf(ARCHcargo,"%s",empleados.nombre);
		fscanf(ARCHcargo,"%s",empleados.apellido);
		fscanf(ARCHcargo,"%s",departa);	
		fscanf(ARCHcargo,"%s",E_cargo);
		fscanf(ARCHcargo,"%s",fecha);
		fscanf(ARCHcargo,"%f",&empleados.sueldo);
		if(strcmp(cargo,E_cargo)==0){
			//cada vez que encuentre un empleado en este cargo incrementamos el acumulador de empleados y de sueldos
			*EMPLE+=1;
			sueldo=empleados.sueldo;
			*sueldos+=sueldo;
		}
	}
	fclose(ARCHcargo);

}
//trabajador con menor y mayor sueldo

void modificar(FILE* arch){
	//abrimos el archivo y la estructura empleados y las variables necesarias
	int ci, exist=0,valiFecha;
	empleado empleados;
	char depa[20],cargo[20],fecha[20];

	if(arch == NULL){
		printf("\nNO SE ENCONTRO ARCHIVO\n");
		return;
	}
	printf("\nINGRESE LA CI DEL USUARIO QUE DESEA MODIFICAR: ");
	scanf("%d",&ci);
	//fread(&empleados,sizeof(empleados),1,modificarARCH);
	//iniciamos el ciclo para encontrar el usuario con la CI ingresada
	while((!(feof(arch))) && (exist==0)){
		fscanf(arch,"%d",&empleados.CI);
		if(empleados.CI ==ci){
			fscanf(arch,"%s",empleados.nombre);
			printf("\nEl nombre asociado es: %s",empleados.nombre);
			fscanf(arch,"%s",empleados.apellido);
			printf("\nEl apellido asociado es: %s",empleados.apellido);
			fscanf(arch,"%s",depa);
			printf("\nEl departamento asociado es: %s",depa);
			fscanf(arch,"%s",cargo);
			printf("\nEl cargo asociado es: %s",cargo);
			fscanf(arch,"%s",fecha);
			printf("\nLa fecha de ingreso es: %s",fecha);
			//fread(&empleados,sizeof(empleados),1,arch);
			exist=1;
		}	
	}
	//si hay ocurrencia establecemos los nuevos datos de ese empleado
	if(exist==1){
		printf("\n\n===============================\n");
		printf("\n\nINGRESE NUEVO NOMBRE: ");
		scanf("%s",empleados.nombre);
		printf("%s",empleados.nombre);
		printf("\nINGRESE NUEVO APELLIDO: ");
		scanf("%s",empleados.apellido);
		printf("\nINGRESE NUEVO DEPARTAMENTO: ");
		departamentos(arch);
		printf("\nINGRESE NUEVO CARGO: ");
		cargos(arch);
		//ciclo de validacion de fecha
		do{
			printf("\nIngresa la fecha que ingreso a la empresa en formato DD/MM/AA: \n");
			printf("Dia: "); scanf("%d",&empleados.fecha.dia);
			printf("\nMes: "); scanf("%d",&empleados.fecha.mes);
			printf("\nAno: "); scanf("%d",&empleados.fecha.ano);
			valiFecha=validarFecha(empleados.fecha.dia,empleados.fecha.mes,empleados.fecha.ano);
		}while(valiFecha !=1);
		//ingresamos el nuevo salario
		printf("\nINGRESE EL NUEVO SALARIO: ");
		scanf("%f",&empleados.sueldo);
		//actualizamos al empleado  en el archivo
		fprintf(arch,"%s",empleados.nombre);
		fprintf(arch,"%s",empleados.apellido);
		fprintf(arch,"%s",depa);
		fprintf(arch,"%s",cargo);
		fprintf(arch,"%d/%d/%d",empleados.fecha.dia,empleados.fecha.mes,empleados.fecha.ano);
		//fwrite(&empleados,sizeof(empleados),1,arch);
	}
	fclose(arch);
}

void eliminar(FILE* arch){
	//abrimos el archivo y la estructura empleados y las variables necesarias
	int ci, exist=0,valiFecha;
	empleado empleados;
	char depa[20],cargo[20],fecha[20];
	FILE* archivotemporal;
	FILE* extrabajadores;
	extrabajadores=fopen("extrabajadores.txt","a+");
	if(arch == NULL){
		printf("\nNO SE ENCONTRO ARCHIVO\n");
		return;
	}
	printf("\nINGRESE LA CI DEL USUARIO QUE DESEA ELIMINAR: ");
	scanf("%d",&ci);
	//iniciamos el ciclo para encontrar el usuario de la CI ingresada
	while((!(feof(arch))) && (exist==0)){
		fscanf(arch,"%d",&empleados.CI);
		fscanf(arch,"%*[^ \t]");
		if(empleados.CI ==ci){
			fscanf(arch,"%s",empleados.nombre);
			printf("\nEl nombre asociado es: %s",empleados.nombre);
			fscanf(arch,"%s",empleados.apellido);
			printf("\nEl apellido asociado es: %s",empleados.apellido);
			fscanf(arch,"%s",depa);
			printf("\nEl departamento asociado es: %s",depa);
			fscanf(arch,"%s",cargo);
			printf("\nEl cargo asociado es: %s",cargo);
			fscanf(arch,"%s",fecha);
			printf("\nLa fecha de ingreso es: %s",fecha);
			exist=1;
		}	
	}
	//si hay ocurrencia establecemos los nuevos datos de ese empleado
	if(exist==1){
		char op;
		//le preguntamos al usuario si esta seguro de eliminar al empleado
		do{
			printf("\n===============================================\n\n");
			printf("\n|     REALMENTE DESEAS ELIMINAR EL USUARIO   	 |\n");
			printf("  |           [1] NO                     		 |\n");
			printf("  |           [2] SI                   		  	 |\n");
			printf("OPCION: ");
			scanf("%s",&op);
			switch (op){
				case '1':
					printf("\n\n============================\n\n");
					printf("    |   ADIOS, VUELVA PRONTO :) |  ");
					printf("\n\n============================\n\n");
				break;
				case '2':
					ingresarEMPLE(empleados, extrabajadores,depa,cargo);
					op='1';
				break;
			default:
					
				break;
			}
		}while(op!='1');
	}else{
		printf("\nLA CI INDICADA NO ESTA ASOCIADA A NINGUN TRABAJADOR DE LA EMPRESA\n");
	}
	fclose(arch);
	fclose(extrabajadores);
}

void ingresarEMPLE(empleado empleados, FILE* ARCH,char depa[20],char cargo[20]){
	int valFecha=0;
	char op2;
		//guardamos la cedula del empleado en el archivo
		fprintf(ARCH,"%d\t\t", empleados.CI); //ingresando en el archivo 
		//guardamos el nombre del empleado en el archivo
		fprintf(ARCH,"%s", empleados.nombre);
		//guardamos el apellido del empleado en el archivo
		fprintf(ARCH," %s\t\t", empleados.apellido);
		//guardamos el departamento donde trabaja el empleado en el archivo
		fprintf(ARCH," %s\t\t", depa);
		//guardamos el cargo que tiene el empleado en el archivo
		fprintf(ARCH," %s\t\t", cargo);
		//pedimos al usuario fecha de desincorporacion y la guardamos en el archivo
		do{
			printf("\nIngresa la fecha de desincorporacion de la empresa en formato DD/MM/AA: \n");
			printf("Dia: "); scanf("%d",&empleados.fecha.dia);
			printf("\nMes: "); scanf("%d",&empleados.fecha.mes);
			printf("\nAno: "); scanf("%d",&empleados.fecha.ano);
			valFecha=validarFecha(empleados.fecha.dia,empleados.fecha.mes,empleados.fecha.ano);
		}while(valFecha !=1);
		if (valFecha == 1){
			fprintf(ARCH,"%d/%d/%d\t\t", empleados.fecha.dia,empleados.fecha.mes,empleados.fecha.ano);
		}
		//pedimos el motivo de desincorporacion y lo guardamos en el archivo
		//menu de eliminacion
		printf("\n===============================================\n\n");
		printf("\n|     	MOTIVO DE ELIMINACION	   		   |\n");
		printf("  |           [1] TRASLADO                     |\n");
		printf("  |           [2] RENUNCIA 					   |\n");
		printf("  |           [3] DESPIDO                      |\n");
		printf("  |           [.] OTRO		 	          	   |\n");
		printf("\n===============================================\n\n");
		printf("OPCION: ");
		scanf("%s",&op2);
		switch (op2){
			case '1':
				//ingresamos el motivo TRASLADO en el acrhivo
				fprintf(ARCH,"TRASLADO\t\t \n");
				printf("TRASLADO");
			break;
			case '2':
				//ingresamos el motivo RENUNCIA en el acrhivo
				fprintf(ARCH,"RENUNCIA\t\t \n");
				printf("RENUNCIA");
			break;
			case '3':
				//ingresamos el motivo DESPIDO en el acrhivo
				fprintf(ARCH,"DESPIDO\t\t \n");
				printf("DESPIDO");
			break;
		default:
				//ingresamos el motivo OTRO en el acrhivo
			fprintf(ARCH,"OTRO\t\t \n");
			printf("OTRO");
		break;
		}
}