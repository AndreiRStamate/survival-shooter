#pragma once

#include <string>
#include "components/simple_scene.h"
#include <lab_m1\tema1\tema1.h>
#include <lab_m1\tema1\rect.h>

using namespace std;

namespace m1
{
	class Collider
	{
	public:
		Collider() {
			bounds = { 0, 0, 0, 0 };
			scale.x = 1;
			scale.y = 1;
			name = "no_name";
			tag = "no_collision";
		};

		Collider(rect bounds, string name, string tag, glm::vec2 scale = glm::vec2(1, 1))
		{
			this->scale = scale;

			this->bounds = bounds;
			this->bounds.w *= scale.x;
			this->bounds.h *= scale.y;

			this->name = name;
			this->tag = tag;
		}

		rect bounds;
		glm::vec2 scale;
		string name;
		string tag;

	private:

	protected:
		// TODO(student): If you need any other class variables, define them here.

	};
}   // namespace m1