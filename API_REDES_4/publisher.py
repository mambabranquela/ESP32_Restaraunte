import paho.mqtt.client as mqtt

# Configuração do broker MQTT
BROKER = "broker.emqx.io"  # Você pode usar outro broker MQTT
PORT = 1883  # Porta padrão do MQTT
TOPIC = "/esteiraapi"
MESSAGE = "ligar"

# Callback para conexão bem-sucedida com o broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT!")
        # Publica uma mensagem no tópico especificado
        client.publish(TOPIC, MESSAGE)
        print(f"Mensagem publicada: {MESSAGE}")
    else:
        print(f"Erro ao conectar, código: {rc}")

# Callback para confirmação de publicação
def on_publish(client, userdata, mid):
    print(f"Mensagem publicada com sucesso! mid: {mid}")

# Criação do cliente MQTT com CallbackAPIVersion.VERSION2
client = mqtt.Client(protocol=mqtt.MQTTv311, userdata=None, transport="tcp")
client._mqttv = mqtt.CallbackAPIVersion.VERSION2

# Configuração dos callbacks
client.on_connect = on_connect
client.on_publish = on_publish

try:
    # Conecta ao broker e inicia o loop
    client.connect(BROKER, PORT, keepalive=60)
    client.loop_start()
except Exception as e:
    print(f"Erro ao conectar ou publicar: {e}")
finally:
    # Aguarda alguns segundos e encerra o loop
    import time
    time.sleep(5)
    client.loop_stop()
    client.disconnect()
