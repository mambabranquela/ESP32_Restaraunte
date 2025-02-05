import paho.mqtt.client as mqtt
from .callbacks import on_connect, on_subscribe, on_message

#Classe do mqtt para a conexão
class MqttClientConnection:
    def __init__(self,broker_ip: str, port: int, client_name: str, keepalive = 60):
        self.__broker_ip = broker_ip
        self.__port = port
        self.__client_name = client_name
        self.__keepalive= keepalive
        self.__mqtt_client = None
#inicia a conexão
    def start_connection(self):
        mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        #callbacks 
        mqtt_client.on_connect = on_connect
        mqtt_client.on_subscribe = on_subscribe
        mqtt_client.on_message = on_message
        #endcallbacks
        mqtt_client.connect(host=self.__broker_ip, port=self.__port, keepalive=self.__keepalive)
        self.__mqtt_client= mqtt_client
        mqtt_client.loop_start()
#termina a conexão 
    def end_connection(self):
        try:
            self.__mqtt_client.loop_stop()
            self.__mqtt_client.disconnect()
            return True
        except:
            return False
