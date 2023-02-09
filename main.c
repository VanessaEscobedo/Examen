#include "lib/include.h"

extern void int_to_char(uint16_t dato) //función para convertir un dato tipo int a un dato tipo char
{
    char string[5];
    int i = 3;
    int j = 0; 
    int n_digit;  
    while (i >= 0)
    {
        n_digit = (int)dato / (int)(pow(10, i));
        string[j] = n_digit + '0';
        dato = dato - (n_digit * pow(10, i)); 
        i = i - 1; 
        j++;
    }
    string[4] = '\n';
    printString(string); //se envia el valor (char) con el UART
}

extern void Hilo_1(void) //tarea que hace la int
{
    uint8_t bandera;
    uint16_t Result[1];
    uint16_t valor1;

    //GPIOB_AHB->DATA = (1<<0);
    //ADC0_InSeq2(Result,duty);
    //GPIOB_AHB->DATA = (0<<0);

    GPIOF->DATA = (0<<3);

    ADC0_InSeq2(Result);
    valor1 = (uint16_t)(Result[0]);
    int_to_char(valor1);
}

extern void GPIOA_int(void) //interrupcion GPIOA (al apretar el boton)
{
  uint16_t Result[1];
  uint16_t valor;
  uint8_t bandera;
  
  if (GPIOA->RIS == (1<<2))
  {
    GPIOF->DATA = (1<<3);

    Configurar_Timer0A(&Hilo_1,4); //periodo
  }

  GPIOA->ICR = (1<<2); //limpiar las barreras de interrupción
}

int main(void)
{
    uint8_t bandera; 
    uint16_t Result[1];
    uint16_t valor1;


    Configurar_PLL(_20MHZ);  //configuracion de velocidad de reloj
    Configurar_UART0();
    Configura_Reg_ADC0();
    //Configura_Reg_PWM1(8);
    Configurar_GPIO();

    while (1)
    {
        /*
        ADC0_InSeq2(Result);
        valor1 = (uint16_t)(Result[0]);
        int_to_char(valor1);
        */
    }
}

