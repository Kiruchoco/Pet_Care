#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>

#define us_trigPin 1	// GPIO	21
#define us_echoPin 29	// GPIO	18
#define pump 21		// BCM_GPIO	5
#define dhtPin 11

#define LEDBAR 26	//BCM_GPIO 12
#define DCMOTOR 23	//BCM_GPIO 13
#define BUZCONTROL 28	//BCM_GPIO 20
#define FAN 22		//BCM_GPIO 6

#define MAXTIMINGS 85

int ret_humid, ret_temp;
int h = 1, l = 0;

static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read);
int read_dht22_dat();

void sig_handler(int signo);	// escape function

int main()
{
	// Timer parameter
	char buf_timer[255];	// timer buffer
	FILE *fp;
	time_t timer;
	struct tm* t;

	// ultra & water pump parameter
	int distance = 0;
	int pulse = 0;
	int received_temp;

	long startTime;
	long travelTime;

	char buf[255];

	//setup Pin
	if(wiringPiSetup()==-1)
	{
		fprintf(stdout,"Unable to start wiringPi : %s\n",strerror(errno));
		return 1;
	}

	if(setuid(getuid()) < 0){
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGINT, (void*)sig_handler);		// escape function
	
	pinMode(LEDBAR,OUTPUT);			// led pin setup
	pinMode(DCMOTOR,OUTPUT);		// dcmotor led pin setup
	pinMode(BUZCONTROL,OUTPUT);		// buzcontorl pin setup
	pinMode(FAN,OUTPUT);			// fan pin setup

	// temp & humid pin setup
	pinMode(us_trigPin, OUTPUT);
	pinMode(us_echoPin, INPUT);
	pinMode(pump, OUTPUT);			// pump pin setup

	while(1)
	{
		//print time
		timer=time(NULL);		//set time
		t=localtime(&timer);
		printf("%d : %d : %d\n",t->tm_hour, t->tm_min, t->tm_sec);		//print present time
		
		//read_server time data
		sprintf(buf_timer,"curl -s '192.168.137.1/read_server.php?d=timer' ");
		printf("%s\n",buf_timer);		// read alarm time
	
		if((fp=popen(buf_timer,"r"))==NULL){
			return 1;
		}

		while(fgets(buf_timer,255,fp)!=NULL){
			printf("%s\n",buf_timer);
		}

		//calc time	
		char *ptr=strtok(buf_timer,":");
		int alarm_hour=atoi(ptr);		// get hour
		ptr=strtok(NULL,":");
		int alarm_min=atoi(ptr);		// get minutes
		printf("%d : %d\n",alarm_hour,alarm_min);
		
		//check time
		if((t->tm_hour==alarm_hour)&&(t->tm_min==alarm_min))		//compare hour & min
		{
			printf("alarm timer\n");
			
			digitalWrite(LEDBAR,1);		// LED on
			digitalWrite(DCMOTOR,1);	// DCMOTOR on
			digitalWrite(BUZCONTROL,1);	// BUZCONTROL on
			//digitalWrite(FAN,1);
		}
		else
		{
			printf("do not alarm\n");

			digitalWrite(LEDBAR,0);		// LED off
			digitalWrite(DCMOTOR,0);	// DCMOTOR off
			digitalWrite(BUZCONTROL,0);	// BUZCONTROL off
			//digitalWrite(FAN,0);
		}
		
		// ultraviolet sensor
		digitalWrite(us_trigPin, LOW);	
		usleep(2);
		digitalWrite(us_trigPin, HIGH);
		usleep(20);
		digitalWrite(us_trigPin, LOW);
		while(digitalRead(us_echoPin) == LOW);
		startTime = micros();
		while(digitalRead(us_echoPin) == HIGH);
		travelTime = micros() - startTime;
		int distance = travelTime / 58;
		printf("Distance : %dcm\n", distance);
		sprintf(buf, "curl '192.168.137.1/getvalue.php?d=ultrasonic&v=%u' ", distance);
		system(buf);	// write ultrasonic
		delay(100);

		read_dht22_dat();		//reading humid & temp

		if(distance > 10){
			printf("PUMP ON\n");
			digitalWrite(pump, 1);	// Pump on
			delay(100);
			sprintf(buf, "curl '192.168.137.1/getvalue.php?d=pump&v=%u' ", h);
			system(buf);	// write pump
			delay(1000);

			continue;
		} else {
			printf("PUMP OFF\n");
			digitalWrite(pump, 0);	// Pump off
			delay(100);
			sprintf(buf, "curl '192.168.137.1/getvalue.php?d=pump&v=%u' ", l);
			system(buf);	// write pump
			delay(1000);

			continue;
		}

		//delay(10000);	//10 seconds	
		delay(60000);	//60 seconds = 1 minutes
	}
	
	pclose(fp);

	return 0;
}

void sig_handler(int signo)
{
	printf("Exit Process\n");

	digitalWrite(pump, 0);
	digitalWrite(LEDBAR, 0);
	digitalWrite(DCMOTOR, 0);
	//digitalWrite(BUZCONTROL, 0);
    	digitalWrite(FAN,0);

	exit(0);
}

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

int read_dht22_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  char buf1[100];

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(dhtPin, OUTPUT);
  digitalWrite(dhtPin, HIGH);
  delay(10);
  digitalWrite(dhtPin, LOW);
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(dhtPin, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(dhtPin, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(dhtPin)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(dhtPin));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
		
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
		
		ret_humid = (int)h;
		ret_temp = (int)t;
		//printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
		printf("Humidity = %d Temperature = %d\n", ret_humid, ret_temp);
		delay(100);
		sprintf(buf1, "curl '192.168.137.1/getvalue.php?d=temp&v=%u' ", ret_temp);
		system(buf1);
		delay(100);
		sprintf(buf1, "curl '192.168.137.1/getvalue.php?d=humid&v=%u' ", ret_humid);
		system(buf1);	
	
    return 1;
  }
  else
  {
    printf("Data not good, skip\n");
    return 0;
  }
}
