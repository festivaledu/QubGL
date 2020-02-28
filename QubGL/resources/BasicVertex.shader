#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 FragColor;
out vec3 FragNormal;

// Color overrides
uniform vec4 XColorOverride;
uniform vec4 YColorOverride;
uniform vec4 ZColorOverride;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	// Calculate the Vertex Position.
	mat4 modelView = ViewMatrix * ModelMatrix;
	gl_Position = ProjectionMatrix * modelView * vec4(position.x, position.y, position.z, 1.0f);

	// Calculate the Vertex Normal.
	mat4 normalMatrix = transpose(inverse(modelView));
	FragNormal = (normalMatrix * vec4(normal, 0.0)).xyz;

	// Pass the Color to the FragmentShader.
	FragColor = color;

    // In contrast to the Vertex class in C++, this time all 3 vertices of the triangle need to be colored or the triangle will have a gradient. That's why we have to check for a range of 3 vertices starting with the known index from the UV mapping
    // Check if the current vertex is part of an x-face
    if (XColorOverride.a == 1.F && ((gl_VertexID >= 3 && gl_VertexID <= 5) || (gl_VertexID >= 12 && gl_VertexID <= 14) || (gl_VertexID >= 81 && gl_VertexID <= 83) || (gl_VertexID >= 90 && gl_VertexID <= 92))) {
        FragColor = XColorOverride;
    }

    // Check if the current vertex is part of an y-face
    if (YColorOverride.a == 1.F && ((gl_VertexID >= 0 && gl_VertexID <= 2) || (gl_VertexID >= 9 && gl_VertexID <= 11) || (gl_VertexID >= 78 && gl_VertexID <= 80) || (gl_VertexID >= 87 && gl_VertexID <= 89))) {
        FragColor = YColorOverride;
    }

    // Check if the current vertex is part of an z-face
    if (ZColorOverride.a == 1.F && ((gl_VertexID >= 6 && gl_VertexID <= 8) || (gl_VertexID >= 75 && gl_VertexID <= 77) || (gl_VertexID >= 84 && gl_VertexID <= 86) || (gl_VertexID >= 129 && gl_VertexID <= 131))) {
        FragColor = ZColorOverride;
    }
}
