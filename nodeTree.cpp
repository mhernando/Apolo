#include "nodeTree.h"
#include "simulatedWorld.h"

NodeTree::NodeTree(PositionableEntity* pos,SimulatedWorld *simu)
{

	simuWorld=simu;
	typeConnection=0;
	server.Port=0;
	server.Clients=0;
	client.Port=0;
	client.Host=wxEmptyString;
	client.Address=wxEmptyString;
	menus.menu_positionable = false;
	menus.menu_solid = false;
	menus.menu_laser = false;
	menus.menu_composed = false;
	menus.menu_wheeledbased = false;
	menus.menu_world = false;
	menus.menu_robotsim = false;
	menus.menu_meshpart = false;
	menus.menu_connection = false;
	menus.menu_design = false;
	menus.menu_simplejoint = false;
	


	//primero las clases bases que pueden ser compuestas
	
	if(dynamic_cast<PositionableEntity *>(pos))	menus.menu_positionable = true;
	if(dynamic_cast<SolidEntity *>(pos))	menus.menu_solid = true; 
	if(dynamic_cast<ComposedEntity *>(pos))	menus.menu_composed = true;
	if(dynamic_cast<WheeledBaseSim *>(pos))	menus.menu_wheeledbased = true;	
	if(dynamic_cast<LaserSensorSim *>(pos)) menus.menu_laser=true;
	if(dynamic_cast<RobotSim *>(pos)) menus.menu_robotsim = true;
	if(dynamic_cast<MeshPart *>(pos)) menus.menu_meshpart = true;
	if(dynamic_cast<SimpleJoint *>(pos)) menus.menu_simplejoint = true;
	


	//Ahora los punteros a las clases base que pueden ser parte
	//de una herencia multiple, los cuales no retornan directamente y son no exclusivos
	//orden de herencia
	if(dynamic_cast<PositionableEntity *>(pos))
	{
		pointer.positionableentity = dynamic_cast<PositionableEntity *>(pos);
		tipo = N_PositionableEntity;
		bit = Bit_positionableentity;
		bitsel = BitSel_positionableentity;
		name =wxString(pos->getName());
		if(name.empty())name = "Positionable Entity";
	}

	if(dynamic_cast<SolidEntity *>(pos))
	{
		pointer.solidentity = dynamic_cast<SolidEntity *>(pos);
		tipo = N_SolidEntity;
		bit = Bit_solidentity;
		bitsel = BitSel_solidentity;
		name =wxString( pos->getName());
		if(name.empty())name = "Solid Entity";
	}

	if(dynamic_cast<ComposedEntity *>(pos))
	{
		pointer.composedentity = dynamic_cast<ComposedEntity *>(pos);
		tipo = N_ComposedEntity;
		bit = Bit_composedentity;
		bitsel = BitSel_composedentity;
		name = wxString(pos->getName());
		if(name.empty())name = "Composed Entity";
	}
	
	if(dynamic_cast<WheeledBaseSim *>(pos))
	{
		pointer.wheeledbasesim = dynamic_cast<WheeledBaseSim *>(pos);
		tipo = N_WheeledBaseSim;
		bit = Bit_wheeledbasesim;
		bitsel = BitSel_wheeledbasesim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		if(name.empty())name = "Wheeled Base";
		server.wheeledBase=new WheeledBaseServer(pointer.wheeledbasesim,name.ToStdString());
		client.wheeledBase=new WheeledBaseClient();
	
	}
	
	
	
	if(dynamic_cast<RobotSim *>(pos))
	{
		pointer.robotsim = dynamic_cast<RobotSim *>(pos);
		tipo = N_RobotSim;
		bit = Bit_robotsim ;
		bitsel = BitSel_robotsim ;
		name = wxString(pos->getName());
		if(name.empty())name = "Robot"; 
		
	}


	if(dynamic_cast<LaserSensorSim *>(pos))
	{
		pointer.lasersensorsim = dynamic_cast<LaserSensorSim *>(pos);
		tipo = N_LaserSensorSim;
		bit = Bit_lasersensorsim;
		bitsel = BitSel_lasersensorsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Laser Sensor";
		server.laserSensor=new LaserSensorServer(pointer.lasersensorsim,name.ToStdString());
		client.laserSensor=new LaserSensorClient();
			
	}
	
	if(dynamic_cast<LaserSensor3DSim *>(pos))
	{
		pointer.lasersensor3dsim = dynamic_cast<LaserSensor3DSim *>(pos);
		tipo = N_LaserSensor3DSim;
		bit = Bit_lasersensor3dsim;
		bitsel = BitSel_lasersensor3dsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Laser Sensor 3D";		
		
	}
	if(dynamic_cast<Joint *>(pos))
	{
		pointer.joint = dynamic_cast<Joint*>(pos);
		tipo = N_Joint;
		bit = Bit_joint;
		bitsel = BitSel_joint;
		name = wxString(pos->getName());
		if(name.empty())name = "Joint";
		
	}
   //Ahora las clases específicas o simples que no tienen herencia multiple
	
	
	
	if(dynamic_cast<SimpleJoint *>(pos))
	{
		pointer.simplejoint = dynamic_cast<SimpleJoint*>(pos);
		tipo = N_SimpleJoint;
		bit = Bit_simplejoint;
		bitsel = BitSel_simplejoint;
		name = wxString(pos->getName());
		if(name.empty())name = "Simple Joint";
		
	}
	else if(dynamic_cast<CylindricalPart *>(pos))
	{
		pointer.cylindricalpart = dynamic_cast<CylindricalPart*>(pos);
		tipo = N_CylindricalPart;
		bit = Bit_cylindricalpart;
		bitsel = BitSel_cylindricalpart;
		name = wxString(pos->getName());
		if(name.empty())name = "Cylinder";
		menus.menu_design = true;
		
	}

	else if(dynamic_cast<SpherePart *>(pos))
	{
		pointer.spherepart = dynamic_cast<SpherePart*>(pos);
		tipo = N_SpherePart;
		bit = Bit_spherepart;
		bitsel = BitSel_spherepart;
		name = wxString(pos->getName());
		if(name.empty())name = "Sphere";
		menus.menu_design = true;
	}
	else if(dynamic_cast<PrismaticPart *>(pos))
	{
		pointer.prismaticpart = dynamic_cast<PrismaticPart *>(pos);
		tipo = N_PrismaticPart;
		bit = Bit_prismaticpart;
		bitsel = BitSel_prismaticpart;
		name = wxString(pos->getName());
		if(name.empty())name = "Prism";
		menus.menu_design = true;
	
	}
	else if(dynamic_cast<NemoLaserSensor3DSim *>(pos))
	{
		pointer.nemolasersensor3dsim = dynamic_cast<NemoLaserSensor3DSim *>(pos);
		tipo = N_NemoLaserSensor3DSim;
		bit = Bit_nemolasersensor3dsim;
		bitsel = BitSel_nemolasersensor3dsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Nemo Laser Sensor";
		
	}
	
	else if(dynamic_cast<LMS200Sim *>(pos))
	{
		pointer.lms200sim = dynamic_cast<LMS200Sim*>(pos);
		tipo = N_LMS200Sim;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		
		if(name.empty())name = "Sick LMS200";
		
	}
	else if(dynamic_cast<LMS100Sim *>(pos))
	{
		pointer.lms100sim = dynamic_cast<LMS100Sim*>(pos);
		tipo = N_LMS100Sim;
		bit = Bit_lms100sim;
		bitsel = BitSel_lms100sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		if(name.empty())name = "Sick LMS100";
		
	}
	else if(dynamic_cast<FaceSetPart *>(pos))
	{
		pointer.facesetpart = dynamic_cast<FaceSetPart*>(pos);
		tipo = N_FaceSetPart;
		bit = Bit_facesetpart;
		bitsel = BitSel_facesetpart;
		name = wxString(pos->getName());
		if(name.empty())name = "Face Set";
	
	}
	else if(dynamic_cast<PatrolbotSim *>(pos))
	{
		pointer.patrolbotsim = dynamic_cast<PatrolbotSim*>(pos);
		tipo = N_PatrolbotSim;
		//bit = Bit_patrolbotsim;
		//bitsel = BitSel_patrolbotsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Patrol Bot";
	
	}
	else if(dynamic_cast<Pioneer3ATSim *>(pos))
	{
		pointer.pioneer3atsim = dynamic_cast<Pioneer3ATSim *>(pos);
		tipo = N_Pioneer3ATSim;
		bit = Bit_pioneer3atsim;
		bitsel = BitSel_pioneer3atsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Pioneer3AT";
		
	}
	else if(dynamic_cast<PowerCube70Sim *>(pos))
	{
		pointer.powercube70sim = dynamic_cast<PowerCube70Sim *>(pos);
		tipo = N_PowerCube70Sim;
		bit = Bit_powercube70sim;
		bitsel = BitSel_powercube70sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		if(name.empty())name = "Power Cube";
	
	}
	
	else if(dynamic_cast<MeshPart *>(pos))
	{
		pointer.meshpart = dynamic_cast<MeshPart *>(pos);
		tipo = N_MeshPart;
		bit = Bit_meshpart;
		bitsel = BitSel_meshpart;
		name = wxString(pos->getName());
		if(name.empty())name = "Meshpart";
		
	}
	else if(dynamic_cast<AdeptOneSim *>(pos))
	{
		pointer.adeptone = dynamic_cast<AdeptOneSim *>(pos);
		tipo = N_AdeptOneSim;
		bit = Bit_adeptonesim;
		bitsel = BitSel_adeptonesim;
		name = wxString(pos->getName());
		if(name.empty())name = "Scara Adept One";

		
	}
	else if(dynamic_cast<PersonSim *>(pos))
	{
		pointer.personSim = dynamic_cast<PersonSim*>(pos);
		tipo = N_PersonSim;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		
		if(name.empty())name = "Person";
		
	}
	else if(dynamic_cast<QuadrotorSim *>(pos))
	{
		pointer.quadrotorSim = dynamic_cast<QuadrotorSim*>(pos);
		tipo = N_QuadrotorSim;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		
		if(name.empty())name = "Quadrotor";
		
	}
	else if(dynamic_cast<CameraSim *>(pos))
	{
		pointer.cameraSim = dynamic_cast<CameraSim*>(pos);
		tipo = N_CameraSim;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		
		if(name.empty())name = "Camera";
		
	}
	else if(dynamic_cast<KinectSim *>(pos))
	{
		pointer.kinectSim = dynamic_cast<KinectSim*>(pos);
		tipo = N_KinectSim;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		
		if(name.empty())name = "Kinect";
		
	}
	else if(dynamic_cast<MobileRobot *>(pos))
	{
		pointer.mobileRobot = dynamic_cast<MobileRobot*>(pos);
		tipo = N_MobileRobot;
		bit = Bit_lms200sim;
		bitsel = BitSel_lms200sim;
		name = wxString(pos->getName());
		menus.menu_connection=true;
		
		if(name.empty())name = "MobileRobot";
	
	}
	else if(dynamic_cast<AseaIRB2000Sim *>(pos))
	{
		pointer.aseaIRB2000Sim = dynamic_cast<AseaIRB2000Sim *>(pos);
		tipo = N_AseaIRB2000;
		bit = Bit_robotsim;
		bitsel = BitSel_robotsim;
		name = wxString(pos->getName());
		if(name.empty())name = "Asea IRB2000";
		
	}


	else return;

	pointer.positionableentity->setName(name.ToStdString());
	return;
}

NodeTree::NodeTree(SimulatedWorld *simu)
{
	simuWorld=simu;
	menus.menu_world = true;
	menus.menu_composed = false;
	menus.menu_positionable = false;
	menus.menu_solid = false;
	menus.menu_wheeledbased = false;
	menus.menu_robotsim=false;
	menus.menu_meshpart=false;
	menus.menu_laser = false;
	menus.menu_simplejoint = false;
	pointer.world = simu->getWorld();
	tipo = N_World;
	bit = Bit_world;
	bitsel = BitSel_world;
	name = simu->getName();
	if(name.empty()) name = "World Special";
}