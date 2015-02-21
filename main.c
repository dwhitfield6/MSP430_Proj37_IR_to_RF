

#include <msp430.h>				

#define TX BIT3
#define LED0 BIT1
#define LED1 BIT2
#define Switch1 BIT0
#define IRrec BIT2
#define IRrec1 BIT3

void send_rf(unsigned char, unsigned char*);
void zero(unsigned char);
void one(unsigned char);
void f(unsigned char);

unsigned char TXdata[12] = {0,2,2,2,2,2,0,0,1,0,0,0};
unsigned char ii =0;
unsigned char iii =0;
unsigned char programCount=0;
unsigned char programMode =0;
int bit =0;
int bitt =0;
int count =0;
int resetcount =0;
int loopcount=0;
int loopcountMAX=0;
int One =0;
int two=0;
int three =0;
int four=0;
int five=0;
int six=0;
int seven=0;
int eight =0;
int ledcount =0;
int odd=0;
unsigned long realdata =0;
unsigned long rxdata =0;
unsigned long programCode =0;



int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  P2DIR |= (LED1 + LED0 + TX);                            // Set all pins but RXD to output
	  P1REN |= Switch1;
	  P1OUT |= Switch1;
	  P1IE |= IRrec; // P1.3 interrupt enabled
	  P1IES &= ~IRrec; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec; // P1.3 IFG cleared
	  P1IE |= IRrec1; // P1.3 interrupt enabled
	  P1IES |= IRrec1; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec1; // P1.3 IFG cleared

	  TA0CCTL0 &= ~CCIE;                             // CCR0 interrupt enabled
	  TA0CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
	  TA0CCR0 =  20;



	  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts
	while(1)
	{
		programCount=0;
		for(ii=10;ii>0;ii--)
		             {
		           	    if((P1IN & Switch1) == Switch1)
		           	    {
		           	    	programCount++;
		           	    }
		           	    _delay_cycles(10);
		             }

		        if(programCount >8)
		        {
		        programMode = 1;
		        }

		       _delay_cycles(100);

        if(resetcount == 200)
        {
          loopcountMAX=loopcount;
          loopcount =0;
          realdata = rxdata >> 1;
          TA0CCTL0 &= ~CCIE;

        }
        if(realdata != 0) //gun 1 code
                   {
               	 P1IE &= ~(IRrec + IRrec1);
                 if(programMode == 1)
                     {
                   	  programMode = 0;
                   	  programCode = realdata;
                   	  P2OUT &= ~(LED0+ LED1);
                   	  for(iii=0;iii<10;iii++)
                   	  {
                   		  P2OUT |= (LED0);
                   		  _delay_cycles(50000);
                   		  P2OUT &= ~(LED0);
                   		  _delay_cycles(50000);
                   	  }
                     }
                     P1IE |= (IRrec + IRrec1);
                   }
               if(realdata == programCode && (programCode != 0)) //gun 1 code
               {
           		send_rf(TX, TXdata);
           		send_rf(TX, TXdata);
           		send_rf(TX, TXdata);
           		send_rf(TX, TXdata);
                 realdata =0;
                     P2OUT |=LED1;
                     P2OUT &=~LED0;
                     ledcount =0;
               }

               if (realdata != 0 && realdata != programCode)
               {
               	realdata =0;
                P2OUT |=LED0;
                P2OUT &=~LED1;
                   ledcount =0;
               }

               ledcount++;
               if(ledcount == 1999)
               {
                   P2OUT &= ~(LED0 + LED1);
               }
               else if(ledcount >2000)
               {
               	ledcount =2000;
               }


                       resetcount++;
                   if(resetcount >1000)
                   {
                     resetcount =999;
                   }
                   if(count >1000)
                   {
                     count =999;
                   }
	}
}

void send_rf(unsigned char pin, unsigned char* data)
{
unsigned char i=0;

for(i=0;i<12;i++)
{
if(data[i] == 0)
{
	zero(pin);
}
else if(data[i] ==1)
{
	one(pin);
}
else
{
	f(pin);
}

}

P2OUT |= pin;
_delay_cycles(250);
P2OUT &= ~pin;
_delay_cycles(7000);
}

void zero(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(690);
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(690);
}
void one(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(210);
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(210);
}
void f(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(700);
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(200);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  TA0CTL |= TACLR;
  TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    One = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
        if(count >13 && count < 24)
            {
                bit = 1;
            }
        else
        {
            bit = 0;
        }

                rxdata += bit;
                rxdata<<=1;
  }

loopcount++;
  count=0;
  resetcount=0;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  count++;
}
