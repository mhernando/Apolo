#ifndef __APOLO__NODE_TREE__H
#define __APOLO__NODE_TREE__H  

#include "mrcore.h"
#include <wx/treebase.h>
#include <iostream>



enum TypeNode 
{
	N_SimpleJoint,
	N_Joint,
	N_CylindricalPart,
	N_PrismaticPart,
	N_IrregularPrismaticPart,
	N_LMS200Sim,
	N_LMS100Sim,
	N_FaceSetPart,
	N_PatrolbotSim,
	N_Pioneer3ATSim,
	N_PowerCube70Sim,
	N_NemoLaserSensor3DSim,
	N_LaserSensor3DSim,
	N_LaserSensorSim,
	N_MeshPart,
	N_WheeledBaseSim,
	N_ComposedEntity,
	N_SolidEntity,
	N_PositionableEntity,
	N_World,
	N_RobotSim,
	N_AdeptOneSim,
	N_SpherePart,
	N_CameraSim,
	N_KinectSim,
	N_MobileRobot,
	N_PersonSim,
	N_QuadrotorSim,
	N_AseaIRB2000,
};
enum Bitmap 
{
	Bit_simplejoint=1,
	Bit_joint=3,
	Bit_cylindricalpart=5,
	Bit_prismaticpart=7,
	Bit_lms200sim=9,
	Bit_lms100sim=11,
	Bit_facesetpart=13,
	Bit_patrolbotsim=15,
	Bit_pioneer3atsim=17,
	Bit_powercube70sim=19,
	Bit_nemolasersensor3dsim=21,
	Bit_lasersensor3dsim=23,
	Bit_lasersensorsim=25,
	Bit_meshpart=27,
	Bit_wheeledbasesim=29,
	Bit_composedentity=31,
	Bit_solidentity=33,
	Bit_positionableentity=35,
	Bit_world=37,
	Bit_robotsim=39,
	Bit_adeptonesim=41,
	Bit_spherepart=43,
	Bit_asea=45,

};
enum BitmapSelect
{
	BitSel_simplejoint=2,
	BitSel_joint=4,
	BitSel_cylindricalpart=6,
	BitSel_prismaticpart=8,
	BitSel_lms200sim=10,
	BitSel_lms100sim=12,
	BitSel_facesetpart=14,
	BitSel_patrolbotsim=16,
	BitSel_pioneer3atsim=18,
	BitSel_powercube70sim=20,
	BitSel_nemolasersensor3dsim=22,
	BitSel_lasersensor3dsim=24,
	BitSel_lasersensorsim=26,
	BitSel_meshpart=28,
	BitSel_wheeledbasesim=30,
	BitSel_composedentity=32,
	BitSel_solidentity=34,
	BitSel_positionableentity=36,
	BitSel_world=38,
	BitSel_robotsim=40,
	BitSel_adeptonesim=42,
	BitSel_spherepart=44,
	BitSel_asea=46,

};
struct MRPointer
{
	union{
	SimpleJoint* simplejoint;
	CylindricalPart* cylindricalpart;
	PrismaticPart* prismaticpart;
	LMS200Sim* lms200sim;
	LMS100Sim* lms100sim;
	FaceSetPart* facesetpart;
	PatrolbotSim* patrolbotsim;
	Pioneer3ATSim* pioneer3atsim;
	PowerCube70Sim* powercube70sim;
	PersonSim* personSim;
	QuadrotorSim* quadrotorSim;
	MobileRobot* mobileRobot;
	CameraSim* cameraSim;
	KinectSim* kinectSim;
	NemoLaserSensor3DSim* nemolasersensor3dsim;
	MeshPart* meshpart;
	AdeptOneSim* adeptone;
	SpherePart * spherepart;
	AseaIRB2000Sim *aseaIRB2000Sim;
	};
	WheeledBaseSim* wheeledbasesim;
	ComposedEntity* composedentity;
	SolidEntity* solidentity;
	PositionableEntity* positionableentity;
	World* world;
	RobotSim* robotsim;
	LaserSensorSim* lasersensorsim;
	LaserSensor3DSim* lasersensor3dsim;
	Joint* joint;
};
struct MRServer
{
	union
	{
	LaserSensorServer *laserSensor;
	LaserSensor3DServer *laserSensor3D;
	WheeledBaseServer *wheeledBase;
	CameraServer *camera;
	KinectServer *kinect;
	VoiceTTSServer *voiceTTS;
	QuadrotorServer *quadrotor;
	};
	int Clients;
	int Port;
	wxString Address;

};

struct MRClient
{

	union 
	{
	LaserSensorClient *laserSensor;
	LaserSensor3DClient *laserSensor3D;
	WheeledBaseClient *wheeledBase;
	CameraClient *camera;
	KinectClient *kinect;
	VoiceTTSClient *voiceTTS;
	QuadrotorClient *quadrotor;
	};
	wxString Host;
	wxString Address;
	int Port;
	

};

struct ContextualMenu
{

	bool menu_positionable;
	bool menu_solid;
	bool menu_composed;
	bool menu_wheeledbased;
	bool menu_robotsim;
	bool menu_world;
	bool menu_meshpart;
	bool menu_connection;
	bool menu_laser;
	bool menu_design;
	bool menu_simplejoint;
	
};

class SimulatedWorld;
class NodeTree : public wxTreeItemData 
{

public:
	NodeTree(PositionableEntity* pos,SimulatedWorld *simu);
	NodeTree(SimulatedWorld *simu);
	TypeNode getTipo(){return tipo;}
	void setTipo(TypeNode type){tipo=type;}
	void setName(wxString _name){name=_name;}
	wxString getNameTree(){return name;}
	SimulatedWorld * getSimu(){return simuWorld;}



	int typeConnection; //0 Nothing //1 Server //2 Client
	Bitmap bit;
	BitmapSelect bitsel;
	MRPointer pointer;
	MRServer server;
	MRClient client;
	ContextualMenu menus;
	TypeNode tipo;
private:

	wxString name;	
	SimulatedWorld *simuWorld;

	
};

#endif // __APOLO__NODE_TREE__H
