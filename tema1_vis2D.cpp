#include "lab_m1/tema1/tema1_vis2D.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1_Vis2D::Tema1_Vis2D()
{
}


Tema1_Vis2D::~Tema1_Vis2D()
{
}

void Tema1_Vis2D::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    Mesh* square1 = object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
    AddMeshToList(square1);

	zoomX = zoomY = zoomZ = 0;
	scaleX = scaleY = 1;
}


// 2D visualization matrix
glm::mat3 Tema1_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->SetPosition(glm::vec3(0 + zoomX, 0 + zoomY, 50 + zoomZ));
	GetSceneCamera()->Update();
}


void Tema1_Vis2D::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1_Vis2D::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    viewSpace = ViewportSpace(0, 0, resolution.x / 2, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix);

    // The viewport is now the right half of the window

    viewSpace = ViewportSpace(resolution.x / 2, 0, resolution.x / 2, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // Compute uniform 2D visualization matrix

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix);
}


void Tema1_Vis2D::FrameEnd()
{
}


void Tema1_Vis2D::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(0, 0) * transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(3, 0) * transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(1.5, 1.5) * transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(0, 3) * transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(3, 3) * transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_LEFT_SHIFT) && !window->MouseHold(1)) {
		viewSpeed = 700;
		zoomSpeed = 7;
	}
	else {
		viewSpeed = 300;
		zoomSpeed = 3;
	}

    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)
	if (window->KeyHold(GLFW_KEY_W) && !window->MouseHold(1)) {
		logicSpace.y += deltaTime * zoomSpeed;
		//zoomY -= deltaTime * viewSpeed;
	}
	if (window->KeyHold(GLFW_KEY_S) && !window->MouseHold(1)) {
		logicSpace.y -= deltaTime * zoomSpeed;
		//zoomY += deltaTime * viewSpeed;
	}
	if (window->KeyHold(GLFW_KEY_A) && !window->MouseHold(1)) {
		logicSpace.x += deltaTime * zoomSpeed;
		//zoomX += deltaTime * viewSpeed;
	}
	if (window->KeyHold(GLFW_KEY_D) && !window->MouseHold(1)) {
		logicSpace.x -= deltaTime * zoomSpeed;
		//zoomX -= deltaTime * viewSpeed;
	}
	
    // TODO(student): Zoom in and zoom out logic window with Z and X
	if (window->KeyHold(GLFW_KEY_Z) && !window->MouseHold(1)) {
		logicSpace.width -= deltaTime * zoomSpeed;
		logicSpace.height -= deltaTime * zoomSpeed;

		logicSpace.x += deltaTime * zoomSpeed * .5f;
		logicSpace.y += deltaTime * zoomSpeed * .5f;

		/*zoomZ += deltaTime * viewSpeed;
		scaleX += deltaTime;
		scaleY += deltaTime;*/
	}
	if (window->KeyHold(GLFW_KEY_X) && !window->MouseHold(1)) {
		logicSpace.height += deltaTime * zoomSpeed;
		logicSpace.width += deltaTime * zoomSpeed;

		logicSpace.x -= deltaTime * zoomSpeed * .5f;
		logicSpace.y -= deltaTime * zoomSpeed * .5f;
		/*zoomZ -= deltaTime * viewSpeed;
		scaleX -= deltaTime;
		scaleY -= deltaTime;*/
	}
}


void Tema1_Vis2D::OnKeyPress(int key, int mods)
{
}


void Tema1_Vis2D::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
