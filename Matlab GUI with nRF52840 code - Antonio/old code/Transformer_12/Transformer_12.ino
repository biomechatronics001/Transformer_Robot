#include <bluefruit.h>
#include <Servo.h>
#include <math.h>

// Servo related
int minAngleFeedback = 20; // Polulu, feedback readings when angle=0;
int maxAngleFeedback = 740; // Polulu, feedback readings when angle=180;
int halfCycleRange = maxAngleFeedback-minAngleFeedback; // Polulu feedback range when rotating 180 degrees


// OTA DFU service
BLEDfu bledfu;

// Peripheral uart service
BLEUart bleuart;

// Central uart client
BLEClientUart clientUart;
unsigned long time_period = 500;
String ID_name = "ID12"; //change the name
int ID = 12;             //change the ID
int Ledstatus = 0;
int packageLength = 30;
char data_rx_cent[31] = {0};
char data_tx_cent[31] = {0};
char data_tx_phpr[31] = {0};
char data_rx_phpr[31] = {0};
int ServoAngle = ID + 10; // -128~127
int servoAngleFeedback = 0;
int command = 0;
unsigned long cur_time = 0;
unsigned long pre_time = 0;
int length_rx_cent = 0;
int length_rx_phpr = 0;
// Servo related
Servo servo;
double servo_k = 0.0; //used for feedback calibration
double servo_b = 0.0; //used for feedback calibration
int current_angle = 0;
int angle_increment = 1;
int increment_delay = 50;

void setup()
{
  delay(3000);
  Serial.begin(115200);
  Serial.println("Bluefruit52 Dual Role BLEUART Example");
  Serial.println("-------------------------------------\n");
  // Initialize Bluefruit with max concurrent connections as Peripheral = 1, Central = 1
  // SRAM usage required by SoftDevice will increase with number of connections
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin(1, 1);
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName(ID_name.c_str()); //change the name

  // Callbacks for Peripheral
  Bluefruit.Periph.setConnectCallback(prph_connect_callback);
  Bluefruit.Periph.setDisconnectCallback(prph_disconnect_callback);
  Bluefruit.Periph.setConnInterval(6, 12);


  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(cent_connect_callback);
  Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();
  bleuart.setRxCallback(prph_bleuart_rx_callback);
  //bleuart.bufferTXD(1);
  // Init BLE Central Uart Serivce
  clientUart.begin();
  clientUart.setRxCallback(cent_bleuart_rx_callback);


  /* Start Central Scanning
     - Enable auto scan if disconnected
     - Interval = 100 ms, window = 80 ms
     - Don't use Filter only accept bleuart service
     - Use active scan
     - Start(timeout) with timeout = 0 will scan forever (until connected)
  */
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  //Bluefruit.Scanner.filterUuid(bleuart.uuid);
  Bluefruit.Scanner.useActiveScan(true);
  Bluefruit.Scanner.start(0);                   // 0 = Don't stop scanning after n seconds

  // Set up and start advertising
  startAdv();
  cur_time = millis();
  pre_time = cur_time;

  // Servo related
  pinMode(A0, INPUT);
  pinMode(5, OUTPUT);
  servo.attach(A2, 480, 2500);
//  calibrateServo(&servo_k, &servo_b);
  servo_k = -0.25;
  servo_b = 207.47;
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)

     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}
void test2() {
  servo.write(180);
  delay(1000);
  servo.write(175);
  delay(1000);
}
void loop()
{
//  test2();
  // do nothing, all the work is done in callback
  cur_time = millis();
  if ((cur_time - pre_time) > time_period)
  {
    pre_time = cur_time;
    if ( bleuart.notifyEnabled())
    {
      for (int i = 0; i < packageLength; i++)
      {
        data_tx_phpr[i] = data_rx_cent[i];
      }
      double positionReading = analogRead(A0);
//      if (positionReading < minAngleFeedback) {
//        positionReading = minAngleFeedback;
//      } else if (positionReading > maxAngleFeedback) {
//        positionReading = maxAngleFeedback;
//      }
//      double angle = (positionReading-minAngleFeedback)*1.0 / halfCycleRange * 180.0;
//      double temp = analogRead(A0);
//      Serial.printf("A0=%f, angle=%f", temp, angle);
//      Serial.println();
//      servoAngleFeedback = round(angle);
      double servoAngleFeedback = ceil(servo_k * positionReading + servo_b);
      data_tx_phpr[ID] = servoAngleFeedback;
      bleuart.write(data_tx_phpr, packageLength);
    }
  }
}

/*------------------------------------------------------------------*/
/* Peripheral
  ------------------------------------------------------------------*/
void prph_connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char peer_name[32] = { 0 };
  connection->getPeerName(peer_name, sizeof(peer_name));
  Serial.printf("[Prph %d] Connected to %s", ID, peer_name);
  Serial.println();
  // request PHY changed to 2MB
  //connection->requestPHY();
  // request to update data length
  //connection->requestDataLengthUpdate();
  // request mtu exchange
  //connection->requestMtuExchange(247);
  // request connection interval of 7.5 ms
  //connection->requestConnectionParameter(6); // in unit of 1.25
  // delay a bit for all the request to complete
  //delay(1000);
}

void prph_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial.println();
  Serial.printf("[Prph %d] Disconnected", ID);
  Serial.println();
}

void prph_bleuart_rx_callback(uint16_t conn_handle)
{
  (void) conn_handle;
  length_rx_phpr = bleuart.available();
  // Forward data from Mobile to our peripheral
  if(length_rx_phpr==10)
  {bleuart.read(&data_rx_phpr[20], length_rx_phpr);}
  if (length_rx_phpr >= packageLength)
  {
    bleuart.read(data_rx_phpr, packageLength);
    command = (int16_t)data_rx_phpr[ID];
    angle_increment = (int16_t)data_rx_phpr[24];
    increment_delay = (int16_t)data_rx_phpr[25];
    Serial.print("command: ");
    Serial.print(command);
    Serial.print("Length: ");
    Serial.print(packageLength);
    Serial.print("; Command: ");
    for (int i = 0; i < packageLength; i++)
    {
      Serial.print(data_rx_phpr[i], DEC);
      Serial.print(" ");
      data_tx_cent[i] = data_rx_phpr[i];
    }
    Serial.println();
    if ( clientUart.discovered() )
    {
      clientUart.write(data_tx_cent, packageLength);
    }
    else
    {
      Serial.printf("[Prph %d] Central role not connected", ID);
      Serial.println();
    }
    int angleCommand = data_rx_phpr[ID];
    Serial.print("Angle command = ");
    Serial.print(angleCommand);
    Serial.println();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (angleCommand != current_angle){
      int angle_diff = angleCommand - current_angle;
      int sign = 0;
      int temp_angle = current_angle;
      
      if (angle_diff > 0){
        sign = 1;
      }
      else {
        sign = -1;
        angle_diff = -angle_diff;
      }
      
      for (int step = 0; step <= angle_diff; step = step + angle_increment) {
        temp_angle = current_angle + step*sign;
        servo.write(temp_angle);
        delay(increment_delay); // milliseconds
      }
      current_angle = temp_angle;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //servo.write(angleCommand);
    if (Ledstatus == 1)
    {
      Ledstatus = 0;
    }
    else if (Ledstatus == 0)
    {
      Ledstatus = 1;
    }
    digitalWrite(LED_BUILTIN, Ledstatus);
  }
}

/*------------------------------------------------------------------*/
/* Central
  ------------------------------------------------------------------*/
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Since we configure the scanner with filterUuid()
  // Scan callback only invoked for device with bleuart service advertised
  // Connect to the device with bleuart service in advertising packet
  /* Complete Local Name */
  uint8_t buffer[32];
  memset(buffer, 0, sizeof(buffer));
  if (Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, buffer, sizeof(buffer)))
  {
    if (buffer[0] == 'I' && buffer[1] == 'D' && buffer[2] == (ID + 48 + 1))
    {
      Bluefruit.Central.connect(report);
    }
    else
    {
      Bluefruit.Scanner.resume();
    }
  }
  else
  {
    Bluefruit.Scanner.resume();
  }
}

void cent_connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char peer_name[32] = { 0 };
  connection->getPeerName(peer_name, sizeof(peer_name));
  Serial.printf("[Cent %d] Connected to %s", ID, peer_name);
  Serial.println();
  Serial.println("con call");
  if ( clientUart.discover(conn_handle) )
  {
    // Enable TXD's notify
    clientUart.enableTXD();
    Serial.println("enableTX");
  } else
  {
    // disconnect since we couldn't find bleuart service
    Bluefruit.disconnect(conn_handle);
    Serial.println("fall to con");
  }
}

void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial.printf("[Cent %d] Disconnected", ID);
  Serial.println();
}

void cent_bleuart_rx_callback(BLEClientUart& cent_uart)
{
  length_rx_cent = cent_uart.available();
  if (length_rx_cent ==packageLength-20)
  {
    cent_uart.read(&data_rx_cent[20], length_rx_cent);
  }
  if (length_rx_cent >= packageLength)
  {
    cent_uart.read(data_rx_cent, length_rx_cent);
    Serial.print("Length: ");
    Serial.print(length_rx_cent);
    Serial.print("; Angles: ");
    for (int i = 0; i < length_rx_cent; i++)
    {
      Serial.print(data_rx_cent[i], DEC);
      Serial.print(" ");
      //data_tx_cent[i]=data_rx_cent[i];
    }
    Serial.println();
  }
  
}

void calibrateServoPosition(double* k, double* b) {
  double angle0 = 0.0;
  double angle1 = 180.0;
  servo.write(angle0);
  delay(2000);
  double x0 = analogRead(A0);
  delay(1);
  servo.write(angle1);
  delay(2000);
  double x1 = analogRead(A0);
  delay(1);
  *k = (angle1 - angle0) / (x1 - x0);
  *b = angle1 - (*k) * x1;
}

void calibrateServo(double* k, double* b) {
//  double k = 0.0;
//  double b = 0.0;
  calibrateServoPosition(k, b);
  Serial.println("Servo feedback calibration complete.");
  Serial.print("k = ");
  Serial.print(*k);
  Serial.print(", b = ");
  Serial.print(*b);
  Serial.println();
//  servo.write(0);
//  delay(1500);
//  for (int i = 0; i <= 180; i++){
//    servo.write(i);
//    delay(10);
//    double positionFeedback = analogRead(A0);
//    delay(1);
////    double angleRead = k * positionFeedback + b;
//    double angleRead = ceil(k * positionFeedback + b);
    // use with serial console
//    Serial.print("Command = ");
//    Serial.print(i);
//    Serial.print(", angleRead = ");
//    Serial.print(angleRead);
//    Serial.println();
    // use with serial plotter
//    Serial.print(i);
//    Serial.print(" ");
//    Serial.println(angleRead);
    //
//    delay(4000/180);
//  }
//  delay(1000);
}
