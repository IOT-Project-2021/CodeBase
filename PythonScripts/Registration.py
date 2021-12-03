import sys
import json
import hashlib
from OM2Mhigh import Create_New_Node

def Hash(Name, Contacts):
  str2hash = Name + "".join(Contacts)
  return (hashlib.md5(str2hash.encode())).hexdigest()



#getting argsument and creating data
N = len(sys.argv)
Name = sys.argv[1]
Contacts = [add for add in sys.argv if add != Name]

New_reg = {}
New_reg["Name"] = Name
New_reg["Modified"] = False
New_reg["Contacts"] = Contacts
New_hash = Hash(Name, Contacts)




#opening file and storing
Data = open("/home/balaramakrishna/Documents/2.1/IOT/Project/CodeBase/PythonScripts/Data.json", "r")
Data_json = json.load(Data)
Data.close()

Data_json[New_hash] = New_reg
Create_New_Node(New_hash)

Data = open("/home/balaramakrishna/Documents/2.1/IOT/Project/CodeBase/PythonScripts/Data.json", "w")
json.dump(Data_json, Data, indent=4)
Data.close()