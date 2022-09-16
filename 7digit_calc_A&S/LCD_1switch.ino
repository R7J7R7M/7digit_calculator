#include"atmega328p_port_init.h"
//#include"keypad.h"
//global variables

volatile char entry[14] ="Calc's hiee";
volatile char character[16];
volatile char character_binary;
volatile char str[16];
volatile char len=0;
int flag=0;
//global varibles for calculation part
int arr1[16]={0};
int arr2[16]={0};
int sum[16]={0};
int remainder[16] ={0};
char symbol;

//function used 
volatile char inport(volatile char);
void outportD(volatile char);

void setup() 
{
  // put your setup code here, to run once:
  init_port(0xFF);
  volatile char a;
  
 //LCD_work
  a = inport(0);
  while(a == 0x00){
    a = inport(0);
    if(a == 0x01) init_LCD();
  }
  write_string(entry);
  
  //keypad_work
  character_conversion();
  write_data(character[16]);
  volatile char keypad_data;
  volatile char i=0;
  while(1){
  keypad_data = scan_keypad();
  while(keypad_data == 0x00){
    keypad_data = scan_keypad();
    if(keypad_data){
        character_binary = keypad_data;
        
      }
  }
  if(character_binary == 13)//clear button
  {
    outportD(0x01);
    pulse_LCD_command();//clear the display
    delay(15);
    }
  else if(character_binary == 15)// equals button
  {
    calculation();
    while(character_binary != 13){
      character_binary = scan_keypad();
      if(character_binary == 13){
        outportD(0x01);
        pulse_LCD_command();//clear the display
        delay(15);
      }
    }
  }
  else{
      write_data(character[character_binary]);
      len++;
      delay(15);
    }
    
  
  keypad_data = 0x00;
 }
  
}
//calculation part
void calculation()
{
   read_LCD();
   char len;
    for(len=0;str[len]!='\0';len++);
    
    int count1=0,count2=0,i;
    //no of elements in the given data
    for(i=0;i<len;i++){
        if(str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/'){
            count1++;
        }
        else{
            symbol = str[i];
            break;
        }
    }
    count2 = len - i - 1;
    int k=0;
    for(i=count1-1;i>=0;i--){
        arr1[15-i] = str[k] - '0';
        k++;
    }
    k++;//neglecting the symbol
    for(i=count2-1;i>=0;i--){
        arr2[15-i] = str[k] - '0';
        k++;
    }
    function(symbol);
   
}
int function(char symbol)
{
    switch(symbol){
        case'+':addition(arr1,arr2);
                break;
        case'-':subraction(arr1,arr2,0);
                break;
       // case'/':division(arr1,arr2);
               // break;

    }
}
int addition(int *arr1,int *arr2)
{
    int i;
    for(i=0;i<16;i++){
        sum[i] = arr1[i] + arr2[i];
    }
    with_carry(sum);
    removing_zeros(sum);
}
int with_carry(int *total)
{
    int i,x,y;
    for(i=15;i>=0;i--){
        x = total[i]/10;
        total[i-1] = total[i-1]+x;
        total[i] = total[i]%10;
    }
}
void removing_zeros(int *sum)
{
    char rem[101];
    int i,count=0;

    for(i=0;i<16;i++){
        rem[i] = sum[i] + '0';
        }
    for(i=0;i<16;i++){
        if(rem[i] == '0')count++;
        else break;
    }
    outportD(0xC0);
    pulse_LCD_command();
    
    if(count == 16){
        write_data('0');
    }
    else{
        for(int x=count;x<16;x++){
          write_data(rem[x]);
        }
    }
}
int subraction(int *arr1,int *arr2,int flag)
{
    int i;
    for(i=15;i>=0;i--){
        if(arr1[i] < arr2[i]){
            arr1[i] = arr1[i]+10;
            arr1[i-1] = arr1[i-1] - 1;
            sum[i] = arr1[i] - arr2[i];
        }
        else{
            sum[i] = arr1[i] - arr2[i];
        }
    }

    if(flag==0){
        removing_zeros(sum);
    }
}


//read_from_LCD
void read_LCD()
{
  int i;
  init_port(0x00);
  outportD(0x80); // move cursor to the first
  pulse_LCD_command();
  outportD(0x06); // automatic shift
  pulse_LCD_command();
  for(i=0;i<len;i++){
    str[i] = single_dataread_LCD();
  }
  init_port(0xFF);
}
volatile char single_dataread_LCD()
{
  volatile char indata;
  outportB(0x01,7);
  outportB(0x01,6);
  outportB(0x00,5);
  delay(1);
  outportB(0x01,5);
  delay(1);
  indata = inportD();
  outportB(0x00,5);
  delay(1);
  return indata;
}
//keypad data
volatile char scan_keypad()
{
  volatile char i,j,row,col;
  volatile char data;
  for(i=0;i<4;i++){
    keypad_output_pin(i);
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



//LCD DATAUM

void write_string(volatile char *arr)
{
  
  while(*arr != 0){
    write_data(*arr);
    arr++;
  }
  
}
void write_data(volatile char data)
{
  outportD(data);
  outportB(0x01,7);
  outportB(0x00,6);
  outportB(0x00,5);
  delay(1);
  outportB(0x01,5);
  delay(1);
  outportB(0x00,5);
  delay(1);
}
void init_LCD()
{
  outportD(0x01); // clear the display of the LCD
  pulse_LCD_command();
  outportD(0x38); // 2 line mode
  pulse_LCD_command();
  outportD(0x06); // incrementing the cursor
  pulse_LCD_command();
  outportD(0x0F); // cursor design
  pulse_LCD_command();
}

void pulse_LCD_command()
{
  outportB(0x00,7); // make register select to 0 (command)
  outportB(0x00,6);
  outportB(0x00,5);
  delay(1);
  outportB(0x01,5);
  delay(1);
  outportB(0x00,5);
  delay(1); // pulsing the enable signal
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
