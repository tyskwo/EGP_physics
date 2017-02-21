///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////	EGP Graphics Framework Demo
////	By Dan Buckstein
////	January 2017
////
////	This file was modified by Wednesday-David Hartman, Ty Wood with permission from author.
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



//things left to do:

//   add drag to particles

//   edit emission type
//   edit number to emit
//   edit ease of lifetime types

//	 comment InputManager, SaveManager






#include "egpfwDemo.h"

//-----------------------------------------------------------------------------
// external includes

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// third party math lib

// OpenGL
#ifdef _WIN32
    #include "GL/glew.h"
#else	// !_WIN32
    #include <OpenGL/gl3.h>
#endif	// _WIN32

// DevIL (images)
#include "IL/ilut.h"


//-----------------------------------------------------------------------------
// framework includes

#include "egpfw/egpfw.h"

#include "Shader.h"
#include "Model.h"
#include "Geometry.h"


#ifdef _WIN32
    #include "gphysics/Mover.h"
    #include "particledomain\ParticleSystem.h"
	#include "utils\SaveManager.h"
	#include "utils\InputManager.h"
	#include "utils\Locator.h"
#else
    #include <string>
    #include "Mover.h"
    #include "ParticleSystem.h"
	#include "SaveManager.h"
	#include "InputManager.h"
	#include "Locator.h"
#endif










//-----------------------------------------------------------------------------
// globals


//-----------------------------------------------------------------------------

// window info
unsigned int win_x = 0;
unsigned int win_y = 0;
unsigned int win_w = 1280;
unsigned int win_h = 720;
const unsigned int viewport_b = 4;
const unsigned int viewport_tb = viewport_b + viewport_b;
const int viewport_nb = -(int)viewport_b;
unsigned int viewport_tw = win_w + viewport_tb;
unsigned int viewport_th = win_h + viewport_tb;
float win_aspect;
float fovy = 1.0f, znear = 0.5f, zfar = 50.0f;
float maxClipDist = 0.0f, minClipDist = 0.0f;

// update flag: play speed as a percentage
unsigned int playing = 0;
unsigned int playrate = 100;


// timers
egpTimer renderTimer[1], secondTimer[1];

// controls
egpMouse mouse[1];
egpKeyboard keybd[1];


// general: camera's view matrix and projection matrix
cbtk::cbmath::mat4 viewMatrix, projectionMatrix, viewProjMat;
// camera controls
float cameraElevation = -0.5f, cameraAzimuth = 0.0f;
float cameraRotateSpeed = 0.1f, cameraMoveSpeed = 1.0f, cameraDistance = 8.0f;
cbtk::cbmath::vec4 cameraPosWorld(0.0f, 6.0f, cameraDistance, 1.0f), deltaCamPos;


// particle system path values
cbmath::vec3 ps_pathStart(-5.0f,0.0f,0.0f), ps_pathEnd(5.0f,0.0f,0.0f);
float ps_pathTime = 3.0f, ps_pathCurrentTime = 0.0f;







//-----------------------------------------------------------------------------
// graphics-related data and handles


// test vertex buffers for built-in primitive data
enum VAOIndex
{
	axesVAO, 

	sphere8x6VAO, sphere32x24VAO, cubeVAO, cubeWireVAO, cubeIndexedVAO, cubeWireIndexedVAO,
    
    quadVAO,
    
    octahedronVAO, isocahedronVAO,

//-----------------------------
	vaoCount
};
enum VBOIndex
{
	axesVBO, 

	sphere8x6VBO, sphere32x24VBO, cubeVBO, cubeWireVBO, cubeIndexedVBO, cubeWireIndexedVBO,
    
    quadVBO,
    
    octahedronVBO, isocahedronVBO,
    
//-----------------------------
	vboCount
};
enum IBOIndex
{
	cubeIndexedIBO, cubeWireIndexedIBO,
    
    octahedronIBO, isocahedronIBO,

//-----------------------------
	iboCount
};

egpVertexArrayObjectDescriptor  vao[vaoCount] = { 0 };
egpVertexBufferObjectDescriptor vbo[vboCount] = { 0 };
egpIndexBufferObjectDescriptor  ibo[iboCount] = { 0 };



















//-----------------------------------------------------------------------------
// our game objects

// InputManager
wh::InputManager *wh_inputManager;

// movables
ParticleSystem *wh_particleSystem;
Model *wh_model;

void loadParticleData()
{
	Locator::getSaveManager()->loadData(wh_inputManager->getSaveFileSelection());
	std::cout << "loaded data from file " << wh_inputManager->getSaveFileSelection() << std::endl;
}

void initParticleData()
{
    Particle::Data particle = Locator::getSaveManager()->prepareData(wh_inputManager->getSaveFileSelection());//wh_saveManager->prepareData(wh_saveFileSelection);
	
	if (wh_particleSystem == nullptr)
	{
		wh_particleSystem = new ParticleSystem(particle, ParticleSystem::Emitter::Mode::Continuous, cbmath::v3y * 2.0f, cbmath::v3y, 5);
	}
	else
	{
		wh_particleSystem->setParticleData(particle);
	}
}

void initParticleSystem()
{
    // coals burning
    Particle::Data particle;
                                                   //v, d,   midpoint
    particle.lifespan = Particle::DeltaType<float> { 5, 5,   false };
    particle.mass     = Particle::DeltaType<float> { 1, 0.5, false };
    
    particle.velocity = Particle::DeltaType<cbmath::vec3> { cbmath::vec3(0.0f, 2.0f, 0.0f), cbmath::vec3(2.0f, 5.0f, 2.0f), true };

    
    particle.color = Particle::LifetimeType<cbmath::vec4> {  cbmath::vec4(1.0f,0.0f,0.0f,1.0f),     /* start */
                                                             cbmath::vec4(0.0f,0.0f,1.0f,0.0f),     /* end   */
                                                             TimingFunctions::CircularEaseOut };    /* ease  */
    
    
//    // smoke
//    Particle::Data particle;
//    
//    particle.lifespan = Particle::DeltaType<float> {  5,   5, false };
//    particle.mass     = Particle::DeltaType<float> { -1, 0.5, false };
//    
//    particle.velocity = Particle::DeltaType<cbmath::vec3> { cbmath::vec3(0.0f, 2.0f, 0.0f), cbmath::vec3(2.0f, 2.5f, 2.0f), true };
//    
//    
//    particle.color = Particle::LifetimeType<cbmath::vec4> {  cbmath::vec4(0.2f,0.2f,0.2f,1.0f),
//                                                             cbmath::vec4(1.0f,1.0f,1.0f,0.0f),
//                                                             TimingFunctions::CircularEaseOut };
 
    
//    // bouncy balls
//    Particle::Data particle;
//    
//    particle.lifespan = Particle::DeltaType<float> {  5, 5, false };
//    particle.mass     = Particle::DeltaType<float> { 10, 5, true  };
//    
//    particle.velocity = Particle::DeltaType<cbmath::vec3> { cbmath::vec3(0.0f, -4.0f, 0.0f), cbmath::vec3(5.0f, 2.5f, 5.0f), true };
//    
//    
//    particle.color = Particle::LifetimeType<cbmath::vec4> {  cbmath::vec4(0.33,0.5f,0.8f,1.0f),
//                                                             cbmath::vec4(0.9f,0.3f,0.5f,1.0f),
//                                                             TimingFunctions::ExponentialEaseIn };

    

    
    
    
	wh_particleSystem = new ParticleSystem(particle, ParticleSystem::Emitter::Mode::Burst, cbmath::v3y * 2.0f, cbmath::v3y, 500);
}

// quickly reset physics
void resetPhysics()
{
	initParticleData();
	if(!wh_particleSystem->isContinuous()) wh_particleSystem->emit(wh_model);
}

// update physics only
void updatePhysics(float dt)
{
	wh_particleSystem->update(dt);
}










//-----------------------------------------------------------------------------
// game functions





// access window info
unsigned int windowWidth()  { return win_w; }
unsigned int windowHeight() { return win_h; }
unsigned int windowPosX()   { return win_x; }
unsigned int windowPosY()   { return win_y; }





// set initial GL states
int initGL()
{
	const float lineWidth = 4.0f;
	const float pointSize = lineWidth;

	// backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// alpha blending
	// result = ( new*[new alpha] ) + ( old*[1 - new alpha] )
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// textures
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	// lines and points
	glLineWidth(lineWidth);
	glPointSize(pointSize);

	// done
	return 1;
}


// initialize third-party image handling using DevIL
int initIL()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	// done
	return 1;
}


// display OpenGL version
void printGLVersion() { printf("%s", glGetString(GL_VERSION)); }





//-----------------------------------------------------------------------------

// load and delete geometry
void setupGeometry()
{
	// prepare sample geometry
	// all of these are statically defined, so there is no cleanup for them
	egpInitializeCube();
	egpInitializeCubeIndexed();
	egpInitializeWireCube();
	egpInitializeWireCubeIndexed();
	egpInitializeSphere8x6();
	egpInitializeSphere32x24();


	// prepare sample geometry using VBOs
	// these should be cleaned

	// good news is they all use the same attribute descriptors, 
	//	so prepare those first
	egpAttributeDescriptor attribs[] = {
		egpCreateAttributeDescriptor(ATTRIB_POSITION, ATTRIB_VEC3, 0),
		egpCreateAttributeDescriptor(ATTRIB_NORMAL,   ATTRIB_VEC3, 0),
		egpCreateAttributeDescriptor(ATTRIB_COLOR,    ATTRIB_VEC3, 0),
		egpCreateAttributeDescriptor(ATTRIB_TEXCOORD, ATTRIB_VEC2, 0),
	};

	// low-res sphere
	attribs[0].data = egpGetSphere8x6Positions();
	attribs[1].data = egpGetSphere8x6Normals();
	attribs[2].data = egpGetSphere8x6Colors();
	attribs[3].data = egpGetSphere8x6Texcoords();
	vao[sphere8x6VAO] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetSphere8x6VertexCount(), (vbo + sphere8x6VBO), 0);

	// high-res sphere
	attribs[0].data = egpGetSphere32x24Positions();
	attribs[1].data = egpGetSphere32x24Normals();
	attribs[2].data = egpGetSphere32x24Colors();
	attribs[3].data = egpGetSphere32x24Texcoords();
	vao[sphere32x24VAO] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetSphere32x24VertexCount(), (vbo + sphere32x24VBO), 0);

	// cube
	attribs[0].data = egpGetCubePositions();
	attribs[1].data = egpGetCubeNormals();
	attribs[2].data = egpGetCubeColors();
	attribs[3].data = egpGetCubeTexcoords();
	vao[cubeVAO] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetCubeVertexCount(), (vbo + cubeVBO), 0);

	// wire cube
	attribs[0].data = egpGetWireCubePositions();
	vao[cubeWireVAO] = egpCreateVAOInterleaved(PRIM_LINES, attribs, 1, egpGetWireCubeVertexCount(), (vbo + cubeWireVBO), 0);

	// indexed cube
	attribs[0].data = egpGetCubeIndexedPositions();
	attribs[1].data = egpGetCubeIndexedNormals();
	attribs[2].data = egpGetCubeIndexedColors();
	attribs[3].data = egpGetCubeIndexedTexcoords();
	vao[cubeIndexedVAO] = egpCreateVAOInterleavedIndexed(PRIM_TRIANGLES, attribs, 4, egpGetCubeIndexedVertexCount(), (vbo + cubeIndexedVBO), INDEX_UINT, egpGetCubeIndexCount(), egpGetCubeIndices(), (ibo + cubeIndexedIBO));

	// indexed wire cube
	attribs[0].data = egpGetWireCubeIndexedPositions();
	vao[cubeWireIndexedVAO] = egpCreateVAOInterleavedIndexed(PRIM_LINES, attribs, 1, egpGetCubeIndexedVertexCount(), (vbo + cubeWireIndexedVBO), INDEX_UINT, egpGetWireCubeIndexCount(), egpGetWireCubeIndices(), (ibo + cubeWireIndexedIBO));


	// axes
	const egpAttributeDescriptor axesAttribs[2] = {
		egpCreateAttributeDescriptor(ATTRIB_POSITION, ATTRIB_VEC3, egpGetAxesPositions()),
		egpCreateAttributeDescriptor(ATTRIB_COLOR, ATTRIB_VEC3, egpGetAxesColors()),
	};
	vao[axesVAO] = egpCreateVAOInterleaved(PRIM_LINES, axesAttribs, 2, egpGetAxesVertexCount(), (vbo + axesVBO), 0);
    
    
    
    
    
    
    
    // CUSTOM GEOMETRY
    
    // octahedron
    egpAttributeDescriptor octAttribs[] = {
        egpCreateAttributeDescriptor(ATTRIB_POSITION, ATTRIB_VEC3, Octahedron::getPositions()),
        egpCreateAttributeDescriptor(ATTRIB_COLOR,    ATTRIB_VEC3, Octahedron::getColors())
    };
    
    vao[octahedronVAO] = egpCreateVAOInterleavedIndexed(PRIM_TRIANGLES, octAttribs, 2, Octahedron::getVertexCount(), vbo+octahedronVBO,
                                                        INDEX_UINT, Octahedron::getIndexCount(), Octahedron::getIndeces(), ibo+octahedronIBO);
    
    // isocahedron
    egpAttributeDescriptor isoAttribs[] = {
        egpCreateAttributeDescriptor(ATTRIB_POSITION, ATTRIB_VEC3, Isocahedron::getPositions()),
        egpCreateAttributeDescriptor(ATTRIB_COLOR,    ATTRIB_VEC3, Isocahedron::getColors()),
        egpCreateAttributeDescriptor(ATTRIB_NORMAL,   ATTRIB_VEC3, Isocahedron::getNormals()),
    };
    
    vao[isocahedronVAO] = egpCreateVAOInterleavedIndexed(PRIM_TRIANGLES, isoAttribs, 2, Isocahedron::getVertexCount(), vbo+isocahedronVBO,
                                                        INDEX_UINT, Isocahedron::getIndexCount(), Isocahedron::getIndeces(), ibo+isocahedronIBO);
}

void deleteGeometry()
{
	// delete VAOs first (because referencing), then VBOs and IBOs
	unsigned int i;
	for (i = 0; i < vaoCount; ++i) egpReleaseVAO(vao + i);
	for (i = 0; i < vboCount; ++i) egpReleaseVBO(vbo + i);
	for (i = 0; i < iboCount; ++i) egpReleaseIBO(ibo + i);
}










// setup and delete shaders
void setupShaders()
{
    // activate a VAO for validation (automatically deactivated in shader constructor)
    egpActivateVAO(vao);

	std::string vertShaderPath = "";
	std::string fragShaderPath = "";


#ifdef _WIN32
    vertShaderPath = "../../../../resource/glsl/4x/vs/passColor_vs4x.glsl";
    fragShaderPath = "../../../../resource/glsl/4x/fs/drawColor_fs4x.glsl";
#else
    vertShaderPath = "../../../../../../../../resource/glsl/4x/vs/passColor_vs4x.glsl";
    fragShaderPath = "../../../../../../../../resource/glsl/4x/fs/drawColor_fs4x.glsl";
#endif


	Shader* shader = new Shader(vertShaderPath.c_str(), fragShaderPath.c_str());

	
    
	wh_model = new Model(shader, vao+isocahedronVAO);

	egpActivateVAO(0);
}

void deleteShaders()
{
    delete wh_model;
	wh_model = nullptr;
}










// restart all timers and time counters
void resetTimers()
{
	const double renderFPS = 30.0;
	const double secondFPS = 1.0;

	egpTimerSet(renderTimer, renderFPS);
	egpTimerStart(renderTimer);

	egpTimerSet(secondTimer, secondFPS);
	egpTimerStart(secondTimer);
}


// configure viewport for correctly drawing to the back buffer
void drawToBackBuffer(int x, int y, unsigned int w, unsigned int h)
{
	// tell OpenGL to draw directly to back buffer
	glDrawBuffer(GL_BACK);

	// reset buffer tests (we're only using color and depth)
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	// reset viewport with borders clipped
	glViewport(x, y, w, h);
}





//-----------------------------------------------------------------------------

// update camera
void updateCameraControlled(float dt, egpMouse *mouse)
{
	// TRACK MOUSE AND KEYS TO CONTROL CAMERA: 
	// remember, the view transform is the INVERSE of the camera's model transform, 
	//	so we can start by making a proper camera transform then invert it
	// 1. compute deltas (velocities)
	// 2. integrate!
	// 3. update view matrix (camera's transformation)
	if (egpMouseIsButtonDown(mouse, 0))
	{
		cameraElevation -= (float)egpMouseDeltaY(mouse) * dt * cameraRotateSpeed;
		cameraAzimuth -= (float)egpMouseDeltaX(mouse) * dt * cameraRotateSpeed;
	}

	viewMatrix = cbtk::cbmath::makeRotationEuler4ZYX(cameraElevation, cameraAzimuth, 0.0f);

	// apply current rotation to our movement vector so that "forward" is the direction the camera is facing
    if(!egpKeyboardIsKeyDown(keybd, ' '))
    {
        deltaCamPos.set(
            (float)egpKeyboardDifference(keybd, 'd', 'a'),
            (float)egpKeyboardDifference(keybd, 'e', 'q'),
            (float)egpKeyboardDifference(keybd, 's', 'w'),
            0.0f);
    }
	deltaCamPos = viewMatrix * deltaCamPos;

	cameraPosWorld += cbtk::cbmath::normalize(deltaCamPos) * dt * cameraMoveSpeed;
}

void updateCameraOrbit(float dt)
{
	// force camera to orbit around center of world
	cameraAzimuth += dt * cameraRotateSpeed;

	viewMatrix = cbtk::cbmath::makeRotationEuler4ZYX(-0.1f, cameraAzimuth, 0.0f);

	cameraPosWorld.set(sinf(cameraAzimuth)*cameraDistance, 1.0f, cosf(cameraAzimuth)*cameraDistance, 1.0f);
}

// this function allows the user to control the movement of the particle system.
// written by: Ty
void updateParticleControl(float dt)
{
    if(egpKeyboardIsKeyDown(keybd, ' ') && !wh_inputManager->isPathControlled())
    {
        cbmath::vec4 delta;
        
        delta.set((float)egpKeyboardDifference(keybd, 'd', 'a'),
                  (float)egpKeyboardDifference(keybd, 'e', 'q'),
                  (float)egpKeyboardDifference(keybd, 's', 'w'), 0.0f);
        
        delta = viewMatrix * delta;
        
        wh_particleSystem->updatePositionDelta(cbmath::normalize(delta.xyz) * dt);
    }

    if(wh_inputManager->isPathControlled())
    {
        ps_pathCurrentTime += dt;
        wh_particleSystem->updatePositionAbsolute(Eases::lerp(ps_pathStart, ps_pathEnd, ps_pathCurrentTime/ps_pathTime, TimingFunctions::ExponentialEaseInOut));
        
        printf("%f\n", ps_pathCurrentTime);
        
        if(ps_pathCurrentTime >= ps_pathTime)
        {
            auto temp = ps_pathStart;
            ps_pathStart = ps_pathEnd;
            ps_pathEnd = temp;
            
            ps_pathCurrentTime = 0.0f;
        }
    }
}





//-----------------------------------------------------------------------------

// initialize game objects
// mainly for good memory management, handling ram and vram
int initGame()
{
	// setup framebuffers
	// don't bother with this here actually... see window resize callback
	//	setupFramebuffers();

	// setup geometry
	setupGeometry();

	setupShaders();

	// physics
#ifdef _WIN32
	SaveManager *saveManager = new SaveManager("..\\..\\..\\..\\source\\egpfwDemo\\utils\\");
	Locator::provide(saveManager);
#else
	SaveManager *saveManager = new SaveManager("../../../../../../../../source/egpfwDemo/utils/");
	Locator::provide(saveManager);
#endif
    
	wh_inputManager = new wh::InputManager();

	loadParticleData();
	//initParticleSystem();
	resetPhysics();


	// other
	egpSetActiveMouse(mouse);
	egpSetActiveKeyboard(keybd);

	// done
	return 1;
}


// destroy game objects (mem)
int termGame()
{
	// TODO: CLEANUP PARTICLE STUFF ****
	delete wh_particleSystem;
	wh_particleSystem = nullptr;

	Locator::getSaveManager()->saveData(wh_inputManager->getSaveFileSelection());
	Locator::cleanup();

	delete wh_inputManager;
	wh_inputManager = nullptr;

	// good practice to do this in reverse order of creation
	//	in case something is referencing something else

	deleteShaders();
	deleteGeometry();

	// done
	return 1;
}





// output controls
void displayControls()
{
	printf("\n-------------------------------------------------------");
	printf("\n CONTROLS: \n");
	printf("\n wasdqe = camera movement");
	printf("\n ~` = display controls");
	printf("\n o = toggle slow-mo playback for all");
	printf("\n p = toggle play/pause for all");

	printf("\n y = reset physics");

	printf("\n\n hold z + 1, 2, 3, 4 = save data to one of the data files");
	printf("\n hold x + 1, 2, 3, 4 = load data to one of the data files");

	printf("\n\n c, v, l, m = edit color, velocity, lifespan, or mass");
	printf("\n 1, 2, 3, 4 = choose xyzw or rgba when applicable");
	printf("\n g = toggle between editing parameter delta or value");
	printf("\n click and drag RMB = adjust selected parameter");

	printf("\n-------------------------------------------------------\n");
}



// process input each frame
void handleInputState()
{
	// display controls
	if (egpKeyboardIsKeyPressed(keybd, '`') ||
		egpKeyboardIsKeyPressed(keybd, '~'))
		displayControls();

	// slowmo: play back at 25%
	if (egpKeyboardIsKeyPressed(keybd, 'o'))
		playrate = (playrate != 100) ? 100 : 25;

	// pause/play
	if (egpKeyboardIsKeyPressed(keybd, 'p'))
		playing = 1 - playing;


	// reset physics
	if (egpKeyboardIsKeyPressed(keybd, 'y'))
		resetPhysics();




	//-----------------------------------------------------------------------------
	// adjustable parameters
	
	wh_inputManager->update(keybd, mouse, win_w);



	// finish by updating input state
	egpMouseUpdate(mouse);
	egpKeyboardUpdate(keybd);
}





// game update to perform each frame
// update game state using the time since the last update (dt) in seconds
void updateGameState(float dt)
{
	// update camera
	updateCameraControlled(dt, mouse);
	//	updateCameraOrbit(dt);
    
    updateParticleControl(dt);

	// update view matrix
	// 'c3' in a 4x4 matrix is the translation part
	viewMatrix.c3 = cameraPosWorld;
	viewMatrix = cbtk::cbmath::transformInverseNoScale(viewMatrix);
	viewProjMat = projectionMatrix * viewMatrix;


	// update game objects
	// scale time first
	dt *= (float)(playrate * playing) * 0.01f;

	// ****update objects here
	{
		updatePhysics(dt);
        
        if(wh_particleSystem->isContinuous()) wh_particleSystem->emit(wh_model);
	}
}





// draw frame
// DRAWING AND UPDATING SHOULD BE SEPARATE (good practice)
void renderGameState()
{
    drawToBackBuffer(viewport_nb, viewport_nb, viewport_tw, viewport_th);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// render particles
	wh_particleSystem->render(viewProjMat, cameraPosWorld);



    // force draw in front of everything
    glDisable(GL_DEPTH_TEST);

	// disable program (to draw the world axes)
	egpActivateProgram(0);

    // center of world (this is useful to see where the origin is and how big one unit is)
    egpfwDrawAxesImmediate(viewProjMat.m, 0);

    // done
    glEnable(GL_DEPTH_TEST);
}





//-----------------------------------------------------------------------------
// callbacks

// process everything
int idle()
{
	int ret = 0;
	// timer checks
	// pro tip: see what happens if you don't do the 'if'  ;)

	// trigger render if it is time
    if (egpTimerUpdate(renderTimer))
	{
		updateGameState(renderTimer->dt);
		handleInputState();
		renderGameState();

        ret = 1;
	}

//    if (egpTimerUpdate(secondTimer))
//	{
//		printf("\n %u fps", renderTimer->ticks / secondTimer->ticks);
//	}

	return ret;
}





// window closed
void onCloseWindow() { termGame(); }



// window resized
#ifdef _WIN32
	void onResizeWindow(int w, int h)
#else
	void onResizeWindow(int w, int h, int scale)
#endif
{
	// set new sizes
	win_w = w;
	win_h = h;
	win_aspect = ((float)w) / ((float)h);

	// calculate total viewport size
#ifdef _WIN32
	int scale = 1;
#endif
	
	viewport_tw = w*scale + viewport_tb;
	viewport_th = h*scale + viewport_tb;

	// update projection matrix
	projectionMatrix = cbtk::cbmath::makePerspective(fovy, win_aspect, znear, zfar);
	viewProjMat = projectionMatrix * viewMatrix;

	// update max clipping distance using pythagorean theorem 
	//	to far corner of frustum; the purpose of this is to 
	//	know how much space there is make the most of (skybox)
	{
		const float tf = zfar*tanf(fovy*0.5f);
		const float rf = tf*win_aspect;
		maxClipDist = (float)((unsigned int)(sqrtf(rf*rf + tf*tf + zfar*zfar)));
		minClipDist = (float)((unsigned int)(sqrtf(zfar*zfar / 3.0f)));
	}
}



// window moved
void onPositionWindow(int x, int y)
{
	win_x = x;
	win_y = y;
}
