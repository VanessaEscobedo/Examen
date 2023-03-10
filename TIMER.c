#include "lib/include.h"

//timer 0 32 bits
//timer 1 32 bits 

void (*PeriodicTask)(void); 
extern void Configurar_Timer0A(void(*task)(void), unsigned long int periodo)
{
    SYSCTL->RCGCTIMER |= (1<<0); //habilitar timer 0 (RCGCTIMER) p. 338
    //SYSCTL->RCGCTIMER |= (1<<1); //habilitar timer 1 
    //0->el timer de 16/32 bits esta inhabilitado
    //1->habilita el timer
    PeriodicTask = task; //asignat dirección de memoria

    TIMER0->CTL = (0<<0); //deshabilitar el timer antes de configurarlo (CTL) p. 722, 737
    //TIMER1->CTL = (0<<0); 
    //bit 0, 0->el timer An esta deshabilitado
    //bit 8, 0->el timer Bn esta deshabilitado

    TIMER0->CFG = 0x00000000; //selección modo de operación (32 bits) p. 728
    //TIMER1->CFG = 0x00000000;
    //0x0->para un timer de 16/32 bits, selecciona la configuración de 32 bits
    TIMER0->TAMR = (0x2<<0); //configurar el modo del timer (Periodic Timer Mode) p. 732
    TIMER0->TAMR = (0<<4); //para count-down
    //0->count-down
    //1->count-up empezando en 0x0
    //TIMER1->TAMR = (0x2<<0); 
    //TIMER1->TAMR = (0<<4);
    //cuando el timer A y el B estan concatenados, este registro (TAMR) controla los modos de ambos timer
 
    TIMER0->TAILR = 20000000 * periodo; //con este registro se establece el periodo de cuenta
    //periodo->tiempo en segundos
    //TIMER1->TAILR = 20000000 * periodo;
    
    //interrupciones
    TIMER0->ICR = 1<<0; //limpiar
    //TIMER1->ICR = 1<<0; 

    //se limpia al escribir un 1 
    TIMER0->IMR = 1<<0; //habilitar la mascara de la interupción (GPTMIMR) p. 751
    //TIMER1->IMR = 1<<0; 

    NVIC->IP[4] = (NVIC->IP[4]&0x00FFFFFF) | 0x20000000; //para habilitar las interrupciones del timer
    //[n]->numero de prioridad 
    //00 en la posición 3 porqu es 4n+3
    NVIC->ISER[0] = (1UL << 19); //tabla interrupciones p. 104 
    // numero de interrupcion TIMER0A = 19 
    // n=4 ----> [4n+3] [4n+2] [4n+1] [4n] ---> [4n+3]

    /*NVIC->IP[5] = (NVIC->IP[5]&0xFFFF00FF) | 0x20000000; 
    NVIC->ISER[0] = (1UL << 21);*/
    // numero de interrupcion TIMER1A = 21
    // n=5 ----> [4n+3] [4n+2] [4n+1] [4n] ---> [4n+1]
    
    TIMER0->CTL = (1<<0);
    //TIMER1->CTL = (1<<0);

    /*
    SYSCTL->RCGCTIMER |= (1<<0); //habilitar timer 0 (RCGCTIMER) p. 338
    //0->el timer de 16/32 bits esta inhabilitado
    //1->habilita el timer
    PeriodicTask = task; //asignat dirección de memoria

    TIMER0->CTL = (0<<8); //deshabilitar el timer antes de configurarlo (CTL) p. 722, 737
    //bit 0, 0->el timer An esta deshabilitado
    //bit 8, 0->el timer Bn esta deshabilitado

    TIMER0->CFG = 0x04; //selección modo de operación p. 728
    //0x0->para un timer de 16/32 bits, selecciona la configuración de 32 bits
    //0x04->para timer de 16 bits
    TIMER0->TBMR = (0x2<<0); //configurar el modo del timer (Periodic Timer Mode) p. 732
    TIMER0->TBMR = (0<<4); //para count-down
    //0->count-down
    //1->count-up empezando en 0x0

    //cuando el timer A y el B estan concatenados, este registro (TAMR) controla los modos de ambos timer
 
    TIMER0->TBILR = 20000000 * periodo; //con este registro se establece el periodo de cuenta
    //periodo->tiempo en segundos
    
    //interrupciones
    TIMER0->ICR = (1<<8); //limpiar

    //se limpia al escribir un 1 
    TIMER0->IMR = (1<<8); //habilitar la mascara de la interupción (GPTMIMR) p. 751

    NVIC->IP[5] = (NVIC->IP[5]&0xFFFFFF00) | 0x00000020; //para habilitar las interrupciones del timer
    //[n]->numero de prioridad 
    NVIC->ISER[0] = (1UL << 20); //tabla interrupciones p. 104 
    
    TIMER0->CTL = (1<<8);
    */
}

//se desborda el timer y va a la rutina de interrupción
extern void Timer0A_Handler(void) //cuando ocurra la interrupción se apunta en esta dirección
{
  (*PeriodicTask)(); //llama a la dirección de periodic task 
  //aquí entra a la función Hilo (que es donde queremos que haga la rutina)
  //GPIOF->DATA = (0<<1);
  
  TIMER0->ICR = (1<<0); //reiniciar la int   
}


