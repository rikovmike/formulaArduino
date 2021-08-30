#include <LiquidCrystalRus.h>
LiquidCrystalRus lcd( 12, 13, 8, 9, 10, 11);

#include "f1_carSetup.h"
#include "f1_roadSetup.h"



int gscore=0;

int state=4;

float distanceMonemtum=0;
float accelerationPoint=0.8;
float acceleration=0;
int distanceBlocks=0;
int blocksCursor=0;

int analogPin = A3;

// car setup
int carPos=0;
int maxCarPos=26;
int steeringVal=0;
int carTilesFilled[4]={99,99,99,99};
int deadTile=0;


// TITLE data
// Too lazy for it. Setting FULL 20-char strings ((
const char title_string1[20] PROGMEM = {' ',' ',' ',' ',' ','F','O','R','M','U','L','A',' ',' ',' ',' ',' ',' ',' ',' '};
const char title_string2[20] PROGMEM = {' ',' ',' ',' ',' ',' ',' ',' ','A','R','D','U','I','N','O',' ',' ',' ',' ',' '};
const char title_message1[] = "steer to start"; // Not nessersary to flash this string. Code to read it will be more painful, than few bytes of RAM, used by it. I think...

int titleStringsAnims[3]={0,0,0};

int animPosChars[12]{5,5,4,4,3,3,2,2,1,1,0,0};

String ingameMessage="";
int ingameMessageTime=0;
 
void setup()
{
  pinMode(7, OUTPUT);

  
  lcd.begin(20, 4); 
  for (int i=0;i<=5;i++){
    byte tmpChar[8];
    for (int j=0;j<8;j++){
      tmpChar[j]=pgm_read_byte_near(carFrames[i]+j);
    }
    lcd.createChar(i, tmpChar);
  }


}
 
void loop()
{

  switch(state){
    case 4:
      statePreStartScreen();
      break;    
    case 5:
      stateStartScreenAnim();
      break;    
    case 6:
      stateStartScreen();
      break;  
    case 7:
      statePostStartScreen();
      break;  
    case 10:
      stateGame();
      break;    
    case 11:
      stateDie();
      break;    
    case 90:
      stateGameOver();
      break;    
    default:
      stateStartScreen();
      break;    
    }

}




void statePreStartScreen(){

    titleStringsAnims[0]=0;
    titleStringsAnims[1]=1;
    titleStringsAnims[2]=0;
    gscore=0;
    state=5;
    memcpy(roadArray, roadArrayInit, sizeof(roadArrayInit));
    distanceBlocks=0;
    blocksCursor=0;
    distanceMonemtum=0;
    lastPatternResolveredPattern=1;
    ingameMessageTime=0;
}


void stateStartScreenAnim(){
    if (titleStringsAnims[0]<20){
        titleStringsAnims[0]++;

        // FORMULA
        
        if (titleStringsAnims[0]<19){
          lcd.setCursor(titleStringsAnims[0],0);
          lcd.write(byte(255));
        }
        for (int i=0;i<titleStringsAnims[0];i++){
           lcd.setCursor(i,0);
           lcd.write(pgm_read_byte_near(&title_string1[i]));
        }

        // ARDUINO
        
        if (titleStringsAnims[0]>0){
          lcd.setCursor(19-titleStringsAnims[0],1);
          lcd.write(byte(255));
        }
        for (int i=19-titleStringsAnims[0];i<20;i++){
           lcd.setCursor(i+1,1);
           lcd.write(pgm_read_byte_near(&title_string2[i+1]));
        }
        
        delay(50);
    }else{

        state=6;                
    }
  
}


void stateStartScreen(){
  int sVal=0;
     
  titleStringsAnims[1]++;

  if (titleStringsAnims[1]>0&&titleStringsAnims[1]<=1){
     lcd.setCursor(3,3);
     lcd.print(title_message1);
  }
  if (titleStringsAnims[1]>1&&titleStringsAnims[1]<=2){
     lcd.setCursor(0,3);
     lcd.print("                    ");
  }
  
  if (titleStringsAnims[1]>2){
    titleStringsAnims[1]=0;
  }
  
  sVal = analogRead(analogPin); 
  
  delay(320);
  
  if (abs(analogRead(analogPin)-sVal)>100){
    state=7;  
  }

}


void ffxAnimateScreenWave(int sybmol=255){
  
    for (int i=-5;i<26;i++){
      //1st string
      if (i>=0&&i<=19){
        for (int j=0;j<4;j++){
          lcd.setCursor(i,j);
          lcd.write(byte(sybmol));
        }

      }
      delay(10);
    }

    for (int i=-5;i<26;i++){
      //1st string
      if (i>=0&&i<=19){
        for (int j=0;j<4;j++){
          lcd.setCursor(i,j);
          lcd.print(" ");
        }

      }
      delay(10);
    }
}


// Trying to use in-state loop. Maybe it's fun ))
void statePostStartScreen(){
    ffxAnimateScreenWave();
    state=10;
}



void stateDie(){
    acceleration=0;
    drawRoad(false);
    char rndChar[9]={'*','*','*','+','+','.','.','.'};
    for (int i=0;i<9;i++){
      lcd.setCursor(i,deadTile);
      lcd.write(rndChar[i]);
      lcd.setCursor(i,deadTile-(int(round(float(i)/4))));
      lcd.write(rndChar[int(float(i)+1.4)]);
      lcd.setCursor(i,deadTile-(int(round(float(i)/1))));
      lcd.write(rndChar[int(float(i)+1.6)]);
      lcd.setCursor(i,deadTile+(int(round(float(i)/4))));
      lcd.write(rndChar[int(float(i)+1.4)]);
      lcd.setCursor(i,deadTile+(int(round(float(i)/1))));
      lcd.write(rndChar[int(float(i)+1.6)]);

      delay(0+i*10);
    }
    state=90;
}




void stateGameOver(){
  lcd.setCursor(4,1);
  lcd.print(" GAME  OVER ");
  delay(1300);
  ffxAnimateScreenWave(42);
  state=4;
}



void stateGame(){

  drawRoad(true);
  ffxIngameMessage();

  if (acceleration<accelerationPoint){
    acceleration+=0.005;
  }

  tone(7, 50+100*acceleration, 50+100*acceleration);

  steeringVal = analogRead(analogPin); 
  carPos=32-int(round(steeringVal/32));
  if (carPos>maxCarPos){carPos=maxCarPos;}

 
  for (int i=0;i<=3;i++){
    carTilesFilled[i]=99;
    checkCarTile(i);
  }

  // Mover
  distanceMonemtum+=acceleration;
  if (distanceMonemtum>1){
    distanceMonemtum=0;
    distanceBlocks++;
    blocksCursor++;
    roadMoveStep();
  }
  drawScore();
}

void drawRoad(bool checkCollisions){
  int curs=0;
  for (int i=26-blocksCursor;i>=9-blocksCursor;i--){
    for (int j=0;j<=3;j++){
      lcd.setCursor((curs),j); 
      if (roadArray[i][j]>0){
        // Check using special roadblocks from custom blocks array
        int roadBlock=0;
        if (curs==0&&checkCollisions){
          int col=checkColisiion(j,roadArray[i][j]);
          // If we get collision with powerup, remove powerup from pattern
          if (col==1){
            roadArray[i][j]=0;
            roadBlock=0;
          }
        }
        if (roadArray[i][j]<9){
          roadBlock=roadBlocks[roadArray[i][j]];
        }else{
          roadBlock=roadArray[i][j];
        }
        lcd.write(byte(roadBlock));
      }else{
        if (curs!=0){
          lcd.write(' ');
        }else{
         if (carTilesFilled[j]==99){
          lcd.write(' ');
         }
        }
      }
    }
    curs++;
  }
}





void roadMoveStep(){

  gscore+=1;
  if (blocksCursor>9){
    for (int i=0;i<9;i++){
      for (int j=0;j<4;j++){
        currPattern[i][j]=pgm_read_byte_near(&patterns[lastPatternResolveredPattern-1][i][j]);
        
        // If epmty space, spawn random powerup:
        if (currPattern[i][j]==0){
          if (random(0,100)==9){
            currPattern[i][j]=bonusBlocks[random(0,2)];
          }
        }
      }
    }

    lastPatternResolveredPattern=pgm_read_byte_near(&pattern_resolvers[lastPatternResolveredPattern-1][random(0, 7)]);

    blocksCursor=0;

    for (int i=17;i>=0;i--){
      for (int j=0;j<=3;j++){
        roadArray[i+9][j]=roadArray[i][j];
      }
    }

    for (int i=0;i<=8;i++){
      for (int j=0;j<=3;j++){
        roadArray[i][j]=currPattern[i][j];
      }
    }
  }

}



void checkCarTile(int tileNum){
  int myCoord=tileNum*8;
    lcd.setCursor(0,tileNum);  
    lcd.write(" ");
  int myCoordPos=myCoord-carPos;
  
  if (myCoordPos<=4&&myCoordPos>=-7){
    carTilesFilled[tileNum]=animPosChars[((myCoord-carPos)+7)];
    lcd.setCursor(0,tileNum);
    lcd.write(byte(carTilesFilled[tileNum]));

  }
}



int checkColisiion(int tileNum, int roadBlock){
   bool collision=true,iamsafe=false,iambonus=false;
   int ret=0;
   
   
   
   if (carTilesFilled[tileNum]!=99){
    for (int i=0;i<(sizeof(safeBlocks) / sizeof(int));i++){
      if (roadBlock==safeBlocks[i]){
        iamsafe=true;  
      }
    }

    for (int i=0;i<(sizeof(bonusBlocks) / sizeof(int));i++){
      if (roadBlock==bonusBlocks[i]){
        iambonus=true;  
      }
    }

    

      if (roadBlock==0||roadBlock>9||iamsafe||iambonus){
        collision=false;
      }

      Serial.println(roadBlock);
  
      if (iambonus){
        ffxLaunchPowerup(tileNum,roadBlock);
        ret=1;
      }
  
     if (collision){
      //DEAD!
      deadTile=tileNum;
      state=11; 
     }
    
    
  }      

  return ret;

}


// Powerup effect. Pauses game for short time to show animation.
void ffxLaunchPowerup(int posy,int powrpTile){
    if (powrpTile==5){
      gscore+=500;
      ingameMessage="+500$";
    }
    if (powrpTile==6){
      accelerationPoint+=accelerationPoint/100;
      ingameMessage="CHARGE!";
    }

    ingameMessageTime=30;
    
}



// Ingame message drawer
void ffxIngameMessage(){
  int startX=0;
  int txtSize=0;
  if (ingameMessageTime>0)  {
    ingameMessageTime--;
    Serial.println(ingameMessageTime);
    startX=9-(ingameMessage.length())/2;
    lcd.setCursor(startX,1);
    lcd.print(ingameMessage);
  }
  
}


void drawScore(){
  lcd.setCursor(18,0);
  lcd.print("$K");
  lcd.setCursor(18,1);
  lcd.print(ExtractDigit(gscore,3));
  lcd.print(ExtractDigit(gscore,6));
  lcd.setCursor(18,2);
  lcd.print(ExtractDigit(gscore,2));
  lcd.print(ExtractDigit(gscore,5));
  lcd.setCursor(18,3);
  lcd.print(ExtractDigit(gscore,1));
  lcd.print(ExtractDigit(gscore,4));
}


int ExtractDigit(int v, int p)
{
  return int(v/(pow(10,p-1))) - int(v/(pow(10,p)))*10; 
}


void templateReverse(int arrTmpl[9][4]){
  int bufsize=4;
  for (int j=0;j<9;j++){
      int buf[4];
      for (int i = 0; i < 4; i++) {
        buf[i] = arrTmpl[j][i];
      }
      for (int i = 0; i < 4; i++) {
        arrTmpl[j][i] = buf[3 - i];
      }
  }
}
