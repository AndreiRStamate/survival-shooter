#pragma once

#include "components/simple_scene.h"
#include <lab_m1\tema1\tema1.h>
#include <lab_m1\tema1\rect.h>
#include <lab_m1\tema1\collider.h>

namespace m1
{
	class Enemy
	{
	public:
		Enemy() {
			position.x = 0;
			position.y = 0;

			collider = Collider();
		};

		glm::vec2 position;

		Collider collider;

		bool shouldRender = true;
		bool startRespawn = false;
		float respawnTime = 2.0f;
		float rotation = 0.0f;
		float speed = 350.0f;
		int damage = 5;
		int maxDamage = 25;

	private:

	protected:
		// TODO(student): If you need any other class variables, define them here.

	};
}   // namespace m1