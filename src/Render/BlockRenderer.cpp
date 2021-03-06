#include "BlockRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

BlockRenderer::BlockRenderer(bool is_water): is_water(is_water) {
	std::vector<GLfloat> v;
	gen_vertices_buffer(&v);

	std::vector<GLuint> e;
	gen_elements_buffer(&e);
}

void BlockRenderer::gen_buffer(Block (*blocks)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], int offsetX, int offsetY, int offsetZ, Block(*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	int elemCount = 0;

	int size = CHUNK_SIZE;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				Block block = (*blocks)[x][y][z];
				if (block.type != AIR_BLOCK && ((block.type == WATER_BLOCK && is_water) || (block.type != WATER_BLOCK && !is_water))) {

					bool fill_faces[6];
					std::fill(std::begin(fill_faces), std::end(fill_faces), true);

					bool neighbors_have_transparency[6];
					std::fill(std::begin(neighbors_have_transparency), std::end(neighbors_have_transparency), false);

					int coords[] = {x, y, z};

					for (int i = 0; i < 6; i++) {

						int originalCoord = coords[i / 2];

						int edgeCoord = CHUNK_SIZE - 1;
						int edgeNeighbor = 0;

						if (i % 2 == 0) {
							coords[i / 2] += 1;
						}
						else {
							coords[i / 2] -= 1;

							edgeCoord = 0;
							edgeNeighbor = CHUNK_SIZE - 1;
						}

						int neighborType = AIR_BLOCK;
						if (originalCoord == edgeCoord && neighbor_blocks[i] != nullptr) {
							coords[i / 2] = edgeNeighbor;
							neighborType = (*neighbor_blocks[i])[coords[0]][coords[1]][coords[2]].type;
						}
						else if (coords[i / 2] >= 0 && coords[i / 2] < CHUNK_SIZE) {
							neighborType = (*blocks)[coords[0]][coords[1]][coords[2]].type;
						}


						if (neighborType != AIR_BLOCK && (BLOCK_FACES[neighborType][6] == 0 || BLOCK_FACES[block.type][6] == 2)) {
							fill_faces[i] = false;
						}

						if (BLOCK_FACES[neighborType][6] == 1) {
							neighbors_have_transparency[i] = true;
						}

						coords[i / 2] = originalCoord;
					}

					bool one_filled = false;
					for (int f = 0; f < NELEMS(fill_faces); f++) {
						if (fill_faces[f]) {
							one_filled = true;
							break;
						}
					}

					if (!one_filled) {
						continue;
					}

					int faces[6];
					std::copy_n(std::begin(BLOCK_FACES[block.type]), 6, std::begin(faces));

					const int blocks_per_image = 8;

					const GLfloat image_size = (1.0f / blocks_per_image);

					GLfloat tex[24];

					for (int i = 0; i < NELEMS(faces); i++) {
						GLfloat x = (faces[i] % 8) * image_size;
						tex[i * 4] =  x;
						tex[i * 4 + 1] = x + image_size;

						GLfloat y = (faces[i] / 8) * image_size;
						tex[i * 4 + 2] = y;
						tex[i * 4 + 3] = y + image_size;
					}

					GLfloat verticesArray[] = {

						//  Position    Texcoords
						//x    y     z

						//front-back
						0.5f,  0.5f, 0.5f, tex[0], tex[2], // Top-right
						0.5f,  -0.5f, 0.5f, tex[1], tex[2], // Top-left
						0.5f, -0.5f, -0.5f, tex[1], tex[3],  // Bottom-left
						0.5f, 0.5f, -0.5f, tex[0], tex[3], // Bottom-right

						-0.5f,  -0.5f, 0.5f, tex[0 + 4], tex[2 + 4], // Top-left
						-0.5f,  0.5f, 0.5f, tex[1 + 4], tex[2 + 4], // Top-right
						-0.5f, 0.5f, -0.5f, tex[1 + 4], tex[3 + 4], // Bottom-right
						-0.5f, -0.5f, -0.5f, tex[0 + 4], tex[3 + 4],  // Bottom-left

						//left-right
						-0.5f,  0.5f, 0.5f, tex[0 + 8], tex[2 + 8], // Top-right
						0.5f,  0.5f, 0.5f, tex[1 + 8], tex[2 + 8], // Top-right
						0.5f, 0.5f, -0.5f, tex[1 + 8], tex[3 + 8], // Bottom-right
						-0.5f, 0.5f, -0.5f, tex[0 + 8], tex[3 + 8], // Bottom-right

						0.5f,  -0.5f, 0.5f, tex[0 + 12], tex[2 + 12], // Top-left
						-0.5f,  -0.5f, 0.5f, tex[1 + 12], tex[2 + 12], // Top-left
						-0.5f, -0.5f, -0.5f, tex[1 + 12], tex[3 + 12],  // Bottom-left
						0.5f, -0.5f, -0.5f, tex[0 + 12], tex[3 + 12],  // Bottom-left

						//top-bottom
						- 0.5f,  -0.5f, 0.5f, tex[0 + 16], tex[2 + 16], // Top-left
						0.5f,  -0.5f, 0.5f, tex[1 + 16], tex[2 + 16], // Top-left
						0.5f,  0.5f, 0.5f, tex[1 + 16], tex[3 + 16], // Top-right
						-0.5f,  0.5f, 0.5f, tex[0 + 16], tex[3 + 16], // Top-right

						-0.5f, 0.5f, -0.5f, tex[0 + 20], tex[2 + 20], // Bottom-right
						0.5f, 0.5f, -0.5f, tex[1 + 20], tex[2 + 20], // Bottom-right
						0.5f, -0.5f, -0.5f, tex[1 + 20], tex[3 + 20],  // Bottom-left
						-0.5f, -0.5f, -0.5f, tex[0 + 20], tex[3 + 20]  // Bottom-left
					};

					GLuint elementArray[] = {
						0, 1, 2,
						2, 3, 0
					};

					for (int i = 0; i < NELEMS(verticesArray); i++) {
						if (fill_faces[i / 20]) {
							switch (i % 5) {
							case 0:
								verticesArray[i] += .5f + x + offsetX;
								break;
							case 1:
								verticesArray[i] += .5f + y + offsetY;
								break;
							case 2:
								verticesArray[i] += .5f + z + offsetZ;
								break;
							}
							vertices.push_back(verticesArray[i]);
						}
					}

					for (int i = 0; i < NELEMS(fill_faces); i++) {
						if (fill_faces[i]) {
							for (int reverse = 0; reverse < 2; reverse++) {
								if (reverse == 1 && (block.type == WATER_BLOCK || (BLOCK_FACES[block.type][6] == 1 && !neighbors_have_transparency[i]))) {
									int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;
								}
								else if (reverse != 0) {
									continue;
								}

								for (int j = 0; j < NELEMS(elementArray); j++) {
									elementArray[j] += elemCount;
									elements.push_back(elementArray[j]);
									elementArray[j] -= elemCount;
								}

								if (reverse == 1) {
									int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;
								}
							}

							elemCount += 4;
						}
					}
				}
				else {

				}
			}
		}
	}

	gen_vertices_buffer(&vertices);
	gen_elements_buffer(&elements);
}
