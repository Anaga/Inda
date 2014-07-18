#include <QString>
#include <QtTest>
#include "arduino/MFC_Sim/massflowcontroller.h"

class MFC_Test : public QObject
{
   Q_OBJECT

public:
   MFC_Test();
   MassFlowController * mfc;
   ComunicParser * comPar;

private Q_SLOTS:
   void initTestCase();
   void cleanupTestCase();
   void testStartupValues();
   void testStartupValues_data();

   void testSetSetPoint();
   void testSetSetPoint_data();

   void testSetValvePos();
   void testSetValvePos_data();

   void testSetGasNumber();
   void testSetGasNumber_data();

   void testSetDeviceId();
   void testSetDeviceId_data();

   void testSetTemp();
   void testSetTemp_data();

   void testSetPresure();
   void testSetPresure_data();

 /*
  * comunication parser
  */

   void testSetStatusRequest();
   void testSetStatusRequest_data();

   void testSetTekstSetPoint();
   void testSetTekstSetPoint_data();
};

MFC_Test::MFC_Test()
{
}

void MFC_Test::initTestCase()
{
   mfc = NULL;
   comPar = NULL;
   qDebug() << "Try to create MassFlowController object";
   mfc = new MassFlowController;
   QVERIFY (mfc!=NULL);

   qDebug() << "Try to create ComunicParser object";
   comPar = new ComunicParser;
   QVERIFY (comPar!=NULL);
   comPar->setController(mfc);
}

void MFC_Test::cleanupTestCase()
{
   qDebug() << "Delete ComunicParser object";
   delete comPar;

   qDebug() << "Delete MassFlowController object";
   delete mfc;
}

void MFC_Test::testStartupValues()
{
   outputType type;
   QString message;
   QString sTestValue;
   char *pCh = NULL;

   QFETCH(int, calFunc);
   QFETCH(QString, expectedVal);

   type = (outputType) calFunc;

   switch (type){
   case ot_massa:
      pCh = mfc->getMassFlowRate();
      message = "MassFlowRate is %1";
      break;
   case ot_volume:
      pCh = mfc->getVoluFlowRate();
      message = "VolumeFlowRate is %1";
      break;
   case ot_setPoint:
      pCh = mfc->getSetPoint();
      message = "SetPoint is %1";
      break;
   case ot_temp:
      pCh = mfc->getTemp();
      message = "Temp is %1";
      break;
   case ot_presure:
      pCh = mfc->getPresure();
      message = "Presure is %1";
      break;
   case ot_device:
      pCh = mfc->getDeviceId();
      message = "Devise ID is %1";
      break;
   case ot_gas:
      pCh = mfc->getGasName();
      message = "Gas Name is %1";
      break;

   default:
      message = "Conv Error %1";

   }
   message = message.arg(pCh);

   sTestValue = QString::fromLocal8Bit(pCh);
   QCOMPARE(sTestValue, expectedVal);
}

void MFC_Test::testStartupValues_data()
{
   QTest::addColumn<int>("calFunc");
   QTest::addColumn<QString>("expectedVal");

   QTest::newRow("Device ID")             << (int)ot_device    << "A";
   QTest::newRow("Gas name")              << (int)ot_gas       << "Air";
   QTest::newRow("Temperature in C")      << (int)ot_temp      << "+032.75";
   QTest::newRow("Incoming gas presure")  << (int)ot_presure   << "+013.60";
   QTest::newRow("Set Point")             << (int)ot_setPoint  << "0.000";
   QTest::newRow("Volumetric flow rate")  << (int)ot_volume    << "+00.000";
   QTest::newRow("Mass flow rate")        << (int)ot_massa     << "+00.000";
}

void MFC_Test::testSetSetPoint()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   char *pInputCh = NULL;
   bool retVal;

   QFETCH(QString, inputValue);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedSetPoint);

   pInputCh = inputValue.toLocal8Bit().data();

   retVal = mfc->setSetPoint(pInputCh,inputValue.length());
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getSetPoint();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedSetPoint);
}

void MFC_Test::testSetSetPoint_data()
{
   QTest::addColumn<QString>("inputValue");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedSetPoint");

   QTest::newRow("Set SP to A.BC")  << "A.BC" << false<< "0.000";
   QTest::newRow("Set SP to 0")     << "0.00" << true << "0.000";
   QTest::newRow("Set SP to 0.04")  << "0.04" << true << "0.040";
   QTest::newRow("Set SP to 4.54")  << "4.54" << true << "4.540";
   QTest::newRow("Set SP to 4.5")   << "4.5"  << false<< "4.540";
   QTest::newRow("Set SP to 4.1x")  << "4.1x" << false<< "4.540";
}

void MFC_Test::testSetValvePos()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   bool retVal;

   QFETCH(int, inputValue);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedFlowRate);
   QFETCH(QString, expectedMassRate);

   retVal = mfc->setValvePos(inputValue);
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getVoluFlowRate();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedFlowRate);

   pOutputCh = mfc->getMassFlowRate();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedMassRate);
}

void MFC_Test::testSetValvePos_data()
{
   QTest::addColumn<int>("inputValue");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedFlowRate");
   QTest::addColumn<QString>("expectedMassRate");

   QTest::newRow("Set valve to -50%")  << -50  << false << "+00.000" << "+00.000";
   QTest::newRow("Set valve to 150%")  << 150  << false << "+00.000" << "+00.000";
   QTest::newRow("Set valve to 0%"  )  << 0    << true  << "+00.000" << "+00.000";
   QTest::newRow("Set valve to 100%")  << 100  << true  << "+10.000" << "+10.000";
   QTest::newRow("Set valve to 50%")   << 50   << true  << "+05.000" << "+05.000";
   QTest::newRow("Set valve to 5%")    << 5    << true  << "+00.500" << "+00.500";

}

void MFC_Test::testSetGasNumber()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   bool retVal;

   QFETCH(int, inputValue);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedGasName);
   QFETCH(int, inputValvePos);
   QFETCH(QString, expectedFlowRate);
   QFETCH(QString, expectedMassRate);

   retVal = mfc->setGasNumber(inputValue);
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getGasName();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedGasName);


   retVal = mfc->setValvePos(inputValvePos);

   pOutputCh = mfc->getVoluFlowRate();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedFlowRate);

   pOutputCh = mfc->getMassFlowRate();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedMassRate);
}

void MFC_Test::testSetGasNumber_data()
{
   QTest::addColumn<int>("inputValue");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedGasName");
   QTest::addColumn<int>("inputValvePos");
   QTest::addColumn<QString>("expectedFlowRate");
   QTest::addColumn<QString>("expectedMassRate");

   QTest::newRow("Set gas to Air, valve to 0%"  )  << 0  << true  << "Air" << 0 <<"+00.000" << "+00.000";

   QTest::newRow("Set gas to C2H6, valve to 0%"  )  << 5 << true  << "C2H6" << 0  <<"+00.000" << "+00.000";
   QTest::newRow("Set gas to C2H6, valve to 10%"  ) << 5 << true  << "C2H6" << 10 <<"+01.000" << "+01.279";

   QTest::newRow("Set gas to Argon, valve the same") << 1 << true  << "Ar"   << 10 <<"+01.000" << "+01.784";
   QTest::newRow("Set gas to Methane"              ) << 2 << true  << "CH4"  << 10 <<"+01.000" << "+00.716";
   QTest::newRow("Set gas to Carbon Monoxide"      ) << 3 << true  << "CO"   << 10 <<"+01.000" << "+00.300";
   QTest::newRow("Set gas to Carbon Dioxide"       ) << 4 << true  << "CO2"  << 10 <<"+01.000" << "+01.976";
   QTest::newRow("Set gas to Ethane"               ) << 5 << true  << "C2H6" << 10 <<"+01.000" << "+01.279";
   QTest::newRow("Set gas to Hydrogen"             ) << 6 << true  << "H2"   << 10 <<"+01.000" << "+00.089";
   QTest::newRow("Set gas to Helium"               ) << 7 << true  << "He"   << 10 <<"+01.000" << "+00.146";
   QTest::newRow("Set gas to Nitrogen"             ) << 8 << true  << "N2"   << 10 <<"+01.000" << "+01.251";

   QTest::newRow("Set gas to outside of table"     ) << 9 << false << "N2"   << 10 <<"+01.000" << "+01.251";

}

void MFC_Test::testSetDeviceId()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   bool retVal;

   QFETCH(char, newDevId);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedDeviceId);

   retVal = mfc->setDeviceId(newDevId);
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getDeviceId();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedDeviceId);
}

void MFC_Test::testSetDeviceId_data()
{
   QTest::addColumn<char>("newDevId");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedDeviceId");

   QTest::newRow("Set deviceId to A")  << 'A'  << true  << "A";
   QTest::newRow("Set deviceId to 1")  << '1'  << false << "A";
   QTest::newRow("Set deviceId to a")  << 'a'  << false << "A";
   QTest::newRow("Set deviceId to B")  << 'B'  << true  << "B";
   QTest::newRow("Set deviceId to H")  << 'H'  << true  << "H";
   QTest::newRow("Set deviceId to N")  << 'N'  << true  << "N";
}

void MFC_Test::testSetTemp()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   char *pInputCh = NULL;
   bool retVal;

   QFETCH(QString, newTemp);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedTemp);

   pInputCh = newTemp.toLocal8Bit().data();

   retVal = mfc->setTemp(pInputCh,newTemp.length());
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getTemp();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedTemp);
}

void MFC_Test::testSetTemp_data()
{
   QTest::addColumn<QString>("newTemp");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedTemp");

   QTest::newRow("Set newTemp to 0.0")  << "00.00"  << true  << "+000.00";

   QTest::newRow("Set newTemp to AB.BC")  << "AB.BC" << false<< "+000.00";
   QTest::newRow("Set newTemp to 0")      << "00.00" << true << "+000.00";
   QTest::newRow("Set newTemp to 0.04")   << "00.04" << true << "+000.04";
   QTest::newRow("Set newTemp to 4.54")   << "04.54" << true << "+004.54";
   QTest::newRow("Set newTemp to 4.5")    << "04.5"  << false<< "+004.54";
   QTest::newRow("Set newTemp to 14.1")   << "14.10" << true << "+014.10";
}

void MFC_Test::testSetPresure()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   char *pInputCh = NULL;
   bool retVal;

   QFETCH(QString, newPresure);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedPresure);

   pInputCh = newPresure.toLocal8Bit().data();

   retVal = mfc->setPresure(pInputCh,newPresure.length());
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getPresure();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedPresure);
}

void MFC_Test::testSetPresure_data()
{
   QTest::addColumn<QString>("newPresure");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedPresure");

   QTest::newRow("Set newPresure to 0.0")  << "00.00"  << true  << "+000.00";

   QTest::newRow("Set newPresure to AB.BC")  << "AB.BC" << false<< "+000.00";
   QTest::newRow("Set newPresure to 0")      << "00.00" << true << "+000.00";
   QTest::newRow("Set newPresure to 0.04")   << "00.04" << true << "+000.04";
   QTest::newRow("Set newPresure to 4.54")   << "04.54" << true << "+004.54";
   QTest::newRow("Set newPresure to 14.05")  << "14.05" << true << "+014.05";
   QTest::newRow("Set newPresure to 43.21")  << "43.21" << true << "+043.21";
}

void MFC_Test::testSetStatusRequest()
{
    QString sTestValue;
    char *pOutputCh = NULL;
    char *pInputCh = NULL;
    bool retVal;

    QFETCH(QString, newInputValue);
    QFETCH(bool, expectedBool);
    QFETCH(QString, expectedValue);

    pInputCh = newInputValue.toLocal8Bit().data();
    retVal = comPar->parseInputRow(pInputCh, newInputValue.length());
    QCOMPARE(retVal, expectedBool);

    pOutputCh = comPar->getOutputRow();
    sTestValue = QString::fromLocal8Bit(pOutputCh);
    QCOMPARE(sTestValue, expectedValue);
}

void MFC_Test::testSetStatusRequest_data()
{
    QTest::addColumn<QString>("newInputValue");
    QTest::addColumn<bool>("expectedBool");
    QTest::addColumn<QString>("expectedValue");

    QTest::newRow("Set input to com parser to long string")  << "00.00aaadgshrthrgbsfghwrtbdfbthrtyrtyue5uethye"  << false  << "";
    QTest::newRow("Set input to com parser to 'A'")  << "A"  << false << "";
    QTest::newRow("Set input to com parser to 'N'")  << "N"  << true  << "N +043.21 +014.10 +01.000 +01.251 4.540 N2";
}

void MFC_Test::testSetTekstSetPoint()
{
   QString sTestValue;
   char *pOutputCh = NULL;
   char *pInputCh = NULL;
   bool retVal;

   QFETCH(QString, inputValue);
   QFETCH(bool, expectedBool);
   QFETCH(QString, expectedValue);

   pInputCh = inputValue.toLocal8Bit().data();

   retVal = comPar->parseInputRow(pInputCh, inputValue.length());
   QCOMPARE(retVal, expectedBool);

   pOutputCh = mfc->getSetPoint();
   sTestValue = QString::fromLocal8Bit(pOutputCh);
   QCOMPARE(sTestValue, expectedValue);
}

void MFC_Test::testSetTekstSetPoint_data()
{
   QTest::addColumn<QString>("inputValue");
   QTest::addColumn<bool>("expectedBool");
   QTest::addColumn<QString>("expectedValue");

   QTest::newRow("Set input to com parser to ASA.BC")  << "ASA.BC" << false<< "4.540";
   QTest::newRow("Set input to com parser to NS0.00")  << "NS0.00" << true << "0.000";
   QTest::newRow("Set input to com parser to NS0.04")  << "NS0.04" << true << "0.040";
   QTest::newRow("Set input to com parser to NS4.54")  << "NS4.54" << true << "4.540";
   QTest::newRow("Set input to com parser to BS5.67")  << "BS5.67" << false<< "4.540";
   QTest::newRow("Set input to com parser to NS4.1x")  << "NS4.1x" << false<< "4.540";
   QTest::newRow("Set input to com parser to NS5.67")  << "NS5.67" << true << "5.670";
   QTest::newRow("Set input to com parser to N 1.23")  << "N 1.23" << false<< "5.670";
   QTest::newRow("Set input to com parser to NS1.23")  << "NS1.23" << true << "1.230";
   QTest::newRow("Set input to com parser to NS5.3")   << "NS5.3"  << false<< "1.230";
}


QTEST_APPLESS_MAIN(MFC_Test)

#include "tst_mfc_test.moc"
