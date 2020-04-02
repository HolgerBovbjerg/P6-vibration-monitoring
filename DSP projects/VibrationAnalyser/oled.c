#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "ezdsp5535_lcd.h"
#include "ezdsp5535_gpio.h"

// OSD9616 I2C address
#define OSD9616_I2C_ADDR 0x3C    

//array storing ASCII letters
int character_set[] = {
	                       0x7C,0x09,0x0A,0x7C,  // A
	                       0x36,0x49,0x49,0x7F,  // B 
                           0x22,0x41,0x41,0x3E,  // C
                           0x3E,0x41,0x41,0x7F,  // D
                           0x41,0x49,0x49,0x7F,  // E
                           0x01,0x09,0x09,0x7F,  // F
                           0x32,0x51,0x41,0x3E,  // G
                           0x7F,0x08,0x08,0x7F,  // H
                           0x00,0x7F,0x00,0x00,  // I
                           0x3F,0x40,0x40,0x20,  // J                             
                           0x41,0x22,0x14,0x7F,  // K
                           0x40,0x40,0x40,0x7F,  // L
                           0x7F,0x06,0x06,0x7F,  // M
                           0x7F,0x18,0x0C,0x7F,  // N
                           0x3E,0x41,0x41,0x3E,  // O
                           0x06,0x09,0x09,0x7F,  // P
                           0x7E,0x61,0x41,0x3E,  // Q  
                           0x76,0x09,0x09,0x7F,  // R
                           0x32,0x49,0x49,0x26,  // S 
                           0x01,0x01,0x7F,0x01,  // T
                           0x3F,0x40,0x40,0x3F,  // U
                           0x0F,0x70,0x70,0x0F,  // V
                           0x7F,0x30,0x30,0x7F,  // W
                           0x63,0x1C,0x1C,0x63,  // X
                           0x03,0x0C,0x78,0x03,  // Y
                           0x43,0x4D,0x51,0x61   // Z
                            
                         };

// Array storing ASCII numbers and special characters
int number_set [] = { 0x10,0x7C,0x10,0x10,  // +	                  
	                  0x00,0x60,0x80,0x00,  // ,
	                  0x10,0x10,0x10,0x10,  // -
	                  0x00,0x60,0x00,0x00,  // .
	                  0x03,0x0C,0x30,0x40,  // /
	                  0x3E,0x41,0x41,0x3E,  // 0
	                  0x00,0x7F,0x00,0x00,  // 1
	                  0x46,0x49,0x49,0x72,  // 2	                  
	                  0x36,0x49,0x49,0x22,  // 3
	                  0x08,0x7C,0x08,0x0F,  // 4
	                  0x31,0x49,0x49,0x2F,  // 5
                      0x33,0x49,0x49,0x3E,  // 6   
                      0x07,0x09,0x71,0x01,  // 7 
                      0x36,0x49,0x49,0x36,  // 8
                      0x3E,0x49,0x49,0x26,  // 9
					  0x00,0x6C,0x00,0x00,  // :
					  0x00,0x6C,0x80,0x00,  // ;
					  0x08,0x14,0x22,0x41,  // >
					  0x14,0x14,0x14,0x14,  // =
					  0x06,0x09,0x59,0x02,  // ?
					  0x7E,0x41,0x59,0x5E   // @
                    };

/* Function for sending data to OSD9616 OLED screen
 * INPUT
 * comdat: command (0x00) or data (0x04)
 * data: command or data for OSD9616
 *  */
Int16 OSD9616_send( Uint16 comdat, Uint16 data )
{
    Uint8 cmd[2]; // command vector
    cmd[0] = comdat & 0x00FF;     // Specifies whether data is Command or Data (ANDed with 8 bit mask)
    cmd[1] = data;                // Command or Data

    return ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 2 );
}

/* Function for sendig multiple bytes of data to OSD9616 OLED scren 
 * INPUT 
 * data: pointer to data
 * len: bytes of data to be sent 
 * 
 * */
Int16 OSD9616_multiSend( Uint8* data, Uint16 len )
{
    Uint16 x; // counter variable
    Uint8 cmd[10]; // command vector
    for(x=0;x<len;x++)               // Command or Data
    {
    	cmd[x] = data[x];
    }
    return ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, len );
}

/* Function for writing a single ASCII character to OSD9616 OLED display
 * INPUT
 * src: pointer to character 
 *  */

Int16 printCharacter(int * src)
{
	// Send character
 	OSD9616_send(0x40,src[0]);
    OSD9616_send(0x40,src[1]);
    OSD9616_send(0x40,src[2]);
    OSD9616_send(0x40,src[3]);
    // Go to next character space
    OSD9616_send(0x40,0x00);
    return 0;
}

/* Function for printing message on OSD 9616 OLED display 
 * Resolves characters and displays all letters as uppercase
 * Default to blanks for unrecognised characters
 * INPUT
 * src: pointer to character array (max 19 characters)
 * */

Int16 printMessage(char * src)
{
 int i,j;
 	
 OSD9616_send(0x40,0x00);
 OSD9616_send(0x40,0x00); // Two spaces at right for alignment	
 
 for ( i = 18 ; i >= 0; i--)
  {
  	  if ( src[i] >= 'A' && src[i] <= 'Z')
  	   {
  	    /* Upper case characters */	 
       printCharacter(&character_set[(src[i]-'A')*4]);
  	   }
   	  else if ( src[i] >= 'a' && src[i] <= 'z')
  	   { 
  	    /* Lower case characters. Print as upper case */	
       printCharacter(&character_set[(src[i]-'a')*4]);
  	   }
  	  else if ( src[i] >= '+' && src[i] <= '@')
  	   {
  	   	/* Numbers 0 to 9 */
  	    printCharacter(&number_set[(src[i]-'+')*4]);
  	   }  
  	  else
  	   {
  	    /* Default to blanks for unrecognised characters */
  	   	for ( j = 0 ; j < 5; j++)
  	   	 {
  	   	  OSD9616_send(0x40,0x00); /* Spaces */
  	   	 }  	    
  	   }  
  }
 return(0);
}

/* Function for initialising OSD9616 OLED display */
int oled_init(void)
{
	Int16 i2c_err;
	Uint8 cmd[10];    // For multibyte commands
	
	/* Initialize I2C */
    ezdsp5535_I2C_init( );
    
    /* Initialize LCD power */
    ezdsp5535_GPIO_setDirection( 12, 1 );  // Output
    ezdsp5535_GPIO_setOutput( 12, 1 );     // Enable 13V 
    
    /* Initialize OSD9616 display */
    i2c_err = OSD9616_send(0x00,0x00); // Set low column address
    i2c_err = OSD9616_send(0x00,0x10); // Set high column address
    if(i2c_err)  // Don't setup display if not connected
        return 1;
    
    OSD9616_send(0x00,0x40); // Set start line address

    cmd[0] = 0x00 & 0x00FF;  // Set contrast control register
    cmd[1] = 0x81;
    cmd[2] = 0x7f;
    ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xa1); // Set segment re-map 95 to 0
    OSD9616_send(0x00,0xa6); // Set normal display

    cmd[0] = 0x00 & 0x00FF;  // Set multiplex ratio(1 to 16)
    cmd[1] = 0xa8; 
    cmd[2] = 0x0f;
    ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xd3); // Set display offset
    OSD9616_send(0x00,0x00); // Not offset
    OSD9616_send(0x00,0xd5); // Set display clock divide ratio/oscillator frequency
    OSD9616_send(0x00,0xf0); // Set divide ratio

    cmd[0] = 0x00 & 0x00FF;  // Set pre-charge period
    cmd[1] = 0xd9;
    cmd[2] = 0x22;
    ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    cmd[0] = 0x00 & 0x00FF;  // Set com pins hardware configuration
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xdb); // Set vcomh
    OSD9616_send(0x00,0x49); // 0.83*vref
    
    cmd[0] = 0x00 & 0x00FF;  //--set DC-DC enable
    cmd[1] = 0x8d;
    cmd[2] = 0x14;
    ezdsp5535_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xaf); // Turn on oled panel    
    
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5
  
	return 0;
}

/* Function for displaying message on OSD9616 OLED display 
 * INPUT
 * topline: pointer to character array containing top text (max 19 characters)
 * bottomline: pointer to character array containing bottom text (max 19 characters)
 * */

int oled_display_message(char * topline, char * bottomline)
{
	Uint8 cmd[10];    // For multibyte commands
	
	OSD9616_send(0x00,0x2E);           // Turn off scrolling
	
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5

    printMessage(&topline[0]);
    
    /* Write to page 1*/ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
   
    printMessage(&bottomline[0]);
    
        /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x03;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2E);           // Turn off scrolling
    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );

	return 0;
}

