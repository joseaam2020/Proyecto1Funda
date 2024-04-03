#include <Arduino.h>
#include <HCSR04.h>
#include <ShiftRegister74HC595.h>

HCSR04 hc(PD2, PD3); //clase sensor HCSR04 (trig pin , echo pin)
ShiftRegister74HC595<1> shiftRegister(10,12,11);


//Funciones 
void convertirDecimalBin(int num,byte *array);
void convertirBinGray(byte *array,int length);
int convertirBinDecimal(byte *array,int length);
void printBin(byte *array);

//Constantes
const float valorMaximo = 35.0;
const byte hexa [10][8] = {
  {1,1,1,1,1,1,0,0},
  {0,1,1,0,0,0,0,0},
  {1,1,0,1,1,0,1,0},
  {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0},
  {1,0,1,1,0,1,1,0},
  {1,0,1,1,1,1,1,0},
  {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0},
  {1,1,1,1,0,1,1,0}
};
//Variables
bool encendido = false;

void setup(){ 
    Serial.begin(9600);
    //inicio modo pins codigo Gray
    pinMode(PD4,OUTPUT);
    pinMode(PD5,OUTPUT);
    pinMode(PD6,OUTPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
}

void loop()
{
    //Sensor
    float distance = hc.dist();
    if(distance > valorMaximo){distance = valorMaximo;}//se reduce a valor maximo
    float decimalCodificado = distance * (7.0 / valorMaximo);
    int intDecimalCodificado = int(trunc(decimalCodificado));//se codifica distancia a decimal
    Serial.print("Decimal: ");
    Serial.println(intDecimalCodificado);
    byte bin[3] = {0,0,0};//array numero binario
    convertirDecimalBin(intDecimalCodificado,bin); 
    convertirBinGray(bin,3);
    Serial.print("Gray: ");
    printBin(bin);
    Serial.println();
    digitalWrite(PD4,bin[0]);
    digitalWrite(PD5,bin[1]);
    digitalWrite(PD6,bin[2]);
    delay(1000);
    bin[0] = digitalRead(7);
    bin[1] = digitalRead(8);
    bin[2] = digitalRead(9);
    Serial.print("Exceso 3: ");
    printBin(bin);
    int indice = convertirBinDecimal(bin,3);
    for(int i=0;i < 8;i++){
        shiftRegister.set(i,hexa[indice][i]);
    }
}

void printBin(byte *array){
  for(int i = 2;i>-1;i--){
    Serial.print(array[i]);
  }
  Serial.println();
}


/*
convertirDecimalBin: convierte un numero decimal a binario (en forma de array)
param:
    -int num: numero decimal a convertir
    -byte* array: array de bytes donde guardar el numero binario
return: void
*/
void convertirDecimalBin(int num,byte *array){
  int x = 0; //indice de array
  while(true){
    int cociente = num/2;
    int residuo = num%2;
    if(cociente > 1){
      array[x] = residuo;
      num = cociente;
      x++;
    } else {
      array[x] = residuo;
      array[x+1] = cociente;
      break;
    }
  }
}


/*
convertirBinGray: convierte el numero binario en forma de array a su equivalente en gray
param:
    -byte* array: array de bytes con numero binario donde array[0] es LSB 
    -int length: largo del array
return: void
*/
void convertirBinGray(byte *array,int length){
  int x = length--; //indice array
  byte newArray[length];
  newArray[x] = array[x];
  x--;
  while(x > -1){
    int sumaByteAdyacentes = array[x] + array[x+1];
    if(sumaByteAdyacentes == 2 || sumaByteAdyacentes == 0){
      newArray[x] = 0;
    } else {
      newArray[x] = 1;
    }
    x--;
  }
  for(int i=0; i < length;i++){
    array[i] = newArray[i];
  }
}

/*
convertirBinDecimal: convierte el numero binario en forma de array a su equivalente en decimal
param:
    -byte* array: array de bytes con numero binario donde array[0] es LSB 
    -int length: largo del array
return: void
*/
int convertirBinDecimal(byte *array,int length){
    float num = 0;
    for(int i=0;i<length;i++){
        float sum = (float(array[i]))*(pow(2.0,float(i)));
        num = num + sum;
    }
    return int(round(num));
}