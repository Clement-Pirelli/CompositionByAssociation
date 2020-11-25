#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include "CBA.h"
#include "vec.h"

using namespace CBA;

struct Gravity
{
	EntityID id;
	float strength;
	vec3 direction;
};

struct Movement
{
	EntityID id;
	vec3 position;
	vec3 velocity;
};

float random01()
{
	return rand() / float(RAND_MAX);
}

int main()
{
	std::cout << std::setprecision(2);
	Components<Gravity> all_gravities;
	Components<Movement> all_movements;

	std::vector<EntityID> ids = std::vector<EntityID>(100);
	for (uint64_t i = 0; i < ids.size(); i++)
	{
		ids[i] = EntityID::get_new();
	}

	for (EntityID current_ID : ids)
	{
		all_gravities.insert(current_ID, { .strength = 9.81f * random01(), .direction = vec3(.0f, -1.0f, .0f) });
		all_gravities.insert(current_ID, { .strength = 9.81f * random01(), .direction = vec3(1.0f, .0f, .0f) });
		all_movements.insert(current_ID, { .position = vec3(random01(), random01(), random01()), .velocity = vec3() });
	}

	all_movements.remove_all_with_id(ids[45]);
	all_gravities.remove_all_with_id(ids[95]);

	char user_wants_to_continue = 'y';
	while (user_wants_to_continue != 'n')
	{
		for (const Gravity &gravity : all_gravities)
		{
			std::span<Movement> movement_span = all_movements.with_id_mut(gravity.id);
			for (Movement &movement : movement_span)
			{
				movement.velocity += gravity.direction * gravity.strength;
			}
		}

		for (Movement &movement : all_movements)
		{
			movement.position += movement.velocity;

			std::cout << "ID " << static_cast<uint64_t>(movement.id) << ": " << movement.position.x() << ", " << movement.position.y() << ", " << movement.position.z() << "\n";
		}

		std::cout << "Do you wish to continue? (y/n)\n";
		std::cin >> user_wants_to_continue;
	}
}