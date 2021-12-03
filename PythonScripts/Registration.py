import hashlib

def Hash(Name, Contacts):
  str2hash = Name + Contacts.join()
  return (hashlib.md5(str2hash.encode())).hexdigest()