/*  =======================================================================
    =                                                                     =
    =             Guillermo Arriaga García  -  Proyecto                   =
    =                                                                     =
    =  TRES RUTAS PECULIARES ENTRE DOS CIUDADES DEL ESTADO DE GUANAJUATO  =
    =                                                                     =
    =           La más corta, la más rápida y la más barata               =
    =                                                                     =
    =                                                                     =
    =                       Análisis de Algoritmos                        =
    =                                                                     =
    =                       Algortimo Bellman-Ford                        =
    =                                                                     =
    =                         Diciembre de 2011                           =
    =                                                                     =
    ======================================================================= 
*/


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cfloat>
#include <list>

using namespace std;



// *************************************************   ESTRUCTURAS DE DATOS
struct arista
   {
      int    de;
      int    hacia;
      int    velocidad;
      float  distancia;
      float  costo;
      float  peso[3]; // 0->distancia , 1->tiempo , 2->precio
   };

struct vertice
   {
      float valor;
      int padre;
      int arista;    // La arista que quedará seleccionada
      string nombre;
   };



// ***********************************************************   FUNCIONES
void Captura(int &cantidad_vertices, int &cantidad_aristas,
             vertice* &V, arista* &A, float &rendim_auto, float &precio_gas);
void Bellman_Ford(int inicio, int fin, int tipo, int cantidad_aristas,
                  int cantidad_vertices, arista* &A, vertice* &V);
void Impresion(int cantidad_vertices, vertice* &V);
void Tiempo(float t);



// ******************************************************   MENÚ PRINCIPAL
int main()
{

    int n,m,seleccion=1;
    int cantidad_aristas;
    int cantidad_vertices;
    float rendim_auto;
    float precio_gas;
    arista* A;
    vertice* V;

    system("COLOR 0A");

    Captura(cantidad_vertices,cantidad_aristas,V,A,rendim_auto,precio_gas);
    
    while(seleccion)
    {
       system("CLS");
       cout<<"\n\n\n\t\tRUTAS PECULIARES ENTRE MUNICIPIOS DE GUANAJUATO"
           <<"\n\n\t\t       Proyecto de Analisis de Algoritmos"
           <<"\n\t\t Guillermo Arriaga Garcia   Diciembre de 2011"
           <<"\n\n\t\t\t ALGORITMO BELLMAN-FORD\n\n"
           <<"NUMEROS IDENTIFICADORES DE CADA MUNICIPIO:\n";
       Impresion(cantidad_vertices,V);
       
       cout<<"\n\n\tRendimiento del automovil: "<<rendim_auto<<" km/litro"
           <<"\n\tPrecio de gasolina: $"<<precio_gas<<"/litro";
       
       cout<<"\n\nOPCIONES:\n\n\t"
           <<"0) Salir del programa\n\t"
           <<"1) Ingresar dos municipios para analisis\n\n"
           <<"SELECCION: ";
       cin>>seleccion;
       
       switch(seleccion)
       {
         default: break;
         case 1 : cout<<"\n\n\tMunicipios: ";
                  cin>>n>>m;
                  if(n<0||n>45||m<0||m>45)
                  {
                     n=6;   // Demo: muestra uso de carreteras de cuota
                     m=38;
                  }

                  if(n!=m)
                  {
                    cout<<"\n\nTres rutas peculiares entre "<<V[n].nombre
                        <<" y "<<V[m].nombre<<" son:\n";
                    for(int k=0;k<3;k++)
                    {
                     Bellman_Ford(n,m,k,cantidad_aristas,cantidad_vertices,A,V);
                    }
                  }
                  
                  else
                  {
                     cout<<"\n\n\tIngresa, de nuevo, ciudades distintas.";
                  }
                  
                  cout<<"\n\n\t";
                  system("PAUSE");
                  break;
       }
    }
    
    system("CLS");
    system("COLOR 0E");
    cout<<"\n\n\n\n\n\n\n\t\tFin del programa\n\n\t";
    system("PAUSE");
    delete [] V;
    delete [] A;
    return EXIT_SUCCESS;
}



       /*============================================================
       =                                                            =
       =                     CAPTURA DE DATOS                       =
       =                                                            =
       =============================================================*/

void Captura(int &cantidad_vertices, int &cantidad_aristas,
             vertice* &V, arista* &A,float &rendim_auto,float &precio_gas)
{    
    int n,m,k;
    
    ifstream in;
    in.open("Datos.txt");
    
    in>>rendim_auto>>precio_gas;
    in>>cantidad_vertices>>cantidad_aristas;
    
    V=new vertice[cantidad_vertices];
    A=new arista[cantidad_aristas];
    
    for(int i=0,k=0;i<cantidad_vertices;i++)
    {
       in>>n;
       in>>V[n].nombre;
       in>>m;
       
       for(int j=0;j<m;j++,k++)
       {
          A[k].de=n;
          in>>A[k].hacia;
          in>>A[k].distancia;
          in>>A[k].costo;
          in>>A[k].velocidad;
          A[k].peso[0]=A[k].distancia;
          A[k].peso[1]=(A[k].distancia/A[k].velocidad);
          A[k].peso[2]=(A[k].costo+(A[k].distancia)*precio_gas/rendim_auto);
       }
    }
    
    in.close();
    return;
}




       /*============================================================
       =                                                            =
       =                       BELLMAN-FORD                         =
       =                                                            =
       =============================================================*/


void Bellman_Ford(int inicio, int fin, int tipo, int cantidad_aristas,
                  int cantidad_vertices, arista* &A, vertice* &V)
{
    int k;
    float rendim_auto;
    float precio_gas;
    float dato1,dato2;
    list<int> R;             // recorrido
    list<int>::iterator it;


    // INICIALIZACIÓN

    for(k=0;k<cantidad_vertices;k++)
    {
       V[k].padre=-1;
       V[k].valor=FLT_MAX;
    }

    V[inicio].valor=0;   // raiz
    V[inicio].padre=inicio;

    
    // RELAJACIÓN
    
    for(k=0;k<cantidad_vertices-1;k++)
    {
       for(int i=0;i<cantidad_aristas;i++)
       {
          if(V[A[i].hacia].valor>V[A[i].de].valor+A[i].peso[tipo])
          {
             V[A[i].hacia].valor=V[A[i].de].valor+A[i].peso[tipo];
             V[A[i].hacia].padre=A[i].de;
             V[A[i].hacia].arista=i;
          }
       }
    }
    // Por la estructura de datos de este proyecto no hay ciclos negativos


    // IDENTIFICACIÓN DEL RECORRIDO

    R.empty();
    R.push_front(fin);
    
    while(R.front()!=V[R.front()].padre)
    {
       R.push_front(V[R.front()].padre);
    }


    // CALCULO DE LOS OTROS DOS DATOS PARA ESTA RUTA HALLADA
    
    dato1=0;
    dato2=0;    
    for(it=R.begin();it!=R.end();it++)
    {
       if(dato1==0)
       {
         it++;
       }
       
       dato1+=A[V[*it].arista].peso[(tipo+1)%3];
       dato2+=A[V[*it].arista].peso[(tipo+2)%3];
    }
    
    
    
    // IMPRESIÓN DEL RESULTADO
    switch(tipo)
    {
       default: break;
       case 0:  cout<<"\n\n\n\tLa ruta con DISTANCIA MINIMA es de "
                    <<V[fin].valor<<" km con ";
                Tiempo(dato1);
                cout<<" y $"<<dato2<<":\n\n";
                break;
                
       case 1:  cout<<"\n\n\n\n\n\tLa ruta con TIEMPO MINIMO es de ";
                Tiempo(V[fin].valor);
                cout<<" con $"<<dato1<<" y "<<dato2<<" km:\n\n";
                break;
                
       case 2:  cout<<"\n\n\n\n\n\tLa ruta con PRECIO MINIMO es de $"
                    <<V[fin].valor<<" con "<<dato1<<" km y ";
                Tiempo(dato2);                      
                cout<<":\n\n";
                break;

    }

    k=0;    
    for(it=R.begin();it!=R.end();it++)
    {
       cout<<V[*it].nombre;
       k++;

       if(k%4==0)
          cout<<"\n";
       
       if((++it)!=R.end())
          cout<<" - ";
          
       it--;
    }

    return;
}



       /*============================================================
       =                                                            =
       =                         IMPRESIÓN                          =
       =                                                            =
       =============================================================*/


void Impresion(int cantidad_vertices, vertice* &V)
{
     cout<<"\n\n";
     int n=cantidad_vertices/3-1;
     for(int k=0; k<n; k++)
     {
        cout.width(18);
        cout<<V[k].nombre;
        cout.width(3);
        cout<<k<<"  ";
        cout.width(21);
        cout<<V[k+n].nombre;
        cout.width(3);
        cout<<k+n<<"  ";

        if(k+2*n<cantidad_vertices-6)  // No imprime lo de cuota
        {        
          cout.width(24);
          cout<<V[k+2*n].nombre;
          cout.width(3);
          cout<<k+2*n;
        }

        cout<<endl;
     }
     return;
}


       /*============================================================
       =                                                            =
       =                     FORMATO DE TIEMPO                      =
       =                                                            =
       =============================================================*/


void Tiempo(float t)
{
   int horas=(int)t;
   int minutos=(int)(t*60-horas*60);
   cout<<horas<<" hr "<<minutos<<" min";
}
