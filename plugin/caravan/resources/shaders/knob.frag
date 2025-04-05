#version 330 core

uniform vec2 resolution;
uniform float time;
uniform float value;
uniform sampler2D compassTexture;
uniform vec2 center;
uniform float radius;
uniform float angle;

out vec4 fragColor;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 texCoord = (gl_FragCoord.xy - center) / (radius * 2.0) + 0.5;
    
    // Distance from center
    float dist = distance(gl_FragCoord.xy, center);
    
    // Check if we're within the knob radius
    if (dist > radius) {
        fragColor = vec4(0.0, 0.0, 0.0, 0.0); // Transparent outside the knob
        return;
    }
    
    // Rotate texture coordinates based on angle
    float s = sin(angle);
    float c = cos(angle);
    texCoord -= 0.5;
    vec2 rotatedCoord = vec2(
        texCoord.x * c - texCoord.y * s,
        texCoord.x * s + texCoord.y * c
    );
    texCoord = rotatedCoord + 0.5;
    
    // Sample compass texture
    vec4 compassColor = texture(compassTexture, texCoord);
    
    // Add dynamic glow based on value
    float glow = value * 0.7 + 0.3;
    float glowPulse = 0.05 * sin(time * 3.0) * value;
    
    // Edge highlight
    float edge = smoothstep(radius * 0.9, radius, dist);
    
    // Apply glow to compass color
    vec3 glowColor = mix(compassColor.rgb, vec3(1.0, 0.65, 0.3), 0.3) * (glow + glowPulse);
    glowColor = mix(glowColor, vec3(1.0, 0.8, 0.4), edge * 0.8);
    
    // Add subtle wave pattern inside the knob
    float wave = sin(dist * 20.0 - time * 2.0) * 0.03 * value;
    glowColor += vec3(wave, wave, wave);
    
    // Final color with full opacity
    fragColor = vec4(glowColor, compassColor.a);
}