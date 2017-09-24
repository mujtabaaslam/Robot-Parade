

    /***********************************************************************
     * Name(s)  Derek Wang Mike Zou Mujtaba Aslam                          *
     * Box(s):  4737        4642       3093                                *
     * Assignment name Project module 111: Robot Parade                    *
     * Assignment for December 9th, 2016                                   *
     ***********************************************************************/

    /* *********************************************************************
     * Academic honesty certification:                                     *
     *   Written/online sources used:     csc 161 labs and readings        *
     *                                    Derek Wang dance project file    *
     *   Help obtained                    none                             *
     *   My/our signature(s) below confirms that the above list of sources *
     *   is complete AND that I/we have not talked to anyone else          *
     *   (e.g., CSC 161 students) about the solution to this problem       *
     *                                                                     *
     *   Signature:                                                        *
     ***********************************************************************/


#include <stdio.h> 
#include "MyroC.h"
#include <ctype.h>
#include <string.h>
#include "scale-notes.h"
#include "time.h"

//song plays a 2 second song
void song();

//loopCommand goes through a line of infile and runs the correct command, no log
int loopCommand(FILE * infile);

//logCommand goes through a line of infile and runs the correct command
// then it logs the command and timestamp and sensor brightness into logFile
int logCommand(FILE * infile, FILE * logFile);

int main(int argc, char *argv[])
{
  //connect to robot
  rConnect("/dev/rfcomm0");




  
  //START UP
  //when a line of input is inputed, the robot will move forward slowly until
  // a second of input is inputed.
  
  //option is used to store the user input of commands
  char option[80];
  printf ("Enter character and press enter:  ");
  fgets(option, 80, stdin);
  rMotors(.25, .25);
  printf("Enter another character: ");
  fgets(option, 80, stdin);
  rMotors(0, 0);


  //PARADE

  //this are the inpute file and the log file
  FILE * infile = stdin;
  FILE * logfile = NULL;

  //declaring values to assist in read the command line
  int count = 1;
  //this will be used as a boolean for input and log
  int input = 0;
  int log = 0;
  char * inputFile;
  char * lgFile;

  //this while loop goes through all of the command line inputs except the first one
  // it checks if the input is -input or -log and reassigns the values accordingly
  while(count < argc)
    {
      if(strcmp(argv[count], "-input") == 0)
        {
          input = 1;
          inputFile = argv[count + 1];
          count++;
        }
      else if(strcmp(argv[count], "-log") == 0)
        {
          log = 1;
          lgFile = argv[count+1];
          count++;
        }
      else
        count++;
       
    }
  
  //if -input has been entered, but not -log, loopCommand is executed and the commands
  // in inputFile are executed. nothing is logged
  if(input && (log == 0))
    {
      infile = fopen (inputFile, "r");
      while(1)
        {
          int a = loopCommand(infile);
          if(a == 0)
            return 0;
        }
      fclose(infile);
    }

  //if -input and -log were entered, logCommand is executed and the commands
  // in inputFile are execute and then logged into logfile
  else if(input && log)
    {
      infile = fopen (inputFile, "r");
      logfile = fopen (lgFile, "w");

      while(1)
        {
          int a = logCommand(infile, logfile);
          if(a == 0)
            return 0;
        }
      fclose(infile);
      fclose(logfile);
    }

  //if -input and -log were not inputed, then the program prints out the
  // available commands and then requires the user to input commands into the terminal
  // it executes commands until quit is entered
  else if(infile == stdin && (log == 0))
    {
      //these printf statements print the avaliable commands
      printf ("quit- robot move forward for 10 seconds, then terminates\n");
      printf ("forward <speed> <duration>\n");
      printf ("turnLeft <speed> <duration>\n");
      printf ("turnRight <speed> <duration>\n");
      printf ("beep <duration> <frequency>\n");
      printf ("song <repetitions> — a 2-second song\n");
      while(1)
        {
          int a = loopCommand(infile);
          if(a == 0)
            return 0;
        }
    }

  //if -log was inputed but not -input, the program prints out all
  // available commands and then requires the user to input commands into the terminal
  // after each command, each command is logged into logfile
  // it executes until quit is entered
  else if(infile == stdin && log)
    {
      logfile = fopen (lgFile, "w");
      //these printf statements print the avaliable commands
      printf ("quit- robot move forward for 10 seconds, then terminates\n");
      printf ("forward <speed> <duration>\n");
      printf ("turnLeft <speed> <duration>\n");
      printf ("turnRight <speed> <duration>\n");
      printf ("beep <duration> <frequency>\n");
      printf ("song <repetitions> — a 2-second song\n");
      while(1)
        {
          int a = logCommand(infile, logfile);
          if(a == 0)
            return 0;
        }
      fclose(logfile);
    }
}



//plays mario song
void song()
{
  rBeep(.125, pitchE6);
  rBeep(.250, pitchE6);
  rBeep(.125, pitchE6);
  rBeep(.125, 0); 
  rBeep(.125, pitchC6);
  rBeep(.250, pitchE6);
  rBeep(.500, pitchG6);
  rBeep(.500, pitchG5);
  
}

//takes in an open file and reads a line in the file
// it executes a command
// it returns 0 if quit was the command, 1 otherwise
// the 0 is used to stop the main program
 int loopCommand(FILE * infile)
 {
   //declaring strings
   char command[80];
   char x[80];

   //this takes a string from infile and puts into command
   fscanf(infile, "%s", command);

   //if command is quit, then the robot moves forward ten seconds
   // then returns 0
   if(strcmp(command, "quit") == 0)
     {
       rForward(0.25, 10);
       return 0;
     }
   //if command is forward, reads two doubles from the file
   // then moves robot forward
   // then returns 1
   else if(strcmp(command, "forward") == 0)
     {
       double speed, duration;
       fscanf(infile, "%lf %lf", &speed, &duration);
       rForward(speed, duration);
       fgets(x, 80, infile);
       return 1;
     }

   //if command is turnleft, reads two doubles from the file
   // then turns robot left
   // then returns 1
   else if(strcmp(command, "turnLeft") == 0) 
     {
       double speed, duration;
       fscanf(infile, "%lf %lf", &speed, &duration);
       rTurnLeft(speed, duration);
       fgets(x, 80, infile);
       return 1;
     }
   //if command is turnright, reads two doubles from the file
   // then turns robot right
   // then returns 1
   else if(strcmp(command, "turnRight") == 0)
     {
       double speed, duration;
       fscanf(infile, "%lf %lf", &speed, &duration);
       rTurnRight(speed, duration);
       fgets(x, 80, infile);
       return 1;
     }
   //if command is beep, reads two doubles from the file
   // then beeps the robot
   // then returns 1
   else if(strcmp(command, "beep") == 0)
     {
       double duration, frequency;
       fscanf(infile, "%lf %lf", &duration, &frequency);
       rBeep(duration, frequency);
       fgets(x, 80, infile);
       return 1;
     }

   //if command is song, reads an int from the file
   // plays a song
   // then returns 1
   else if(strcmp(command, "song") == 0)
     {
       int repetition;
       fscanf(infile, "%d", &repetition);
       for (int i = 0; i < repetition; i++)
         song();
       fgets(x, 80, infile);
       return 1;
     }
   //moves the readings to the next line
   else
     {
       fgets(x, 80, infile);
       return 1;
     }
 }

//takes in an open file and reads a line in the file
// it executes a command
// it returns 0 if quit was the command, 1 otherwise
// the 0 is used to stop the main program
int logCommand(FILE * infile, FILE * logFile)
{
  //declaring strings
  char command[80];
  char x[80];

  ////this takes a string from infile and puts into command
  fscanf(infile, "%s", command);
  
  /* determine time and print it */
  time_t mytime = time (NULL);
  char * timeStr = ctime (&mytime);
  
  /*remove '\n' at end of time string, if present */
  if (timeStr[strlen(timeStr)-1] == '\n')
    {
      timeStr[strlen(timeStr)-1] = 0;
    }
  int sensor[3];

  //if command is quit, then the robot moves forward ten seconds
  // then logs the command, time stamp and sensor values into logFile
  // then returns 0
  
  if(strcmp(command, "quit") == 0)
    {
      fprintf(logFile, "%s", command);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      rForward(0.25, 10);
      return 0;
    }
  //if command is forward, reads two doubles from the file
  // then moves robot forward
  // then logs the command, time stamp and sensor values into logFile
  // then returns 1
  else if(strcmp(command, "forward") == 0)
    {
      double speed, duration;
      fscanf(infile, "%lf %lf", &speed, &duration);
      rForward(speed, duration);
      fprintf(logFile, "%s %lf %lf", command, speed, duration);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      fgets(x, 80, infile);
      return 1;
    }

  //if command is turnLeft, reads two doubles from the file
  // then turns robot left
  // then logs the command, time stamp and sensor values into logFile
  // then returns 1
  else if(strcmp(command, "turnLeft") == 0) 
    {
      double speed, duration;
      fscanf(infile, "%lf %lf", &speed, &duration);
      rTurnLeft(speed, duration);
      fprintf(logFile, "%s %lf %lf", command, speed, duration);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      fgets(x, 80, infile);
      return 1;
    }

  //if command is turnRight, reads two doubles from the file
  // then turns robot right
  // then logs the command, time stamp and sensor values into logFile
  // then returns 1
  else if(strcmp(command, "turnRight") == 0)
    {
      double speed, duration;
      fscanf(infile, "%lf %lf", &speed, &duration);
      rTurnRight(speed, duration);
      fprintf(logFile, "%s %lf %lf", command, speed, duration);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      fgets(x, 80, infile);
      return 1;
    }
  
  //if command is beep, reads 2 doubles from the file
  // then beeps the robot
  // then logs the command, time stamp and sensor values into logFile
  // then returns 1
  else if(strcmp(command, "beep") == 0)
    {
      double duration, frequency;
      fscanf(infile, "%lf %lf", &duration, &frequency);
      rBeep(duration, frequency);
      fprintf(logFile, "%s %lf %lf", command, duration, frequency);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      fgets(x, 80, infile);
      return 1;
    }

  //if command is song, reads one integer from the file
  // then plays a song
  // then logs the command, time stamp and sensor values into logFile
  // then returns 1
  else if(strcmp(command, "song") == 0)
    {
      int repetition;
      fscanf(infile, "%d", &repetition);
      for (int i = 0; i < repetition; i++)
        song();
      fprintf(logFile, "%s %d", command, repetition);
      fprintf (logFile, " %s ", timeStr);
      rGetLightsAll(sensor, 5);
      fprintf(logFile, "(%d, %d, %d)\n", sensor[0], sensor[1], sensor[2]);
      fgets(x, 80, infile);
      return 1;
    }
  //moves the readings into the next line
  else
    {
      fgets(x, 80, infile);
      return 1;
    }
}
