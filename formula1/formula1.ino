#include <LiquidCrystalRus.h>
LiquidCrystalRus lcd( 6, 7, 2, 3, 4, 5);

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
char title_string1[20]={' ',' ',' ',' ',' ','F','O','R','M','U','L','A',' ',' ',' ',' ',' ',' ',' ',' '};
char title_string2[20]={' ',' ',' ',' ',' ',' ',' ',' ','A','R','D','U','I','N','O',' ',' ',' ',' ',' '};
char title_message1[]="steer to start";

int titleStringsAnims[3]={0,0,0};

int animPosChars[12]{5,5,4,4,3,3,2,2,1,1,0,0};



 
void setup()
{
  lcd.begin(20, 4); 
  for (int i=0;i<=5;i++){
    lcd.createChar(i, carFrames[i]);
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
           lcd.write(title_string1[i]);
        }

        // ARDUINO
        
        if (titleStringsAnims[0]>0){
          lcd.setCursor(19-titleStringsAnims[0],1);
          lcd.write(byte(255));
        }
        for (int i=19-titleStringsAnims[0];i<20;i++){
           lcd.setCursor(i+1,1);
           lcd.write(title_string2[i+1]);
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
    drawRoad();
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

  drawRoad();

  if (acceleration<accelerationPoint){
    acceleration+=0.005;
  }



  steeringVal = analogRead(analogPin); 
  carPos=32-int(steeringVal/32);
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

void drawRoad(){
  int curs=0;
  for (int i=26-blocksCursor;i>=9-blocksCursor;i--){
    for (int j=0;j<=3;j++){
      lcd.setCursor((curs),j); 
      if (roadArray[i][j]>0){
        // Check using special roadblocks from cistom blocks array
        int roadBlock=0;
        if (roadArray[i][j]<9){
          roadBlock=roadBlocks[roadArray[i][j]];
        }else{
          roadBlock=roadArray[i][j];
        }
        lcd.write(byte(roadBlock));
        if (curs==0){
          checkColisiion(j,roadArray[i][j]);
        }
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
    switch (lastPatternResolveredPattern) {
      case 1:
         memcpy(currPattern, pattern1, sizeof(pattern1));
         lastPatternResolveredPattern=pattern1_resolver[random(0, (sizeof(pattern1_resolver) / sizeof(int)))];
        break;
      case 2:
         memcpy(currPattern, pattern2, sizeof(pattern2));
         lastPatternResolveredPattern=pattern2_resolver[random(0, (sizeof(pattern2_resolver) / sizeof(int)))];
        break;
      case 3:
         memcpy(currPattern, pattern3, sizeof(pattern3));
         lastPatternResolveredPattern=pattern3_resolver[random(0, (sizeof(pattern3_resolver) / sizeof(int)))];
        break;
      case 4:
         memcpy(currPattern, pattern4, sizeof(pattern4));
         lastPatternResolveredPattern=pattern4_resolver[random(0, (sizeof(pattern4_resolver) / sizeof(int)))];
        break;
      case 5:
         memcpy(currPattern, pattern5, sizeof(pattern5));
         lastPatternResolveredPattern=pattern5_resolver[random(0, (sizeof(pattern5_resolver) / sizeof(int)))];
        break;
      case 6:
         memcpy(currPattern, pattern6, sizeof(pattern6));
         lastPatternResolveredPattern=pattern6_resolver[random(0, (sizeof(pattern6_resolver) / sizeof(int)))];
        break;
      case 7:
         memcpy(currPattern, pattern7, sizeof(pattern7));
         lastPatternResolveredPattern=pattern7_resolver[random(0, (sizeof(pattern7_resolver) / sizeof(int)))];
        break;
      case 8:
         memcpy(currPattern, pattern8, sizeof(pattern8));
         lastPatternResolveredPattern=pattern8_resolver[random(0, (sizeof(pattern8_resolver) / sizeof(int)))];
        break;
      case 9:
         memcpy(currPattern, pattern9, sizeof(pattern9));
         lastPatternResolveredPattern=pattern9_resolver[random(0, (sizeof(pattern9_resolver) / sizeof(int)))];
        break;
      case 10:
         memcpy(currPattern, pattern10, sizeof(pattern10));
         lastPatternResolveredPattern=pattern10_resolver[random(0, (sizeof(pattern10_resolver) / sizeof(int)))];
        break;
      case 11:
         memcpy(currPattern, pattern11, sizeof(pattern11));
         lastPatternResolveredPattern=pattern11_resolver[random(0, (sizeof(pattern11_resolver) / sizeof(int)))];
        break;
      case 12:
         memcpy(currPattern, pattern12, sizeof(pattern12));
         lastPatternResolveredPattern=pattern12_resolver[random(0, (sizeof(pattern12_resolver) / sizeof(int)))];
        break;
      case 13:
         memcpy(currPattern, pattern13, sizeof(pattern13));
         lastPatternResolveredPattern=pattern13_resolver[random(0, (sizeof(pattern13_resolver) / sizeof(int)))];
        break;
      case 14:
         memcpy(currPattern, pattern14, sizeof(pattern14));
         lastPatternResolveredPattern=pattern14_resolver[random(0, (sizeof(pattern14_resolver) / sizeof(int)))];
        break;
      case 15:
         memcpy(currPattern, pattern15, sizeof(pattern15));
         lastPatternResolveredPattern=pattern15_resolver[random(0, (sizeof(pattern15_resolver) / sizeof(int)))];
        break;
      default:
         memcpy(currPattern, pattern1, sizeof(pattern1));
         lastPatternResolveredPattern=pattern1_resolver[random(0, (sizeof(pattern1_resolver) / sizeof(int)))];
        break;
    }
    
    
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



void checkColisiion(int tileNum, int roadBlock){
   bool collision=false;
    
    if (carTilesFilled[tileNum]!=99){
      collision=true;
      if (roadBlock>9){
        collision=false;
      }else{
        for (int i=0;i<(sizeof(safeBlocks) / sizeof(int));i++){
          if (roadBlock!=safeBlocks[i]){
            collision=true;  
          }else{
            collision=false;  
          }
        }

      }
    }      

   if (collision){
    //DEAD!
    deadTile=tileNum;
    state=11; 
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
