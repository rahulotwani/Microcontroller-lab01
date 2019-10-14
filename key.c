		 /*******************************************************************************************/
 /*  Object : Performs a single digit calculator using keypad & 7segment interface

	Connections : Connect Keyboard Display interface to 26pin FRC connector cable given 
	              by ESA on ESA I/F ADP FOR MCBx51              
	working procedure:  Enter number1  ( 0-9 )
					    Enter Opcode  ( +,-,/,*)
						  if you give '+' it displays 'A'						              
									  '*' it displays 'M'
									  '-' it displays '-'
									  '/' it displays 'd'

						Ente number2  ( 0-9)
						Press '='  -> displays result			   
						Press 'AC' to clear the display
						Repeat the same .	

											 
/*****************************************************************************************/

#include <at89c51xd2.h>
#include <stdio.h>
#include <intrins.h>
														  
unsigned char xdata porta _at_ 0xe000;						/*Port A address */
unsigned char xdata portb _at_ 0xe001;		   			   /*Port B address */
unsigned char xdata portc _at_ 0xe002;					   /*Port C address */
unsigned char xdata control _at_ 0xe003;	               /* control port*/

void DispChar(unsigned char ch);
void ClrLED();
unsigned char getkey();
unsigned int getnum();
//unsigned char getOp();
unsigned char oper;

 main()
{
                                                                                                                                                                                                                                                                                                                                                                                                                                                   unsigned char tmp=0x0ff,n1=0,n2,Op,Res;
  unsigned char NumTab[10] = { 0x0c0,0x0f9,0x0a4,0xb0,0x99,0x92,0x82,0x0f8,0x80,0x90 };			  
  unsigned char OpTab[7] = { 0x88,0x0c8,0xff,0xff,0x0Bf,0xff,0x0a1};	
 						 
  bit Neg = 0;
  control = 0x90;			
 ClrLED();				      /* Clear 7 segment display */			 
  while(1)
  {	
    Neg = 0;			      /* Negative flag */			 
	n1=getnum();		      /* Get 1st number */		  
		  /* Display number */		  

	Op = oper - 0x0C;	  /* Get Opcode. 0x0C is keycode of '+'(see keyboard Display schematics)*/		 

	DispChar(OpTab[Op]);

	n2=getnum();				/* Get 2nd number */	  

	while(getkey()!=0x0B);			/* wait for '=' key */ 
	ClrLED();
	switch(Op)						 /* Perform corresponding operation */ 
	{
	   case 0:						   
	   	
	            Res = n1 + n2;  		/* Perform addition */
	   			
				
	   			break;
	   case 1:	
	   			
				Res = n1 * n2;			 /* Perform multiplication */
	   			break;

	 
	  
				
	   case 4:
	   			if(n2>n1)			  	 /* check for negative number */
	   			{						 /* Perform substaction */
	              Neg = 1;
				  Res = n2 - n1;
				  break;
				}
				Res = n1 - n2;	
				 			
	   			break;
	   
	   case 6:								 /* Perform division */
	   			Res = n1 / n2;
	   			break;
	}
	DispChar(NumTab[Res%10]);		 		  /* Display number */
	DispChar(NumTab[Res/10]);
	if(Neg)							 
	 DispChar(0x0Bf);						  /* if negative result display '-' */
	while(getkey()!= 0x13);		 
	{										   /* wait for 'AC' key */
		ClrLED();
	}
  }
}


void DispChar(unsigned char ch)		 		 /* Routine to display char on 7 segment */
{
  unsigned char i,tmp;
  
  for(i=0;i<8;i++)					 
  {
   	tmp = ch & 0x80;	                        /* write data depending on MSB */
	if(tmp)									
	 		control = 0x03;						 
	else
	 		control = 0x02;
control =0x00;								 /* Give Clk Pulse for synchronization */
control =0x01;
	ch = ch << 1;							/* Get next bit */
  }
}

void ClrLED()
{
  unsigned char i;
  for(i=0;i<6;i++)
   DispChar(0x0ff);				  		   /* 0xff for clear segment 
   										( see 7 segment manual for more info) */
   									
}

unsigned char getkey()			   
{								   
  unsigned char i,j,indx,t;	   
  control=0x90;	   				     		   	/* initialise 8255 ports */
  
 porta = 0x0ff;				      
  									   
  while(1)
  {	
  indx = 0x00;		                           /* Index for storing the first value of 
  									   	       scanline */

   for(i=1;i<=8;i<<=1)			      		    /* for 4 scanlines */
   {					
   			   
  portb = 0x0f & i;  	      			  /* write data to scanline */
  t = porta ;		
  t=0x1f & t;			 				 /* Read readlines connected to P0*/
	  				   
	 if(t>0)					    			 /* If key press is true */
	 {							   
	   for(j=0;j<=4;j++)		           /* Check for 5 lines */
	   {						   				  
	     t >>=1;				   			 /* if get pressed key*/
		 if(t==0x0) 				     			 
		 {
		 	return(indx+j);  		     			 /* Return index of the key pressed */
		 }						   
	   }						   
	 } 							   
	indx += 5; 					    				  /* If no key pressed increment index */
   }
   
  }	 
}
													/* Method for getting number */
unsigned int getnum()			 
{
unsigned char NumTab[10] = { 0x0c0,0x0f9,0x0a4,0xb0,0x99,0x92,0x82,0x0f8,0x80,0x90 };
  unsigned char tmp;
  unsigned int tmp2;
  while(1)
  {
    tmp = getkey();
    if(tmp < 0x0a || tmp==0x13){			 	 /* if pressed key is number, return */
     	tmp2=(tmp2*10) + tmp;
		DispChar(NumTab[tmp]);
	}
	else if(tmp==0x0c|| tmp==0x0d|| tmp==0x10|| tmp ==0x12||tmp ==0x13 ){
		oper=tmp;
		ClrLED();
		return tmp2;
	}
	else{
		ClrLED();
		return tmp2;
	}
  }


}
