#include "model.h"
#include <subsystems/asserts.h>
#include <glm/gtc/matrix_transform.hpp>

model::model(glm::vec3 position, glm::vec3 scale, bool no_texture, bool flipTexturesOnLoad)
	: scale(scale), m_no_texture(no_texture), m_flip_textures(flipTexturesOnLoad)
{
	rbody.rb_pos = position;
}

model::~model()
{
	clean_up();
}

void model::load(const std::string & file_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		KRAZY_ERROR("Failed to load model at location : {} .", file_path);
		return;
	}

	directory = file_path.substr(0, file_path.find_last_of("/"));
	process_node(scene->mRootNode, scene);
}

void model::render(const std::shared_ptr<shader>& shader_program, bool set_model_matrix, bool do_draw_call)
{
	if (set_model_matrix)
	{
		glm::mat4 model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, rbody.rb_pos);
		model_matrix = glm::scale(model_matrix, scale);
		shader_program->set_mat4("Model", model_matrix);
	}

	shader_program->set_float("u_Material.shininess", 0.5f);

	for(std::shared_ptr<mesh>& m : meshes)
	{
		m->render(shader_program, do_draw_call);
	}
}

void model::process_node(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(Mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}
}

std::shared_ptr<mesh> model::process_mesh(aiMesh * ai_mesh, const aiScene * scene)
{
	std::vector<vertex> vertices;
	std::vector<u32> indices;
	std::vector<std::shared_ptr<texture>> textures;

	for (size_t i = 0; i < ai_mesh->mNumVertices; i++)
	{
		vertex vert;
		vert.position = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
		vert.normal = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
		
		if(ai_mesh->mTextureCoords[0])
		{
			vert.tex_coord = glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vert.tex_coord = glm::vec2(0.0f);
		}

		vertices.push_back(vert);
	}


	for (size_t i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace face = ai_mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if(ai_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

		if(m_no_texture)
		{
			aiColor4D diffuse(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

			aiColor4D specular(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);

			return std::make_shared<mesh>(vertices, indices, diffuse, specular);
		}
		else
		{
			std::vector<std::shared_ptr<texture>> diffuse_textures = load_texture(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());

			std::vector<std::shared_ptr<texture>> specular_textures = load_texture(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
		}
	}

	return std::make_shared<mesh>(vertices, indices, textures);
}

std::vector<std::shared_ptr<texture>> model::load_texture(aiMaterial * mat, aiTextureType type)
{
	std::vector<std::shared_ptr<texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (size_t j = 0; j < loaded_textures.size(); j++)
		{
			if(std::strcmp(loaded_textures[j]->get_path().data(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::shared_ptr<texture> tex = std::make_shared<texture>(directory, str.C_Str(), type, m_flip_textures);

			textures.push_back(tex);
			loaded_textures.push_back(tex);
		}
	}

	return textures;
}

void model::clean_up()
{
}