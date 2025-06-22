#include "objects/terrain.hpp"
#include "core/camera.hpp"
#include "core/core.hpp"
#include "shaders/light.hpp"

#include <random>
#include <vector>
#include <stb_image.h>



float Terrain::get_height_at(float x, float z) {
   if (x >= 0 && x < height && z >= 0 && z < width)
      return heights[x][z];
   return 0.0f;
}

void Terrain::create_vertex(){
   vert.create_VBO(&vbo_data[0], vbo_data.size() * sizeof(float));
   vert.create_EBO(&indices[0], indices.size() * sizeof(uint));
   vert.add_atrib(0, 3, GL_FLOAT, 8 * sizeof(float)); //pos
   vert.add_atrib(1, 3, GL_FLOAT, 8 * sizeof(float), (3*sizeof(float))); 
   vert.add_atrib(2, 2, GL_FLOAT, 8 * sizeof(float), (6*sizeof(float))); 
}
void Terrain::generate_indices(){
   for(uint i = 0; i < height-1; i++)
          for(uint j = 0; j < width; j++)      
              for(uint k = 0; k < 2; k++)     
                  indices.push_back(j + width * (i + k));

   indices_count = (height - 1) * width * 2 + height - 1;
}

void Terrain::generate_tex_coords(){
	const auto textureStepU = 0.1f;
	const auto textureStepV = 0.1f;

   tex_coords.resize(height, std::vector<glm::vec2>(width));
	for (auto i = 0; i < height; i++){
		for (auto j = 0; j < width; j++) {
         tex_coords[i][j] = glm::vec2(textureStepU * j, textureStepV * i);
		}
	}
}
void Terrain::generate_vertices(){
   vertices.resize(height, std::vector<glm::vec3>(width));
   for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
         vertices[i][j] = glm::vec3{float(i), heights[i][j], float(j)};
      }
   }
}
void Terrain::generate_normals(){
	std::vector<std::vector<glm::vec3> > temp[2];
	for (auto i = 0; i < 2; i++) {
		temp[i] = std::vector(height-1, std::vector<glm::vec3>(width-1));
	}

	for (auto i = 0; i < height- 1; i++){
		for (auto j = 0; j < width- 1; j++){
			const auto& vertex_a = vertices[i][j];
			const auto& vertex_b = vertices[i][j+1];
			const auto& vertex_c = vertices[i+1][j+1];
			const auto& vertex_d = vertices[i+1][j];

			const auto trig_a = glm::cross(vertex_b - vertex_a, vertex_a - vertex_d);
			const auto trig_b = glm::cross(vertex_d - vertex_c, vertex_c - vertex_b);

			temp[0][i][j] = glm::normalize(trig_a);
			temp[1][i][j] = glm::normalize(trig_b);
		}
	}

   normals.resize(height, std::vector<glm::vec3>(width));

	for (auto i = 0; i < height; i++){
		for (auto j = 0; j < width; j++){
			auto norm = glm::vec3(0.0f, 0.0f, 0.0f);
			if (i != 0 & j != 0) {
				norm += temp[0][i-1][j-1];
			}

			if (i != 0 && j != width-1) {
				for (auto k = 0; k < 2; k++) {
					norm += temp[k][i - 1][j];
				}
			}

			if (i != height-1 && j != width-1) {
				norm += temp[0][i][j];
			}

			if (i != height-1 && j != 0) {
				for (auto k = 0; k < 2; k++) {
					norm += temp[k][i][j-1];
				}
			}

			normals[i][j] = -glm::normalize(norm);
		}
	}
}

void Terrain::prepare_data(){
   log_info("preparing data for terrain");
   for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
         glm::vec3 pos = vertices[i][j]; 
         glm::vec2 tex = tex_coords[i][j];
         glm::vec3 norm = normals[i][j];

         vbo_data.push_back(pos.x);
         vbo_data.push_back(pos.y);
         vbo_data.push_back(pos.z);
         vbo_data.push_back(norm.x);
         vbo_data.push_back(norm.y);
         vbo_data.push_back(norm.z);
         vbo_data.push_back(tex.x);
         vbo_data.push_back(tex.y);

      }
   }
}

bool Terrain::is_within(glm::vec2 pos, float offset){
   return is_within({pos.x, 0.0f, pos.y}, offset);

}
bool Terrain::is_within(glm::vec3 pos, float offset){
      return pos.x < height + offset && pos.x > offset && pos.z < width + offset && pos.z > offset;
}
void Terrain::draw_terrain(){
   shd->use();
   texture->use();
   
   glm::mat4 model(1.0f);

   model = glm::translate(model, center_pos);
   model = glm::scale(model, glm::vec3(1.0));

   shd->set_mat4fv("_view", Camera::get_instance()->get_view());
   shd->set_mat4fv("_projection", Camera::get_instance()->get_projection());
   shd->set_mat4fv("_model", model);
   Light::get_instance()->set_all(shd);
   vert.bind();
   glDrawElements(GL_TRIANGLE_STRIP, indices_count, GL_UNSIGNED_INT, 0);
}

void Terrain::generate_random_coordinates(int count, std::vector<glm::vec2>* coordinates){
	std::random_device rd;
	std::mt19937 generator(rd());
   std::uniform_int_distribution row(0, height-1);
   std::uniform_int_distribution column(0, width-1);

   for (int i = 0; i < count; i++){
      const int x = row(generator);
      const int y = column(generator);
      coordinates->push_back({x, y});
   }
}


void Terrain::generate_heights(){
   log_info("generating heights for terrain");

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution hill_radius_distr(MIN_RADIUS, MAX_RADIUS);
	std::uniform_real_distribution hill_height_distr(MIN_HEIGHT, MAX_HEIGHT);
	std::uniform_int_distribution hill_center_row_distr(0, height-1);
	std::uniform_int_distribution hill_center_col_distr(0, width-1);

   heights.resize(height+1, std::vector<float>(width+1, 0));
	for (int i = 0; i < height*2; i++)
	{
		const auto center_row = hill_center_row_distr(generator);
		const auto center_col = hill_center_col_distr(generator);
		const auto radius = hill_radius_distr(generator);
		const auto hill_height = hill_height_distr(generator);

		for (auto r = center_row - radius; r < center_row + radius; r++){
			for (auto c = center_col - radius; c < center_col + radius; c++){
				if (r < 0 || r >=  height || c < 0 || c >= width) {
					continue;
				}
				const auto r2 = radius * radius; 
				const auto x2x1 = center_col - c; 
				const auto y2y1 = center_row - r; 
				const auto height = float(r2 - x2x1 * x2x1 - y2y1 * y2y1);
				if (height < 0.0f) continue;

				const auto factor = height / r2;
				heights[r][c] += hill_height * factor;
				if (heights[r][c] > 1.0f) {
					heights[r][c] = 1.0f;
				}
			}
		}
	}
}
