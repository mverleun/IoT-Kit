from network import Bluetooth, WLAN
# from mqtt import MQTTClient
from binascii import hexlify
import machine
import time
import ujson

def hexlifyNone( object ):
    return None if object is None else hexlify( object )

def scan( ):
    devices = {}
    print( '***   Starting   ***' )
    bt = Bluetooth( )
    print( 'Scan...', end = '' )
    bt.start_scan( 10 )
    time.sleep( 10 )
    print( 'OK' )
    advs = bt.get_advertisements()
    print( advs )

    for adv in advs:
        devices[ hexlify( adv.mac ) ] = {
                            'Mac': hexlify( adv.mac ),
                            'Rssi': adv.rssi,
                            'Data': hexlifyNone( adv.data ),
                            'MfrData': hexlifyNone( bt.resolve_adv_data( adv.data, Bluetooth.ADV_MANUFACTURER_DATA ) ),
                            'NameShort': hexlifyNone( bt.resolve_adv_data( adv.data, Bluetooth.ADV_NAME_SHORT ) ),
                            'NameComplete': hexlifyNone( bt.resolve_adv_data( adv.data, Bluetooth.ADV_NAME_CMPL ) ),
                            'Flag': bt.resolve_adv_data( adv.data, Bluetooth.ADV_FLAG ),
                            'Appearance': hexlifyNone( bt.resolve_adv_data( adv.data, Bluetooth.ADV_APPEARANCE ) )
                        }
#        """
        print( 'MAC: {}\n\tRSSI: {}\n\tData: {}\n\tName: {}\n\tMfrData: {}'.format(
                hexlify( adv.mac ),
                adv.rssi,
                hexlify( adv.data ),
                bt.resolve_adv_data( adv.data, Bluetooth.ADV_NAME_SHORT ),
                hexlify( bt.resolve_adv_data( adv.data, Bluetooth.ADV_MANUFACTURER_DATA ) )
                )
              )
#        """
    print( "\n***********\nDevices: ", devices )
    for key, data in devices.items():
        print( "\n***********\n", data )
        msgJson = ujson.dumps( data )
#        client.publish( topic="/test", msg = msgJson )

    print( '***   End   ***' )

def sub_cb(topic, msg):
   print(msg)




wlan = WLAN( mode=WLAN.STA )
wlan.connect( "MarCoach-IoT", auth=( WLAN.WPA2, "800ballen" ), timeout=5000 )
while not wlan.isconnected( ): 
    machine.idle( )
print( "Connected to Wifi\n" )

#client = MQTTClient( "device_id", "MQTT broker address",user="user if any", password="password if any", port=1883, ssl=True )
#client.set_callback( sub_cb )
#client.connect( )
#client.subscribe( topic="/test" )
#client.publish( topic="/test", msg="Started" )
scan( )