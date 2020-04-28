#include "Bullet.h"


bool Bullet::shooting(bool shoot_ball)
{
	if (shoot_ball == true && counter < shoot_length)
	{
		shoot = shoot + bullet_vector;
		counter += 1;
		return true;
	}
	else
	{
		shoot = glm::vec3(1.0f, 0.0f, 0.0f);
		collision = false;
		counter = 0;
		first_frame_shot = true;
		return false;
	}
}

void Bullet::generate(glm::mat4 M_wieza, glm::vec3 lufa_cords)
{
		if (first_frame_shot == true) {
			M_copy = M_wieza;
			first_frame_shot = false;
		}

		Mp1 = glm::translate(M_copy, lufa_cords + glm::vec3(shoot[0] - 1.0f, shoot[1], shoot[2])); //...i macierz przesuniêcia
		Mp1 = glm::scale(Mp1, glm::vec3(1 / 0.8f, 1 / 0.3f, 1 / 0.7f));
		Mp1 = glm::scale(Mp1, glm::vec3(radius, radius, radius));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mp1));  //Za³adowanie macierzy modelu do programu cieniuj¹cego
		glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Planeta jest zielona

		Models::sphere.drawSolid(); //Narysowanie obiektu

}

void Bullet::collision_detector(glm::vec3 object_position, glm::vec3 object_size)
{
	glm::vec4 bullet_position = Mp1 * Position;

	// get center point circle first 
	glm::vec2 center(bullet_position.x, bullet_position.z);


	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents( glm::vec2(object_size.x, object_size.z)/ 2.0f);
	glm::vec2 aabb_center(object_position.x, object_position.z);

	glm::vec2 difference = center - aabb_center;

	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;

	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;

	if (glm::length(difference) < radius)
	{
		counter = shoot_length;
		collision = true;
		Mp1 = glm::translate(M_copy, glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else 
	{
		collision = false;
	}
}

bool Bullet::hasCollision(glm::vec3 object_position, glm::vec3 object_size)
{
	collision_detector(object_position, object_size);
	return collision;
}
