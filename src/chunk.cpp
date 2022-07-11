#include "chunk.h"

Chunk::Chunk(int cx, int cy, World *world, ResourceManager *resourceManager) {
  this -> cx = cx;
  this -> cy = cy;
  this -> resourceManager = resourceManager;
  this -> world = world;

  glGenBuffers(1, &bufferId);
}

std::string Chunk::getBlock(int x, int y, int z) {
  if (y > 255 || y < 0) {
    return "air";
  }

  short block = blockData[x][z][y];
  if (block == 0) {
    return "air";
  }

  return blockMap[block];
}

void Chunk::fill(std::string block) {
  blockMap.empty();

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 16; z++) {
        if (rand() % 200 <= 198) {
          setBlock(x, y, z, "grass");
        } else {
          setBlock(x, y, z, block);
        }
      }
    }
  }
}

std::vector<Vertex> Chunk::genVertexArray() {
  Chunk* northChunk = world->getChunk(cx - 1, cy);
  Chunk* eastChunk = world->getChunk(cx, cy + 1);
  Chunk* southChunk = world->getChunk(cx + 1, cy);
  Chunk* westChunk = world->getChunk(cx, cy - 1);

  std::cout << (northChunk == NULL) << std::endl;

  std::vector<Vertex> vertexVector;
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 16; z++) {
        int bx = x + (cx * 16);
        int bz = z + (cy * 16);

        short id = blockData[x][z][y];
        if (id == 0) continue;

        std::string block = blockMap[id];

        if ((x < 15 && blockData[x + 1][z][y] == 0) || (x == 15 && southChunk != NULL && southChunk->getUnmappedBlock(0, y, z) == 0)) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 3, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }

        if ((x > 0 && blockData[x - 1][z][y] == 0) || (x == 0 && northChunk != NULL && northChunk->getUnmappedBlock(15, y, z) == 0)) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 1, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }

        if (y == 255 || blockData[x][z][y + 1] == 0) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 0, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }

        if (y == 0 || blockData[x][z][y - 1] == 0) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 5, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }

        if ((z < 15 && blockData[x][z + 1][y] == 0) || (z == 15 && eastChunk != NULL && eastChunk->getUnmappedBlock(x, y, 0) == 0)) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 2, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }

        if ((z > 0 && blockData[x][z - 1][y] == 0) || (z == 0 && westChunk != NULL && westChunk->getUnmappedBlock(x, y, 15) == 0)) {
          Vertex faceVertices[6];
          resourceManager->getBlock(block).getVerticies(bx, y, bz, 4, faceVertices);
          for (int i = 0; i < 6; i++) {
            vertexVector.push_back(faceVertices[i]);
          }
        }
      }
    }
  }

  return vertexVector;
}

void Chunk::draw() {
  glBindBuffer(GL_ARRAY_BUFFER, bufferId);

  // Position Vertex Attribute
	glVertexAttribPointer (
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex), 
		(void*)offsetof(Vertex, position)
	);
	glEnableVertexAttribArray(0);
	
	// Texture Coords Attribute
	glVertexAttribPointer (
		1, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex), 
		(void*)offsetof(Vertex, textureCoords)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer (
		2,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, lighting)
	);
	glEnableVertexAttribArray(2);

  glDrawArrays(GL_TRIANGLES, 0, vboSize);
}

void Chunk::update() {
  std::cout << "Updating chunk " << cx << " " << cy << std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  std::vector data = genVertexArray();
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Finished chunk " << cx << " " << cy << " in " << duration.count() << " microseconds" << std::endl;

  vboSize = data.size();

  glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
}

void Chunk::unload() {
  glDeleteBuffers(1, &bufferId);
}

void Chunk::setBlock(int x, int y, int z, std::string block) {
  int id = 0;

  if (block != "air") {
    for (auto &i : blockMap) {
      if (i.second == block) {
        id = i.first;
        break;
      }
    }
    if (id == 0) {
      id = blockMap.size() + 1;
      blockMap.insert({id, block});
    }
  }

  blockData[x][z][y] = id;
}

int Chunk::getUnmappedBlock(int x, int y, int z) {
  return blockData[x][z][y];
}