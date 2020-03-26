#include "TerrainChunk.hpp"

TerrainChunk::~TerrainChunk() {
    delete chunk;
};

TerrainChunk::TerrainChunk(TerrainChunk* _terrainChunk) {
    // copy chunk model
    chunk = new Model(_terrainChunk->chunk);
};
TerrainChunk::TerrainChunk(int posGridX, int posGridZ, bool isFlat) {
    // instintiate new model
    chunk = new Model();
    // chunk->setPolygoneMode(GL_LINE);
    // chunk->setMainShaderProgram(ShaderLoader::getShader("Normal"));

    // textures
    unsigned char* heightMap_pixels = new unsigned char[Config_Terrain::chunksNbrTiles * Config_Terrain::chunksNbrTiles * 3];
    int heightMapPixelIndex = 0;

    unsigned char* normalMap_pixels = new unsigned char[Config_Terrain::chunksNbrTiles * Config_Terrain::chunksNbrTiles * 3];
    int normalMapPixelIndex = 0;

    // calculate pos
    float posX = posGridX * Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize;
    float posZ = posGridZ * Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize;

    static float tilePosX, tilePosZ;
    static float uvPosX, uvPosZ;
    static float uvPixelSize = 1.0f / float(Config_Terrain::chunksNbrTiles);
    static glm::vec3 normal;

    // chunk model generate & add vertecies
	unsigned int indices[(int)(pow(Config_Terrain::chunksNbrTiles, 2) * 6)];
    for (int j = 0; j < Config_Terrain::chunksNbrTiles; j++) {
	    for (int i = 0; i < Config_Terrain::chunksNbrTiles; i++) {
            tilePosX = i * Config_Terrain::tileSize + posX;
            tilePosZ = j * Config_Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

            uvPosX = float(i) / float(Config_Terrain::chunksNbrTiles);
            uvPosZ = float(j) / float(Config_Terrain::chunksNbrTiles);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX, uvPosZ)
    			// glm::vec2(0.0f, 0.0f)
    		);

            //------------------------ Textures --------------------------------
            if (!isFlat) {
                // hights for collider
                // heights.push_back(Bioms::getHight(tilePosX, tilePosZ));

                // hight map
                unsigned char pixelHeight = static_cast<unsigned char>(255 * ((Bioms::getHight(tilePosX, tilePosZ)) / pow(Config_Terrain::amplitude, Config_Terrain::elevation) / 2 + 0.5f));
                for (int k = 0; k < 3; k++) {
                    // heightMap_pixels[heightMapPixelIndex++] = pixelHeight;
                    heightMap_pixels[heightMapPixelIndex++] = static_cast<unsigned char>(255 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
                };

                // normal map
                float gradient = glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f));
                for (int k = 0; k < 3; k++) {
                    normalMap_pixels[normalMapPixelIndex++] = static_cast<unsigned char>(255 * gradient);
                };
            };
            //------------------------------------------------------------------

            tilePosX = (i + 1) * Config_Terrain::tileSize + posX;
            tilePosZ = j * Config_Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX + uvPixelSize, uvPosZ)
    			// glm::vec2(1.0f, 0.0f)
    		);

            tilePosX = i * Config_Terrain::tileSize + posX;
            tilePosZ = (j + 1) * Config_Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX, uvPosZ + uvPixelSize)
    			// glm::vec2(0.0f, 1.0f)
    		);

            tilePosX = (i + 1) * Config_Terrain::tileSize + posX;
            tilePosZ = (j + 1) * Config_Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX + uvPixelSize, uvPosZ + uvPixelSize)
    			// glm::vec2(1.0f, 1.0f)
    		);

            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 0] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 2;
            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 1] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 1;
            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 2] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 0;

            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 3] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 2;
            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 4] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 3;
            indices[(i * Config_Terrain::chunksNbrTiles + j) * 6 + 5] = (i * Config_Terrain::chunksNbrTiles + j) * 4 + 1;
        };
	};
    // load vertecies
	chunk->loadVertecies(indices, pow(Config_Terrain::chunksNbrTiles, 2) * 6, Material());

    //--------------------------------------------------------------------------
    //--------------------------- Textures -------------------------------------
    // generate image
    // std::string defuseTextureName = std::string("white");
    std::string defuseTextureName = std::string("grass1Defuse");

    if (!isFlat) {
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // generate hightmap texture
        std::string heightTextureName = "terrainHeight_" + std::to_string(posGridX) + "_" + std::to_string(posGridZ);
        // defuseTextureName = heightTextureName;


        // stbi_write_jpg(("tmp/" + heightTextureName + ".jpg").c_str(), Config_Terrain::chunksNbrTiles, Config_Terrain::chunksNbrTiles, 3, heightMap_pixels, 100);
        // TextureLoader::createTexture(heightTextureName, "tmp/" + heightTextureName + ".jpg", false);

        // create gl texture object
        unsigned int heightMapTexture;
        glGenTextures(1, &heightMapTexture);
        glBindTexture(GL_TEXTURE_2D, heightMapTexture);

        // // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Config_Terrain::chunksNbrTiles, Config_Terrain::chunksNbrTiles, 0, GL_RGB, GL_UNSIGNED_BYTE, heightMap_pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

        TextureLoader::addTexture(heightTextureName, heightMapTexture);

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // generate normalmap texture
        std::string normalTextureName = "terrainNormal_" + std::to_string(posGridX) + "_" + std::to_string(posGridZ);
        defuseTextureName = normalTextureName;

        // stbi_write_jpg(("tmp/" + normalTextureName + ".jpg").c_str(), Config_Terrain::chunksNbrTiles, Config_Terrain::chunksNbrTiles, 3, normalMap_pixels, 100);
        // TextureLoader::createTexture(normalTextureName, "tmp/" + normalTextureName + ".jpg", false);

        // create gl texture object
        unsigned int normalMapTexture;
        glGenTextures(1, &normalMapTexture);
        glBindTexture(GL_TEXTURE_2D, normalMapTexture);

        // // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // // set texture filtering parameters
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Config_Terrain::chunksNbrTiles, Config_Terrain::chunksNbrTiles, 0, GL_RGB, GL_UNSIGNED_BYTE, normalMap_pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

        TextureLoader::addTexture(normalTextureName, normalMapTexture);

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // hights for collider
        for (int j = 0; j < Config_Terrain::chunksNbrTiles + 1; ++j) {
            for (int i = 0; i < Config_Terrain::chunksNbrTiles + 1; ++i) {
                tilePosX = i * Config_Terrain::tileSize + posX;
                tilePosZ = j * Config_Terrain::tileSize + posZ;
                // for collider
                heights.push_back(Bioms::getHight(tilePosX, tilePosZ));
            };
        };

        // terrain chunk collider
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(posX + Config_Terrain::chunksNbrTiles* Config_Terrain::tileSize / 2, 0.0f, posZ + Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize / 2));

/*
        // plane shape
        t.setOrigin(btVector3(
            posX + Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize/2,
            -1.0f,
            posZ + Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize/2
        ));
        btBoxShape* terrainShape = new btBoxShape(btVector3(Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize/2, 1.0f, Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize/2));
*/

// /*
        // create terrain hight field shape
        btHeightfieldTerrainShape* terrainShape = new btHeightfieldTerrainShape(
            Config_Terrain::chunksNbrTiles + 1,
            Config_Terrain::chunksNbrTiles + 1,
            &heights[0],

            0.0f,
           -pow(Config_Terrain::amplitude, Config_Terrain::elevation),
            pow(Config_Terrain::amplitude, Config_Terrain::elevation),

            1,
            PHY_FLOAT,
            false
        );
// */
        btMotionState* motion = new btDefaultMotionState(t);

        // create box body
        btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, terrainShape);
        info.m_friction = 0.5f; // terrain friction

        // create rigidBody
        btRigidBody* body = new btRigidBody(info);

        // add body to simulation
        Physics::addRigidBody(body);

        // set some attribs
        body->setActivationState(DISABLE_DEACTIVATION);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        // body->setUserPointer(body);
    };
    delete[] heightMap_pixels;
    delete[] normalMap_pixels;

    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
/*
    // chunk model set transform
    chunk->setPos(glm::vec3(
        posX,
        0.0f,
        posZ
    ));
    chunk->updateTransform();
*/
    // cast shadows
    // chunk->castShadow = false;

    // chunk model set material
    glm::vec3 color = glm::vec3(
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
    );
    chunk->meshGroups[0]->material.ambient = color;
    chunk->meshGroups[0]->material.diffuse = color;
    chunk->meshGroups[0]->material.specular = glm::vec3(color.r/4, color.g/4, color.b/4);
    chunk->meshGroups[0]->material.shininess = 8.0f;

    chunk->meshGroups[0]->material.diffuseMap = TextureLoader::getTexture(defuseTextureName);
    chunk->meshGroups[0]->material.ambient = glm::vec3(2.0f, 2.0f, 2.0f);
    chunk->meshGroups[0]->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    chunk->meshGroups[0]->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
};

glm::vec3 TerrainChunk::calculateVertexNormal(int x, int z) {
    // return glm::vec3(0.0f, 1.0f, 0.0f);

    // get nearby verticies height
    float H_Up, H_Down, H_Right, H_Left;

    H_Up    = Bioms::getHight(x, z + Config_Terrain::tileSize);
    H_Down  = Bioms::getHight(x, z - Config_Terrain::tileSize);
    H_Right = Bioms::getHight(x + Config_Terrain::tileSize, z);
    H_Left  = Bioms::getHight(x - Config_Terrain::tileSize, z);

    // calculate & return vertex normal
    return glm::normalize(glm::vec3(
        H_Left - H_Right,
        1.0f,
        H_Down - H_Up
    ));
};

void TerrainChunk::render() {
    // render model
    chunk->render();
};
void TerrainChunk::renderShadow() {
    // render model
    chunk->renderShadow();
};
