#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "camera.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow * window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


string path           = "/Users/walker/myOpenGL_Light/openGL-Light/openGL-Light/";
string sourcevertex   = path + "/shaders/source.vs";
string sourcefragment = path + "/shaders/source.fs";
string objectvertex   = path + "/shaders/object.vs";
string objectfragment = path + "/shaders/object.fs";
string container2     = path + "/image/container2.png";
string specular       = path + "/image/specular.png";
//string specular2      = path + "/image/specular2.png";
//string emission       = path + "/image/emission.jpeg";
//string container      = path + "/image/container.jpeg";
//string awesomeface    = path + "/image/awesomeface.png";

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f;    // Time of last frame

int main () {
    
    //init the glfw
    glfwInit();
    //set some attributes
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //set the window (width , height , name , default , default)
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //initial GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" <<endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glEnable(GL_DEPTH_TEST);
    Shader lighteningShader(sourcevertex.c_str() , sourcefragment.c_str());
    Shader objectShader(objectvertex.c_str() , objectfragment.c_str());
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    unsigned int cubeVAO , VBO ;
    glGenVertexArrays(1, &cubeVAO); //generate one VAO (return an ID for each)
    glGenBuffers(1, &VBO); //generate one VBO (return an ID for each)
    //bind first VAO (object)
    glBindVertexArray(cubeVAO);
    //copy our vertices array in VBO for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //set the Vertex Attribute Pointers
    //glVertexAttribPointer (position , size , datatype , whether to normalized , strides , offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int diffuseMap     = loadTexture(container2.c_str());
    unsigned int specularMap    = loadTexture(specular.c_str());
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    //bind secondt VAO (light source)
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //turn the image upside down
    //stbi_set_flip_vertically_on_load(true);
    
    //render loop
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // lighting settings
        glm::vec3 lightPos(1.5f , 1.0f , 2.0f);
        glm::vec3 lightColor(1.0f , 1.0f , 1.0f);
        glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
        /*
        float time = glfwGetTime();
        float radius = 2.0f;
        
       
        glm::vec3 lightColor;
        lightColor.x = sin(time);
        lightColor.y = cos(time);
        lightColor.z = ( sin(time) + cos(time) ) / sqrt(2);
        
        glm::vec3 ambientColor = lightColor * 0.5f;
        glm::vec3 diffuseColor = lightColor * 0.2f;
        
        glm::vec3 lightPos(radius * cos(time), 1.0f, radius * sin(time));
        */
        // input
        processInput(window); //check if any button is triggered
        // rendering commands here
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        //draw the object
        //draw the lightened-object first
        objectShader.use();
        
        glUniform3fv(glGetUniformLocation(objectShader.ID, "lightPos"), 1 , &lightPos[0] );
        //set material
        glUniform1i(glGetUniformLocation(objectShader.ID, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(objectShader.ID, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(objectShader.ID, "material.shininess"), 64.0f);
        //set directional light
        glUniform3f(glGetUniformLocation(objectShader.ID, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform3fv(glGetUniformLocation(objectShader.ID, "dirLight.direction"), 1 , &lightDir[0]);
        //set point light1
        glUniform3fv(glGetUniformLocation(objectShader.ID, "pointLights[0].position"), 1 , &pointLightPositions[0][0]);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[0].quadratic"), 0.032f);
        //set point light2
        glUniform3fv(glGetUniformLocation(objectShader.ID, "pointLights[1].position"), 1 , &pointLightPositions[1][0]);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[1].quadratic"), 0.032f);
        //set point light3
        glUniform3fv(glGetUniformLocation(objectShader.ID, "pointLights[2].position"), 1 , &pointLightPositions[2][0]);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[2].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[2].quadratic"), 0.032f);
        //set point light3
        glUniform3fv(glGetUniformLocation(objectShader.ID, "pointLights[3].position"), 1 , &pointLightPositions[3][0]);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[3].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "pointLights[3].quadratic"), 0.032f);
        //set spotlight
        glUniform3fv(glGetUniformLocation(objectShader.ID, "spotlight.position"), 1 , &camera.Position[0]);
        glUniform3fv(glGetUniformLocation(objectShader.ID, "spotlight.direction"), 1 , &camera.Front[0]);
        glUniform3f(glGetUniformLocation(objectShader.ID, "spotlight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "spotlight.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objectShader.ID, "spotlight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "spotlight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "spotlight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "spotlight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(objectShader.ID, "spotlight.InnerCutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(objectShader.ID, "spotlight.OuterCutOff"), glm::cos(glm::radians(17.5f)));

        //create matrices
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        
        
        glBindVertexArray(cubeVAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glm::mat4 inverseModel = inverse(view * model);
            glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "inverseModel"), 1, GL_FALSE, glm::value_ptr(inverseModel));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    
        //then draw the lightening-object
        lighteningShader.use();
        
        glUniform3fv(glGetUniformLocation(lighteningShader.ID, "lightColor"), 1 , &lightColor[0]);
        
        glUniformMatrix4fv(glGetUniformLocation(lighteningShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lighteningShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(lightVAO);
        for(unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            glUniformMatrix4fv(glGetUniformLocation(lighteningShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glm::mat4 inverseModel = inverse(view * model);
            glUniformMatrix4fv(glGetUniformLocation(lighteningShader.ID, "inverseModel"), 1, GL_FALSE,glm::value_ptr(inverseModel));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //check and call events and swap the buffers
        glfwSwapBuffers(window); //swap the on-ready buffer with the current buffer
        glfwPollEvents(); //check if any events are triggered
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    
    glfwTerminate();
    return 0;
}
//each time the user resizes the window , it will goes to this callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //gives the cooidinates of the left-bottom point and the right-top point to normalize
    cout << width <<' '<< height << endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
unsigned int loadTexture(char const * path) {
    unsigned int textureID;
    glGenTextures(1 , &textureID);
    
    int width=0, height=0, nrComponents=0;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        cout << "texture width: "  << width << std::endl;
        cout << "texture height: " << height << std::endl;
        cout << "texture nrComponents: "  << nrComponents << std::endl;
        
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            cout << "texture nrComponents not support" << std::endl;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}
