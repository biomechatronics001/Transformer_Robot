#include <bluefruit.h>

// OTA DFU service
BLEDfu bledfu;

// Peripheral uart service
BLEUart bleuart;

// Central uart client
BLEClientUart clientUart;
String ID_name = "Master"; //change the name
int IDd1 = 0; //digit 1
int IDd2 = 0; //digit 2, IDd2*10 + IDd1
int Ledstatus = 0;
int packageLength = 30;
int rs232Length = 32;
char data_rx_cent[31] = {0};
char data_tx_cent[31] = {0};
char data_tx_rs232[33] = {0};
char data_rx_rs232[33] = {0};
int command = 0;
unsigned long cur_time = 0;
unsigned long pre_time = 0;
unsigned long time_period = 250;
int length_rx_cent = 0;
int length_rx_phpr = 0;
void setup()
{
  delay(1000);
//  Serial.begin(115200);
//  Serial.println("Bluefruit52 Dual Role BLEUART Example");
//  Serial.println("-------------------------------------\n");
  // Initialize Bluefruit with max concurrent connections as Peripheral = 1, Central = 1
  // SRAM usage required by SoftDevice will increase with number of connections
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin(0, 1);
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName(ID_name.c_str()); //change the name

  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(cent_connect_callback);
  Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

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

  cur_time = millis();
  pre_time = cur_time;
  data_tx_rs232[0] = 165;
  data_tx_rs232[1] = 90;
}


void loop()
{
  // do nothing, all the work is done in callback
  cur_time = millis();
  if ((cur_time - pre_time) > time_period)
  {
    pre_time = cur_time;
    Serial.write(data_tx_rs232,rs232Length);
//        if ( Bluefruit.Central.connected() && clientUart.discovered() )
//        {
//          clientUart.write(&data_rx_rs232[2], packageLength);
//        }
  }
  // Get Serial input and send to Peripheral
  if (Serial.available() >= rs232Length )
  {
    data_rx_rs232[0] = Serial.read();
    if (data_rx_rs232[0] == 165)
    {
      data_rx_rs232[1] = Serial.read();
      if (data_rx_rs232[1] == 90)
      {
        Serial.readBytes(&data_rx_rs232[2], packageLength);
        if ( Bluefruit.Central.connected() && clientUart.discovered() )
        {
          clientUart.write(&data_rx_rs232[2], packageLength);
        }
      }
    }
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
    if (buffer[0] == 'I' && buffer[1] == 'D' && buffer[2] == IDd1+48 && buffer[3] == IDd2+48)
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
//  Serial.printf("[Cent %d] Connected to %s", ID, peer_name);
//  Serial.println();
//  Serial.println("con call");
  if ( clientUart.discover(conn_handle) )
  {
    // Enable TXD's notify
    clientUart.enableTXD();
//    Serial.println("enableTX");
  } else
  {
    // disconnect since we couldn't find bleuart service
    Bluefruit.disconnect(conn_handle);
//    Serial.println("fall to con");
  }
}

void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
//  Serial.printf("[Cent %d] Disconnected", ID);
//  Serial.println();
}

void cent_bleuart_rx_callback(BLEClientUart& cent_uart)
{
  length_rx_cent = cent_uart.available();
  if (length_rx_cent >= packageLength)
  {
    cent_uart.read(&data_tx_rs232[2], length_rx_cent);
//    Serial.print("Length: ");
//    Serial.print(length_rx_cent);
//    Serial.print("; Angles: ");
//    for (int i = 0; i < length_rx_cent; i++)
//    {
//      Serial.print(data_rx_cent[i], DEC);
//      Serial.print(" ");
//      //data_tx_cent[i]=data_rx_cent[i];
//    }
//    Serial.println();
  }
}
