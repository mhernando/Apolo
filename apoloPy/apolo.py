AP_NONE = 0
AP_SETJOINTS = b'J'
AP_PLACE = b'P'
AP_CHECKJOINTS = b'j'
AP_UPDATEWORLD = b'U'
AP_TRUE = b'T'
AP_FALSE = b'F'
AP_PLACE_WB = b'p'
AP_MOVE_WB = b'm'
AP_GETLOCATION = b'G'
AP_GETLOCATION_WB = b'g'
AP_DVECTOR = b'D'
AP_LINK_TO_ROBOT_TCP = b'L'

def apolo_message(code):
    def message_decorator(func):
        def function_wrapper(data,*args):
            data.append(b'a')
            data.append(b'a')
            data.append(code)
            func(data,*args)
            size=int(2+len(data)).to_bytes(2,byteorder='big')
            data.insert(2,size[0])
            data.insert(3, size[1])
        return function_wrapper
    return message_decorator

def writeData(data, *args):
    for thing in args:
        if type(thing) is str:
            data.extend(thing.encode())
            data.append(b'\x00')
        elif type(thing) is int:
            data.extend(thing.to_bytes(2,byteorder='big'))
        elif type(thing) is float:
            #data.extend(thing.to_bytes(2,byteorder='big'))
        elif type(thing) is list:
            #data.extend(thing.to_bytes(2,byteorder='big'))

@apolo_message(AP_SETJOINTS)
def setJoints(data,world):
    writeData(data, world)

@apolo_message(AP_PLACE)
def setPlace(data,world, robot, num=5):
    writeData(data, world, robot, num)


data=[]
setJoints(data,"HOLA")
print(data)

data=[]
setPlace(data,"HOLA", "ASEA")
print(data)
