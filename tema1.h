#pragma once

#include "components/simple_scene.h"
#include <lab_m1\tema1\player.h>
#include <lab_m1\tema1\rect.h>
#include <lab_m1\tema1\bullet.h>
#include <lab_m1\tema1\enemy.h>
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
		Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
		bool Tema1::CheckCollision(rect obj1, rect obj2);
		Collider Tema1::CheckCollisions(rect player, std::vector<Collider> objects);
		void Tema1::ReloadGun(float reload, float deltaTime);
		void Tema1::InitializeEnemyPositions();
		Collider Tema1::CheckCollisions(rect obj, vector<Enemy> enemies);
		bool Tema1::KillEnemy(string name);
		int Tema1::GetDamage(string name);
		inline void Tema1::InitializeSpawnZones();

     protected:
		float cx = 0, cy = 0;
		double circlecx = .0l;
		double circlecy = .0l;
		glm::mat3 modelMatrix;
		glm::mat3 offsetMatrix;
		const float squareSide = 100;
		const float circleOffset = -50;

        float translateX = 0, translateY = 0;
        float scaleX = 0, scaleY = 0;
        float angularStep = 0;
		float dX;
		float dY;

        // TODO(student): If you need any other class variables, define them here.
		Player player;

		float mapSide;
		float bottomLimit;
		float topLimit;
		float leftLimit;
		float rightLimit;

		Collider col_checker;

		Collider obstacle1;
		Collider obstacle2;
		Collider obstacle3;

		Collider borderLeft, borderRight, borderTop, borderBottom;

		std::vector<Collider> colliders;

		bool gunReady = true;
		bool shouldFire = false;
		float reloadTime = 0.0f;
		float reloadDuration = .33f;

		std::vector<Bullet> bullets;
		float bulletScale = .1f;
		uint16_t currentBullet;
		uint16_t magazineSize = 10;

		std::vector<Enemy> enemies;
		uint16_t enemyCount = 7;
		Enemy enemy_checker;

		float consoleDebugSeconds = 0.0f;

		std::vector<rect> spawnZones;
    };
}   // namespace m1