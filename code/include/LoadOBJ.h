#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

//class LoadOBJ
//{
//
//};

namespace loadObject
{
	// Read path, write in out_vertices / out_uvs / out_normals, return false if something went wrong
	bool loadOBJ(const char* path,
		std::vector < glm::vec3 >& out_vertices,
		std::vector < glm::vec2 >& out_uvs,
		std::vector < glm::vec3 >& out_normals)
	{
		// Generate variable to store .obj contents
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		// Open file at path
		FILE* file = fopen(path, "r");
		if (file == NULL)
		{
			// Something went wrong
			printf("Impossible to open the file!\n");
			return false;
		}

		// Read the file untill EOF
		while (1)
		{
			char lineHeader[128];
			// Read the first word of the line
			int res = fscanf(file, "%s", lineHeader);

			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			// Read vertex (v)
			if (strcmp(lineHeader, "v") == 0) {

				glm::vec3 vertex;
				// The rest are 3 floats (v 1.000000 -1.000000 -1.000000)
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			// Read UVs (vt)
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				// The rest are 2 floats (vt 0.748573 0.750412)
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			// Read normals (vn)
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				// The rest are 3 floats (vn 0.000000 0.000000 -1.000000)
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			// Read faces (f)
			else if (strcmp(lineHeader, "f") == 0)
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				// Save 9 ints (f 5/1/1 1/2/1 4/3/1)
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9)
				{
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		// INDEX DATA
		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			// Index to the vertex position
			unsigned int vertexIndex = vertexIndices[i];
			// Position
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			// Position of new vertex
			out_vertices.push_back(vertex);

			// UVs
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);

			// Normals
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			out_normals.push_back(normal);
		}
	}
}