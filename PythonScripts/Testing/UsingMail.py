from Sendmail import Notify
import threading

# email with body second argument is passed to mail. (string)
Name = "xxxyyyzzz"
Contacts = ["balaramakrishna.p.varma@gmail.com", "Krishna.p.2020.2002@gmail.com"]
threading.Thread(target=Notify, args=(Name, Contacts)).start()
while(1):
  pass
