#ifndef MASSFLOWCONTROLLER_H
#define MASSFLOWCONTROLLER_H

#define MAX_GAS_NAME_LENGTH 10
#define MAX_ROW_LENGTH 45

enum outputType {
   ot_device   = 1,
   ot_gas      = 2,
   ot_temp     = 3,
   ot_presure  = 4,
   ot_setPoint = 5,
   ot_volume   = 6,
   ot_massa    = 7
};

typedef struct {
   char gasName[MAX_GAS_NAME_LENGTH];
   float gasDens;
} gasTable ;


class MassFlowController
{
   unsigned int maxVolFlowRate;
   unsigned int valvePos;
   unsigned int presure;
   signed int temp;
   unsigned int setPoint;
   unsigned int volFlowRate;
   unsigned int masFlowRate;
   char deviceId[2];
   unsigned int  gasNumber;
   char gasName[MAX_GAS_NAME_LENGTH];
   char chRow[MAX_ROW_LENGTH];

public:
   MassFlowController();
   char * getPresure()     { return convertIntToString(presure, ot_presure);     }
   char * getTemp()        { return convertIntToString(temp, ot_temp);           }
   char * getSetPoint()    { return convertIntToString(setPoint, ot_setPoint);   }
   char * getVoluFlowRate(){ return convertIntToString(volFlowRate, ot_volume);  }
   char * getMassFlowRate(){ return convertIntToString(masFlowRate, ot_massa);   }
   char * getDeviceId()    { return deviceId;    }
   char * getGasName()     { return gasName;     }


   bool setPresure (const char * input, const int length);
   bool setTemp (const char * input, const int length);
   bool setSetPoint (const char * input, const int length);

   bool setValvePos (const unsigned int pos);   
   bool setGasNumber (const unsigned int gasNewNumber);

   bool setDeviceId (const char newDevId);


private:
   char * convertIntToString(const int iVal, const outputType type);

   bool isCharDigit(const char ch) {
      if (ch < '0') return false;
      if (ch > '9') return false;
      return true;
   }

   bool isCharNotDigit(const char ch) { return (!isCharDigit(ch)); }
   unsigned int convertValvePosToFlow(void);
   unsigned int convertValvePosToMass(void);
};

#endif // MASSFLOWCONTROLLER_H
