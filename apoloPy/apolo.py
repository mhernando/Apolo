import struct
import socket
AP_NONE = 0
AP_GET_LASER_LM = 'B'
AP_DVECTOR = 'D'
AP_FALSE = 'F'
AP_GETLOCATION = 'G'
AP_SETJOINTS = 'J'
AP_LINK_TO_ROBOT_TCP = 'L'
AP_PLACE = 'P'
AP_TRUE = 'T'
AP_UPDATEWORLD = 'U'
AP_LM_INFO = 'b'
AP_GET_DEP_USENSORS = 'd'
AP_GETLOCATION_WB = 'g'
AP_CHECKJOINTS = 'j'
AP_GET_LASER_DATA = 'l'
AP_MOVE_WB = 'm'
AP_GET_WB_ODOMETRY ='o'
AP_PLACE_WB = 'p'
AP_GET_USENSOR = 'u'




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
def extractData(data):
    if len(data)<5 or data[0:2] != b'aa':
        return None
    size = int.from_bytes(data[2:4],byteorder='little')
    if data[4]==ord(AP_TRUE):
        return True
    elif data[4]==ord(AP_FALSE):
        return False
    elif data[4]==ord(AP_DVECTOR):
        tam = int.from_bytes(data[5:7],byteorder='little')
        if tam*8+7>len(data):
            print("BAD FORMATED MESSAGE")
        else:
            return [x[0] for x in struct.iter_unpack('d',data[7:])]
    elif data[4]==ord(AP_LM_INFO):
        tam = int.from_bytes(data[5:7], byteorder='little')
        if tam * (8+8+2) + 7 > len(data):
            print("BAD FORMATED MESSAGE")
        else:
            print("TO BE IMPLEMENTED")
            
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
def m_placeObject(data, object, x,y,z,roll,pitch,yaw, world = ''):
    writeData(data, world, object,
              float(x), float(y),float(z),
              float(roll), float(pitch),float(yaw))

@apolo_message(AP_MOVE_WB)
def m_moveWheeledBase(data, robot, speed, rotspeed, time, world = ''):
    writeData(data, world, robot,
              float(speed), float(rotspeed), float(time))

@apolo_message(AP_PLACE_WB)
def m_placeWheeledBase(data, robot,  x,y,z,yaw,world = ''):
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

@apolo_message(AP_GET_LASER_LM)
def m_getLaserLM(data,laser,world=''):
    writeData(data, world, laser)

@apolo_message(AP_GET_DEP_USENSORS)
def m_getDepUSensors(data, object, world=''):
    writeData(data, world, object)

@apolo_message(AP_GET_LASER_DATA)
def m_getLaserData(data, laser, world=''):
    writeData(data, world, laser)

@apolo_message(AP_GET_USENSOR)
def m_getUSensor(data, sensor, world=''):
    writeData(data, world, sensor)

@apolo_message(AP_GET_WB_ODOMETRY)
def m_getOdometry(data, robot, last, noise, world=''):
    writeData(data, world, robot, float(last[0]),
              float(last[1]), float(last[2]), float(noise))


class Apolo:
    APOLO_IP = "127.0.0.1"
    APOLO_PORT = 12000

    def __init__(self, address = APOLO_IP):
        self.sock = socket.socket()
        self.sock.connect((address, self.APOLO_PORT))
        self.data=[]

    def updateWorld(self,world=''):
        self.sock.send(m_updateWorld(self.data,world))

    def setRobotJoints(self,robot,values,world=''):
        self.sock.send(m_setRobotJoints(self.data, robot, values, world))

    def checkRobotJoints(self,robot, values, world=''):
        self.sock.send(m_checkJoints(self.data, robot, values, world))
        return extractData(self.sock.recv(1024))

    def getLocation(self, object, world=''):
        self.sock.send(m_getLocation(self.data, object, world))
        return extractData(self.sock.recv(1024))

    def moveWheeledBase(self, robot, speed, rotspeed, time, world=''):
        self.sock.send(m_moveWheeledBase(self.data, robot, speed, rotspeed, time, world))

    def placeWheeledBase(self, robot, x,y,z,yaw, world=''):
        self.sock.send(m_placeWheeledBase(self.data, robot, x,y,z,yaw,world))
        return extractData(self.sock.recv(1024))

    def getLocationWB(self, robot, world=''):
        self.sock.send(m_getLocationWB(self.data, robot, world))
        return extractData(self.sock.recv(1024))

    def linkToRobotTCP(self, robot, object, world=''):
        self.sock.send(m_linkToRobotTCP(self.data, robot, object, world))

    def getLaserLandMarks(self, laser, world=''):
        self.sock.send(m_getLaserLM(self.data, laser, world))

    def getDependentUSensors(self, object, world=''):
        self.sock.send(m_getDepUSensors(self.data, object, world))

    def getLaserData(self, laser, world=''):
        self.sock.send(m_getLaserData(self.data, laser, world))

    def getUSensor(self, sensor, world=''):
        self.sock.send(m_getUSensor(self.data, sensor, world))

    def getOdometry(self, robot, last, noise=0, world=''):
        self.sock.send(m_getOdometry(self.data, robot, last, noise, world))


if __name__ == "__main__":
    import time
    ap=Apolo()
    ap.updateWorld()
    ap.checkRobotJoints("Puma 560",[0.5, 1, 0.5])
    for i in range(-100,100):
        print(ap.checkRobotJoints("Puma 560",[i*0.01, i*0.03, i*0.01]))
        #ap.updateWorld()
    print(ap.getLocation('Puma 560'))
