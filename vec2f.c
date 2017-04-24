typedef struct Vector2f{
	float x;
	float y;
} Vector2f;

Vector2f newVector2f(float x, float y) {
	Vector2f out = {x, y};
	return out;
}
