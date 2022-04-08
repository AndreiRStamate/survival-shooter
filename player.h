#pragma once

#include "components/simple_scene.h"
#include <lab_m1\tema1\tema1.h>
#include <lab_m1\tema1\rect.h>
//#include <lab_m1\tema1\collider.h>
#include <lab_m1\tema1\collider.h>

namespace m1
{
	class Player
	{
	public:
		Player() {
			position.x = 0;
			position.y = 0;

			rotationSpeed = movementSpeed = 0.0f;
			angularStep = 0.0f;

			collider = Collider();
		};

		glm::vec2 position;
		float movementSpeed;

		float angularStep;
		float rotationSpeed;

		Collider collider;

		float currentHealth = 250;
		float maxHealth = 250;
		int score = 0;

	private:

	protected:
		// TODO(student): If you need any other class variables, define them here.
		
	};
}   // namespace m1