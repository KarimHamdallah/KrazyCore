#version 460 core

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 5

struct material
{
   vec4 diffuse;
   vec4 specular;
   float shininess;
};

uniform sampler2D diffuse0;
uniform sampler2D specular0;

struct point_light
{
   vec3 position;
   
   float k0,k1,k2;

   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
};

struct spot_light
{
   vec3 position;
   vec3 direction;

   float cut_off_point;
   float outer_cut_off_point;

   float k0,k1,k2;

   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
};
struct dir_light
{
   vec3 direction;

   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
};

out vec4 o_color;

in vec2 v_Texcoord;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform material u_Material;
uniform int u_NPointLights;
uniform int u_NSpotLights;
uniform int u_NoTex;

uniform dir_light u_DirLight;
uniform point_light u_PointLights[MAX_POINT_LIGHTS];
uniform spot_light u_SpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D u_Texture;
uniform vec3 u_ViewPos;

vec4 calculate_point_light(int idx,vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture);
vec4 calculate_spot_light(int idx,vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture);
vec4 calculate_dir_light(vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture);

void main()
{
   vec3 norm = normalize(v_Normal);
   vec3 view_dir = normalize(u_ViewPos - v_FragPos);
   vec4 diffuse_texture;
   vec4 specular_texture;


   if(u_NoTex == 1)
   {
      diffuse_texture = u_Material.diffuse;
	  specular_texture = u_Material.specular;
   }
   else
   {
     diffuse_texture = texture(diffuse0, v_Texcoord);
	 specular_texture = texture(specular0, v_Texcoord);
   }

   vec4 result_color;

   result_color = calculate_dir_light(norm,view_dir,diffuse_texture,specular_texture);

   for (int i = 0; i < u_NPointLights; i++)
	{
	  result_color += calculate_point_light(i, norm, view_dir, diffuse_texture, specular_texture);
	}

	for (int i = 0; i < u_NSpotLights; i++)
	{
	  result_color += calculate_spot_light(i, norm, view_dir, diffuse_texture, specular_texture);
	}

    o_color = result_color;
}

vec4 calculate_point_light(int idx,vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture)
{
     // calculate ambient light >> light ambient * diffuse_texture
     vec4 ambient_color = u_PointLights[idx].ambient * diffuse_texture;
	 // calculate light dir vector - and diffuse factor which is cos(theta) between two vectors(normal vector and light dir)
	 vec3 ligt_dir = normalize(u_PointLights[idx].position - v_FragPos);
	 float diffuse_factor = max(dot(norm, ligt_dir),0.0);

	 // calculate Final diffuse color
	 vec4 diffuse_color = u_PointLights[idx].diffuse * (diffuse_factor * diffuse_texture);

	 // calcukate specular component fo light
	 vec3 reflect_dir = reflect(-ligt_dir, norm);
	 float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess * 64);

	 // calculate Final specular color
	 vec4 specular_color = u_PointLights[idx].specular * (specular_factor * specular_texture);


	 float dist = length(u_PointLights[idx].position - v_FragPos);
	 float attenuation = 1.0 / (u_PointLights[idx].k0 + u_PointLights[idx].k1 * dist * u_PointLights[idx].k2 * (dist * dist));

     return vec4(ambient_color + diffuse_color + specular_color) * attenuation;
}

vec4 calculate_spot_light(int idx,vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture)
{

    // calculate light dir vector and angle between spot light direction vector and light to fragment vector
	vec3 ligt_dir = normalize(u_SpotLights[idx].position - v_FragPos);
	float theta = dot(ligt_dir, normalize(-u_SpotLights[idx].direction));

	vec4 ambient_color = u_SpotLights[idx].ambient * diffuse_texture;

	if(theta > u_SpotLights[idx].outer_cut_off_point)
	{
	  float diffuse_factor = max(dot(norm, ligt_dir),0.0);
	  vec4 diffuse_color = u_SpotLights[idx].diffuse * (diffuse_factor * diffuse_texture);
	  
	  vec3 reflect_dir = reflect(-ligt_dir, norm);
	  float specular_factor = pow(max(dot(view_dir,reflect_dir),0.0), u_Material.shininess * 64);
	  vec4 specular_color = u_SpotLights[idx].specular * (specular_factor * specular_texture);

	  float intenisity = (theta - u_SpotLights[idx].outer_cut_off_point) / (u_SpotLights[idx].cut_off_point - u_SpotLights[idx].outer_cut_off_point);
	  intenisity = clamp(intenisity, 0.0, 1.0);
	  diffuse_color *= intenisity;
	  specular_color *= intenisity;

	  float dist = length(u_SpotLights[idx].position - v_FragPos);
	  float attenuation = 1.0 / (u_SpotLights[idx].k0 + u_SpotLights[idx].k1 * dist * u_SpotLights[idx].k2 * (dist * dist));

      return vec4(ambient_color + diffuse_color + specular_color) * attenuation;
	}
	else
	{
	  return ambient_color;
	}
}

vec4 calculate_dir_light(vec3 norm, vec3 view_dir, vec4 diffuse_texture, vec4 specular_texture)
{
   // calculate ambient light >> light ambient * diffuse_texture
     vec4 ambient_color = u_DirLight.ambient * diffuse_texture;
	 // normalize light dir vector - and calculate diffuse factor which is cos(theta) between two vectors(normal vector and light dir)
	 vec3 ligt_dir = normalize(-u_DirLight.direction);
	 float diffuse_factor = max(dot(norm, ligt_dir),0.0);

	 // calculate Final diffuse color
	 vec4 diffuse_color = u_DirLight.diffuse * (diffuse_factor * diffuse_texture);

	 // calculate specular component fo light
	 vec3 reflect_dir = reflect(-ligt_dir, norm);
	 float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess * 64);

	 // calculate Final specular color
	 vec4 specular_color = u_DirLight.specular * (specular_factor * specular_texture);

     return vec4(ambient_color + diffuse_color + specular_color);
}