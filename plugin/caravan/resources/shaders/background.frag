#version 120

uniform vec2 resolution;
uniform float time;
uniform float dustDriveValue;
uniform float widthValue;
uniform float airValue;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    // Base colors
    vec3 skyColor = vec3(0.529, 0.808, 0.922);  // Light blue
    vec3 horizonColor = vec3(0.98, 0.643, 0.533);  // Sunset orange
    vec3 sandColor = vec3(0.761, 0.698, 0.502);  // Sand color
    
    // Horizon line position
    float horizonLine = 0.65;
    
    // Create color based on position
    vec3 color;
    if (uv.y < horizonLine) {
        // Sky gradient
        float blend = pow(uv.y / horizonLine, 1.5);
        color = mix(skyColor, horizonColor, blend * (0.7 + dustDriveValue * 0.3));
        
        // Add sun
        float sunRadius = 0.1 + 0.05 * dustDriveValue;
        vec2 sunPos = vec2(0.75, 0.25);
        float sunDist = distance(uv, sunPos);
        if (sunDist < sunRadius) {
            vec3 sunColor = vec3(1.0, 0.7, 0.3);
            color = mix(color, sunColor, 0.8);
        }
        
        // Add sun glow
        float glow = 1.0 - smoothstep(sunRadius, sunRadius + 0.2, sunDist);
        color = mix(color, vec3(1.0, 0.7, 0.3), glow * 0.5);
    } else {
        // Sand with subtle waves
        float waves = sin(uv.x * 10.0 + time * 0.1) * 0.01 * widthValue;
        color = sandColor + vec3(waves);
        
        // Sand dunes
        float duneHeight = sin(uv.x * 5.0 + time * 0.05) * 0.05;
        float duneGradient = smoothstep(horizonLine, horizonLine + 0.05, uv.y - duneHeight);
        color = mix(horizonColor, color, duneGradient);
        
        // Add dust effect
        float dust = fract(sin(uv.x * 1000.0 + uv.y * 1000.0 + time) * 1000.0);
        dust = smoothstep(0.99, 1.0, dust) * dustDriveValue * 0.3;
        color += vec3(dust);
    }
    
    // Apply vignette effect
    float vignette = 1.0 - smoothstep(0.5, 1.5, length((uv - 0.5) * 2.0));
    color *= mix(1.0, vignette, 0.3);
    
    gl_FragColor = vec4(color, 1.0);
}