import time
from API_REDES.configs.broker_configs import mqtt_broker_configs
from .mqtt_connection.mqtt_connection_client import MqttClientConnection
#inicia o cliente recebendo as configurações do broker
def start():
    mqtt_connection_client = MqttClientConnection(
        mqtt_broker_configs["HOST"],
        mqtt_broker_configs["PORT"],
        mqtt_broker_configs["CLIENT_NAME"],
        mqtt_broker_configs["KEEPALIVE"],
    )
#Para começar a conexão
    mqtt_connection_client.start_connection()
#While para manter rodando o programa
    while True: time.sleep(0.001)   