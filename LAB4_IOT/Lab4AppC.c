configuration Lab4AppC
{
}
implementation
{
  components MainC, LedsC, Lab4C;
  components PrintfC;
  components SerialStartC;
  components new TimerMilliC();
  components new SensirionSht11C() as SensorC;
  components new HamamatsuS10871TsrC() as LightC;
  components ActiveMessageC;
  components new AMSenderC(AM_BLINKTORADIO);
  components new AMReceiverC(AM_BLINKTORADIO);

  Lab4C -> MainC.Boot;
  Lab4C.Leds -> LedsC;
  Lab4C.Timer -> TimerMilliC;
  Lab4C.Timer0 -> TimerMilliC;
  Lab4C.Temperature -> SensorC.Temperature;
  Lab4C.Humidity -> SensorC.Humidity;
  Lab4C.Light -> LightC.Read;
  Lab4C.Packet -> AMSenderC;
  Lab4C.AMPacket -> AMSenderC;
  Lab4C.AMSend -> AMSenderC;
  Lab4C.AMControl -> ActiveMessageC;
  Lab4C.Receive -> AMReceiverC;
}
