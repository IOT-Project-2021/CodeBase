from requests.api import get
from onem2m import *
import json

uds = "Ultrasonic_Distance_sensor"
pir = "PIR_motion_sensor"
ldr = "LDR_sensor"
uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"



#Getting data for a sensor for a node
def get_data_from_onem2m(ae, nodekey):
    uri_cnt = uri_cse + "/" + ae + "/"+ nodekey 
    uri = uri_cnt+"/?rcn=4"
    headers = {'X-M2M-Origin': 'admin:admin', 'Content-type': 'application/json'}

    response = requests.get(uri, headers=headers)
    result = json.loads(response.text)
    x = []
    y = []
    if result["m2m:cnt"].get("m2m:cin") is not None:
        x = [i["ct"] for i in result["m2m:cnt"]["m2m:cin"]]
        y = [float(i["con"]) for i in result["m2m:cnt"]["m2m:cin"]]

    return x, y


# Creating new node should have created the ae befeore
def Create_New_Node(node_key):
    Create_New_Node_Sensor(uds, node_key)
    Create_New_Node_Sensor(pir, node_key)
    Create_New_Node_Sensor(ldr, node_key)

def Create_New_Node_Sensor(ae, node_key):
    uri_ae = uri_cse + "/" + ae
    create_cnt(uri_ae, node_key)



#Deleting a node
def Delete_Node(node_key):
    Delete_Node_sensor(uds, node_key)
    Delete_Node_sensor(pir, node_key)
    Delete_Node_sensor(ldr, node_key)

def Delete_Node_sensor(ae, node_key):
    uri_ae = uri_cse + "/" + ae
    uri_cnt = uri_ae + "/" + node_key
    delete_ae(uri_cnt)
    