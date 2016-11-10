#include "printf.h"
#include "Timer.h"
#include "Lab4.h"

 
module Lab4C @safe()
{
  uses interface Boot;
  uses interface Leds;
  uses interface Timer<TMilli> as Timer;
  uses interface Timer<TMilli> as Timer0;
  uses interface Read<uint16_t> as Temperature;
  uses interface Read<uint16_t> as Humidity; 
  uses interface Read<uint16_t> as Light; 
  uses interface Packet;
  uses interface AMPacket;
  uses interface AMSend;
  uses interface Receive;
  uses interface SplitControl as AMControl;
}

implementation
{
	uint16_t tem;
	uint16_t lum;
	uint16_t hum;
  uint16_t temperature;
  uint16_t humidity;
  uint16_t light;
  bool busy = FALSE;
  message_t pkt;

  event void Boot.booted()
  {
	call AMControl.start();
 	call Timer.startPeriodic(2000);
  }
 
  event void AMControl.startDone(error_t err) 
	{
		if (err != SUCCESS) 
		{
			call AMControl.start();
		}
	}

  event void AMControl.stopDone(error_t err) {}

  event void Timer.fired()
  {

    call Temperature.read();
    call Leds.led0Toggle();
    call Humidity.read();
    call Leds.led1Toggle();
    call Light.read();
    call Leds.led2Toggle();

   }

  event void Temperature.readDone(error_t result, uint16_t data)
  {
  	if (result == SUCCESS && TOS_NODE_ID ==0){
	  temperature = data;
	}
	
  }
 
  event void Humidity.readDone(error_t result, uint16_t data)
  {
  	if (result == SUCCESS && TOS_NODE_ID ==0){
	  humidity = data;
	}
	
  }
  
  event void Light.readDone(error_t result, uint16_t data)
  {
  	if (result == SUCCESS && TOS_NODE_ID ==0){
	  light = data;
	}
	
  }

  
  event void Timer0.fired()
  {
    if(TOS_NODE_ID ==0)
    {
	    if (!busy) 
	    {
		Lab4Msg* btrpkt = (Lab4Msg*)(call Packet.getPayload(&pkt, sizeof (Lab4Msg)));
		btrpkt->nodeid = TOS_NODE_ID;
		btrpkt->tmprt = temperature;
		btrpkt->hmdt = humidity;
		btrpkt->lght = light;
	    }
	    if (call AMSend.send(AM_BROADCAST_ADDR, &pkt, sizeof(Lab4Msg)) == SUCCESS) 
	    {
		busy = TRUE;
	    }
    }
  }

    event void AMSend.sendDone(message_t* msg, error_t error) 
  {
	if (&pkt == msg) 
	{
		busy = FALSE;
	}
  }
  
  event message_t* Receive.receive(message_t* msg, void* payload, uint8_t len) 
	{
	  if(TOS_NODE_ID == 1){
		if (len == sizeof(Lab4Msg)) 
		{
			Lab4Msg* btrpkt = (Lab4Msg*)payload;
			tem = (uint16_t) btrpkt->tmprt;
			tem= (-39.60 + 0.01*tem);
			lum = (uint16_t) btrpkt->hmdt;
			lum = 2.5 * (lum/4096.0)*6250.0;
			hum = (uint16_t) btrpkt->lght;
			hum = -4.0 + 0.0405 * hum + (-2.8*hum* hum)/1000000;
			printf("Temperature: %d\n",tem);
			printf("Humidity: %d\n",lum);
			printf("Light: %d\n",hum);
		}
		return msg;
	  }
	}
}
