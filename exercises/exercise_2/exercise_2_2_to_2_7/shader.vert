#version 330 core
layout (location = 0) in vec2 pos;   // the position variable has attribute position 0
layout (location = 1) in vec2 velocity;
layout (location = 2) in float birthTime;

uniform float curTime;
const float maxAge = 10.0;
const float minSize = 0.1;
const float maxSize = 20.0;

out float elapsedTime;
out float maxAgeFrag;

void main()
{
    vec2 finalPos = pos;
    float age = curTime - birthTime;

    if(birthTime == 0.0 || age > maxAge) {
        finalPos = vec2(-2.0, -2.0);
    }
    else {
        finalPos += velocity * age;
        gl_PointSize = age * (maxSize - minSize) / maxAge + minSize;
    }

    gl_Position = vec4(finalPos, 0.0, 1.0);
    elapsedTime = age;
    maxAgeFrag = maxAge;
}