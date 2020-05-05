/**
 * This H file holds all global defined static values/ parametrs
 */



// should we run in debug momde? in debug we print more messages to serial / USB
#define DEBUG_ENABLED 1

// how long (in milliseonds) between each time we tx a beacon to the air...
#define BEACON_INTEVAL 2000

// how long (in milliseonds) between each time we get telematics data and store in SD
#define TELEMATICS_INTEVAL 10000

// how much delay time in main loop
#define MAIN_DELAY 100

// max amount of bytes we allow in a buffer
#define BUFFER_SIZE 32 // more than 32 we run into issues !!
