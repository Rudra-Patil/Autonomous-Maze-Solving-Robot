//declaring all the pins and to which we need to connect to the Arduino
const int trig1 = 3; // front right
const int echo1 = 4;
const int trig2 = 8; // right 
const int echo2 = 9;
const int trig3 = 7;// front left
const int echo3 = 2;
const int m1_1 = 13;
const int m1_2 = 12;
const int m2_1 = 11;
const int m2_2 = 10;
const int en_1 = 6;
const int en_2 = 5;

int left_sat = 0;
int center_sat = 0;
int right_sat = 0;

long dur1;
int dist1;
long dur2;
int dist2,dist3,dist,dur;

// ultra is a function defined to calculate the distance between the robot and the wall in front
int ultra(int trig,int echo){
  digitalWrite(trig,LOW);
  delay(10);
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  dur=pulseIn(echo,HIGH);//pulseIN checks the duration of the pulse by calculating the time it takes to go from HIGH to LOW. 
  dist=dur*0.034/2;//calculating the distance between robot and wall based on the speed of sound.
  return dist;// returns distance
}

void StopMotor()// function defined to stop all motors by setting LOW as input.
{
  digitalWrite(m1_1,LOW);
  digitalWrite(m1_2,LOW);
  digitalWrite(m2_1,LOW);
  digitalWrite(m2_2,LOW);
  delay(100);
}

void left(int PwmC = 100){ // function defined to turn left by making a point turn by turning motor on left side to LOW and on right to HIGH.
  digitalWrite(m1_1,HIGH);
  digitalWrite(m1_2,LOW);
  digitalWrite(m2_1,LOW);
  digitalWrite(m2_2,HIGH);
  analogWrite(en_1,PwmC);
  analogWrite(en_2,PwmC);
}
void right(int PwmC = 100){// function defined to right left by making a point turn by turning motor on left side to HIGH and on right to LOW.
  digitalWrite(m1_1,LOW);
  digitalWrite(m1_2,HIGH);
  digitalWrite(m2_1,HIGH);
  digitalWrite(m2_2,LOW);
  analogWrite(en_1,PwmC);
  analogWrite(en_2,PwmC);
}
void forward(int PwmC = 100){ // function for moving forward
  digitalWrite(m1_1,HIGH);
  digitalWrite(m1_2,LOW);
  digitalWrite(m2_1,HIGH);
  digitalWrite(m2_2,LOW);
  analogWrite(en_1,PwmC);
  analogWrite(en_2,PwmC);
  //Serial.print(dist1);
}
void setup(){
  Serial.begin(9600);
  pinMode(trig1,OUTPUT);// trigger and echo pins are set for the 3 ultrasonic sensors to send and recieve data.
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(trig3,OUTPUT);
  pinMode(echo3,INPUT);
  pinMode(m1_1,OUTPUT);// motors output is initialized. 
  pinMode(m1_2,OUTPUT);
  pinMode(m2_1,OUTPUT);
  pinMode(m2_2,OUTPUT);
  pinMode(en_1,OUTPUT);
  pinMode(en_2,OUTPUT); //putyoursetupcodehere,torunonce:
}

bool Align(){
  int d1 = ultra(trig3,echo3),d2=ultra(trig1,echo1); // aligning the robot so that it moves in a straight line and corrects itself if found moving slanting
//  if((d1 > 40) || (d2 > 40))
//    return false;
  if(d1>d2)
  {  
    while(d1>d2)    //Turn Right untill Alignment 
    {
      right(100);
      d1 = ultra(trig3,echo3);
      d2=ultra(trig1,echo1);
    }
    return true;
  }
  else if(d2>d1)  // Turn left untill Alignmnet
  {
    while(d2>d1)
    {
      
      left(100);
      d1 = ultra(trig3,echo3);
      d2 = ultra(trig1,echo1);
    }
    return true;
  }
  return false;
  
}

void loop(){
  dist1 = ultra(trig1,echo1); // to check the distances between the robot and the wall
  dist2 = ultra(trig2,echo2);
  dist3 = ultra(trig3,echo3);
  Serial.print("\ndist1=");
  Serial.print(dist1);
  Serial.print("\ndist2=");
  Serial.print(dist2);
  Serial.print("\ndist3=");
  Serial.print(dist3);
  
  forward(150);
  
  if(((dist1<12)||(dist3<12))&&(dist2>20)){ //TurnRight
    StopMotor();
    right_sat=1;
  }
  
  else if(((dist1<12)||(dist3<12))&&(dist2<20)){ //TurnLeft
    StopMotor();
  left_sat=1;}
  
  if(left_sat==1){ // Given a delay after every turn so that it can align itself properly before moving ahead
    delay(500);
    left(100);
    delay(1550);
    left_sat=0;
    Align();
    StopMotor();
    Serial.println("Alignment Done");
  }
  else if(right_sat==1){
    delay(500);
    right(100);
    delay(1550);
    right_sat=0;
    Align();
    StopMotor();
    Serial.println("Alignment Done");
  }
}


