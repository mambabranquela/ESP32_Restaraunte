from API_REDES.configs.broker_configs import mqtt_broker_configs

#Callback para on connect 
def on_connect(client, userdatas,flags,reason_code,properties=None):
    if reason_code == 0 :
        print(f'Conexão feita com sucesso {client}')
        client.subscribe(mqtt_broker_configs["TOPIC"])
    else :
        print(f'erro ao conectar codigo={reason_code}')
#Callback para inscrição 
def on_subscribe(client,userdata,mid,granted_qos,properties=None):
    print(f'Cliente inscrito com sucesso {mqtt_broker_configs["TOPIC"]}')
#Callback para a menssagem  
def on_message(client, userdata,msg, properties=None):
    print(f'Mensagem recebida') 
    print(client)
    print(msg.payload) 
#Callback para desconectar 
def on_disconnect(client, userdata, reason_code, properties=None):
    if reason_code == 0:
        print('DESCONECTADO COM SUCESSO')
    else:
        print('ERRO AO DESCONECTAR')
