#include <bluefruit.h>

// OTA DFU service
BLEDfu bledfu;

// Peripheral uart service
BLEUart bleuart;

// Central uart client
BLEClientUart clientUart;
String ID_name = "ID2"; //change the name
int ID = ID_name[2] - 48; //48-30
int Ledstatus = 0;
int packageLength = 30;
char data_rx_cent[31] = {0};
char data_tx_cent[31] = {0};
char data_tx_phpr[31] = {0};
char data_rx_phpr[31] = {0};
int ServoAngle = ID + 10; // -128~127
int command = 0;
unsigned long cur_time = 0;
unsigned long pre_time = 0;
unsigned long time_period = 200;
int length_rx_cent = 0;
int length_rx_phpr = 0;
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

void loop()
{
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
      data_tx_phpr[ID] = ServoAngle;
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
  if (length_rx_phpr >= packageLength)
  {
    bleuart.read(data_rx_phpr, packageLength);
    command = (int16_t)data_rx_phpr[ID];
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
