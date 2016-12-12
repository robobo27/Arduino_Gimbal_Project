//used for spinning a motor in the opposite direction of the z-axis reading

#include <Wire.h> 
#include <Adafruit_L3GD20.h>

//motor stuff
int motorpina = 9;
int motorpinb = 10;
int motorpinc = 11;

const int a[] = {1,1,1,0,0,0};
const int b[] = {1,0,0,0,1,1};
const int c[] = {0,0,1,1,1,0};

int aspot = 0;
int bspot = 0;
int cspot = 0;

int dir = 0;

//gimbal stuff
#ifdef USE_I2C
  // The default constructor uses I2C
  Adafruit_L3GD20 gyro;
#else
  // To use SPI, you have to define the pins
  #define GYRO_CS 4 // labeled CS, cs
  #define GYRO_DO 5 // labeled SA0, mosi
  #define GYRO_DI 6  // labeled SDA, miso
  #define GYRO_CLK 7 // labeled SCL, clk
  Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif

void setup(){
  //motor stuff
  pinMode(motorpina,OUTPUT);
  pinMode(motorpinb,OUTPUT);
  pinMode(motorpinc,OUTPUT);

  //gimbal stuff
  gyro.begin(gyro.L3DS20_RANGE_500DPS);
  Serial.begin(9600);
}

int rzero[] = {0, 0, 0};
//float scalar_2000dps = .7;
int scalar_500dps = 18;
//int scalar_250dps = 9;
int count = 1;
int truez;
void loop(){

  gyro.read();
  if(count<100){
    rzero[0] = ((int)gyro.data.x + rzero[0])/count;
    rzero[1] = ((int)gyro.data.y + rzero[1])/count;
    rzero[2] = ((int)gyro.data.z + rzero[2])/count;
    count += 1;
  }
  else{
  //int truex = scalar_500dps*(gyro.data.x-rzero[0]);//following the technical data sheets
//  this accounts for turn on and turn off instability
  //int truey = scalar_500dps*(gyro.data.y-rzero[1]);
  truez = gyro.data.z-rzero[2];
  if(truez > 3){
    dir = 1;
  }
  else if(truez < -3){
    dir = 0;
  }
  else{
    dir = -1;
  }
  Serial.print("TrueZ: "); Serial.print(truez); Serial.print("Dir: "); Serial.print(dir); Serial.println(" ");
  move();
  delay(15);
  }
}

void move(){
  if(dir == 1){//for clockwise direction
    if(aspot == 5){
      aspot = 0;
      bspot = 0;
      cspot = 0;
    }
    else{
      aspot ++;
      bspot ++;
      cspot ++;
    }
  }
  else if(dir == 0){
    if(aspot == 0){
      aspot = 5;
      bspot = 5;
      cspot = 5;
    }
    else{
      aspot --;
      bspot --;
      cspot --;
    }
  }
  //if dir == -1 we will hold state
  digitalWrite(motorpina,a[aspot]);
  digitalWrite(motorpinb,b[bspot]);
  digitalWrite(motorpinc,c[cspot]);
}

