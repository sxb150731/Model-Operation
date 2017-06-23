#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <sstream>

struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

void load_obj(std::string obj_path, std::vector<Vertex> &vertices) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coods;
	std::ifstream ifs;
	try {
		ifs.open(obj_path);
		std::string one_line;
		while (std::getline(ifs, one_line)) {
			std::stringstream ss(one_line);
			std::string type;
			ss >> type;
			if (type == "v") {
				glm::vec3 vert_pos;
				ss >> vert_pos[0] >> vert_pos[1] >> vert_pos[2];
				positions.push_back(vert_pos);
			}
			else if (type == "vt") {
				glm::vec2 tex_coord;
				ss >> tex_coord[0] >> tex_coord[1];
				tex_coods.push_back(tex_coord);
			}
			else if (type == "vn") {
				glm::vec3 vert_norm;
				ss >> vert_norm[0] >> vert_norm[1] >> vert_norm[2];
				normals.push_back(vert_norm);
			}
			else if (type == "f") {
				std::string s_vertex_0, s_vertex_1, s_vertex_2;
				ss >> s_vertex_0 >> s_vertex_1 >> s_vertex_2;
				int pos_idx, tex_idx, norm_idx;
				sscanf_s(s_vertex_0.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				// We have to use index -1 because the obj index starts at 1
				Vertex vertex_0;
				vertex_0.Position = positions[pos_idx - 1];
				vertex_0.TexCoords = tex_coods[tex_idx - 1];
				vertex_0.Normal = normals[norm_idx - 1];
				sscanf_s(s_vertex_1.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				Vertex vertex_1;
				vertex_1.Position = positions[pos_idx - 1];
				vertex_1.TexCoords = tex_coods[tex_idx - 1];
				vertex_1.Normal = normals[norm_idx - 1];
				sscanf_s(s_vertex_2.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				Vertex vertex_2;
				vertex_2.Position = positions[pos_idx - 1];
				vertex_2.TexCoords = tex_coods[tex_idx - 1];
				vertex_2.Normal = normals[norm_idx - 1];
				vertices.push_back(vertex_0);
				vertices.push_back(vertex_1);
				vertices.push_back(vertex_2);
			}
		}
	}
	catch (const std::exception&) {
		std::cout << "Error: Obj file cannot be read\n";
	}
}


void loadModel(std::vector<Vertex> &model, MODEL_TYPE &m_lastModel) {
	std::string path = "cube.obj";
	model.clear();
	if (m_currentModel == CUBE) {
		path = "cube.obj";
		m_lastModel = CUBE;
	}
	else if (m_currentModel == CYBORG) {
		path = "cyborg.obj";
		m_lastModel = CYBORG;
	}
	else if (m_currentModel == ROCK) {
		path = "rock.obj";
		m_lastModel = ROCK;
	}
	else if (m_currentModel == TWO_CUBES) {
		path = "two_cubes.obj";
		m_lastModel = TWO_CUBES;
	}
	path = "models/" + path;
	load_obj(path, model);
}
#endif