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
    chunk->setMainShaderProgram(ShaderLoader::getShader("Terrain"));
    // chunk->setPolygoneMode(GL_LINE);

    // textures
    unsigned char* heightMap_pixels = new unsigned char[Config::Terrain::chunksNbrTiles * Config::Terrain::chunksNbrTiles * 3];
    int heightMapPixelIndex = 0;

    // calculate pos
    float posX = posGridX * Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize;
    float posZ = posGridZ * Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize;

    static float tilePosX, tilePosZ;
    static float uvPosX, uvPosZ;
    static float uvPixelSize = 1.0f / float(Config::Terrain::chunksNbrTiles);
    static glm::vec3 normal;

    // chunk model generate & add vertecies
	unsigned int indices[(int)(pow(Config::Terrain::chunksNbrTiles, 2) * 6)];
    for (int j = 0; j < Config::Terrain::chunksNbrTiles; j++) {
	    for (int i = 0; i < Config::Terrain::chunksNbrTiles; i++) {
            tilePosX = i * Config::Terrain::tileSize + posX;
            tilePosZ = j * Config::Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

            uvPosX = float(i) / float(Config::Terrain::chunksNbrTiles);
            uvPosZ = float(j) / float(Config::Terrain::chunksNbrTiles);

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
                unsigned char pixelHeight = static_cast<unsigned char>(255 * ((Bioms::getHight(tilePosX, tilePosZ)) / pow(Config::Terrain::amplitude, Config::Terrain::elevation) / 2 + 0.5f));
                for (int k = 0; k < 3; k++) {
                    // heightMap_pixels[heightMapPixelIndex++] = pixelHeight;
                    heightMap_pixels[heightMapPixelIndex++] = static_cast<unsigned char>(255 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
                };
            };
            //------------------------------------------------------------------

            tilePosX = (i + 1) * Config::Terrain::tileSize + posX;
            tilePosZ = j * Config::Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX + uvPixelSize, uvPosZ)
    			// glm::vec2(1.0f, 0.0f)
    		);

            tilePosX = i * Config::Terrain::tileSize + posX;
            tilePosZ = (j + 1) * Config::Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX, uvPosZ + uvPixelSize)
    			// glm::vec2(0.0f, 1.0f)
    		);

            tilePosX = (i + 1) * Config::Terrain::tileSize + posX;
            tilePosZ = (j + 1) * Config::Terrain::tileSize + posZ;
            normal = isFlat ? glm::vec3(0.0f, 1.0f, 0.0f) : calculateVertexNormal(tilePosX, tilePosZ);

    		chunk->addVertex(
    			glm::vec3(tilePosX, isFlat ? 0.0f : Bioms::getHight(tilePosX, tilePosZ), tilePosZ),
    			normal,
    			glm::vec2(uvPosX + uvPixelSize, uvPosZ + uvPixelSize)
    			// glm::vec2(1.0f, 1.0f)
    		);

            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 0] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 2;
            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 1] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 1;
            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 2] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 0;

            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 3] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 2;
            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 4] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 3;
            indices[(i * Config::Terrain::chunksNbrTiles + j) * 6 + 5] = (i * Config::Terrain::chunksNbrTiles + j) * 4 + 1;
        };
	};
    // load vertecies
	chunk->loadVertecies(indices, pow(Config::Terrain::chunksNbrTiles, 2) * 6, Material());

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


        // stbi_write_jpg(("tmp/" + heightTextureName + ".jpg").c_str(), Config::Terrain::chunksNbrTiles, Config::Terrain::chunksNbrTiles, 3, heightMap_pixels, 100);
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Config::Terrain::chunksNbrTiles, Config::Terrain::chunksNbrTiles, 0, GL_RGB, GL_UNSIGNED_BYTE, heightMap_pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

        TextureLoader::addTexture(heightTextureName, heightMapTexture);

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // hights for collider
        for (int j = 0; j < Config::Terrain::chunksNbrTiles + 1; ++j) {
            for (int i = 0; i < Config::Terrain::chunksNbrTiles + 1; ++i) {
                tilePosX = i * Config::Terrain::tileSize + posX;
                tilePosZ = j * Config::Terrain::tileSize + posZ;
                // for collider
                heights.push_back(Bioms::getHight(tilePosX, tilePosZ));
            };
        };

        // terrain chunk collider
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(posX + Config::Terrain::chunksNbrTiles* Config::Terrain::tileSize / 2, 0.0f, posZ + Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize / 2));

/*
        // plane shape
        t.setOrigin(btVector3(
            posX + Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize/2,
            -1.0f,
            posZ + Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize/2
        ));
        btBoxShape* terrainShape = new btBoxShape(btVector3(Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize/2, 1.0f, Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize/2));
*/

// /*
        // create terrain hight field shape
        btHeightfieldTerrainShape* terrainShape = new btHeightfieldTerrainShape(
            Config::Terrain::chunksNbrTiles + 1,
            Config::Terrain::chunksNbrTiles + 1,
            &heights[0],

            0.0f,
           -pow(Config::Terrain::amplitude, Config::Terrain::elevation),
            pow(Config::Terrain::amplitude, Config::Terrain::elevation),

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
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
        // body->setUserPointer(body);
    };
    delete[] heightMap_pixels;

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
    // chunk->meshGroups[0]->material.ambient = color;
    // chunk->meshGroups[0]->material.diffuse = color;
    // chunk->meshGroups[0]->material.specular = glm::vec3(color.r/4, color.g/4, color.b/4);
    // chunk->meshGroups[0]->material.shininess = 8.0f;

    // chunk->meshGroups[0]->material.diffuseMap = TextureLoader::getTexture(defuseTextureName);
    // chunk->meshGroups[0]->material.ambient = glm::vec3(2.0f, 2.0f, 2.0f);
    // chunk->meshGroups[0]->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    // chunk->meshGroups[0]->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);


    chunk->meshGroups[0]->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    chunk->meshGroups[0]->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    chunk->meshGroups[0]->material.specular = glm::vec3(0.1f, 0.1f, 0.1f);
    chunk->meshGroups[0]->material.shininess = 50.0f;
};

glm::vec3 TerrainChunk::calculateVertexNormal(int x, int z) {
    // return glm::vec3(0.0f, 1.0f, 0.0f);

    // get nearby verticies height
    float H_Up, H_Down, H_Right, H_Left;

    H_Up    = Bioms::getHight(x, z + Config::Terrain::tileSize);
    H_Down  = Bioms::getHight(x, z - Config::Terrain::tileSize);
    H_Right = Bioms::getHight(x + Config::Terrain::tileSize, z);
    H_Left  = Bioms::getHight(x - Config::Terrain::tileSize, z);

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
