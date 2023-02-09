#include "lib/include.h"
extern void Configurar_GPIO(void)
{
    SYSCTL->RCGCGPIO |=(1<<0) | (1<<5); 

    GPIOF->LOCK= 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIOF->CR = 0x1F;           // allow changes to PF4-0
    
    //puerto A
    GPIOA->AMSEL = 0x00;        // 3) disable analog on PF
    GPIOA->PCTL = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIOA->DIR = (0<<2);          // 5) PF4,PF0 in, PF3-1 out
    //1->salida
    //0->entrada
    GPIOA->AFSEL = 0x00;        // 6) disable alt funct on PF7-0
    GPIOA->PUR = (1<<2);          // enable pull-up on PF0 and PF4
    GPIOA->DEN = (1<<2);          // 7) enable digital I/O on PF4-0

    //puerto F
    GPIOF->AMSEL = 0x00;        // 3) disable analog on PF
    GPIOF->PCTL = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIOF->DIR = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
    GPIOF->AFSEL = 0x00;        // 6) disable alt funct on PF7-0
    GPIOF->PUR = 0x11;          // enable pull-up on PF0 and PF4
    GPIOF->DEN = 0x1F;          // 7) enable digital I/O on PF4-0

    //interrupción para pin PA2 (entrada pull-up)
    GPIOA->IM |= (0<<2);
    GPIOA->IS |= (0<<2); //p. 657
    //se pone un 0 (edge) o un 1 (level) en el pin
    GPIOA->IBE &= ~(1<<2); 
    GPIOA->IEV &= ~(1<<2);
    GPIOA->RIS |= (1<<2); //*duda entre el registro RIS y ICR
    GPIOA->IM |= (1<<2); 

    //GPIOA->interrupcion #0 (p. 104, 153)
    NVIC->IP[0] = (NVIC->IP[0]&0xFFFFFF00) | 0x00000020; //para habilitar las interrupciones del timer
    //[n]->numero de prioridad 
    //00 en la posición 0 porque es 4n
    NVIC->ISER[0] = (1UL << 0); //tabla interrupciones p. 104 
}

extern void Delay(void)
{
  unsigned long volatile time;
  time = 1600000;
  while(time)
  {
		time--;
  }
}