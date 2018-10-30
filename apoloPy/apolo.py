import struct
import socket
import math
AP_NONE = 0
AP_GET_LASER_LM = 'B'
AP_DVECTOR = 'D'
AP_FALSE = 'F'
AP_GETLOCATION = 'G'
AP_SETJOINTS = 'J'
AP_LINK_TO_ROBOT_TCP = 'L'
AP_PLACE = 'P'
AP_RESET_ODOMETRY = 'R'
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
            return [x for x in struct.iter_unpack('<Hdd', data[7:])]

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
def m_getOdometry(data, robot, world=''):
    writeData(data, world, robot)

@apolo_message(AP_RESET_ODOMETRY)
def m_resetOdometry(data, robot, pose2d=(0, 0, 0), world=''):
    writeData(data, world, robot, float(pose2d[0]), float(pose2d[1]),float(pose2d[2]))


class Apolo:
    APOLO_IP = "127.0.0.1"
    APOLO_PORT = 12000

    def __init__(self, address = APOLO_IP):
        self.sock = socket.socket()
        self.sock.connect((address, self.APOLO_PORT))
        self.data=[]

    def getMessage(self ):
        m = bytearray(self.sock.recv(1024))
        tam = int.from_bytes(m[2:4],byteorder='little')
        while len(m)<tam:
            aux = self.sock.recv(tam-len(m))
            m.extend(aux)
        return m

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
        return extractData(self.sock.recv(1024))

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
        return extractData(self.getMessage())

    def getDependentUSensors(self, object, world=''):
        self.sock.send(m_getDepUSensors(self.data, object, world))
        return extractData(self.getMessage())

    def getLaserData(self, laser, world=''):
        self.sock.send(m_getLaserData(self.data, laser, world))
        return extractData(self.getMessage())

    def getUSensor(self, sensor, world=''):
        self.sock.send(m_getUSensor(self.data, sensor, world))
        return extractData(self.getMessage())

    def getOdometry(self, robot, world=''):
        self.sock.send(m_getOdometry(self.data, robot, world))
        return extractData(self.getMessage())

    def resetOdometry(self, robot, pose2d=(0, 0, 0), world=''):
        self.sock.send(m_resetOdometry(self.data, robot, pose2d, world))


def createRoom(vertex, height, name, fileName):
    '''utility function that creates a room, given de 2D x,y coordinates of the floor
    vertexes and a height
    e.g.: createRoom([(1,0),(1,1),(0,1),(0,0)], 2.0, name, 'myfile.xml')
    '''
    f = open(fileName,"w+")
    f.write('<FaceSetPart name="%s">\n'%name)
    f.write('\t<face>\n\t\t<vertex>\n')
    for x,y in vertex:#floor
        f.write('\t\t\t{%f , %f, 0}\n'%(x,y))
    f.write('\t\t</vertex>\n\t\t<colour r="0.2" g="0.2" b="1"/>\n\t</face>\n')
    #walls

    n = len(vertex)
    for i in range(0,n):
        f.write('\t<face>\n\t\t<vertex>\n')
        dx = vertex[(i+1)%n][0] - vertex[i][0]
        dy = vertex[(i+1)%n][1] - vertex[i][1]
        d=(dx*dx+dy*dy)**0.5
        f.write('\t\t\t{%f , %f, 0}\n' % (0, 0))
        f.write('\t\t\t{%f , %f, 0}\n' % (0, height))
        f.write('\t\t\t{%f , %f, 0}\n' % (d, height))
        f.write('\t\t\t{%f , %f, 0}\n' % (d, 0))
        f.write('\t\t</vertex>\n')
        f.write('\t\t<orientation>{%f , %f, %f}</orientation>\n' % (math.pi/2, 0, math.atan2(dy,dx)))
        f.write('\t\t<position>{%f , %f, 0}</position>\n' % (vertex[i][0],vertex[i][1]))
        f.write('\t\t<colour r="0.2" g="0.2" b="0.8"/>\n')

        f.write('\t</face>\n')
    f.write('</FaceSetPart>\n')

if __name__ == "__main__":
    import time
    ap=Apolo()
    ap.updateWorld()
    '''ap.checkRobotJoints("Puma 560",[0.5, 1, 0.5])
    for i in range(-100,100):
        print(ap.checkRobotJoints("Puma 560",[i*0.01, i*0.03, i*0.01]))
        #ap.updateWorld()
    print(ap.getLocation('Puma 560'))'''
    b=ap.getLaserData('LMS100')
    print(b)
    c=ap.getLaserLandMarks('LMS100')
    print(c)
