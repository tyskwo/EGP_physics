///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////	EGP Graphics Framework Demo
////	By Dan Buckstein
////	January 2017
////
////	****Modified by: ______________________________________________________
////	^^^^
////	NOTE: 4 asterisks anywhere means something you will have to either 
////		modify or complete to get it working! Good practice for starters: 
////		write your name beside "modified by" above!
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "egpfwDemo.h"

//-----------------------------------------------------------------------------
// external includes

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// third party math lib
#include "cbmath/cbtkMatrix.h"

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


#include "gphysics/egpfwMover.h"
#include "gphysics/egpfwForces.h"
#include "gphysics/egpfwShapes.h"
#include "gphysics/egpfwCollisions.h"


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
float fovy = 1.0f, znear = 0.5f, zfar = 5000.0f;
float maxClipDist = 0.0f, minClipDist = 0.0f;

// update flag: play speed as a percentage
unsigned int playing = 0;
unsigned int playrate = 100;


// timers
egpTimer physicsTimer[1], renderTimer[1], secondTimer[1];

// controls
egpMouse mouse[1];
egpKeyboard keybd[1];


// general: camera's view matrix and projection matrix
cbtk::cbmath::mat4 viewMatrix, projectionMatrix, viewProjMat;
// camera controls
float cameraElevation = 0.0f, cameraAzimuth = 0.0f;
float cameraRotateSpeed = 0.1f, cameraMoveSpeed = 4.0f, cameraDistance = 16.0f;
cbtk::cbmath::vec4 cameraPosWorld(0.0f, 0.0f, cameraDistance, 1.0f), deltaCamPos;


// colors
const cbmath::vec4 RED(1.0f, 0.0f, 0.0f, 1.0f), GREEN(0.0f, 1.0f, 0.0f, 1.0f), BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const cbmath::vec4 ORANGE(1.0f, 0.5f, 0.0f, 1.0f), LIME(0.0f, 1.0f, 0.5f, 1.0f), PURPLE(0.5f, 0.0f, 1.0f, 1.0f);
const cbmath::vec4 WHITE(1.0f);



//-----------------------------------------------------------------------------
// graphics-related data and handles
// good practice: default values for everything


// test vertex buffers for built-in primitive data
enum ModelIndex
{
	axesModel, 

	sphere8x6Model, sphere32x24Model, cubeModel, cubeWireModel, cubeIndexedModel, cubeWireIndexedModel,

	contactModel, 

//-----------------------------
	modelCount
};
enum IBOIndex
{
	cubeIndexedIBO, cubeWireIndexedIBO,

	contactIBO, 

//-----------------------------
	iboCount
};

egpVertexArrayObjectDescriptor vao[modelCount] = { 0 };
egpVertexBufferObjectDescriptor vbo[modelCount] = { 0 };
egpIndexBufferObjectDescriptor ibo[iboCount] = { 0 };


// shaders
enum GLSLProgramIndex
{
	solidColorProgram, 

//-----------------------------
	glslProgramCount
};
enum GLSLCommonUniformIndex
{
	mvpLocation, 
	solidColorLocation, 

//-----------------------------
	glslCommonUniformCount
};
egpProgram glslProgram[glslProgramCount] = { 0 };
int glslCommonUniform[glslProgramCount][glslCommonUniformCount] = { -1 };



//-----------------------------------------------------------------------------
// our game objects


// gravity constant: ACCELERATION due to gravity
const cbmath::vec3 gravityAccel(0.0f, -9.81f, 0.0f);

// movables
const unsigned int numMovers = 6;
egpfwMover mover[numMovers];

// shapes
const unsigned int numSpheres = 2;
const unsigned int numBoxes = 4;
const egpfwSphere sphere[numSpheres] = { {1.0}, {2.0} };
const egpfwBox box[numBoxes] = { {1.0, 2.0, 3.0}, {2.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {0.5, 2.0, 1.5} };

// colliders
egpfwBoundingVolumeSphere sphereVolume[numSpheres] = {
	createSphereVolume(mover + 0, sphere + 0), 
	createSphereVolume(mover + 1, sphere + 1), 
};
egpfwBoundingVolumeBox boxVolume[numBoxes] = { 
	createBoxVolume(mover + 2, box + 0, 1), 
	createBoxVolume(mover + 3, box + 1, 1), 
	createBoxVolume(mover + 4, box + 2, 0), 
	createBoxVolume(mover + 5, box + 3, 0), 
};

// collision flags
int collision[numMovers] = { 0 };

// contacts to be drawn
#define MAX_COLLISIONS 64
#define MAX_CONTACTS MAX_COLLISIONS*4
egpfwCollisionContact contact[MAX_CONTACTS];
unsigned int numContacts = 0;


// update physics only
void updatePhysics(float dt)
{
	unsigned int i, j, k, end, numCollisions = numContacts = 0;
	egpfwMover *m;
	egpfwBoundingVolumeSphere *sbv, *sbv2;
	egpfwBoundingVolumeBox *bbv, *bbv2;
	egpfwCollision collisionFlags, collisionDescriptors[MAX_COLLISIONS], *collisionDesc;

	dt *= (float)(playrate * playing) * 0.01f;

	// basic physics update: 
	//	-> integrate
	//	-> update anything that has to do with graphics
	for (i = 0, m = mover; i < numMovers; ++i, ++m)
	{
		// physics
		updateMoverFirstOrder(m, dt);

		// graphics
		updateMoverGraphics(m);
	}


	// update collision volumes
	for (i = 0, bbv = boxVolume; i < numBoxes; ++i, ++bbv)
		updateBoxVolume(bbv);


	// reset collision flags
	memset(collision, 0, sizeof(collision));


	// test collisions
	// sphere-sphere
	for (i = 0, end = numSpheres - 1, sbv = sphereVolume; i < end; ++i, ++sbv)
		for (j = i + 1, sbv2 = sbv + 1; j < numSpheres; ++j, ++sbv2)
		{
			collisionFlags = testCollisionSphereSphere(sbv, sbv2);
			if (collisionFlags.colliderA != 0)
			{
				collision[sbv->mover - mover] = collision[sbv2->mover - mover] = 1;
				collisionDescriptors[numCollisions++] = collisionFlags;
				for (k = 0; k < collisionFlags.numContacts; ++k)
					contact[numContacts++] = collisionFlags.contact[k];
			}
		}

	// sphere-box
	for (i = 0, sbv = sphereVolume; i < numSpheres; ++i, ++sbv)
		for (j = 0, bbv = boxVolume; j < numBoxes; ++j, ++bbv)
		{
			collisionFlags = testCollisionSphereBox(sbv, bbv);
			if (collisionFlags.colliderA != 0)
			{
				collision[sbv->mover - mover] = collision[bbv->mover - mover] = 1;
				collisionDescriptors[numCollisions++] = collisionFlags;
				for (k = 0; k < collisionFlags.numContacts; ++k)
					contact[numContacts++] = collisionFlags.contact[k];
			}
		}

	// box-box
	for (i = 0, end = numBoxes - 1, bbv = boxVolume; i < end; ++i, ++bbv)
		for (j = i + 1, bbv2 = bbv + 1; j < numBoxes; ++j, ++bbv2)
		{
			collisionFlags = testCollisionBoxBox(bbv, bbv2);
			if (collisionFlags.colliderA != 0)
			{
				collision[bbv->mover - mover] = collision[bbv2->mover - mover] = 1;
				collisionDescriptors[numCollisions++] = collisionFlags;
				for (k = 0; k < collisionFlags.numContacts; ++k)
					contact[numContacts++] = collisionFlags.contact[k];
			}
		}


	// PROCESS COLLISIONS
	for (i = 0, collisionDesc = collisionDescriptors; i < numCollisions; ++i, ++collisionDesc)
	{
		// ****
	}
}

// quickly reset physics
void resetPhysics()
{
	// sphere
	mover[0] = { cbmath::m4Identity, cbmath::v3zero, cbmath::vec3(0.0f, 3.0f, 0.0f), gravityAccel };
	setMass(mover + 0, 1.0f);

	// other sphere
	mover[1] = { cbmath::m4Identity, cbmath::vec3(-6.0f, -2.0f, 0.0f), cbmath::vec3(4.0f, 6.0f, 0.0f), gravityAccel };
	setMass(mover + 1, 4.0f);

	// axis-aligned box
	mover[2] = { cbmath::m4Identity, cbmath::vec3(8.0f, 4.0f, 2.0f), cbmath::vec3(-3.0f, 2.0f, -1.0f), gravityAccel };
	setMass(mover + 2, 6.0f);

	// other axis-aligned box
	mover[3] = { cbmath::m4Identity, cbmath::vec3(0.0f, 8.0f, 0.0f), cbmath::v3zero, gravityAccel };
	setMass(mover + 3, 2.0f);

	// another box, not axis-aligned
	mover[4] = { cbmath::makeRotationEuler4XYZ(3.0f, 2.0f, 2.0f), cbmath::vec3(1.0f, 3.0f, 2.0f), cbmath::v3z, gravityAccel };
	setMass(mover + 4, 1.0f);

	// one more box, not axis-aligned
	mover[5] = { cbmath::makeRotationEuler4XYZ(-2.0f, -2.0f, 4.0f), cbmath::vec3(-2.0f, 4.0f, 1.0f), cbmath::vec3(2.0f, 2.0f, 2.0f), gravityAccel };
	setMass(mover + 5, 1.5f);


	// force physics update
	updatePhysics(0.0f);
}



//-----------------------------------------------------------------------------
// game functions

// access window info
unsigned int windowWidth()
{
	return win_w;
}

unsigned int windowHeight()
{
	return win_h;
}

unsigned int windowPosX()
{
	return win_x;
}

unsigned int windowPosY()
{
	return win_y;
}


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
void printGLVersion()
{
	printf("%s", glGetString(GL_VERSION));
}



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
		egpCreateAttributeDescriptor(ATTRIB_COLOR, ATTRIB_VEC3, 0),
		egpCreateAttributeDescriptor(ATTRIB_NORMAL, ATTRIB_VEC3, 0),
		egpCreateAttributeDescriptor(ATTRIB_TEXCOORD, ATTRIB_VEC2, 0),
	};

	// axes
	attribs[0].data = egpGetAxesPositions();
	attribs[1].data = egpGetAxesColors();
	vao[axesModel] = egpCreateVAOInterleaved(PRIM_LINES, attribs, 2, egpGetAxesVertexCount(), (vbo + axesModel), 0);

	// low-res sphere
	attribs[0].data = egpGetSphere8x6Positions();
	attribs[1].data = egpGetSphere8x6Colors();
	attribs[2].data = egpGetSphere8x6Normals();
	attribs[3].data = egpGetSphere8x6Texcoords();
	vao[sphere8x6Model] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetSphere8x6VertexCount(), (vbo + sphere8x6Model), 0);

	// high-res sphere
	attribs[0].data = egpGetSphere32x24Positions();
	attribs[1].data = egpGetSphere32x24Colors();
	attribs[2].data = egpGetSphere32x24Normals();
	attribs[3].data = egpGetSphere32x24Texcoords();
	vao[sphere32x24Model] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetSphere32x24VertexCount(), (vbo + sphere32x24Model), 0);

	// cube
	attribs[0].data = egpGetCubePositions();
	attribs[1].data = egpGetCubeNormals();
	attribs[2].data = egpGetCubeColors();
	attribs[3].data = egpGetCubeTexcoords();
	vao[cubeModel] = egpCreateVAOInterleaved(PRIM_TRIANGLES, attribs, 4, egpGetCubeVertexCount(), (vbo + cubeModel), 0);

	// wire cube
	attribs[0].data = egpGetWireCubePositions();
	vao[cubeWireModel] = egpCreateVAOInterleaved(PRIM_LINES, attribs, 1, egpGetWireCubeVertexCount(), (vbo + cubeWireModel), 0);

	// indexed cube
	attribs[0].data = egpGetCubeIndexedPositions();
	attribs[1].data = egpGetCubeIndexedNormals();
	attribs[2].data = egpGetCubeIndexedColors();
	attribs[3].data = egpGetCubeIndexedTexcoords();
	vao[cubeIndexedModel] = egpCreateVAOInterleavedIndexed(PRIM_TRIANGLES, attribs, 4, egpGetCubeIndexedVertexCount(), (vbo + cubeIndexedModel), INDEX_UINT, egpGetCubeIndexCount(), egpGetCubeIndices(), (ibo + cubeIndexedIBO));

	// indexed wire cube
	attribs[0].data = egpGetWireCubeIndexedPositions();
	vao[cubeWireIndexedModel] = egpCreateVAOInterleavedIndexed(PRIM_LINES, attribs, 1, egpGetCubeIndexedVertexCount(), (vbo + cubeWireIndexedModel), INDEX_UINT, egpGetWireCubeIndexCount(), egpGetWireCubeIndices(), (ibo + cubeWireIndexedIBO));


	// simple procedural model for contacts
	{
		const float contactPoints[] = {
			+0.1f,  0.0f,  0.0f, 
			 0.0f, +0.1f,  0.0f, 
			-0.1f,  0.0f,  0.0f, 
			 0.0f, -0.1f,  0.0f, 
			 0.0f,  0.0f, -0.1f, 
			 0.0f,  0.0f, +1.0f, 
		};
		const unsigned int contactIndices[] = {
			0, 1, 1, 2, 2, 3, 3, 0, 
			0, 4, 1, 4, 2, 4, 3, 4, 
			0, 5, 1, 5, 2, 5, 3, 5
		};
		attribs[0].data = contactPoints;
		vao[contactModel] = egpCreateVAOInterleavedIndexed(PRIM_LINES, attribs, 1, 6, (vbo + contactModel), INDEX_UINT, 24, contactIndices, (ibo + contactIBO));
	}
}

void deleteGeometry()
{
	// delete VAOs first (because referencing), then VBOs and IBOs
	unsigned int i;
	for (i = 0; i < modelCount; ++i)
	{
		egpReleaseVAO(vao + i);
		egpReleaseVBO(vbo + i);
	}
	for (i = 0; i < iboCount; ++i)
		egpReleaseIBO(ibo + i);
}


// setup GLSL shader programs
void setupShaders()
{
	egpFileInfo file[2];
	egpShader shader[2];


	egpActivateVAO(vao + sphere8x6Model);


	// solid color program
	{
		file[0] = egpLoadFileContents("../../../../resource/glsl/4x/vs/setClipPos_vs4x.glsl");
		file[1] = egpLoadFileContents("../../../../resource/glsl/4x/fs/drawSolid_fs4x.glsl");
		shader[0] = egpCreateShaderFromSource(EGP_SHADER_VERTEX, file[0].contents);
		shader[1] = egpCreateShaderFromSource(EGP_SHADER_FRAGMENT, file[1].contents);

		glslProgram[0] = egpCreateProgram();
		egpAttachShaderToProgram(glslProgram + solidColorProgram, shader + 0);
		egpAttachShaderToProgram(glslProgram + solidColorProgram, shader + 1);
		egpLinkProgram(glslProgram + solidColorProgram);
		egpValidateProgram(glslProgram + solidColorProgram);

		egpReleaseShader(shader + 0);
		egpReleaseShader(shader + 1);
		egpReleaseFileContents(file + 0);
		egpReleaseFileContents(file + 1);
	}


	// uniforms
	const char *unifName[] = {
		(const char *)("mvp"), 
		(const char *)("color"), 
	};

	egpProgram *prog;
	int *unifs;
	unsigned int i, j;
	for (i = 0; i < glslProgramCount; ++i)
	{
		prog = glslProgram + i;
		unifs = glslCommonUniform[i];
		for (j = 0; j < glslCommonUniformCount; ++j)
			unifs[j] = egpGetUniformLocation(prog, unifName[j]);
	}


	// done, deactivate
	egpActivateVAO(0);
}

void deleteShaders()
{
	unsigned int i;
	for (i = 0; i < glslProgramCount; ++i)
		egpReleaseProgram(glslProgram + i);
}


// restart all timers and time counters
void resetTimers()
{
	const double physicsFPS = 200.0;
	const double renderFPS = 30.0;
	const double secondFPS = 1.0;

	egpTimerSet(physicsTimer, physicsFPS);
	egpTimerStart(physicsTimer);

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
	deltaCamPos.set(
		(float)egpKeyboardDifference(keybd, 'd', 'a'),
		(float)egpKeyboardDifference(keybd, 'e', 'q'),
		(float)egpKeyboardDifference(keybd, 's', 'w'),
		0.0f);
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


//-----------------------------------------------------------------------------

// initialize game objects
// mainly for good memory management, handling ram and vram
int initGame()
{
	// setup geometry
	setupGeometry();

	// shaders
	setupShaders();


	// physics
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
	// good practice to do this in reverse order of creation
	//	in case something is referencing something else

	// delete shaders
	deleteShaders();

	// delete geometry
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

	printf("\n-------------------------------------------------------");
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

	// update view matrix
	// 'c3' in a 4x4 matrix is the translation part
	viewMatrix.c3 = cameraPosWorld;
	viewMatrix = cbtk::cbmath::transformInverseNoScale(viewMatrix);
	viewProjMat = projectionMatrix * viewMatrix;


	// update game objects
	// scale time first
	dt *= (float)(playrate * playing) * 0.01f;

	// update objects here
	{

	}
}


// draw frame
// DRAWING AND UPDATING SHOULD BE SEPARATE (good practice)
void renderGameState()
{
//-----------------------------------------------------------------------------
	// DRAW ALL OBJECTS - ALGORITHM: 
	//	- activate shader program if different from last object
	//	- bind texture we want to apply (skybox)
	//	- send appropriate uniforms if different from last time we used this program
	//	- call appropriate draw function, based on whether we are indexed or not

//-----------------------------------------------------------------------------

	// target back-buffer and clear
	drawToBackBuffer(viewport_nb, viewport_nb, viewport_tw, viewport_th);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TEST YOUR SHAPES
	{
		const cbmath::vec4 shapeColors[] = {
			BLUE, ORANGE, BLUE, ORANGE, LIME, PURPLE
		}, *col = shapeColors;

		// can use this to change whose space we draw relative to
		// (do not assign if using strictly world space)
		const cbmath::mat4 inverseCenterModelMatrix;// = ((boxVolume + 2)->planes + 0)->localOrientationInverse * cbmath::transformInverseNoScale((mover + 4)->modelMatrix);
		const cbmath::mat4 viewProjMatFinal = viewProjMat * inverseCenterModelMatrix;
		cbmath::mat4 mvp, modelMatrixFinal;

		unsigned int i;
		const egpfwMover *m = mover;
		const egpfwSphere *ss = sphere;
		const egpfwBox *bb = box;
		const egpfwCollisionContact *cc = contact;

		int *unifs = glslCommonUniform[solidColorProgram];
		egpActivateProgram(glslProgram + solidColorProgram);

		// draw shapes with some sort of visible outline
		// spheres
		for (i = 0; i < numSpheres; ++i, ++m, ++col, ++ss)
		{
			modelMatrixFinal = m->modelMatrix * cbmath::makeScale4((float)ss->radius);
			mvp = viewProjMatFinal * modelMatrixFinal;
			egpSendUniformFloatMatrix(unifs[mvpLocation], UNIF_MAT4, 1, 0, mvp.m);
			egpSendUniformFloat(unifs[solidColorLocation], UNIF_VEC4, 1, col->v);
			egpActivateVAO(vao + sphere8x6Model);
			egpDrawActiveVAO();
			glCullFace(GL_FRONT);
			egpSendUniformFloat(unifs[solidColorLocation], UNIF_VEC4, 1, (collision[m - mover] ? GREEN.v : RED.v));
			egpActivateVAO(vao + sphere32x24Model);
			egpDrawActiveVAO();
			glCullFace(GL_BACK);
		}

		// boxes
		for (i = 0; i < numBoxes; ++i, ++m, ++col, ++bb)
		{
			modelMatrixFinal = m->modelMatrix * cbmath::makeScale4((float)bb->width, (float)bb->height, (float)bb->depth);
			mvp = viewProjMatFinal * modelMatrixFinal;
			egpSendUniformFloatMatrix(unifs[mvpLocation], UNIF_MAT4, 1, 0, mvp.m);
			egpSendUniformFloat(unifs[solidColorLocation], UNIF_VEC4, 1, col->v);
			egpActivateVAO(vao + cubeModel);
			egpDrawActiveVAO();
			egpSendUniformFloat(unifs[solidColorLocation], UNIF_VEC4, 1, (collision[m - mover] ? GREEN.v : RED.v));
			egpActivateVAO(vao + cubeWireModel);
			egpDrawActiveVAO();
		}


		// TEST DRAW: coordinate axes at center of spaces
		//	and other line objects
		{
			cbmath::vec3 upAxis;

			// force draw in front of everything
			glDisable(GL_DEPTH_TEST);

			// contacts
			egpSendUniformFloat(unifs[solidColorLocation], UNIF_VEC4, 1, WHITE.v);
			egpActivateVAO(vao + contactModel);
			for (i = 0; i < numContacts; ++i, ++cc)
			{
				upAxis = cc->normal.y < +0.99f && cc->normal.y > -0.99f ? cbmath::v3y : cbmath::v3x;
				modelMatrixFinal = cbmath::makeFrenet4((cc->location).xyz, (cc->location - cc->normal).xyz, upAxis);
				mvp = viewProjMatFinal * modelMatrixFinal;
				egpSendUniformFloatMatrix(unifs[mvpLocation], UNIF_MAT4, 1, 0, mvp.m);
				egpDrawActiveVAO();
			}

			// done
			egpActivateProgram(0);

			// center of actual world space relative to whatever 
			//	is acting as the center
			egpfwDrawAxesImmediate(viewProjMatFinal.m, 0);

			// center of observer's world
			// (this is useful to see where the origin is relative 
			//	to the camera and how big one unit is)
			egpfwDrawAxesImmediate(viewProjMat.m, 0);

			// done
			glEnable(GL_DEPTH_TEST);
		}
	}
}



//-----------------------------------------------------------------------------
// callbacks

// process everything
int idle()
{
	int ret = 0;
	// timer checks
	// pro tip: see what happens if you don't do the 'if'  ;)

	// physics
	if (egpTimerUpdate(physicsTimer))
	{
		updatePhysics(physicsTimer->dt);
	}

	// trigger render if it is time
	if (egpTimerUpdate(renderTimer))
	{
		///////////////////////////////////////////////////////////////////////
		updateGameState(renderTimer->dt);
		handleInputState();
		renderGameState();
		///////////////////////////////////////////////////////////////////////
		ret = 1;
	}

//	if (egpTimerUpdate(secondTimer))
//	{
//		printf("\n %u frames rendered over %u seconds", renderTimer->ticks, secondTimer->ticks);
//	}

	return ret;
}

// window closed
void onCloseWindow()
{
	// clean up
	termGame();
}

// window resized
void onResizeWindow(int w, int h)
{
	// set new sizes
	win_w = w;
	win_h = h;
	win_aspect = ((float)w) / ((float)h);

	// calculate total viewport size
	viewport_tw = w + viewport_tb;
	viewport_th = h + viewport_tb;

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
