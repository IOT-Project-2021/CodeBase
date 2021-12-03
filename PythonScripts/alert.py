import threading
import json
from time import sleep
from itertools import chain

from OM2Mhigh import * 
from Sendmail import Notify


uds = "Ultrasonic_Distance_sensor"
pir = "PIR_motion_sensor"
ldr = "LDR_sensor"
uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"



DataFile = open("Data.json")
Active_users = set()
Accident_users = set()
Data_users = {}
Sync = threading.Lock()

# Uregister users to be removed form the system OM2M and code data.
# Users who have accident and have been updated are to chagned to OM2M
# Deletion of users accident is not implemented.
def JsonUpdate():
  while(1):
    input()
    Sync.acquire()
    Data_users = json.load(DataFile)

    #Unregistered users
    Unregistered = set()
    for user in chain(Active_users, Accident_users):
      if Data_users.has_key(user) == False:
        Unregistered.add(user)

    Active_users = Active_users.difference(Unregistered)    
    Accident_users = Accident_users.difference(Unregistered)    
    for user in Unregistered:
      Delete_Node(user)

    #transacktion
    Discard = set()
    for user in Accident_users:
      if Data_users[user]["Modified"] == True:
        Discard.add(user)

    Accident_users = Accident_users.difference(Discard)    
    Active_users = Active_users.union(Discard) 
    for user in Discard:
      Data_users[user]["Modified"] = False

    json.dump(DataFile, indent=4)
    Sync.release()


def Alerting():
  while(1):
    Sync.acquire()
    sleep(1)
    for user in Active_users:
      if(Accident_happend(user)):
        Accident_users.add(user)
        threading.Thread(taget=Notify, args=(Data_users[user]["Name"], Data_users[user]["Contacts"])).start()
    Active_users = Active_users.difference(Accident_users)
    Sync.release()


def Accident_happend(user):
  Distance_Data = get_data_from_onem2m(uds, user)
  for instance in Distance_Data:
    if instance <= 3:
      return True
  return False


Json_thread = threading.Thread(target=JsonUpdate)
Alert_thread = threading.Thread(target=Alerting)

Json_thread.start()
Alert_thread.start()