#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

int Tmax=30;
int Tmin=18;
int PAmax=25;
int PBmin=0.5;

// Ajuste_presion se utiliza para asignar la presión en función de la temperatura.
// Se toma como referencia la presión que aumenta siendo su rango de 5 a 25.
// Al variar en función de la temperatura se ha hecho un ajuste matemático, si la diferencia de temperatura es mayor que 20 la presión será máxima.
// Si la presión es inferior a 20 la presión se calcula a través de la diferencia de la temperatura.
// Si el usuario pide una temperatura fuera del rango (18-30 grados) se informará del error.En funcion de si la temperatura es mayor o menor 
//que la temperatura ambiente se asignará la temnperatura máxima o mínima automáticamente.

int ajuste_presion(int t,int T_amb, FILE *archivoCSV){                                          // En esta funcion se asigna la presión del sistema en funcion de la temperatura 

    int dif;
    int presion;
    double PA=5;
    double PB=5;

    if(t<Tmin){
        printf("Error: La temperatura minima que alcanza el sistema es %d\n",Tmin);
    }else if(t>Tmax){
        printf("Error: La temperatura maxima que alcanza el sistema es %d\n",Tmax);
        t=Tmax;
    }

    if (t == T_amb){
       PA=5;
       PB=5;
    
    }else if (t > T_amb){
        dif=t-T_amb;
        if (dif <= 20){
            PA=5+dif;
            PB=0.5+((25-dif)*0.16);// Ajuste para que aumente a corde a la temperatura
            presion=PA;
        }else if (dif >= 21){
            PA=25;
            PB=0.5;
            presion=PA;
        }
    }
    else if (t < T_amb){

        dif=T_amb-t;
        if (dif <= 20){
            PB=5+dif;
            PA=0.5+((25-dif)*0.16);
            presion=PB;
        }else if (dif >= 21){
            PB=25;
            PA=0.5;
            presion=PB;
        }


    }
        fprintf(archivoCSV, "Presion de alta %.2lf Grados\n", PA);// Salida fichero CSV
        fprintf(archivoCSV, "Presion de baja %.2lf Grados\n", PB);
        printf("Presion de alta %.2lf Grados\n", PA);// Salida por pantalla
        printf("Presion de baja %.2lf Grados\n", PB);
        presion = (int)presion;
        return presion;
}

// Para frio-----------------------------------------------------
// El compresor puede estar en 4 estados. estos 4 estados van a depender de la presion.
// Primero se comprueba que hay una diferencia entre la temperatura ambiente y la deseada.
// despues se ha asignado a cada rango de presion un estado del compresor.
// esta funcion devuelve esl estado de compresor siendo 0=OFF; 1=MIN; 2=MED; 3=MAX;
int compresor(int t,int T_amb, int pres)                                                     // Se activa cuando se requiere enfriar y puede estar en 4 estados OFF-MIN-MED-MAX. Esta en OFF cuando no hay demanda de frio y el resto depende de la demanda de frio.
{
    int i;
    if ( t!=T_amb){
        if (pres==5){
            i=0;
        }else if(pres>=6 && pres<=10){
            i=1;
        }else if(pres>=11 && pres<=15){
            i=2;
        }else if(pres>=16){
            i=3;
        }
    }else{
        i=0;
    }
    //getchar();                                                                             // pausa para el sistema, usado para corrgir errores parando cada iteracion
return i;
} 
// En los ventiladores de condensacion se usa la funcion de compresor para obtener su estado.
// En base al estado del compresor se ha asignado que los ventiladores se inicien en 4 estados diferentes.
// Se llama a la funcion ajuste_presion para obtener la presion del sistema, en funcion de el valor de la presion,
// el ventilador de condensacion variará su estado, partiendo siempre del valor inicial que indica el compresor.
int ventiladores_condensacion(int t,int T_amb, FILE *archivoCSV)
{
    int i;
    int r;
    int pres;
    pres = ajuste_presion(t, T_amb,archivoCSV);
    r=compresor(t,T_amb, pres);

    if (r==0){
        i=0;
        fprintf(archivoCSV, "Compresor estado OFF");
        printf("Compresor estado OFF\n");
    } else if (r==1){
        i=33-((25-pres));
        fprintf(archivoCSV, "Compresor estado MIN");
        printf("Compresor estado MIN\n");
    } else if (r==2){
        i=66-((25-pres));
        fprintf(archivoCSV, "Compresor estado MED");
        printf("Compresor estado MED\n");
    } else if (r==3){
        i=100-((25-pres));
        fprintf(archivoCSV, "Compresor estado MAX");
        printf("Compresor estado MAX\n");
    }

    return i;
}
// Para calor----------------------------------------------
// funciona de forma analoga a la funcion de compresor

int resistencia_electrica(int t,int T_amb, int pres)                                        // Se activa cuando se requiere enfriar y puede estar en 4 estados OFF-MIN-MED-MAX. Esta en OFF cuando no hay demanda de calor y el resto depende de la demanda de calor.
{
    int i;
    if ( t!=T_amb){
        if (pres == 5){
            i=0;
        }else if(pres>=6 && pres<=10){
            i=1;
        }else if(pres >=11 && pres<=15){
            i=2;
        }else if(pres >=16){
            i=3;
        }
         }else{
        i=0;
    }
    //getchar();                                                                            // pausa para el sistema, usado para corrgir errores
return i;
} 
// funciona de forma analoga a la funcion de ventiladores de condensacion
int bomba_agua(int t,int T_amb, FILE *archivoCSV)
{
    int i;
    int r;
    int pres;
    pres = ajuste_presion(t, T_amb,archivoCSV);

    r=resistencia_electrica(t,T_amb,pres);
    
    if (r==0){
        i=0;
        fprintf(archivoCSV, "Compresor estado OFF");
        printf("Compresor estado OFF\n");
    } else if (r==1){
        i=33-((25-pres));
        fprintf(archivoCSV, "Compresor estado MIN");
        printf("Compresor estado MIN\n");
    } else if (r==2){
        i=66-((25-pres));
        fprintf(archivoCSV, "Compresor estado MED");
        printf("Compresor estado MED\n");
    } else if (r==3){
        i=100-((25-pres));
        fprintf(archivoCSV, "Compresor estado MAX");
        printf("Compresor estado MAX\n");
    }

    return i;
}
// Elementos comunes ----------------------------------------------
// La funcion ventiladores_evaporadoras primero diferencia si se necesita frio o calor,
// en base a ello llamará a la función de ventiladores de condensacion o bomba de agua.
// En base a lo que devuelva la función llamada, asignara 100 cuando devuelva un valor mayor o igual que 90 o
// le sumará diez al valor dado. Este ajuste es devido a que el rango de los ventiladores de evaporadoras es de 10-100
// mientras el valor recibido tiene un rango de 0-100.
int ventiladores_evaporadoras(int t,int T_amb, FILE *archivoCSV){
    int i;
    if ((T_amb-t)>=0){                                                                      // Necesita frio
        int comp;
        comp=ventiladores_condensacion(t, T_amb,archivoCSV); 
        fprintf(archivoCSV, "Ventiladores de condensacion %d de potencia \n", comp);
        printf("Ventiladores de condensacion %d de potencia \n", comp);
        if (comp>=90){                                                                      // Al tener un rango las evaporadoras de 10 a 100 y el rango de la condensacion de 0-100, para hacer una equivalencia, el rango de 90-100 del de condensacion hará que la evaporadora este a 100
            i=100;
        } else{
            i=comp+10;
        }

    } else if ((t-T_amb)>=0){                                                               // Necesita calor
        int bom;
        bom=bomba_agua(t, T_amb,archivoCSV);
        fprintf(archivoCSV, "Bomba de agua %d de potencia\n", bom);
        printf("Bomba de agua %d de potencia\n", bom);
        if (bom>=90){                                                                       // Al tener un rango las evaporadoras de 10 a 100 y el rango de la condensacion de 0-100, para hacer una equivalencia, el rango de 90-100 del de condensacion hará que la evaporadora este a 100
            i=100;
        } else{
            i=bom+10;
        } 
    }
    
    return i;
}
// La funcion sensor_temperatura se encargará de llamar a la funcion ventilador de evaporadoras 
// hasta que esta devuelva 10. En este caso se habrá conseguido la temperatura adecuada.
// Se ha simulado que en cada iteracion va a haber un grado de diferencia. En función de si se
// necesita calor o frio se restara o sumara un grado a la temperatura ambiente por cada iteracion.

void sensor_temperatura(int t,int T_amb, FILE *archivoCSV){
    int a=0;
    int b=0;
    if (t<T_amb){
    while(a != 10){
        b++;
        fprintf(archivoCSV, "Iteracion %d \n,La temperatura es %d Grados\n", b, T_amb);
        printf("Iteracion %d ,La temperatura es %d Grados\n", b, T_amb); 

        a=ventiladores_evaporadoras(t,T_amb,archivoCSV);
        T_amb=T_amb-1;   

        fprintf(archivoCSV, "Ventiladores de evaporadoras al %d por ciento de su capacidad\n\n\n", a);
        printf("Ventiladores de evaporadoras %d por ciento de su capacidad \n\n\n", a);

    }
    }else if(t>T_amb){
    while(a != 10){
        b++;
        fprintf(archivoCSV, "Iteracion %d \n,La temperatura es %d Grados\n", b, T_amb);
        printf("Iteracion %d ,La temperatura es %d Grados\n", b, T_amb);  
 

        a=ventiladores_evaporadoras(t,T_amb,archivoCSV);
        T_amb=T_amb+1;       

        fprintf(archivoCSV, "Ventiladores de evaporadoras al %d por ciento de su capacidad\n\n\n", a);
        printf("Ventiladores de evaporadoras al %d por ciento de su capacidad\n\n\n", a);
    }        
    }
    printf("Se ha alcanzado la temperatura deseada de %d Grados", t);
    fprintf(archivoCSV, "Se ha alcanzado la temperatura deseada de %d Grados", t);
    
}
// En main se declara el archivo csv donde se guardaran los datos, despues
// se calcula una temperatura ambiente aleatoria comprendida entre 0 y 41.
// despues se pedirá por pantalla la temperatura deseada y seguidamente se llamara
// a la funcion sensor de temperatura.
int main(){
    FILE *archivoCSV = fopen("datos.csv", "w");                                 // Se crea el archivo csv llamado datos.csv y se habilita la escritura
    srand(time(NULL));
    int T_ambiente = (rand()%41);                                               // Se presupone que la temperatura ambiente puede estar entre un valor aleatorio de 0 y 40 grados 
    int temperatura;
    

    printf("Temperatura actual: %d \n ¿Que temperatura desea? ",T_ambiente);
    scanf("%d", &temperatura);                                                  // Se pide al usuario que temperatura desea
    fprintf(archivoCSV, "TEMPERATURA DESEADA= %d\n", temperatura);
    fprintf(archivoCSV, "TEMPERATURA AMBIENTE= %d\n", T_ambiente);
    printf("TEMPERATURA DESEADA= %d\n", temperatura);
    printf("TEMPERATURA AMBIENTE= %d\n", T_ambiente);


   sensor_temperatura(temperatura ,T_ambiente, archivoCSV);                     // Se llama a la función que ajustara todo el sistema
}