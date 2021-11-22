import cIntf

def cat(fname):
   with open(fname, 'rb') as inp:
      c = inp.read()
   return c

def cmd(cmdStr):
   return cIntf.system(cmdStr.decode("utf-8"))




