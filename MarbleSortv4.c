#pragma config(Sensor, in1,    sensorLine,     sensorLineFollower)
#pragma config(Sensor, in2,    sensorLight,    sensorReflection)
#pragma config(Sensor, in3,    potLine,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  sensorEncoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, button1,        sensorTouch)
#pragma config(Sensor, dgtl12, button2,        sensorTouch)
#pragma config(Motor,  port1,           colorFlashLight, tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port5,           lineMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           tickServo,     tmotorServoStandard, openLoop)

//VARLIBES AND CONSTANTS:
	int stage = 1;
	int ACTIONTIME = 4000;
	int PLANNINGAVG;

	int MOTORPOWER = 21;

	int SERVOLETIN = -110;
	int SERVOLETOUT = 110;

	int timeOfMyLife;

	int solidGlassTol = 0;
	int solidGlassI = 0;
	int solidGlassAvg = 0;
	int solidGlassFin = 0;
	int LINESOLID = 2500;
	int LINETRESH = 50;
	int marbleTypeLine;

	int solidGlassSolid;
	int solidGlassGlass;

	int POTFALLRIGHT = 650;
	int POTFALLLEFT = 3200;
	int POTNEUTRAL= 1900;

	int SEAVALUE = 20;


//FUNCTIONS:
	void planReset(){
		//solidGlassPLan:
			solidGlassTol = 0;
			solidGlassI = 0;
			solidGlassAvg = 0;
			solidGlassFin = 0;
			LINESOLID = 2200;
			LINETRESH = 50;
	}

	void solidGlassPlanSea(){
		if(solidGlassFin == 0){
			if(SensorValue(sensorLine) < LINESOLID + LINETRESH){
				solidGlassSolid ++;
				solidGlassGlass = 0;
			}
			if(SensorValue(sensorLine) > LINESOLID - LINETRESH){
				solidGlassSolid = 0;
				solidGlassGlass ++;
			}
			if(solidGlassGlass >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 2;
			}
			if(solidGlassSolid >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 1;
			}
		}
	}

	void colorGlassPlanSea(){
		if(solidGlassFin == 0){
			if(SensorValue(sensorLine) < LINESOLID + LINETRESH){
				solidGlassSolid ++;
				solidGlassGlass = 0;
			}
			if(SensorValue(sensorLine) > LINESOLID - LINETRESH){
				solidGlassSolid = 0;
				solidGlassGlass ++;
			}
			if(solidGlassGlass >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 2;
			}
			if(solidGlassSolid >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 1;
			}
		}
	}

	void servoAction(){
		if(time1(T1) > ((3 * ACTIONTIME)/4) && time1(T1) < ACTIONTIME){
			startMotor(tickServo,SERVOLETIN);
		}
		if(time1(T1) >= ACTIONTIME){
			startMotor(tickServo,SERVOLETOUT);
		}
	}

	void solidGlassAction(){
		if((time1(T1) > ((2 * ACTIONTIME)/4)) && (time1(T1) < ((3 * ACTIONTIME)/4))){
			if(marbleTypeLine == 1){
				if(SensorValue(potLine) > POTFALLRIGHT){
					startMotor(lineMotor,MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
			if(marbleTypeLine == 2){
				if(SensorValue(potLine) < POTFALLLEFT){
					startMotor(lineMotor,-MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
		}
		if(time1(T1) > (3 * ACTIONTIME)/4){
			if(SensorValue(potLine) < POTNEUTRAL + 100){
				startMotor(lineMotor,-MOTORPOWER);
			}
			if(SensorValue(potLine) > POTNEUTRAL - 100){
				startMotor(lineMotor,MOTORPOWER);
			}
			if((SensorValue(potLine) < POTNEUTRAL + 100) && (SensorValue(potLine) > POTNEUTRAL - 100)){
				startMotor(lineMotor,0);
			}
		}
	}

	void colorGlassAction(){
		if((time1(T1) > ((2 * ACTIONTIME)/4)) && (time1(T1) < ((3 * ACTIONTIME)/4))){
			if(marbleTypeLine == 1){
				if(SensorValue(potLine) > POTFALLRIGHT){
					startMotor(lineMotor,MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
			if(marbleTypeLine == 2){
				if(SensorValue(potLine) < POTFALLLEFT){
					startMotor(lineMotor,-MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
		}
		if(time1(T1) > (3 * ACTIONTIME)/4){
			if(SensorValue(potLine) < POTNEUTRAL + 100){
				startMotor(lineMotor,-MOTORPOWER);
			}
			if(SensorValue(potLine) > POTNEUTRAL - 100){
				startMotor(lineMotor,MOTORPOWER);
			}
			if((SensorValue(potLine) < POTNEUTRAL + 100) && (SensorValue(potLine) > POTNEUTRAL - 100)){
				startMotor(lineMotor,0);
			}
		}

	}

task main(){
	while(true){
		timeOfMyLife = time1(T1);
		if(stage == 1){
			solidGlassPlanSea();
			colorGlassPlanSea();
			clearTimer(T1);
			if(solidGlassFin == 1){
				stage = 2;
				solidGlassFin = 0;
			}

		}
		if(stage == 2){
			servoAction();
			solidGlassAction();
			colorGlassAction();
			if(time1(T1) > ACTIONTIME + 1000){
				startMotor(lineMotor,0);
				solidGlassFin = 0;
				stage = 1;
			}

		}
	}
}