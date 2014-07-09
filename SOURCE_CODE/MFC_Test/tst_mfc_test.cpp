#include <QString>
#include <QtTest>
#include "arduino/MFC_Sim/massflowcontroller.h"

class MFC_Test : public QObject
{
   Q_OBJECT

public:
   MFC_Test();
   MassFlowController * mfc;

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

};

MFC_Test::MFC_Test()
{
}

void MFC_Test::initTestCase()
{
   mfc = NULL;
   qDebug() << "Try to create MassFlowController object";
   mfc = new MassFlowController;
   QVERIFY (mfc!=NULL);
}

void MFC_Test::cleanupTestCase()
{
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

   QTest::newRow("Set gas to 6, valve to 10%"  )   << 6 << false  << "Air" << 10 <<"+01.000" << "+01.000";

   /* {5  , "C2H6", 1.28}, */
   QTest::newRow("Set gas to C2H6, valve to 0%"  )  << 5 << true  << "C2H6" << 0  <<"+00.000" << "+00.000";
   QTest::newRow("Set gas to C2H6, valve to 10%"  ) << 5 << true  << "C2H6" << 10 <<"+01.000" << "+01.279";
}

QTEST_APPLESS_MAIN(MFC_Test)

#include "tst_mfc_test.moc"
