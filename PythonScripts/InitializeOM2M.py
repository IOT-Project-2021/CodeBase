from onem2m import *
uds = "Ultrasonic_Distance_sensor"
pir = "PIR_motion_sensor"
ldr = "LDR_sensor"
uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"


create_ae(uri_cse, uds)
create_ae(uri_cse, pir)
create_ae(uri_cse, ldr)