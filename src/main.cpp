#include <Arduino.h>

//PID: 05-DyeController
//Name: Dye Controller
//Version: 1.0
//Author: Noor Automation Solutions
//Date: 14/02/2023
//Contact: +91 70097-08849




#include "BlockNot.h"
#include "Encoder.h"
#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"
#include <String.h>


#define ok PIN_PD4


Encoder enc(PIN_PD2, PIN_PD3);
LiquidCrystal_I2C lcd(0x20, 16, 2);

BlockNot blinktime(1, SECONDS);
BlockNot cycle(3, SECONDS);

byte menu, menudata;
byte home, submenu;

byte activeprogram = 0;
int encnew, encold;
bool edit, auth;
byte view;
String timestring;
int rapidtemp[ 10 ], risingtemp [ 10 ], risingres [ 10 ], holdhr [ 10 ], holdmin [ 10 ], holdsec [ 10 ], motorruntime [ 10 ], motordirwait [ 10 ];
bool repeatmotor [ 10 ];
byte timeview;
bool read(byte pin) {
  return digitalRead(pin);
}
bool aread(byte pin) {
  return analogRead(pin);
}
void setin(byte pin) {
  pinMode(pin, INPUT);
}
void setout(byte pin) {
  pinMode(pin, OUTPUT);
}
void cursor(byte x, byte y) {
  lcd.setCursor(x, y);
}
void write(String text) {
  lcd.print(text);
}
void writenum(int num) {
  char numstring[5];
  sprintf(numstring, "%02d", num);
  lcd.print(numstring);
}
void clear() {
  lcd.clear();
}
void blink() {
  lcd.blink();
}
void blinkoff() {
  lcd.noBlink();
}
void save() {
  EEPROM.update(1+(10*activeprogram), rapidtemp[activeprogram]);
  EEPROM.update(2+(10*activeprogram), risingtemp[activeprogram]);
  EEPROM.update(3+(10*activeprogram), risingres[activeprogram]);
  EEPROM.update(4+(10*activeprogram), holdhr[activeprogram]);
  EEPROM.update(5+(10*activeprogram), holdmin[activeprogram]);
  EEPROM.update(6+(10*activeprogram), holdsec[activeprogram]);
  EEPROM.update(7+(10*activeprogram), motorruntime[activeprogram]);
  EEPROM.update(8+(10*activeprogram), repeatmotor[activeprogram]);
  EEPROM.update(9+(10*activeprogram), motordirwait[activeprogram]);
}

bool okold = false;
//check for input on button ok
bool checkok() {
  if(read(ok) && !okold){
    okold = true;
    return true;
  }
  if(!read(ok) && okold){
    okold = false;
    return false;
  }
  if(okold){
    return false;
  }
}

void setup() {
  lcd.begin();
  setin(ok);
}

void loop() {
  encnew = enc.read();
if(menu == 0){
  if(cycle.triggered()){
    clear();
    home++;
  }
  if(checkok()){
    clear();
    encold = -1;
    menu = 1;
    enc.write(0);
    }
  switch (home)
  {
  case 0:
    cursor(0,0);
    write("System");
    cursor(0,1);
    write("Loading..");
    break;
  case 1:
    cursor(0,0);
    write("Noor Auto Sol.");
    cursor(0,1);
    write("+91 70097-08849");
    break;
  case 2:
    cursor(0,0);
    write("Dye ControllerV1");
    cursor(0,1);
    write("Status: Ready!");
    break;
  case 3:
    home = 1;
    break;
  default:
    break;
  }
}
if(menu == 1){
  if(encold != encnew){
    encold = encnew;
    clear();
  }
  if(checkok()){
    clear();
    switch (encnew)
    {
    case 1:
      menu = 2;
      menudata = 1;
      enc.write(1);
      encold = -1;
      break;
    case 2:
      menu = 2;
      menudata = 2;
      enc.write(1);
      encold = -1;
      break;
    case 3:
      menu = 2;
      menudata = 3;
      enc.write(1);
      encold = -1;
      break;
    case 4:
      menu = 0;
      home = 1;
      break;
    default:
      enc.write(1);
      break;
    }
  }
  switch (encnew)
  {
  case 1:
    if(activeprogram == 0){
      cursor(0,0);
      write("No Program Active");
      cursor(0,1);
      write("PressOKtoSelect");
    } else {
      cursor(0,0);
      write("Current Program");
      cursor(0,1);
      writenum(activeprogram);
    }
    break;
  case 2:
    cursor(0,0);
    write("View Program");
    cursor(0,1);
    write("PressOKtoSelect");
    break;
  case 3:
    cursor(0,0);
    write("Edit Program");
    cursor(0,1);
    write("PressOKtoSelect");
    break;
  case 4:
    cursor(0,0);
    write("Exit");
    cursor(0,1);
    write("PressOKtoExit");
    break;
  default:
    enc.write(1);
    break;
  }
}
if(menu == 2){
  
  if(encold != encnew){
      encold = encnew;
      clear();
      cursor(0,0);
      write("Select Program");
      cursor(4,1);
      writenum(encnew);
      //if(encnew < 10){cursor(5,1);} else {cursor(6,1);}
      blink();
      }
  if(encnew > 10){encnew = 1;} else if(encnew < 1){encnew = 10;}
  if(checkok()){
      clear();
      blinkoff();
      activeprogram = encnew;
      edit = true;
      menu = 3;
      enc.write(0);
      encold = -1;
      }
}
  

if(menu == 3){
  if(edit){
    if(encnew < 0){enc.write(100);}
    switch (view){
      case 0:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Rapid Temp:");
          cursor(0,1);
          writenum(encnew);
          blink();
          encold = encnew;
        }
        break;
      case 1:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Rising Temp:");
          cursor(0,1);
          writenum(encnew);
          blink();
          encold = encnew;
        }
        break;
      case 2:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Rising Res:");
          cursor(0,1);
          writenum(encnew);
          blink();
          encold = encnew;
        }
        break;
      case 3:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Hold Time:");
          if(encnew > 60) {encnew = 0;} else if(encnew < 0) {encnew = 60;}
          switch (timeview)
          {
          case 0:
            timestring = (String)encnew + ":" + (String)holdmin[activeprogram] + ":" + (String)holdsec[activeprogram];
            break;
          case 1:
            timestring = (String)holdhr[activeprogram] + ":" + (String)encnew + ":" + (String)holdsec[activeprogram];
            break;
          case 2:
            timestring = (String)holdhr[activeprogram] + ":" + (String)holdmin[activeprogram] + ":" + (String)encnew;
            break;
          default:
            break;
          }
          cursor(0,1);
          write(timestring);
          switch(timeview) {
            case 0:
              if(encnew < 10) {cursor(0,1);} else {cursor(1,1);}
              blink();
              break;
            case 1:
              if(encnew < 10) {cursor(2,1);} else {cursor(4,1);}
              blink();
              break;
            case 2:
              if(encnew < 10) {cursor(4,1);} else {cursor(7,1);}
              blink();
              break;
          }
          encold = encnew;
        }
        break;
      case 4:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Motor Run Time:");
          cursor(0,1);
          writenum(encnew);
          blink();
          encold = encnew;
        }
        break;
      case 5:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Motor Rev?");
          cursor(0,1);
          if(encnew == 0){
            write("No");
          } else {
            write("Yes");
          }
          blink();
          encold = encnew;
        }
        if(encnew > 1){enc.write(0);} else if(encnew < 0){enc.write(1);}
        break;
      case 6:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Motor Rev Wait:");
          cursor(0,1);
          writenum(encnew);
          blink();
          encold = encnew;
        }
      case 7:
        if(encnew != encold){
          clear();
          cursor(0,0);
          write("Save Changes?");
          cursor(0,1);
          if(encnew == 0){
            write("No");
          } else {
            write("Yes");
          }
          blink();
          encold = encnew;
        }
        if(encnew > 1){enc.write(0);} else if(encnew < 0){enc.write(1);}
        break;
      default:
        view = 0;
        blinkoff();
        break;
    }

  if(checkok()){
    clear();
    blinkoff();
    encold = -1;
    switch (view) {
      case 0:
        rapidtemp[activeprogram] = encnew;
        view++;
        break;
      case 1:
        risingtemp[activeprogram] = encnew;
        view++;
        break;
      case 2:
        risingres[activeprogram] = encnew;
        view++;
        break;
      case 3:
        switch (timeview)
        {
        case 0:
          holdhr[activeprogram] = encnew;
          timeview++;
          break;
        case 1:
          holdmin[activeprogram] = encnew;
          timeview++;
          break;
        case 2:
          holdsec[activeprogram] = encnew;
          timeview = 0;
          view++;
          break;
        default:
          break;
        }
        break;
      case 4:
        motorruntime[activeprogram] = encnew;
        view++;
        break;
      case 5:
        repeatmotor[activeprogram] = encnew;
        view++;
        break;
      case 6:
        motordirwait[activeprogram] = encnew;
        view++;
        break;
      case 7:
        if(encnew == 1){
          save();
        }
        edit = false;
        view = 0;
        menu = 1;
        break;
      default:
        break;
      }
      } 
    }
  }
}





