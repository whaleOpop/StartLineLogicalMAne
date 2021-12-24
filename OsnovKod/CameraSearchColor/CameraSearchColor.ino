#include "TrackingCamDxlUart.h"
#include <DxlSlave.h>
TrackingCamDxlUart trackingCam;

void setup() {
/*TrackingCamDxlUartInit(CAM_ID, Serial_port, CAM_BaudRate, PC_BaudRate, TrackingCamTimeout);
 * CAM_ID - default 51
 * Serial_port:
 * Serial (Rx\Tx) - 0
 * Serial1 (Rx1\Tx1) - 1
 * Serial2 (Rx2\Tx2) - 2
 * Serial3 (Rx3\Tx3) - 3
 * CAM_BaudRate - default 115200
 * PC_BaudRate - default 115200
 * TrackingCamTimeout - in ms - default 30
 */
trackingCam.TrackingCamDxlUartInit(51, 2, 115200, 115200, 30);
}

void loop() {
  int n = 0;
//read data about blobs
  n = trackingCam.TrackingCamDxl_ReadBlobs();
//print numbers of blobs
  Serial.println(n);
//print information about first blob
    if(n)
    {
      Serial.print(trackingCam.blob[0].type, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].dummy, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].cx, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].cy, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].area, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].left, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].right, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].top, DEC);
      Serial.print(" ");
      Serial.print(trackingCam.blob[0].bottom, DEC);
      Serial.println(" ");
    }
  
  delay(20);        // delay in between reads for stability
}
