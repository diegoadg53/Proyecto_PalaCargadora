import time
import sys
import serial

from Adafruit_IO import MQTTClient

ADAFRUIT_IO_KEY = "aio_JCDA50M7ydGNZZETGxTTnptb3sK6"

ADAFRUIT_IO_USERNAME = "dua22426"


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))

feedTraccionTrasera = 'Traccion_Trasera_Value'
feedTrenDelantero = 'Tren_Delantero_Value'
feedElevacionPala = 'Elevacion_Pala_Value'
feedGiroPala = 'Giro_Pala_Value'

mandar_informacion = False

def connected(client):
    client.subscribe(feedTraccionTrasera)
    print("Escuchando a Traccion_Trasera_Value")
    client.subscribe(feedTrenDelantero)
    print("Escuchando a Tren_Delantero_Value")
    client.subscribe(feedElevacionPala)
    print("Escuchando a Elevacion_Pala_Value")
    client.subscribe(feedGiroPala)
    print("Escuchando a Giro_Pala_Value")

def disconnected(client):
    print("Desconectado de AdafruitIO")
    sys.exit(1)

def message(client, topic_id, payload):
    print('Topic {0} received new value: {1}'.format(topic_id, payload))
    if mandar_informacion:
        if (topic_id == feedTraccionTrasera):
            write_read(("4"+chr(int(payload))+"\n"))
            print(bytes("4"+chr(int(payload)), 'utf-8'))
            print(bytes(payload, 'ascii'))
        if (topic_id == feedTrenDelantero):
            write_read(("3"+chr(int(payload))+"\n"))
            print(bytes("3"+chr(int(payload)), 'utf-8'))
        if (topic_id == feedElevacionPala):
            write_read(("2"+chr(int(payload))+"\n"))
            print(bytes("2"+chr(int(payload)), 'utf-8'))
        if (topic_id == feedGiroPala):
            write_read(("1"+chr(int(payload))+"\n"))
            print(bytes("1"+chr(int(payload)), 'utf-8'))

try:
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    client.on_connect = connected
    client.on_disconnect = disconnected
    client.on_message = message
    
    client.connect()

    client.loop_background()

    arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)

    while True:
        mensaje = arduino.readline().decode('utf-8')
        if(mensaje == "2\n"):
            print("modo EEPROM")
            mandar_informacion = True
        elif((mensaje == "1\n") or (mensaje == "0\n")):

            mandar_informacion = False
        elif (mensaje != ""):
            print(mensaje)

except KeyboardInterrupt:
    print("Salimos del programa")
    if arduino.is_open:
        arduino.close()
    sys.exit(1)
