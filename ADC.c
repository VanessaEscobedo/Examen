#include "lib/include.h"

//para el ADC (se puede usar cualquier modulo, canal y sec)
//modulo 0->sec1
//canal 2->PE1 (se necesitan dos canales para las dos señales)
//canal 1->PE2

extern void Configura_Reg_ADC0(void)
{
    /*Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada,
    * para adquirir 6 señales analógicas a una velocidad de 1 millón de muestras 
    * por segundo, por los canales asignados  y guardar los valores en un arreglo 
    * para ser enviados con un botones  externos asociado al gpio D a través del protocolo de 
    * comunicación asíncrona a una velocidad todo esto usando interrupciones.
    * b) 
    * canales->2,4,6,7,10,1 
    * fclk->20MHZ 
    * Baud-rate->57600 
    * Secuenciadores->sec3, sec1, sec2*/
    
    //modulo 0->sec1 (tu decides que sec usar con que modulo y que canales)
    //modulo 1->sec2
    //modulo 1->sec3

    //p. 801 esquema (vienen los canales y su pin)
    //1->PE2

    SYSCTL->RCGCADC = (1<<0); //inicializar el modulo 0 y 1 de reloj del adc (RCGCADC) p. 352 
    //1->habilitado
    //0->deshabilitado
    //                     F     E      D       C      B     A      
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<0); //habilitar reloj puertos p. 340, 801
    //B, D y E->ADC (por los pines que se ocupan) 
    //F-LEDs
    
    //p. 663 (GPIODIR) habilita los pines como I/O 
    GPIOE->DIR &= ~(1<<2); //PE2 y PE1
    //1->salida
    //0->entrada
    //por la operación bitwise, se establecen como entrada (0)
    //entrada pq reciben un valor analógico
    
    //habilitar funciones alternativas, para que el modulo analógico tenga control de esos pines (GPIOAFSEL) p. 672 
    GPIOE->AFSEL = (1<<2); //PE2 y PE1
    //aquí solo decimos que no van a trabajar como GPIO
    
    GPIOE->DEN = (0<<2);  //deshabilita el modo digital (GPIODEN) p. 683

    //este registro solo habilita/deshabilita la función digital
    
    //registro combinado con el GPIOAFSEL y la tabla p. 1351, p. 688 (GPIOPCTL) 
    GPIOE->PCTL = GPIOE->PCTL & (0xFFFFF0FF); //la posición indica el pin, el 0 que es ADC
    
    GPIOE->AMSEL = (1<<2); //activar la función analogica (pq es ADC) (GPIOAMSEL) p. 687 

    //el registro (ADCPC) establece la velocidad de conversión por segundo p. 891
    ADC0->PC = (1<<2)|(1<<1)|(1<<0); //para config. a 1 millones de muestra/s (para la tiva grande lo max es 2 m/s)
    
    //este registro (ADCSSPRI) configura la prioridad de los secuenciadores p. 841
    ADC0->SSPRI = 0x3012; //mayor prioridad sec1
    //0->mayor prioridad
    //3->menor prioridad 
    
    //(ADCACTSS) este registro controla la activación de los secuenciadores p. 821
    ADC0->ACTSS  = (0<<3) | (0<<2) | (0<<1) | (0<<0); //primero se desactivan para config.
    
    //este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger) p. 834
    ADC0->EMUX = (0x0000);
    //0x0->por procesador, muestreo continuo
    /*ADC0->EMUX  = (0x0040); //se config. con interrupciones sec1
    ADC1->EMUX  = (0x4400); //sec2, sec3*/
    
    //este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado p. 867
    ADC0->SSMUX2 = 0x0001; //canal 1, sec2

    //este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción p. 868
    ADC0->SSCTL2 = 0x00000006; //1 muestras
    //se pone un 1 en "n sample interrupt enable" para cada canal
    //solo se pone un 1 en ""n sample is end of sequence" en el último canal 
    
    //*enable ADC Interrupt interrumpe la conversión p. 825
    ADC0->IM |= (1<<2); //* Unmask ADC0 sequence 2 interrupt2*
    //habilita la interrupción para el ADC (cuando la conversión ha finalizado, se genera la interrupción)
    //sirve para detectar cuando ha finalizado la conversión
    
    /*NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    NVIC_EN0_R = 0x00010000;*/
    
    ADC0->ACTSS |= (0<<3) | (1<<2) | (0<<1) | (0<<0); //activar secuenciadores p. 821

    ADC0->PSSI |= (0<<2);
    //se habilita el registro cuando se configura por procesador p. 845
    //este registro se utiliza iniciar la adquisión

    ADC0->ISC = 2; // p. 828

    /*ISC: registro que se utiliza para indicar cuándo se ha producido una interrupción 
    y para borrar el indicador de interrupción*/
}


extern void ADC0_InSeq2(uint16_t Result[1])
{
       ADC0->PSSI = 0x00000004; //(1<<2)
       while((ADC0->RIS&0x04)==0){}; 
       Result[0] = ADC0->SSFIFO2&0xFFF; 
       ADC0->ISC = 0x0004;  
}


