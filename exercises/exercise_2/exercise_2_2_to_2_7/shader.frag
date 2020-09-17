#version 330 core

in vec2 gl_PointCoord;
in float elapsedTime;
in float maxAgeFrag;

const vec3 startColor = vec3(1.0, 1.0, 0.05);
const vec3 midColor = vec3(1.0, 0.5, 0.01);
const vec3 endColor = vec3(0.0, 0.0, 0.0);

out vec4 fragColor;

void main()
{
    // Color interpolation
    vec3 curColor = startColor;
    if(elapsedTime < maxAgeFrag / 2.0) {
        curColor = mix(startColor, midColor, elapsedTime / (maxAgeFrag / 2));
    }
    else {
        curColor = mix(midColor, endColor, (elapsedTime - maxAgeFrag / 2) / (maxAgeFrag / 2));
    }

    float distance = distance(gl_PointCoord, vec2(0.5, 0.5));
    //float distance = sqrt(pow((gl_PointCoord.x - 0.5), 2.0) + pow((gl_PointCoord.y - 0.5), 2.0));
    float posAlpha = 1.0 - distance * 2;
    float actualAlpha = mix(posAlpha, 0.0, elapsedTime/maxAgeFrag);
    fragColor = vec4(curColor, actualAlpha);

}