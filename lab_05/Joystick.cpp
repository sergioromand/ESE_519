//Joystick (INCOMPLETE IMPLIMENTATION)
//Author: Carter Sharer 
//Date: 10/18/16

#include "Joystick.h"
#include "mbed.h"

  //float      val_pos_beforescaling;
 // float      val_neg_beforescaling;


//Member function definitions including constructor
Joystick::Joystick(PinName pinA, PinName pinB) : horiz(pinA), vert(pinB) {
    //Wait 1 second for voltage to settle
    wait(1); 
    printf("yes\n");
    //(1)Set Raw Center Values, this is where the joystick sits naturaly 
    //set raw horizontal center to current value (average with 5 readings)
    //set raw vertial center to current value (average with 5 readings) 
    
    raw_hc = 0;
    raw_vc = 0; 
    
    for (int i=0; i<5; i++) {
        raw_hc += horiz.read();
        raw_vc += vert.read();
        }
        
    raw_hc = raw_hc / 5 ;
    raw_vc = raw_vc / 5 ;



    
    //(2)Initalize the Raw Max to some value less then the real max value.  
    //We dont know what the max value will be until we read it, thats ok. 
    //But we can assume it will be greater then 0.8 (center + 0.3) so lets 
    //set it to that for now.  We will update this later if we see a larger value
    //Now do the same for the the Raw Min
    float delta = 0.3;
    rawMinH = raw_hc - delta;
    rawMaxH = raw_hc + delta;
    rawMinV = raw_vc - delta;
    rawMaxV = raw_vc + delta;
}

//Returns the scaled vertial value of joystick
float Joystick::horizontal(void) {
    //(3)Get average val (5 samples)
    float avg = horiz.read();
    
    for (int i=0; i<4; i++) {
        avg += horiz.read();
        }
    avg = avg / 5;
    
    //(4)Watch for Max and Min Values, if we see a new max/min update Raw Max/Min
    
    if ( avg < rawMinH ){
        rawMinH = avg;
        }
    if ( avg > rawMaxH ){
        rawMaxH = avg;
        }
        
    
    //(5)Here we will calculate the total range (Travel) of the joystick 
    //using the rawMax/rawMin values we have seen thus far
    //Since the joystick is not symetrical we have to calculate two ranges
    //Calculate the range from [center, max] and [center, min]
    float Hrange_pos = rawMaxH - raw_hc;
    float Hrange_neg = raw_hc - rawMinH;
    
    //(6)Here we will calculate how much our current reading is in one 
    //of the ranges, this will give us a percentage of our _max value we 
    //set in setScale. Then we can apply the scale by multiplying it by our 
    //scale (_max/_min). 
    float val;
    
    if(avg >= raw_hc) {     //Positive Range
        val = (avg-raw_hc) / Hrange_pos; 
        //val_pos_beforescaling = val;
        val = val * _max;
        }
    else if (avg < raw_hc) {         //Negative Range
        val = (raw_hc - avg) / Hrange_neg;
        //val_neg_beforescaling = val;
        val = val * _min;
        } 
    
         
    //(7)Here we will apply a dead zone.  If the |value| is <= our deadzone then 
    //set it to 0.  Otherwise we need to shift the value closer to 0 by dead zone
     
    
    if ((val >= -.25) && (val <= .2)) {
        val = 0;
        }
    else {
        if (val >= 0) {
            val -= .2;
        }
        else if (val < 0) {
            val += .25;
        }
    }
    
    if (val>100) {
        val = 100;
        }
    if (val<-100) {
        val = -100;
        }
    
    //printf("H reading = %f  raw_hc = %f  rawMinH = %f  rawMaxh = %f  val = %0.7f \r\n", avg, raw_hc, rawMinH, rawMaxH, val);
        
    return val;
}

//(8) Impliment vertial the same as you did for horizontal
//Returns the scaled horizontal value of joystick


float Joystick::vertical(void) {
    //Get average value (5 samples)
   float avg = vert.read();
    
    for (int i=0; i<4; i++) {
        avg += vert.read();
        }
    avg = avg / 5;
    
    //(4)Watch for Max and Min Values, if we see a new max/min update Raw Max/Min
    
    if ( avg < rawMinV ){
        rawMinV = avg;
        }
    if ( avg > rawMaxH ){
        rawMaxV = avg;
        }
    
    //(5)Here we will calculate the total range (Travel) of the joystick 
    //using the rawMax/rawMin values we have seen thus far
    //Since the joystick is not symetrical we have to calculate two ranges
    //Calculate the range from [center, max] and [center, min]
    float Vrange_pos = rawMaxV - raw_vc;
    float Vrange_neg = raw_vc - rawMinV;
    
    //(6)Here we will calculate how much our current reading is in one 
    //of the ranges, this will give us a percentage of our _max value we 
    //set in setScale. Then we can apply the scale by multiplying it by our 
    //scale (_max/_min).
    float val;
    
    if(avg >= raw_vc) {     //Positive Range
        val = (avg-raw_vc) / Vrange_pos; 
        //val_pos_beforescaling = val;
        val = val * _max;
        }
    else if (avg < raw_vc) {         //Negative Range
        val = (raw_vc - avg) / Vrange_neg;
        //val_neg_beforescaling = val;
        val = val * _min;
        } 
    
    //(7)Here we will apply a dead zone.  If the |value| is <= our deadzone then 
    //set it to 0.  Otherwise we need to shift the value closer to 0 by dead zone    
    
    if ((val >= -.3) && (val <= .2)) {
        val = 0;
        }
    else {
        if (val >= 0) {
            val -= .2;
        }
        else if (val < 0) {
            val += .3;
        }
    }
    
    if (val>100) {
        val = 100;
        }
    if (val<-100) {
        val = -100;
        }
    
        //printf("V reading = %f  raw_vc = %f  rawMinV = %f  rawMaxV = %f  val = %0.6f \r\n", avg, raw_vc, rawMinV, rawMaxV, val);

    return val;
}

//Set the Min and Max Values of joystick ex: -100, +100
void Joystick::setScale(float min, float max) {
       _min = min-1; //Add 1 for round off error
       _max = max+1; //Add 1 for round off error
}