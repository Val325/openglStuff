#include <glad/glad.h>
#include <GLFW/glfw3.h>
//include <stb_image.h>

#include <model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <loadall.hpp>
#include <camera.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

//Size skybox
const unsigned int width = 800;
const unsigned int height = 800;

// Камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;
float lastFrame = 0.0f;
 
// Освещение 
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//framebuffer
bool framebufOn = true;

//VBO Pyramide
GLuint VBOpyramide, VAOpyramide, EBOpyramide, texturePyramide;

//VBO Points
GLuint VBOpoints, VAOpoints, EBOpoints;

float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};



unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}  


float random_float(){
  float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  return r;
}

void renderPoints()
{
GLfloat vertices[] =
{
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),   
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float(),
    random_float(), random_float(), random_float(), random_float()

};

    // This creates our identifier and puts it in vbo
    glGenBuffers(1, &VBOpoints);
    // This binds our vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBOpoints);
    // This hands the vertices into the vbo and to the rendering pipeline    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // "Enable a port" to the shader pipeline
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOpoints);
    // pass information about how vertex array is composed
    glVertexAttribPointer(0, // same as in glEnableVertexAttribArray(0)
                      4, // # of coordinates that build a vertex
                      GL_FLOAT, // data type
                      GL_FALSE, // normalized?
                      0,        // stride
                      (void*)0);// vbo offset
    glPointSize(3);
    glDrawArrays(GL_POINTS, 0, 18);
    glDisableVertexAttribArray(0);
}


void PyramidBuffer()
{
// Position and color data
 // Define a 3D pyramid. 
    GLfloat vertices[] = {

        // Triangle 1
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        -1.0,-1.0,1.0,
        0.0,1.0,0.0,

        1.0,-1.0,1.0,
        0.0,0.0,1.0,

        //Triangle 2
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        1.0,-1.0,1.0,
        0.0,0.0,1.0,

        1.0,-1.0,-1.0,
        0.0,1.0,0.0,

        //Triangle 3
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        1.0,-1.0,-1.0,
        0.0,1.0,0.0,

        -1.0,-1.0,-1.0,
        0.0,0.0,1.0,

        //Triangle 4
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        -1.0,-1.0,-1.0,
        0.0,0.0,1.0,

        -1.0,-1.0,1.0,
        0.0,1.0,0.0

    };
    GLubyte indices[] = {
    // First triangle
        0, 1, 2,
    // Second triangle
        3, 4, 5,
    // Third triangle
        6, 7, 8,
    // Fourth triangle
        9, 10, 11,
    };
  
	// Generate buffer ids
	glGenVertexArrays(1, &VAOpyramide);
	glGenBuffers(1, &VBOpyramide);
	glGenBuffers(1, &EBOpyramide);

	// Activate the vertex array object before binding and settomg any VBOs or vertex attribute pointers
	glBindVertexArray(VAOpyramide);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOpyramide);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Activate the lement buffer object / indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOpyramide);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
			GL_STATIC_DRAW);

	// set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	
	glEnableVertexAttribArray(0);

	// Set attribute pointer 1 to hold Color data
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Sets polygon mode allows me to see wireframe view
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAOpyramide);

}



int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light shit", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    //stbi_set_flip_vertically_on_load(true);
    // Конфигурирование глобального состояния OpenGL
    glEnable(GL_DEPTH_TEST);

    // Компилирование нашей шейдерной программы
    Shader pyramideShader("shaders/basic_lighting_pyramide.vs", "shaders/basic_lighting_pyramide.fs");
    Shader lightingShader("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    Shader lampShader("shaders/lamp.vs", "shaders/lamp.fs");
    Shader pointShader("shaders/point.vs","shaders/point.fs");
    Shader modelShader("shaders/model_loading.vs", "shaders/model_loading.fs");
    Shader framebufferProgram("shaders/framebuffer.vs", "shaders/framebuffer.fs");
    Shader shaderCubemap("shaders/cubemaps.vs", "shaders/cubemaps.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    //loading model
    Model ourModel("model/cat_folder/Cat.obj");
    
    // Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

        
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    // cube VAO skybox
    unsigned int cubeboxVAO, cubeboxVBO;
    glGenVertexArrays(1, &cubeboxVAO);
    glGenBuffers(1, &cubeboxVBO);
    glBindVertexArray(cubeboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);




    
    // load textures
    // -------------
    vector<std::string> faces
    {
        "skybox/negx.jpg",
	"skybox/posx.jpg",
        "skybox/posy.jpg",
        "skybox/negy.jpg",
        "skybox/negz.jpg",
        "skybox/posz.jpg",
    };

    unsigned int cubemapTexture = loadCubemap(faces);

	shaderCubemap.use();
	shaderCubemap.setInt("skybox", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


    	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Cull Facing
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
    
    framebufferProgram.use();
    glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);





    // 1. Настраиваем VAO (и VBO) куба
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // Координатные атрибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
    // Атрибуты нормалей
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2. Настраиваем VAO света (VBO остается неизменным; вершины те же и для светового объекта, который также является 3D-кубом)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Обратите внимание, что мы обновляем шаг атрибута положения лампы, чтобы отразить обновленные данные буфера
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    TextureFromFile("Cat_diffuse.jpg", "model/cat_folder", false);
    TextureFromFile("Cat_bump.jpg", "model/cat_folder", true);

    
    	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
    // Create Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {

        // Логическая часть работы со временем для каждого кадра
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

	
	// Bind the custom framebuffer
	if (framebufOn){
	  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}
        // Обработка ввода
        processInput(window);

	
        // Рендеринг
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	
        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        lightingShader.use(); 
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
	lightingShader.setVec3("viewPos", camera.Position);

	// cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


	
        // Преобразования Вида/Проекции
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);




	
        // Мировое преобразование
        glm::mat4 transform = glm::mat4(1.0f); // сначала инициализируем единичную матрицу
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // Рендеринг куба

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_DEPTH_TEST);
			   // draw scene as normal
        shaderCubemap.use();
        model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        //view = camera.GetViewMatrix();
        //projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderCubemap.setMat4("model", model);
        shaderCubemap.setMat4("view", view);
        shaderCubemap.setMat4("projection", projection);
        shaderCubemap.setVec3("cameraPos", camera.Position);
        // cubes
        glBindVertexArray(cubeboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw skybox as last
        //glDepthFunc(GL_ALWAYS);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        //view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	//glm::mat4 skybox_model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
	skyboxShader.setMat4("model", model);
	skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //glDepthFunc(GL_ALWAYS); // set depth function back to default
	glEnable(GL_DEPTH_TEST);
        // Также отрисовываем наш объект-"лампочку" 
        lampShader.use();               
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f); 
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // куб меньшего размера
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        //PyramidBuffer();
        //

        PyramidBuffer();
        // Создаем преобразование
        glm::mat4 transformPyramide = glm::mat4(1.0f); // сначала инициализируем единичную матрицу
        transformPyramide = glm::translate(transform, glm::vec3(0.0f, -0.5f, 3.0f));
        transformPyramide = glm::scale(transformPyramide, glm::vec3(0.5f)); // куб меньшего размера
        //transformPyramide = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования

        lightingShader.use(); 
        lightingShader.setVec3("objectColor", 0.0f, 0.5f, 0.0f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setMat4("model", transformPyramide);
	
        glBindVertexArray(VAOpyramide);
        glDrawArrays(GL_TRIANGLES, 0, 36);
	//glStencilMask(0xFF);


	lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        renderPoints();
	
        modelShader.use();

        // Преобразования Вида/Проекции
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // Рендеринг загруженной модели
        glm::mat4 model_obj = glm::mat4(1.0f);
        model_obj = glm::translate(model_obj, glm::vec3(2.0f, 2.0f, 0.0f)); // смещаем вниз чтобы быть в центре сцены
        model_obj = glm::scale(model_obj, glm::vec3(0.01f, 0.01f, 0.01f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
	model_obj = glm::rotate(model_obj, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	modelShader.setMat4("model", model_obj);
        ourModel.Draw(modelShader);

	if (framebufOn){
	//
	// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.use();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
;

     



 
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &skyboxVAO);
    if (framebufOn){
      glDeleteFramebuffers(1, &FBO);
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}


// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
