import sys
import json


#getting argsument and creating data
Node_key = sys.argv[1]


#getting data
Data = open("/home/balaramakrishna/Documents/2.1/IOT/Project/CodeBase/PythonScripts/Data.json", "r")
Data_json = json.load(Data)
Data.close()

Data_json.pop(Node_key)

#writting data to file
Data = open("/home/balaramakrishna/Documents/2.1/IOT/Project/CodeBase/PythonScripts/Data.json", "w")
json.dump(Data_json, Data, indent=4)
Data.close()