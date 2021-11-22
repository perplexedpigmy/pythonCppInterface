import cIntf

def multiply(a,b):
   c = a*b;
   print(a,"*",b,"=",c)
   return c

def divide(a,b):
   c=a/b
   print(a,"/",b,"=",c)
   return c

def add(a,b):
   if isinstance(a,int) and isinstance(b,int):
      c = cIntf.addInt(a,b)
   else:
      c = cIntf.addDec(a,b)
   print(a,"+",b,"=",c)
   return c


