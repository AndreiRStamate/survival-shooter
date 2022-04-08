#pragma once

#include "components/simple_scene.h"
#include <lab_m1\tema1\tema1.h>
#include <lab_m1\tema1\rect.h>

namespace m1
{
	class Bullet
	{
	public:
		glm::vec2 position;

		Collider col;
		float speed = 1500.4f;
		float rotation;
		float time = 0.0f;
		float limit = .2f;
		
		bool shouldRender = false;

	private:

	protected:
		// TODO(student): If you need any other class variables, define them here.

	};
}   // namespace m1