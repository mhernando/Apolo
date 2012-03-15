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

};
struct MRPointer
{
	union{
	SimpleJoint* simplejoint;
	Joint* joint;
	CylindricalPart* cylindricalpart;
	PrismaticPart* prismaticpart;
	LMS200Sim* lms200sim;
	LMS100Sim* lms100sim;
	FaceSetPart* facesetpart;
	PatrolbotSim* patrolbotsim;
	Pioneer3ATSim* pioneer3atsim;
	PowerCube70Sim* powercube70sim;
	NemoLaserSensor3DSim* nemolasersensor3dsim;
	LaserSensor3DSim* lasersensor3dsim;
	LaserSensorSim* lasersensorsim;
	MeshPart* meshpart;
	AdeptOneSim* adeptone;
	};
	WheeledBaseSim* wheeledbasesim;
	ComposedEntity* composedentity;
	SolidEntity* solidentity;
	PositionableEntity* positionableentity;
	World* world;
	RobotSim* robotsim;
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
};
class SimulatedWorld;

class NodeTree : public wxTreeItemData 
{
public:
	NodeTree(World* world);
	NodeTree(PositionableEntity* pos);
	TypeNode getTipo(){return tipo;}
	string getNameTree(){return name;}
	SimulatedWorld * getSimu(){return simuWorld;}
	const char *getCharName(){return name.c_str();} 

	Bitmap bit;
	BitmapSelect bitsel;
	MRPointer pointer;
	ContextualMenu menus;
	SimulatedWorld *simuWorld;
private:
	TypeNode tipo;
	string name;
	
};

#endif // __APOLO__NODE_TREE__H
