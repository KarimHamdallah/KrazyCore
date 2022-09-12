#pragma once
#include <defines.h>
#include <subsystems/Renderer/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <subsystems/physics/rigid_body.h>

class model
{
public:
	model(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), bool no_texture = false, bool flipTexturesOnLoad = false);
	~model();

	virtual void init() {}
	void load(const std::string& file_path);
	virtual void render(const std::shared_ptr<shader>& shader_program, bool set_model_matrix = true, bool do_draw_call = true);

	rigid_body rbody;
	glm::vec3 scale;

	std::vector<std::shared_ptr<mesh>> meshes;
protected:
	bool m_no_texture;


	std::string directory;
	std::vector<std::shared_ptr<texture>> loaded_textures;

	void process_node(aiNode* node, const aiScene* scene);
	std::shared_ptr<mesh> process_mesh(aiMesh* ai_mesh, const aiScene* scene);
	std::vector <std::shared_ptr<texture>> load_texture(aiMaterial* mat, aiTextureType type);
private:
	bool m_flip_textures;
	void clean_up();
};