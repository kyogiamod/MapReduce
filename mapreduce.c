#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Entrada: 2 enteros
Salida: 1 entero
Funcion: Eleva la base al exponente
*/
int potencia(int base, int expo)
{
	if(expo == 0)	{return 1;}
	int i;
	int resultado = base;
	for(i = 1; i < expo; i++)
	{
		resultado = resultado*base;
	}
	return resultado;
}

/*
Entrada: String
Salida: Entero
Funcion: Convierte un string a entero.
*/
int charToInt(char* str)
{
	int length = strlen(str);
	int i;
	int resultado = 0;
	for(i = 0; i < length; i++)
	{
		resultado += (str[i]-48)*potencia(10,(length - 1 - i));
	}
	return resultado;
}


/*
Entrada: 2 enteros
Salida: Arreglo bidimensional
Funcion: Crea una matriz de N*M
*/
int** create_matrix(int N, int M)
{ 
	
	int i, j;
	//Pide memoria sufucuente para crear las columnas
	int **matriz = (int**)malloc(sizeof(int*)*M); 
	
	//Pide memoria suficiente para la creacion de las filas
	for(i=0;i<M;i++)
	{
		matriz[i]=(int*)malloc(sizeof(int)*N); //a cada elemento de la lista "matriz" se le asigna una nueva lista creando asi la matriz
		for(j = 0; j < N; j++)
		{
			matriz[i][j] = 0;
		}
	}
	return matriz;
}

/*
Entrada: String
Salida: Entero
Funcion: Cuenta las lineas del archivo
*/
int contar_lineas(char* input)
{
	FILE* file = fopen(input,"r");
	int saltos = 0;
	char bs = ' ';
	while(!feof(file))
	{
		fscanf(file, "%c", &bs);
		if(bs == '\n')	{saltos++;}
	}
	return saltos;
}

/*
Entrada: String
Salida: Entero
Funcion: 1 si es visa el string, 0 si no
*/
int isVisa(char* str)
{
	if(str[0] == 'V')
		if(str[1] == 'i')
			if(str[2] == 's')
				if(str[3] == 'a')
					return 1;
	return 0;
}

/*
Entrada: string
Salida: lista de 2 strings.
Funcion: se le entrega una linea del archivo y la divide, entregando un arreglo de 2 strings.
*/
char** split(char* linea)
{
	//Se guarda la fecha
	char* fecha = (char*)malloc(sizeof(char)*3);
	fecha[0] = linea[5];
	fecha[1] = linea[6];
	fecha[2] = '\0';

	//se hace esto para guardar el tipo de tarjeta. Solo guarda 4 caracteres (visa).
	char* tipoTarjeta = (char*)malloc(sizeof(char)*15);
	int tabs = 0, i = 0;
	char caracter;
	while(tabs < 5)
	{
		caracter = linea[i];
		i++;
		if(caracter == 9)	{tabs++;}
	}

	tipoTarjeta[0] = linea[i];
	tipoTarjeta[1] = linea[i+1];
	tipoTarjeta[2] = linea[i+2];
	tipoTarjeta[3] = linea[i+3];
	tipoTarjeta[4] = '\0';

	char** ft = (char**)malloc(sizeof(char*)*2);
	ft[0] = fecha;
	ft[1] = tipoTarjeta;

	return ft;
}


/*
Entrada: 
Salida:
Funcion: 
*/
char* intToMes(int mes)
{
	switch(mes)
	{
		case(0):
			return "Enero";
		case(1):
			return "Febrero";
		case(2):
			return "Marzo";
		case(3):
			return "Abril";
		case(4):
			return "Mayo";
		case(5):
			return "Junio";
		case(6):
			return "Julio";
		case(7):
			return "Agosto";
		case(8):
			return "Septiembre";
		case(9):
			return "Octubre";
		case(10):
			return "Noviembre";
		case(11):
			return "Diciembre";
		default:
			"Mes invalido.";
	}
}


/*
Entrada: arreglo de enteros y su longitud
Salida: entero
Funcion: Suma los elementos del arreglo
*/
int sumarArray(int* arr, int length)
{
	int i;
	int final = 0;
	for(i = 0; i < length ; i++)
	{
		final += arr[i];
	}
	return final;
}

int main(int argc, char* argv[])
{
	int iflag = 0;
	char* input = NULL;

	int oflag = 0;
	char* output = NULL;

	int nflag = 0;
	char* strNodos = NULL;
	int cantidad_nodos;

	int dflag = 0;

	char c;
	opterr = 0;


	while((c = getopt(argc, argv, "-i:-o:-n:-d:")) != -1)
	{
		switch(c)
		{
			case 'i':
				iflag = 1;
				input = optarg;
				break;

			case 'o':
				oflag = 1;
				output = optarg;
				break;

			case 'n':
				nflag = 1;
				strNodos = optarg;
				break;

			case 'd':
				dflag = 1;
				break;

			case '?':
				printf("Se ingeso algo mal.\n");
				getchar();
				return 1;

			default:
				abort();
		}	
	}

	int pid; //para ver si es hijo o padre
	int id_linea = 0, linea_cursor = 0;	//Para saber las lineas que debe leer
	int* meses = calloc(12,sizeof(int));
	
	int i;
	cantidad_nodos = charToInt(strNodos);

	if(dflag)
	{
		printf("Input file: %s\n", input);
		printf("Output file: %s\n", output);
		printf("Cantidad de nodos: %d\n", cantidad_nodos);	
		getchar();	
	}

	int* pipes = malloc(sizeof(int)*2*cantidad_nodos);
	int* pipes2 = malloc(sizeof(int)*2*cantidad_nodos);

	for(i = 0; i < 2*cantidad_nodos; i+=2)
	{
		if(pipe(&pipes[i]) < 0)
		{
			printf("No se pudo crear algun pipe.");
			return -1;
		}
	}

	for(i = 0; i < 2*cantidad_nodos; i+=2)
	{
		if(pipe(&pipes2[i]) < 0)
		{
			printf("No se pudo crear algun pipe.");
			return -1;
		}
	}
	if(dflag)	{printf("Se crearon los pipes\n");}

	//Aca se empiezan a crear los workers
	for(i = 0; i < cantidad_nodos; i++)
	{
		//printf("Creando proceso %d\n", i+1);
		pid = fork();
		if(pid == 0)	{ break; }
		id_linea ++;
	}

	if(dflag)	{ printf("Se crearon los workers\n"); }
	//Aca ya se empieza a ejecutar todo el trabajo de los workers

	if(!pid)
	{ //hijo

		close(pipes[2*id_linea]);
		close(pipes2[2*id_linea+1]);

		if(dflag)	{ printf("Se cerraron los pipes no utilizables\n"); }

		FILE* file = fopen(input, "r");
		if(!file)	{printf("No se pudo abrir el archivo.\n"); return -1; }

		//printf("El hijo %d entra\n", id_linea);
		char c;
		//leer las lineas correspondientes
		int flag_pos = 0;
		char* linea = malloc(sizeof(char)*100);
		while(!feof(file))
		{
			if((c = fgetc(file)) == '\n')	{ break; }
			fseek(file,-1,SEEK_CUR);
			//Primero me posiciono en la primera linea que debo leer.
			if(!flag_pos)
			{
				while((linea_cursor < id_linea))
				{
					if(((c = fgetc(file)) == '\n'))	{ break; }
					fseek(file,-1,SEEK_CUR);
					fgets(linea, 100, file);
					linea_cursor++;
				}
				flag_pos++;
			}
			//Luego debo rescatar la siguiente linea
			if(!feof(file))
			{
				if(((c = fgetc(file)) == '\n'))	{ break; }
				fseek(file,-1,SEEK_CUR);
				fgets(linea, 100, file);
				//printf("%d: %s\n", id_linea, linea);
				char** infoLinea = split(linea);
				if(isVisa(infoLinea[1]))
				{
					int mesInt = charToInt(infoLinea[0]);
					meses[mesInt-1] += 1;
				}

				int contador = 1;
				while((contador < cantidad_nodos) && (!feof(file)))
				{
					if(((c = fgetc(file)) == '\n'))	{ break; }
					fseek(file,-1,SEEK_CUR);
					fgets(linea, 100, file);
					contador++;
				}
			}
			else { fclose(file); break; }
		}

		if(dflag)	{ printf("Se leyo el archivo\n"); }

		//manda respuesta al padre
		/*
		printf("Hijo %d: %d %d %d %d %d %d %d %d %d %d %d %d escribo en el pipe %d\n", 
				id_linea, meses[0],meses[1],meses[2],meses[3],meses[4],meses[5],
				meses[6],meses[7],meses[8],meses[9],meses[10],meses[11], pipes[2*id_linea+1]);
		*/

		if(dflag)	{ printf("Worker ID: %d, escribirá al master.\n", id_linea); }

		write(pipes[2*id_linea+1], meses, 12*sizeof(int));
		//Ya se envio con cada worker al master el numero que se leyó por cada mes
		//Ahora se debe analizar por mes


		
		//Ahora se ocupan los workers para sumar los meses, por lo que llega
		// un array del porte de la cantidad_workers + 1 para saber el mes
		int* codigoMes = (int*)malloc(sizeof(int)*(cantidad_nodos+1));
		int mes;
		int total;
		int* mes_cantidad = (int*)malloc(sizeof(int)*2);
		
		for(i = 0; i <= (12/cantidad_nodos); i++)
		{
			//se va a leer todos los numeros traidos por workers

			if(dflag)	{ printf("Worker ID: %d, esta leyendo del padre.\n", id_linea); }
			read(pipes2[2*id_linea], codigoMes, sizeof(int)*(cantidad_nodos+1));	
			
			mes = codigoMes[0];
			//Si es mayor a diciembre, entonces se suicida por depresion, no tiene trabajo que hacer.
			if(mes > 11)	{ return 1; }

			/*
			printf("Soy el hijo %d y lei %d bytes. Recibo por el pipe: ",id_linea, reads);
			for(k = 0; k < cantidad_nodos + 1; k++)
			{
				printf("%d ", codigoMes[k]);
			}
			printf("\n");
			*/

			total = sumarArray(codigoMes, cantidad_nodos + 1) - codigoMes[0];
			mes_cantidad[0] = mes;
			mes_cantidad[1] = total;
			//printf("Hijo %d manda %d y %d\n", id_linea, mes_cantidad[0], mes_cantidad[1]);
			if(dflag)	{ printf("Worker ID %d manda [%s,%d] \n", id_linea, intToMes(mes), total); }
			write(pipes[2*id_linea+1], mes_cantidad, 2*sizeof(int));
		}
	}
	else
	{//padre
		int j;

		if(dflag)	{ printf("Soy el master y hago la matriz\n"); }
		int** matriz = create_matrix(12, cantidad_nodos);

		for(i = 0; i < 2*cantidad_nodos; i += 2)
		{
			close(pipes[i+1]);
			close(pipes2[i]);
			read(pipes[i], meses, 12*sizeof(int));
			
			/*
			printf("Padre recibe %d %d %d %d %d %d %d %d %d %d %d %d\n", 
				meses[0],meses[1],meses[2],meses[3],meses[4],meses[5],
				meses[6],meses[7],meses[8],meses[9],meses[10],meses[11]);
			*/
			
			for(j = 0; j < 12; j++)
			{
				matriz[i/2][j] = meses[j];
			}
		}

		/*
		printf("La matriz es: \n");
		for(i = 0; i < cantidad_nodos; i++)
		{
			for(j=0;j<12;j++)
			{
				printf("%d ",matriz[i][j]);
			}
			printf("\n");
		}
		*/
		
		//Para este punto ya se lleno la matriz total.
		//Ahora se debe enviar a cada worker el mes para que este los sume y le entregue un resultado.


		if(dflag)	{ printf("Soy el master y tengo la matriz lista\n"); }
		printf("\n");

		int mes = 0;
		int* arregloFinal = (int*)calloc(12,sizeof(int));
		int* arr_lectura = (int*)malloc(sizeof(int)*2);
		int k;
		int cont = 0;
		for(i = 0; i <= (12/cantidad_nodos); i++)
		{
			for(j = 0; j < cantidad_nodos; j++)
			{
				int* mes_especifico = (int*)malloc(sizeof(int)*cantidad_nodos+1);
				mes_especifico[0] = mes;
				mes++;
				for(k = 1; k < cantidad_nodos + 1; k++)
				{
					mes_especifico[k] = matriz[k-1][cont];
				}
				cont++;
				/*
				printf("Soy el papa y escribo en el pipe %d: ", 2*j+1);
				for(k = 0; k < cantidad_nodos + 1; k++)
				{
					printf("%d ", mes_especifico[k]);
				}
				*/

				if(dflag)	{ printf("Soy el master y Le envio al worker un mes especifico para que sume\n"); }
				write(pipes2[2*j+1], mes_especifico, (1+cantidad_nodos)*sizeof(int));
			}
			
			for(j = 0; j < cantidad_nodos; j++)
			{
				if(dflag)	{ printf("Soy el master y leo el resultado final del mes\n"); }
				read(pipes[2*j], arr_lectura, 2*sizeof(int));
				arregloFinal[arr_lectura[0]] =+ arr_lectura[1];
			}
			
		}

		printf("El arreglo final es: \n");
		for(i = 0; i < 12; i++)
		{
			printf("%d ", arregloFinal[i]);
		}
		printf("\n");

		FILE* out = fopen(output, "w");

		if(out)
		{
			if(dflag)	{ printf("Soy el master y procedo a escribir en el archivo.\n"); }
			fprintf(out, "%-11s: %-9s\n\n", "Mes", "Cantidad");
			int m;
			for(m = 0; m < 12; m++)
			{
				fprintf(out, "%-11s: %-6d\n", intToMes(m), arregloFinal[m]);
			}

			if(dflag)	{ printf("Archivo %s creado correctamente.\n", output); }
		}
	}


	return 0;
}








