#include "BluetoothInput.h"

// Callbacks for BLE 
class serverCallbacks: public BLEServerCallbacks {
public:
    serverCallbacks(BluetoothInput* parent) : parent(parent) {}

    void onConnect(BLEServer *pServer) override {
        parent->deviceConnected = true;
        BLEDevice::startAdvertising();
    }

    void onDisconnect(BLEServer *pServer) override {
        parent->deviceConnected = false;
    }
private:
    BluetoothInput* parent;
};

BluetoothInput::BluetoothInput() {}

void BluetoothInput::setup() {

  // Create the BLE Device
  BLEDevice::init("LAMP-09-00");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new serverCallbacks(this));

  // Create the BLE Service
  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      BLE_CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());

  /*
  pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID_RX,
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
  */
  pCharacteristic = pService->createCharacteristic(BLE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR );

  pCharacteristic->setCallbacks(new BTCallbacks(this));

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  //Serial.println("Waiting a client connection to notify...");
}

void BluetoothInput::onBLEWrite(const std::string& value) {
    // Example: append to a buffer or process as needed
    // For now, just print it
    Serial.print("Received BLE write: ");
    Serial.println(value.c_str());
    messageQueue.push(value); // Store for processing in loop()
}

int BluetoothInput::getMessageCount() {
    return messageQueue.size();
}

LampMessage BluetoothInput::loop()
{
    if (!messageQueue.empty()) {
        std::string msgStr = messageQueue.front();
        messageQueue.pop();
        String msg = String(msgStr.c_str());
        Serial.print("BLE received: '");
        Serial.print(msg);
        Serial.println("'");
        return processMessage(msg);
    }
    return {LAMP_NONE, 0, 0, ""};
}

LampMessage BluetoothInput::processMessage(String msg)
{
    String empty = ""; // for returns with no value;
    //on
    if (msg == "st:on")
    {
        Serial.println("LAMP_ON!");
        return {LAMP_ON, 0, 0, empty};
    }

    if (msg == "st:off")
    {
        return {LAMP_OFF, 0, 0, empty};
    }

    if (msg == "an:slow")
    { // an is for animation
        return {SET_ANIM_SPEED, 10000, 0, empty};
    }
    else if (msg == "an:fast")
    {
        return {SET_ANIM_SPEED, 100, 0, empty};
    }

    else if (msg.startsWith("an"))
    { // an:<int>
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float duration = float(atoi(text));
        Serial.println(duration);
        return {SET_ANIM_SPEED, duration, 0, empty};
    }

    if (msg.startsWith("am"))
    { // am is for animation multiplier
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float mult = atof(text);

        return {MULT_ANIM_SPEED, mult, 0, empty};
    }

    if (msg.startsWith("br"))
    { // br is for multiply brightness
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float mult = atof(text);
        return {INC_BRIGHTNESS, mult, 0, empty};
        
    }

    if (msg.startsWith("bs"))
    {
        // bs is for set brightness
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float value = atof(text);
        Serial.println(value);
        return {SET_BRIGHTNESS, value, 0, empty};
    }

    // pl:10:11:20:21:30:31:40:41:50:51:60:61
    // pl:10:100:20:100:30:100:40:100:50:100
    //
    if (msg.startsWith("pl"))
    { // pl is for palette
        char input[100];
        ssData.toCharArray(input, 99);
        Serial.print("PALETTE: ");
        Serial.println(input);
        return {SET_PALETTE, 0, 0, ssData};
    }

    if (msg.startsWith("v:pal"))
    {
        // v:pal is for get palette
        return {GET_PALETTE, 0, 0, empty};
    }

    if (msg.startsWith("cs"))
    {
        // cs is for set colour
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float hue = atof(text);
        text = strtok(0, ":");
        float sat = atof(text); // @TODO handle this if it's null :-/
        return {SET_COLOUR, hue, sat, text};
    }

    if (msg.startsWith("v:get"))
    {
        // v:get is for get version
        Serial.println("v:get");
        return {GET_VERSION, 0, 0, empty};
        // sendVersionOverBluetooth();
    }

    if (msg.startsWith("v:lvl"))
    {
        // v:lvl is for get levels
        Serial.println("v:lvl");
        return {GET_LEVELS, 0, 0, empty};
    }

    if (msg.startsWith("d:sendsens:1"))
    {
        return {DEBUG_SENSITIVITY, 1, 0, empty};
        // sendTouchValue = true;
    }
    else if (msg.startsWith("d:sendsens:0"))
    {
        return {DEBUG_SENSITIVITY, 0, 0, empty};
        // sendTouchValue = false;
    }

    if (msg.startsWith("md")){
        // md is for set mode
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float modeId = float(atoi(text));
        return {SET_MODE, modeId, 0, empty};
    }

    Serial.print("Message unknown '");
    Serial.print(msg);
    Serial.println("'");
    // return a default
    
    return {LAMP_NONE, 0, 0, empty};
}

void BluetoothInput::sendMessage(char* message) {
    Serial.print("BluetoothInput::sendMessage >>> ");
    Serial.println(message);
    
    // BLE_Serial.write(message);
    // BLE_Serial.write("\n");
    /*
    char charBuf[128];
    buf.toCharArray(charBuf, 128);

    // convert from message to charBuf
    String buf = String(message);
    if (buf.length() > 128) {
        buf = buf.substring(0, 128); // truncate to fit
    }
    if (buf.length() == 0) {
        Serial.println("BluetoothInput::sendMessage - empty message, not sending");
        return; // don't send empty messages
    }
    static bool fullPkt = false; // flag to indicate if we are sending a full packet
    if (buf.length() >= 128) {
        fullPkt = true; // we are sending a full packet
    }
    */

    pCharacteristic->setValue(message);
    pCharacteristic->notify();
    
}
