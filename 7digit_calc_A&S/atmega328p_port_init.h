#define portD_ddr_register 0x2A
#define portD_data_register 0x2B
#define portD_input_register 0x29

#define portB_ddr_register 0x24
#define portB_data_register 0x25
#define portB_input_register 0x23

#define portC_ddr_register 0x27
#define portC_data_register 0x28
#define portC_input_register 0x26

//port initalization
volatile char outputB;
void init_port(volatile char data)
{
  char *portD_ddr = (char *) portD_ddr_register;
  char *portB_ddr = (char *) portB_ddr_register;
  char *portC_ddr = (char *) portC_ddr_register;

  *portD_ddr = data;
  *portB_ddr = 0xFF;
  *portC_ddr = 0x00;
}
void outportD(volatile char data)
{
    volatile char *portD_data = (volatile char *) portD_data_register;
    *portD_data = data;
}
volatile char inportD()
{
  volatile char *portD_data_input = (volatile char*) portD_input_register;
  volatile char input_data;
  input_data = *portD_data_input;
  return input_data;
}
void outportB(volatile char data,volatile char pin)
{
  volatile char *portB_data = (volatile char *)portB_data_register;
  if(data == 0x01){
    outputB = outputB | (data << pin);
  }
  else{
    outputB = outputB & (~(0x01 << pin));
  }
  *portB_data = outputB;
}
volatile char inport(volatile char pin)
{
  volatile char *portC_data = (volatile char *) portC_input_register;
  volatile char data;
  data = *portC_data;
  data = 0x01 & (data >> pin);
  return data;
}
void loop() {
  // put your main code here, to run repeatedly:

}
