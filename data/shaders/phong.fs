const vec3 sunLight = normalize(vec3(-1,1,0.2));

varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D u_texture;

void main()
{
    float brightness = dot(v_normal, sunLight) / (length(sunLight) * length(v_normal));
    brightness = clamp(brightness, 0, 1);
    gl_FragColor = brightness;

    vec4 color = texture2D(u_texture, v_uv);
    vec3 diffuse = 0.2 * color.rgb;
    gl_FragColor = vec4(diffuse + brightness * color.rgb, color.a);
}
