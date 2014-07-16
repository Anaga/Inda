#include "massflowcontroller.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/***** Alicat Standard Gas Select List **********
//
//
//    Air      Air
//    Ar       Argon
//    CH4      Methane
//    CO       Carbon Monoxide
//    CO2      Carbon Dioxide
//    C2H6     Ethane
//    H2       Hydrogen
//    He       Helium
//    N2       Nitrogen
//    N2O      Nitrous Oxide
//    Ne       Neon
//    O2       Oxygen
//    C3H8     Propane
//    n-C4H10  normal-Butane
//    C2H2     Acetylene
//    C2H4     Ethylene
//    i-C2H10  iso-Butane
//    Kr       Krypton
//    Xe       Xenon
//    SF6      Sulfur Hexafluoride
//    C-25     75% Argon / 25% CO2
//    C-10     90% Argon / 10% CO2
//    C-8      92% Argon / 8% CO2
//    C-2      98% Argon / 2% CO2
//    C-75     75% CO2 / 25% Argon
//    A-75     75% Argon / 25% Helium
//    A-25     75% Helium / 25% Argon
//    A1025    90% Helium / 7.5% Argon / 2.5% CO2  (Praxair - Helistar® A1025)
//    Star29   90% Argon / 8% CO2 / 2% Oxygen      (Praxair - Stargon® CS)
//    P-5      95% Argon / 5% Methane
*/

const gasTable gas_table[] =
{
   {"Air" , 1.0},
   {"Ar"  , 1.784},
   {"CH4" , 0.7168},
   {"CO"  , 0.301},
   {"CO2" , 1.9768},
   {"C2H6", 1.28},
   {"H2"  , 0.089},
   {"He"  , 0.147},
   {"N2"  , 1.251},
};

#define GAS_TABLE_LENGTH (sizeof(gas_table) / sizeof(gasTable))

MassFlowController::MassFlowController()
{
   maxVolFlowRate = 10;
   valvePos = 0;
   presure = 1360;
   temp = 3275;
   setPoint = 0;

   volFlowRate= 0;
   masFlowRate= 0;

   deviceId[0]= 'A';
   deviceId[1]= 0;

   gasName[0] = 'A';
   gasName[1] = 'i';
   gasName[2] = 'r';
   gasName[3] = 0;

   gasNumber = 0;
}

char *MassFlowController::convertIntToString(const int iVal, const outputType type)
{
   int  intPart;
   int  devPart;

   intPart = iVal / 1000;
   devPart = iVal % 1000;

   switch (type){

   case ot_massa:
   case ot_volume:
      sprintf(chRow, "%+03d.%03d",intPart, abs(devPart));
      break;

   case ot_setPoint:
      sprintf(chRow, "%d.%03d",intPart, abs(devPart));
      break;

   case ot_temp:
   case ot_presure:
      intPart = iVal / 100;
      devPart = iVal % 100;
      sprintf(chRow, "%+04d.%02d",intPart, abs(devPart));
      break;

   default:
      sprintf(chRow, "Conv Error");
   }
   return chRow;
}

bool MassFlowController::setSetPoint(const char *input, const int length)
{
   if ( length > 5) return false;

   if ( isCharNotDigit(input[0])) return false;
   if ( input[1] != '.') return false;
   if ( isCharNotDigit(input[2])) return false;
   if ( isCharNotDigit(input[3])) return false;
   if ( input[4] != 0) return false;

   int  intPart;
   int  devPart;

   intPart = input[0] - '0';
   devPart = (input[2] - '0') * 10 + (input[3] - '0');

   setPoint = intPart*1000 + devPart * 10;
   return true;
}

unsigned int MassFlowController::convertValvePosToFlow()
{
   return ( ( maxVolFlowRate * valvePos )*10 );
}

unsigned int MassFlowController::convertValvePosToMass()
{
   float gasDens = gas_table[gasNumber].gasDens;
   int flowRate = convertValvePosToFlow() ;
   return (int) ( flowRate* gasDens);
}

bool MassFlowController::setValvePos(const unsigned int pos)
{
   if (pos > 100) return false;

   valvePos = pos;
   volFlowRate = convertValvePosToFlow();
   masFlowRate = convertValvePosToMass();

   return true;
}

bool MassFlowController::setGasNumber(const unsigned int gasNewNumber)
{
   if (gasNewNumber >= GAS_TABLE_LENGTH) return false;
   gasNumber = gasNewNumber;
   sprintf(gasName, "%s", gas_table[gasNumber].gasName);
   masFlowRate = convertValvePosToMass();

   return true;
}

bool MassFlowController::setDeviceId(const char newDevId)
{
   if (newDevId < 'A') return false;
   if (newDevId > 'Z') return false;
   deviceId[0] = newDevId;
   return true;
}

bool MassFlowController::setPresure(const char *input, const int length)
{
   if ( length > 6) return false;
   if ( isCharNotDigit(input[0])) return false;
   if ( isCharNotDigit(input[1])) return false;
   if ( input[2] != '.') return false;
   if ( isCharNotDigit(input[3])) return false;
   if ( isCharNotDigit(input[4])) return false;
   if ( input[5] != 0) return false;

   int  intPart;
   int  devPart;

   intPart = (input[0] - '0') * 10 + (input[1] - '0');
   devPart = (input[3] - '0') * 10 + (input[4] - '0');

   presure = intPart*100 + devPart;

   return true;
}

bool MassFlowController::setTemp(const char *input, const int length)
{
   if ( length > 6) return false;
   if ( isCharNotDigit(input[0])) return false;
   if ( isCharNotDigit(input[1])) return false;
   if ( input[2] != '.') return false;
   if ( isCharNotDigit(input[3])) return false;
   if ( isCharNotDigit(input[4])) return false;
   if ( input[5] != 0) return false;

   int  intPart;
   int  devPart;

   intPart = (input[0] - '0') * 10 + (input[1] - '0');
   devPart = (input[3] - '0') * 10 + (input[4] - '0');

   temp = intPart*100 + devPart;
   return true;
}

ComunicParser::ComunicParser()
{
    inputputRow[0] = 0;
    outputRow[0] = 0;
    mfc = NULL;
}

bool ComunicParser::parseInputRow(const char *input, const int length)
{
   if (length >= MAX_ROW_LENGTH) return false;

   outputRow[0] = 0;

   if ((length == 1) && (input[0] == mfc->getDeviceId()[0])){
       sprintf(outputRow, "%s", mfc->getDeviceId());
       sprintf(outputRow, "%s %s", outputRow, mfc->getGasName());
       sprintf(outputRow, "%s %s", outputRow, mfc->getMassFlowRate());
       sprintf(outputRow, "%s %s", outputRow, mfc->getPresure());
       sprintf(outputRow, "%s %s", outputRow, mfc->getSetPoint());
       sprintf(outputRow, "%s %s", outputRow, mfc->getTemp());
       sprintf(outputRow, "%s %s", outputRow, mfc->getVoluFlowRate());
       return true;
   }
   return false;
}
