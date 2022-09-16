//keypad data
//volatile char character[16];
void keypad_output_in(volatile char);
volatile char scan_keypad()
{
  volatile char i,j,row,col;
  volatile char data;
  for(i=0;i<4;i++){
    //keypad_output_pin(i);
    col = (inport(1) | inport(2) | inport(3) | inport(4));
    if(col){
      row = i;
      for(j=1;j<=4;j++){
        if(inport(j) == 0x01){
          col = j;
          data = row*4 + (col - 1) + 1;
        return data;
        }
      }
    }
  }
   return 0;
}
void keypad_output_pin(volatile char pin)
{
  switch(pin){
    case 0: outportB(0x01,0);
            outportB(0x00,1);
            outportB(0x00,2);
            outportB(0x00,3);
            break;
    case 1: outportB(0x00,0);
            outportB(0x01,1);
            outportB(0x00,2);
            outportB(0x00,3);
            break;
    case 2: outportB(0x00,0);
            outportB(0x00,1);
            outportB(0x01,2);
            outportB(0x00,3);
            break;
    case 3: outportB(0x00,0);
            outportB(0x00,1);
            outportB(0x00,2);
            outportB(0x01,3);
            break;
    default: break;
    
  }
  
}
void character_conversion()
{
  character[1] = '7';
  character[2] = '8';
  character[3] = '9';
  character[4] = '/';
  character[5] = '4';
  character[6] = '5';
  character[7] = '6';
  character[8] = '*';
  character[9] = '1';
  character[10] = '2';
  character[11] = '3';
  character[12] = '-';
  character[13] = 'o';
  character[14] = '0';
  character[15] = '=';
  character[16] = '+';
}
