import struct
import socket
AP_NONE = 0
AP_SETJOINTS = 'J'
AP_PLACE = 'P'
AP_CHECKJOINTS = 'j'
AP_UPDATEWORLD = 'U'
AP_TRUE = 'T'
AP_FALSE = 'F'
AP_PLACE_WB = 'p'
AP_MOVE_WB = 'm'
AP_GETLOCATION = 'G'
AP_GETLOCATION_WB = 'g'
AP_DVECTOR = 'D'
AP_LINK_TO_ROBOT_TCP = 'L'
class Uint8:
    def __init__(self, val):
        if(val>255):val=255
        if(val<0):val=0
        self.value=int(val)
def apolo_message(code):
    def message_decorator(func):
        def function_wrapper(data, *args):
            data=[]
            data.append(ord('a'))
            data.append(ord('a'))
            data.append(ord(code))
            func(data,*args)
            size=int(2+len(data)).to_bytes(2,byteorder='little')
            data.insert(2,size[0])
            data.insert(3, size[1])
            return bytearray(data)
        return function_wrapper
    return message_decorator

def writeData(data, *args):
    for thing in args:
        if type(thing) is str:
            if thing:
               data.append(1+len(thing))
            data.extend(thing.encode())
            data.append(0)
        elif type(thing) is int:
            data.extend(thing.to_bytes(2,byteorder='little'))
        elif type(thing) is float:
            #data.extend(thing.to_bytes(2,byteorder='big'))
            data.extend(struct.pack("d", thing))
        elif type(thing) is list:
            for x in thing:
                writeData(data,x)
        elif type(thing) is Uint8:
            data.append(thing.value)

@apolo_message(AP_SETJOINTS)
def m_setRobotJoints(data,robot,values,world=''):
    f_values = [float(x) for x in values]
    writeData(data, world, robot,Uint8(len(values)),f_values)

@apolo_message(AP_PLACE)
def m_placeObject(data,world, object, x,y,z,roll,pitch,yaw):
    writeData(data, world, object,
              float(x), float(y),float(z),
              float(roll), float(pitch),float(yaw))

@apolo_message(AP_MOVE_WB)
def m_moveWheeledBase(data,world, robot, speed, rotspeed, time):
    writeData(data, world, robot,
              float(speed), float(rotspeed), float(time))

@apolo_message(AP_PLACE_WB)
def m_placeWheeledBase(data,world, robot,  x,y,z,yaw):
    writeData(data, world, robot,
              float(x), float(y),float(z),float(yaw))
@apolo_message(AP_CHECKJOINTS)
def m_checkJoints(data,robot,values,world=''):
    writeData(data, world, robot,Uint8(len(values)),values)

@apolo_message(AP_GETLOCATION)
def m_getLocation(data,object,world=''):
    writeData(data, world, object)

@apolo_message(AP_GETLOCATION_WB)
def m_getLocationWB(data,object,world=''):
    writeData(data, world, object)

@apolo_message(AP_UPDATEWORLD)
def m_updateWorld(data, world=''):
    writeData(data, world)

@apolo_message(AP_LINK_TO_ROBOT_TCP)
def m_linkToRobotTCP(data,robot,object,world=''):
    writeData(data, world, robot, object)

class Apolo:
    APOLO_IP = "127.0.0.1"
    APOLO_PORT = 12000

    def __init__(self, address = APOLO_IP):
        self.sock = socket.socket()
        self.sock.connect((address, self.APOLO_PORT))
        self.data=[]

    def updateWorld(self,world=''):
        m=m_updateWorld(self.data,world)
        self.sock.send(m)

    def setRobotJoints(self,robot,values,world=''):
        m=m_setRobotJoints(self.data, robot, values, world)
        self.sock.send(m)

if __name__ == "__main__":
    import time
    ap=Apolo()
    ap.updateWorld()
    ap.setRobotJoints("ASEA",[0.5, 1, 0.5])
    for i in range(-100,100):
        ap.setRobotJoints("ASEA",[i*0.01, i*0.02, i*0.01])
        ap.updateWorld()
        time.sleep(0.02)
