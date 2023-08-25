#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "loadall.cpp"
#include "camera.cpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

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

//VBO Pyramide
GLuint VBOpyramide, VAOpyramide, EBOpyramide, texturePyramide;

//VBO Points
GLuint VBOpoints, VAOpoints, EBOpoints;



void renderPoints()
{
GLfloat vertices[] =
{
    2.0f, 1.5f, 0.0f, 1.0f,
    1.0f, 2.0f, 0.0f, 1.0f,
    1.5f, 1.5f, 0.0f, 1.0f
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
    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, 3);
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

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Конфигурирование глобального состояния OpenGL
    glEnable(GL_DEPTH_TEST);

    // Компилирование нашей шейдерной программы
    Shader pyramideShader("basic_lighting_pyramide.vs", "basic_lighting_pyramide.fs");
    Shader lightingShader("basic_lighting.vs", "basic_lighting.fs");
    Shader lampShader("lamp.vs", "lamp.fs");

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


    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Логическая часть работы со временем для каждого кадра
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Обработка ввода
        processInput(window);

        // Рендеринг
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
        lightingShader.use(); 
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

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
        lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setMat4("model", transformPyramide);

        glBindVertexArray(VAOpyramide);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        renderPoints();
        glDrawArrays(GL_POINTS, 0, 1);
        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
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
