#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <string>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include "lab_extra/basic_text/basic_text.h"

using namespace std;
using namespace m1;

constexpr auto PI = 3.14159265;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	obstacle1 = Collider(rect(1275, 165, squareSide, squareSide), "obstacle1", "obstacol");
	obstacle2 = Collider(rect(55, -45, squareSide, squareSide), "obstacle2", "obstacol", glm::vec2(10, 1));
	obstacle3 = Collider(rect(-150, -300, squareSide, squareSide), "obstacle3", "obstacol", glm::vec2(1, 5));

	colliders.push_back(obstacle1);
	colliders.push_back(obstacle2);
	colliders.push_back(obstacle3);

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
	cx = (corner.x + squareSide) / 2.0f;
	cy = (corner.y + squareSide) / 2.0f;

	dX = dY = 0;

	// centrul ecranului
	translateX = window->GetResolution().x / 2 - cx;
	translateY = window->GetResolution().y / 2 - cy / 2;

	// initialize player
	player.position.x = translateX;
	player.position.y = translateY;
	player.movementSpeed = 0.0f;

	player.collider = Collider(rect(translateX, translateY, squareSide, squareSide), "Player", "player");

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

	// Initialize map limits
	mapSide = 2000;

	float mapLimitMultiplier = 2.0f;
	float squareCornerTriangleSide = 25.0f;

	/*topLimit = resolution.y * mapLimitMultiplier - squareSide - cy - squareCornerTriangleSide;
	bottomLimit = 0 - resolution.y * .5f - squareSide - squareSide - cy + squareCornerTriangleSide;

	leftLimit = 0 - resolution.x * .5f + squareSide + squareSide + squareSide;
	rightLimit = resolution.x + squareSide + squareSide + squareCornerTriangleSide;*/

	borderLeft = Collider(rect(translateX - mapSide * .5f, translateY - mapSide * .5f, squareSide, squareSide), "borderLeft", "border", glm::vec2(1, mapSide / squareSide));
	borderRight = Collider(rect(translateX + mapSide * .5f, translateY - mapSide * .5f, squareSide, squareSide), "borderRight","border" , glm::vec2(1, mapSide / squareSide));
	borderTop = Collider(rect(translateX - mapSide * .5f, translateY + mapSide * .5f, squareSide, squareSide), "borderTop","border", glm::vec2(mapSide / squareSide, 1));
	borderBottom = Collider(rect(translateX - mapSide * .5f, translateY - mapSide * .5f, squareSide, squareSide), "borderBottom","border", glm::vec2(mapSide / squareSide, 1));

	colliders.push_back(borderLeft);
	colliders.push_back(borderRight);
	colliders.push_back(borderTop);
	colliders.push_back(borderBottom);

    Mesh* square1 = object2D::CreatePlayer("playerRoot", corner, squareSide, glm::vec3(.24f, .36f, .24f));
    AddMeshToList(square1);

	Mesh* circle = object2D::CreateCircle("circle", corner, squareSide, glm::vec3(.24f, .36f, .24f), true);
	AddMeshToList(circle);

	Mesh* littleCircle = object2D::CreateCircle("littleCircle", corner, squareSide*.2f, glm::vec3(255 / 255, 85 / 255, 17 / 255), true);
	AddMeshToList(littleCircle);

	Mesh* obstacle1 = object2D::CreateSquare("obstacle1", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(obstacle1);

	Mesh* border = object2D::CreateSquare("border", corner, squareSide, glm::vec3(.36f, .86f, .26f), true);
	AddMeshToList(border);

	Mesh* bullet = object2D::CreateSquare("bullet", corner, squareSide, glm::vec3(1.0f, 1.0f, 1.0f), true);
	AddMeshToList(bullet);

	Mesh* enemy = object2D::CreateEnemy("enemy", corner, squareSide, glm::vec3(.89f, .2f, 0.0f));
	AddMeshToList(enemy);

	Mesh* healthBar = object2D::CreateSquare("healthBar", corner, squareSide, glm::vec3(1.0f, 0.0f, 0.0f), true);
	AddMeshToList(healthBar);

	Mesh* healthBarOutline = object2D::CreateSquare("healthBarOutline", corner, squareSide, glm::vec3(1.0f, 0.0f, 0.0f), false);
	AddMeshToList(healthBarOutline);

	Mesh* spawnZone = object2D::CreateSquare("spawnZone", corner, squareSide, glm::vec3(.2f, .2f, .2f), true);
	AddMeshToList(spawnZone);
	

	//bullets.reserve(magazineSize);
	currentBullet = 0;

	Bullet b;
	for (int i = 0; i < magazineSize; i++) {
		bullets.push_back(b);
	}

	Enemy e;
	for (int i = 0; i < enemyCount; i++) {
		enemies.push_back(e);
	}

	InitializeSpawnZones();
	InitializeEnemyPositions();
}

inline void Tema1::InitializeSpawnZones() {
	spawnZones.push_back(rect(25, -475, 250, 250));
	spawnZones.push_back(rect(1250, -475, 250, 250));
	spawnZones.push_back(rect(-200, 1000, 250, 250));
	spawnZones.push_back(rect(1250, 1000, 250, 250));
}

inline void Tema1::InitializeEnemyPositions() {
	for (int i = 0; i < enemies.size(); i++) {

		int zone = rand() % spawnZones.size();

		enemies[i].position = glm::vec2(
			spawnZones[zone].x + (rand() % (int)spawnZones[zone].w),
			spawnZones[zone].y + (rand() % (int)spawnZones[zone].h));

		enemies[i].collider = Collider(rect(enemies[i].position.x, enemies[i].position.y, squareSide, squareSide), "Enemy" + to_string(i), "enemy");
	}
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(.13f, .2f, .37f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
	GetSceneCamera()->SetPosition(glm::vec3(player.position.x - window->GetResolution().x * .5f, player.position.y - window->GetResolution().y * .5f, 50));
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (player.currentHealth > 0.0f) {
		ReloadGun(reloadDuration, deltaTimeSeconds);

		// console
		{
			if (consoleDebugSeconds <= 1.0f) {
				consoleDebugSeconds += deltaTimeSeconds;
			}
			else {
				cout << "Health: " << player.currentHealth << "\nScore: " << player.score << endl;
				consoleDebugSeconds = 0.0f;
			}
		}

		//UI
		{
			if (player.currentHealth > 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= transform2D::Translate(player.position.x + window->GetResolution().x * .5f - squareSide, player.position.y - window->GetResolution().y * .5f) *
					transform2D::Scale(1, window->GetResolution().y / squareSide * player.currentHealth / player.maxHealth);

				RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);


				modelMatrix = glm::mat3(1);
				modelMatrix *= transform2D::Translate(player.position.x + window->GetResolution().x * .5f - squareSide, player.position.y - window->GetResolution().y * .5f) *
					transform2D::Scale(1, window->GetResolution().y / squareSide);

				RenderMesh2D(meshes["healthBarOutline"], shaders["VertexColor"], modelMatrix);
			}
		}

		//player
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(player.position.x, player.position.y) *
				transform2D::Rotate(player.angularStep - PI * .5f);

			offsetMatrix = modelMatrix;
			offsetMatrix *= transform2D::Translate(27, 20);
			RenderMesh2D(meshes["littleCircle"], shaders["VertexColor"], offsetMatrix);

			offsetMatrix = modelMatrix;
			offsetMatrix *= transform2D::Translate(-27, 20);
			RenderMesh2D(meshes["littleCircle"], shaders["VertexColor"], offsetMatrix);

			
			//RenderMesh2D(meshes["playerRoot"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

			
		}

		//enemies
		{
			for (int i = 0; i < enemyCount; i++) {
				if (enemies[i].shouldRender) {
					dX = (player.position.x - enemies[i].position.x);
					dY = (player.position.y - enemies[i].position.y);

					enemies[i].rotation = atan2(dY, dX);

					enemies[i].position.x += cos(enemies[i].rotation) * deltaTimeSeconds * enemies[i].speed;
					enemies[i].position.y += sin(enemies[i].rotation) * deltaTimeSeconds * enemies[i].speed;

					enemies[i].collider.bounds.x = enemies[i].position.x;
					enemies[i].collider.bounds.y = enemies[i].position.y;

					modelMatrix = glm::mat3(1);
					modelMatrix *= transform2D::Translate(enemies[i].position.x + cx, enemies[i].position.y + cy)
						* transform2D::Rotate(enemies[i].rotation - PI * .5f)
						* transform2D::Translate(-cx, -cy);
					RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
				}
			}
		}

		//obstacle1
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(obstacle1.bounds.x /*- obstacle1.bounds.w / obstacle1.scale.x * .5f*/,
				obstacle1.bounds.y /*- obstacle1.bounds.h / obstacle1.scale.y * .5f*/);
			RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
		}

		//obstacle2
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(obstacle2.bounds.x /*- obstacle2.bounds.w / obstacle2.scale.x * .5f*/,
				obstacle2.bounds.y /*- obstacle2.bounds.h / obstacle2.scale.y * .5f*/)
				* transform2D::Scale(obstacle2.scale.x, obstacle2.scale.y);
			RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
		}

		//obstacle3
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(obstacle3.bounds.x /*- obstacle3.bounds.w / obstacle3.scale.x * .5f*/,
				obstacle3.bounds.y /*- obstacle3.bounds.h / obstacle3.scale.y * .5f*/)
				* transform2D::Scale(obstacle3.scale.x, obstacle3.scale.y);
			RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
		}

		//border
		{
			//left
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(translateX - mapSide * .5f /*- cx*/, translateY - mapSide * .5f /*- cy*/)
				*= transform2D::Scale(1, mapSide / squareSide);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

			//right
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(translateX + mapSide * .5f /*- cx*/, translateY - mapSide * .5f /*- cy*/)
				*= transform2D::Scale(1, mapSide / squareSide);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

			//top
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(translateX - mapSide * .5f /*- cx*/, translateY + mapSide * .5f /*- cy*/)
				*= transform2D::Scale(mapSide / squareSide + 1, 1);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

			//bottom
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(translateX - mapSide * .5f /*- cx*/, translateY - mapSide * .5f /*- cy*/)
				*= transform2D::Scale(mapSide / squareSide + 1, 1);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);
		}

		//bullets
		{
			for (uint16_t i = 0; i < magazineSize; i++)
			{
				if (bullets[i].shouldRender) {
					bullets[i].position.x += cos(bullets[i].rotation) * deltaTimeSeconds * bullets[i].speed;
					bullets[i].position.y += sin(bullets[i].rotation) * deltaTimeSeconds * bullets[i].speed;

					bullets[i].col.bounds.x = bullets[i].position.x;
					bullets[i].col.bounds.y = bullets[i].position.y;

					modelMatrix = glm::mat3(1);
					modelMatrix *= transform2D::Translate(bullets[i].position.x, bullets[i].position.y)
						* transform2D::Scale(bulletScale, bulletScale)
						* transform2D::Rotate(bullets[i].rotation);
					;
					RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

					bullets[i].time += deltaTimeSeconds;
					if (bullets[i].time > bullets[i].limit) {
						bullets[i].shouldRender = false;
					}
				}
			}
		}

		// spawnZones
		{
			for (int i = 0; i < spawnZones.size(); i++) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= transform2D::Translate(spawnZones[i].x, spawnZones[i].y) *
					transform2D::Scale(spawnZones[i].w / squareSide, spawnZones[i].h / squareSide);

				RenderMesh2D(meshes["spawnZone"], shaders["VertexColor"], modelMatrix);
			}
		}

#pragma region bullet collisions
		for (uint16_t i = 0; i < magazineSize; i++) {

#pragma region obstacle/border collision
			if (bullets[i].shouldRender) {
				col_checker = CheckCollisions(bullets[i].col.bounds, colliders);
				if (col_checker.tag == "obstacol" || col_checker.tag == "border") {
					bullets[i].shouldRender = false;
				}
				col_checker = CheckCollisions(bullets[i].col.bounds, enemies);
				if (col_checker.tag == "enemy") {
					bullets[i].shouldRender = false;
					KillEnemy(col_checker.name);
					player.score++;
				}
			}
#pragma endregion

		}
#pragma endregion

#pragma region enemyRespawn
		for (int i = 0; i < enemyCount; i++) {
			if (enemies[i].startRespawn) { // se respawneaza
				if (enemies[i].respawnTime > 0.0f) {
					enemies[i].respawnTime -= deltaTimeSeconds;
				}
				else { // trebuie respawnat
					enemies[i].shouldRender = true;
					enemies[i].startRespawn = false;
					enemies[i].respawnTime = 2.0f;

					int zone = rand() % spawnZones.size();

					enemies[i].position = glm::vec2(
						spawnZones[zone].x + (rand() % (int)spawnZones[zone].w),
						spawnZones[zone].y + (rand() % (int)spawnZones[zone].h));

					enemies[i].collider = Collider(rect(enemies[i].position.x, enemies[i].position.y, squareSide, squareSide), "Enemy" + to_string(i), "enemy");
					enemies[i].damage = rand() % (enemies[i].maxDamage - enemies[i].damage) + enemies[i].damage;
				}
			}
		}
#pragma endregion

	}
	else {

	}
}


void Tema1::FrameEnd()
{
}



void Tema1::ReloadGun(float reload, float deltaTime) {
	if (!gunReady && reloadTime >= reload) {
		reloadTime = 0.0f;
		gunReady = true;
	}
	else {
		reloadTime += deltaTime;
	}
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (player.currentHealth > 0.0f) {
		if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
			player.movementSpeed = 1200.0f;
			player.rotationSpeed = 12.654f;
		}
		else {
			player.movementSpeed = 600.0f;
			player.rotationSpeed = 5.57f;
		}

		float lastX = player.position.x;
		float lastY = player.position.y;

#pragma region input_resolver
		if (window->KeyHold(GLFW_KEY_W)) {
			player.position.y += deltaTime * player.movementSpeed;
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			player.position.y -= deltaTime * player.movementSpeed;
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			player.position.x -= deltaTime * player.movementSpeed;
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			player.position.x += deltaTime * player.movementSpeed;
		}
#pragma endregion

#pragma region player collisions

		player.collider.bounds.x = player.position.x + circleOffset;
		player.collider.bounds.y = player.position.y + circleOffset;

		col_checker = CheckCollisions(player.collider.bounds, colliders);

		if (col_checker.tag != "no_collision") {
			// miscarea duce la coliziune => dam inapoi
			player.position.x = lastX;
			player.position.y = lastY;

			player.collider.bounds.x = player.position.x + circleOffset;
			player.collider.bounds.y = player.position.y + circleOffset;
		}

		col_checker = CheckCollisions(player.collider.bounds, enemies);
		if (col_checker.tag != "no_collision") {
			// coliziune cu inamic => pierdem viata

			player.currentHealth -= GetDamage(col_checker.name);

			KillEnemy(col_checker.name);
		}

#pragma endregion

	}
}

void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

	if (key == GLFW_KEY_1) {
		player.currentHealth = player.maxHealth = 1000000;
	}
	else if (key == GLFW_KEY_2) {
		player.currentHealth = player.maxHealth = 250;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	dX = (mouseX - translateX - cx);
	dY = (window->GetResolution().y - mouseY - translateY - cy);

	player.angularStep = atan2(dY, dX);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (gunReady) {
		gunReady = false;

		dX = (mouseX - translateX - cx);
		dY = (window->GetResolution().y - mouseY - translateY - cy);

		bullets[currentBullet].rotation = atan2(dY, dX);

		bullets[currentBullet].position = glm::vec2(player.position.x, player.position.y);
		bullets[currentBullet].col.bounds = rect(player.position.x, player.position.y, squareSide * bulletScale, squareSide * bulletScale);
		bullets[currentBullet].col.name = "bullet" + to_string(currentBullet);
		bullets[currentBullet].col.tag = "bullet";
		bullets[currentBullet].col.scale = glm::vec2(squareSide * bulletScale, squareSide * bulletScale);

		bullets[currentBullet].time = 0.0f;

		bullets[currentBullet].shouldRender = true;

		currentBullet++;
		if (currentBullet == magazineSize) {
			currentBullet = 0;
		}
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

bool Tema1::CheckCollision(rect obj1, rect obj2) {
	// collision x-axis?
	bool collisionX = 
		obj1.x + obj1.w >= obj2.x &&
		obj2.x + obj2.w >= obj1.x;
	// collision y-axis?
	bool collisionY = 
		obj1.y + obj1.h >= obj2.y &&
		obj2.y + obj2.h >= obj1.y;
	// collision only if on both axes
	return collisionX && collisionY;
}

Collider Tema1::CheckCollisions(rect obj, vector<Collider> objects) {
	for each (Collider col in objects)
	{
		if (CheckCollision(obj, col.bounds)) {
			return col;
		}
	}
	Collider no_col;
	return no_col;
}

Collider Tema1::CheckCollisions(rect obj, vector<Enemy> enemies) {
	for each (Enemy e in enemies)
	{
		if (e.shouldRender) {
			if (CheckCollision(obj, e.collider.bounds)) {
				return e.collider;
			}
		}
	}
	Collider no_col;
	return no_col;
}

bool Tema1::KillEnemy(string name) {
	for (int i = 0; i < enemyCount; i++) {
		if (enemies[i].collider.name == name) {
			enemies[i].startRespawn = true;
			enemies[i].shouldRender = false;

			return true;
		}
	}
	return false;
}

int Tema1::GetDamage(string name) {
	for (int i = 0; i < enemyCount; i++) {
		if (enemies[i].collider.name == name) {
			return enemies[i].damage;
		}
	}
	return -1;
}