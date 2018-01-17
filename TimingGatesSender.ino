    #include "Wire.h"
    #include <Sparkfun_APDS9301_Library.h>
    #include <VirtualWire.h>

    APDS9301 apds;

    #define INT_PIN 2 // We'll connect the INT pin from our sensor to the
                      // INT0 interrupt pin on the Arduino.
    bool lightIntHappened = false; // flag set in the interrupt to let the
                      //  mainline code know that an interrupt occurred.


char *controller;
    
    void setup() 
    {
      delay(5);    // The CCS811 wants a brief delay after startup.
      Serial.begin(115200);
      Wire.begin();

vw_set_ptt_inverted(true); //
vw_set_tx_pin(12);
vw_setup(4000);// speed of data transfer Kbps
    
      // APDS9301 sensor setup.
      apds.begin(0x39);  // We're assuming you haven't changed the I2C
                         //  address from the default by soldering the
                         //  jumper on the back of the board.
      apds.setGain(APDS9301::LOW_GAIN); // Set the gain to low. Strictly
                         //  speaking, this isn't necessary, as the gain
                         //  defaults to low.
      apds.setIntegrationTime(APDS9301::INT_TIME_13_7_MS); // Set the
                         //  integration time to the shortest interval.
                         //  Again, not strictly necessary, as this is
                         //  the default.
      apds.setLowThreshold(5); // Sets the low threshold to 0, effectively
                         //  disabling the low side interrupt.
      apds.setHighThreshold(50000); // Sets the high threshold to 500. This
                         //  is an arbitrary number I pulled out of thin
                         //  air for purposes of the example. When the CH0
                         //  reading exceeds this level, an interrupt will
                         //  be issued on the INT pin.
      apds.setCyclesForInterrupt(1); // A single reading in the threshold
                         //  range will cause an interrupt to trigger.
      apds.enableInterrupt(APDS9301::INT_ON); // Enable the interrupt.
      apds.clearIntFlag();

      // Interrupt setup
      pinMode(INT_PIN, INPUT_PULLUP); // This pin must be a pullup or have
                         //  a pullup resistor on it as the interrupt is a
                         //  negative going open-collector type output.
      attachInterrupt(digitalPinToInterrupt(2), lightInt, FALLING);
      Serial.println(apds.getLowThreshold());
      Serial.println(apds.getHighThreshold());
    }
    
    void loop() 
    {
      static unsigned long outLoopTimer = 0;
      apds.clearIntFlag();       
         if(lightIntHappened)
         {
              controller="0" ;
              vw_send((uint8_t *)controller, strlen(controller));
              vw_wait_tx(); // Wait until the whole message is gone
              Serial.println("Sent message");
              lightIntHappened = false;
              delay(2000);
         }
         
      ////
      
    }
    
    void lightInt()
    {
      lightIntHappened = true;
    }

