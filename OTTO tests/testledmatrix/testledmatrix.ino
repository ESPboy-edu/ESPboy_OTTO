
//*** OTTO font 6x8 ***
const uint8_t Character_font_6x8[] PROGMEM ={
    0x00,0x7C,0x82,0x82,0x7C,0x00,//0
    0x00,0x42,0xFE,0x02,0x00,0x00,//1
    0x00,0x46,0x8A,0x92,0x62,0x00,//2
    0x00,0x44,0x92,0x92,0x6C,0x00,//3
    0x00,0x1C,0x64,0xFE,0x04,0x00,//4
    0x00,0xF2,0x92,0x92,0x8C,0x00,//5
    0x00,0x7C,0x92,0x92,0x4C,0x00,//6
    0x00,0xC0,0x8E,0x90,0xE0,0x00,//7
    0x00,0x6C,0x92,0x92,0x6C,0x00,//8
    0x00,0x64,0x92,0x92,0x7C,0x00,//9
    0x00,0x00,0x14,0x00,0x00,0x00,//:
    0x00,0x02,0x24,0x00,0x00,0x00,//;
    0x00,0x10,0x28,0x44,0x82,0x00,//<
    0x00,0x28,0x28,0x28,0x28,0x00,//=
    0x00,0x82,0x44,0x28,0x10,0x00,//>
    0x00,0x20,0x4a,0x30,0x00,0x00, //?
    0x00,0x00,0x00,0x00,0x00,0x00,//@
    0x00,0x7E,0x88,0x88,0x7E,0x00,//A
    0x00,0xFE,0x92,0x92,0x6C,0x00,//B
    0x00,0x7C,0x82,0x82,0x44,0x00,//C
    0x00,0xFE,0x82,0x82,0x7C,0x00,//D
    0x00,0xFE,0x92,0x92,0x82,0x00,//E
    0x00,0xFE,0x90,0x90,0x80,0x00,//F
    0x00,0x7C,0x82,0x92,0x5C,0x00,//G
    0x00,0xFE,0x10,0x10,0xFE,0x00,//G
    0x00,0x82,0xFE,0x82,0x00,0x00,//I
    0x00,0x0C,0x02,0x02,0xFC,0x00,//J
    0x00,0xFE,0x10,0x28,0xC6,0x00,//K
    0x00,0xFE,0x02,0x02,0x02,0x00,//L
    0x00,0xFE,0x40,0x30,0x40,0xFE,//M
    0x00,0xFE,0x40,0x30,0x08,0xFE,//N
    0x00,0x7C,0x82,0x82,0x82,0x7C,//O
    0x00,0xFE,0x90,0x90,0x60,0x00,//P
    0x00,0x7C,0x82,0x8A,0x84,0x7A,//Q
    0x00,0xFE,0x98,0x94,0x62,0x00,//R
    0x00,0x64,0x92,0x92,0x4C,0x00,//S
    0x00,0x80,0x80,0xFE,0x80,0x80,//T
    0x00,0xFC,0x02,0x02,0xFC,0x00,//U
    0x00,0xF0,0x0C,0x02,0x0C,0xF0,//V
    0x00,0xFE,0x04,0x38,0x04,0xFE,//W
    0x00,0xC6,0x38,0x38,0xC6,0x00,//X
    0xC0,0x20,0x1E,0x20,0xC0,0x00,//Y
    0x00,0x86,0x9A,0xB2,0xC2,0x00,//Z
    0x00,0x00,0x7a,0x00,0x00,0x00,//!
    0x00,0x00,0x00,0x00,0x00,0x00,//' '  
};
 

const uint32_t Gesturetable[4][10] PROGMEM = {
  {
    0b00000000000000001100001100000000, // littleUuh_code1
    0b00000000000000000110000110000000, // littleUuh_code2
    0b00000000000000000011000011000000, // littleUuh_code3
    0b00000000000000000110000110000000, // littleUuh_code4
    0b00000000000000001100001100000000, // littleUuh_code5
    0b00000000000000011000011000000000, // littleUuh_code6
    0b00000000000000110000110000000000, // littleUuh_code7
    0b00000000000000011000011000000000
  } // littleUuh_code8
  ,
  {
    0b00000000000000000000110000110000, // dreamMouth_code1
    0b00000000000000010000101000010000, // dreamMouth_code2
    0b00000000011000100100100100011000, // dreamMouth_code3
    0b00000000000000010000101000010000  // dreamMouth_code4
  } 
  ,
  {
    0b00100001000000000000000000100001, // adivinawi_code1
    0b00010010100001000000100001010010, // adivinawi_code2
    0b00001100010010100001010010001100, // adivinawi_code3
    0b00000000001100010010001100000000, // adivinawi_code4
    0b00000000000000001100000000000000, // adivinawi_code5
    0b00000000000000000000000000000000  // adivinawi_code6
  } 
  ,
  {
    0b00001100010010100001000000000000, // wave_code1
    0b00000110001001010000100000000000, // wave_code2
    0b00000011000100001000010000100000, // wave_code3
    0b00000001000010000100001000110000, // wave_code4
    0b00000000000001000010100100011000, // wave_code5
    0b00000000000000100001010010001100, // wave_code6
    0b00000000100000010000001001000110, // wave_code7
    0b00100000010000001000000100000011, // wave_code8
    0b00110000001000000100000010000001, // wave_code9
    0b00011000100100000010000001000000  // wave_code10
  } 
};


//-- Mouth
const uint32_t Mouthtable[31] PROGMEM = {
  0b00001100010010010010010010001100, //zero_code
  0b00000100001100000100000100001110, //one_code
  0b00001100010010000100001000011110, //two_code
  0b00001100010010000100010010001100, //three_code
  0b00010010010010011110000010000010, //four_code
  0b00011110010000011100000010011100, //five_code
  0b00000100001000011100010010001100, //six_code
  0b00011110000010000100001000010000, //seven_code
  0b00001100010010001100010010001100, //eight_code
  0b00001100010010001110000010001110, //nine_code
  0b00000000100001010010001100000000, //smile_code
  0b00000000111111010010001100000000, //happyOpen_code
  0b00000000111111011110000000000000, //happyClosed_code
  0b00010010101101100001010010001100, //heart_code
  0b00001100010010100001010010001100, //bigSurprise_code
  0b00000000000000001100001100000000, //smallSurprise_code
  0b00111111001001001001000110000000, //tongueOut_code
  0b00111111101101101101010010000000, //vamp1_code
  0b00111111101101010010000000000000, //vamp2_code
  0b00000000000000111111000000000000, //lineMouth_code
  0b00000000001000010101100010000000, //confused_code
  0b00100000010000001000000100000010, //diagonal_code
  0b00000000001100010010100001000000, //sad_code
  0b00000000001100010010111111000000, //sadOpen_code
  0b00000000001100011110110011000000, //sadClosed_code
  0b00000001000010010100001000000000, //okMouth_code
  0b00100001010010001100010010100001, //xMouth_code
  0b00001100010010000100000100000100, //interrogation_code
  0b00000100001000011100001000010000, //thunder_code
  0b00000000100001101101010010000000, //culito_code
  0b00000000011110100001100001000000  //angry_code
} ;

//OTTO Mouths 
#define M_zero          0
#define M_one           1                
#define M_two           2               
#define M_three         3  
#define M_four          4
#define M_five          5 
#define M_six           6 
#define M_seven         7  
#define M_eight         8 
#define M_nine          9
#define M_smile         10                  
#define M_happyOpen     11  
#define M_happyClosed   12  
#define M_heart         13
#define M_bigSurprise   14  
#define M_smallSurprise 15
#define M_tongueOut     16
#define M_vamp1         17  
#define M_vamp2         18  
#define M_lineMouth     19
#define M_confused      20  
#define M_diagonal      21          
#define M_sad           22
#define M_sadOpen       23 
#define M_sadClosed     24
#define M_okMouth       25 
#define M_xMouth        26
#define M_interrogation 27
#define M_thunder       28
#define M_culito        29
#define M_angry         30  


//LED matrix internal commands 
#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f



uint8_t bufColMatrix[8];
uint8_t  PIN_Matrix_Dta, PIN_Matrix_CLK, PIN_Matrix_CS;


//-- LED MATRIX AND MOUTH ---------------------------------------//
void initMatrix(uint8_t Dta, uint8_t Clk, uint8_t CS){
  PIN_Matrix_Dta = Dta;
  PIN_Matrix_CLK = Clk;
  PIN_Matrix_CS = CS;
  pinMode(PIN_Matrix_Dta,  OUTPUT);
  pinMode(PIN_Matrix_CLK, OUTPUT);
  pinMode(PIN_Matrix_CS,  OUTPUT);
  digitalWrite(PIN_Matrix_CLK, HIGH); 
  setCommandMatrix(max7219_reg_scanLimit, 0x07);      
  setCommandMatrix(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  setCommandMatrix(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  setCommandMatrix(max7219_reg_displayTest, 0x00); // no display test
  clearMouth();  // empty registers, turn all LEDs off
  intensityMatrix(0x0f); //max brightness
}


void intensityMatrix(int intensity){
  setCommandMatrix(max7219_reg_intensity, intensity);
}


void setCommandMatrix(uint8_t command, uint8_t value){
  digitalWrite(PIN_Matrix_CS, LOW);    
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, command);
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, value);
  digitalWrite(PIN_Matrix_CS, LOW);
  digitalWrite(PIN_Matrix_CS, HIGH);
}


void setDotMatrix(int8_t col, int8_t row){
 if(col<8 && row<8 && col>=0 && row>=0){
    bitWrite(bufColMatrix[col], row, 1);
    digitalWrite(PIN_Matrix_CS, LOW);  
    shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, col+1);
    shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, bufColMatrix[col]);
    digitalWrite(PIN_Matrix_CS, LOW);
    digitalWrite(PIN_Matrix_CS, HIGH);
  }
}


void setColumnMatrix(uint8_t col, uint8_t value){
  digitalWrite(PIN_Matrix_CS, LOW);    
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, col+1);
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, value);
  digitalWrite(PIN_Matrix_CS, LOW);
  digitalWrite(PIN_Matrix_CS, HIGH);
  bufColMatrix[col] = value;
}


void putMouth(uint8_t mouth){
  uint32_t value = pgm_read_dword(&Mouthtable[mouth]);
  uint8_t bufMouth[5];
 
  for(uint8_t i=0; i<5; i++){
    bufMouth[i] = value & 63;
    value >>= 6;
  }
  clearMouth();
 for(uint8_t i=0; i<5; i++)
   for(uint8_t j=0; j<6; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(6-j,5-i+2);
}


void putMouth5x6(uint32_t mouth){
  uint32_t value = mouth;
  uint8_t bufMouth[5];
  for(uint8_t i=0; i<5; i++){
    bufMouth[i] = value & 63;
    value >>= 6;
  }
  clearMouth();
 for(uint8_t i=0; i<5; i++)
   for(uint8_t j=0; j<6; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(6-j,5-i+2);
}


void putMouth8x8(uint64_t mouth){
  uint64_t value = mouth;
  uint8_t bufMouth[8];
  for(uint8_t i=0; i<8; i++){
    bufMouth[i] = value & 255;
    value >>= 8;
  }
 clearMouth();
 for(uint8_t i=0; i<8; i++)
   for(uint8_t j=0; j<8; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(8-j,8-i);
}


void clearMouth(){
  for (int i=0; i<8; i++){ 
    setColumnMatrix(i,0);
    bufColMatrix[i]=0;
  }
}


void writeTextMatrix(char *str, uint16_t scrollspeed){
 uint8_t charNo = 0;
 char currentChar;
 char printChar;
  while (currentChar = str[charNo++]){
    if ((currentChar < 48) || (currentChar > 91))
       printChar = 44;
    else 
       printChar = currentChar-48;
    sendCharMatrix (printChar, scrollspeed);
  }
}



void sendCharMatrix (uint8_t printChar, uint16_t scrollspeed){
 uint8_t bufSymbol[8];
  memcpy_P (&bufSymbol, &Character_font_6x8[printChar*6], 6);
  clearMouth();
  for(int8_t a=6; a>-7; a--){
    for(uint8_t i=0; i<6; i++)
      for (uint8_t j=0; j<8; j++)
        if ((bufSymbol[i]>>j) & 1) setDotMatrix(i+1+a, 8-j);
  delay(scrollspeed);
  clearMouth();   
  }
}



void setup(){
 initMatrix(7,16,6); //dta, cls, cs
}


void loop(){
 clearMouth();
 putMouth5x6 (M_smile);
 clearMouth();
 for (int i=0; i<8; i++)
   for(int j=0; j<8; j++){
     setDotMatrix(j, i);
     delay(10);
   }
 writeTextMatrix((char *)"LET'S TEST THIS STRING   ", 200);
}
