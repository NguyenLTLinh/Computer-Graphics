#pragma once

#include "Mesh.h"

#include <glm/glm.hpp>

Mesh createAncientBlock(float width, float height, float depth);
Mesh createColumn(float radius, float height, int segments);
Mesh createSeaweedBlade(float height, float width, int joints);
Mesh createBubbleQuad(float size);
