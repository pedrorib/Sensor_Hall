// Programa Sensor magnético

#include <Wire.h>

  byte val = 0, stat=0, ack;
  int c2=0;
  short int temp_swt, swt=1;
  int i=0;  
  unsigned int atime=0, ptime=0, rtime;
  int b1=0, b2=0;
  char char1[5], char2[5], char3[5];
  String str1, str2, str3;
  byte byte1=0, byte2=0, byte3=0;
  unsigned int st;

  union field{
    int valor;
    byte a[2];
  };

  field x, y, z;

void setup() {
  pinMode(4,INPUT);
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop() {

  if(Serial.available()&&swt==1){
    temp_swt=Serial.readString().toInt();
    if(temp_swt==0){
      swt=temp_swt;
      Serial.println("MODO DE FUNCIONAMENTO: CONFIGURACAO");
      delay(1500);
    }
    else{
      Serial.println(temp_swt);
      Serial.println("Introduzir 0 para modo de configuracao");
      delay(1500);
    }
  }

  switch (swt){

    case 0:
      Wire.beginTransmission(0xc); // transmit to device #44 (0x2c)
      // device address is specified in datasheet
            
      Wire.write(byte(0x50));            //[0101 0000] sends instruction byte (consultar memória)
      Wire.write(byte3);                 //[0000 0100] morada 0x02
      ack=Wire.endTransmission();
    
      Serial.print("A ler morada 0x");
      Serial.println(byte3 ,HEX);
      Serial.println();
      
      Serial.println(ack);
      Wire.requestFrom(0xc, 3);    // request 2 bytes from slave device #0xe (status)
      
      stat=Wire.read();
      b1=Wire.read();
      b2=Wire.read();
    
      Serial.println(stat,BIN);
      Serial.print(b1,BIN);
      Serial.print("  ");
      Serial.print(b2,BIN);
      Serial.println("\n");
             
      if(Serial.available()){
        str1=Serial.readString();
        str1.toCharArray(char1, 5);
        byte1=strtoul(char1,NULL,16);
        st=1;
        if (char1[0]=='s'){
          swt=1;
          Serial.println("A SAIR DO MODO DE CONFIGURACAO");
          delay(1500); 
          break;
        }
        Serial.println("Introduzir byte 2");
        while(!Serial.available()&&st==1);
        str2=Serial.readString();
        str2.toCharArray(char2, 5);
        byte2=strtoul(char2,NULL,16);
        st=2;
        Serial.println("Introduzir MORADA em hexadecimal");
        while(!Serial.available()&&st==2);
        str3=Serial.readString();
        str3.toCharArray(char3, 5);
        byte3=strtoul(char3,NULL,16);
        byte3=byte3 << 2;
        if(char2[0]!='p'){
          Wire.beginTransmission(0xc);
          Wire.write(0x60); //Instruction - write
          Wire.write(byte1); 
          Wire.write(byte2); 
          Wire.write(byte3); 
          ack=Wire.endTransmission();
          Wire.requestFrom(0xc, 1);
          stat=Wire.read();
          Serial.println(ack);
          Serial.println(stat);
          Serial.println("Dados enviados \n");
        }
        else
          Serial.println("Nao foi enviada instrucao \n");
      }
      else
        Serial.println("Pronto para receber instrucao");
        Serial.println("(s para sair deste modo)");
      delay(2000);
    break;

    case 1:
      Wire.beginTransmission(0xc); // transmit to device #44 (0x2c)
      // device address is specified in datasheet
            
      Wire.write(byte(0x3e));            //[0011 1110] sends instruction byte (single read, XYZ sem temperatura)
      
      Wire.endTransmission();
    
      //ptime=millis();
      //while(digitalRead(4)==LOW);
      //atime=millis();
      //rtime=atime-ptime;
      delay(10);
        
      Wire.requestFrom(0xc, 1);    // request 1 byte from slave device #0xc (status)
      //Serial.print("\n");
      
      c2=0;
      
      while(Wire.available())    // slave may send less than requested
      {    
        c2 = Wire.read();    // lê todos os bytes e guarda num vector
      }
    
      Wire.beginTransmission(0xc); 
    
      Wire.write(byte(0x4e)); //0100 1110 - Ler uma vez campo (zyx)
    
      Wire.endTransmission();
    
      Wire.requestFrom(0xc, 7);    // request 1 byte from slave device #0xc (status)
      //Serial.print("\n");
      
      c2=0;
    
      
      while(Wire.available())    // slave may send less than requested
      {    
        stat=Wire.read();
        z.a[1] = Wire.read();
        z.a[0] = Wire.read();
        y.a[1] = Wire.read();
        y.a[0] = Wire.read();
        x.a[1] = Wire.read();
        x.a[0] = Wire.read();
        // lê todos os bytes e guarda num vector
      }  
    
      /*Serial.print("Status BYTE -> ");
      Serial.print(stat);
      Serial.println();*/
      Serial.println("BEGIN");
      Serial.println(stat);
      Serial.println(x.valor);
      Serial.println(y.valor);
      Serial.println(z.valor);
      delay(10);  
      break;
        }
  }
