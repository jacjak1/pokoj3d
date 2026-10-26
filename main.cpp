#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "model.h"

// Zmienne globalne i kamera
Camera cam;
bool cursorFree = false, windowOpen = false, isLampOn = true, isDoorOpen = false;
bool isWallLampOn = true;
double lastX, lastY; bool firstMouse = true;
float doorCurrentAngle = 0.0f;
float windowCurrentAngle = 0.0f;

void mouseCB(GLFWwindow* w, double x, double y) { if (!cursorFree) { if (firstMouse) { lastX = x;lastY = y;firstMouse = false; }cam.mouse(float(x - lastX), float(y - lastY));lastX = x;lastY = y; } }
void fbSizeCB(GLFWwindow* w, int width, int height) { glViewport(0, 0, width, height); }
void processInput(GLFWwindow* w, float dt) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(w, 1);
    auto key = [&](int k, char c) { if (glfwGetKey(w, k) == GLFW_PRESS) cam.key(c, dt); };
    key(GLFW_KEY_W, 'W'); key(GLFW_KEY_S, 'S'); key(GLFW_KEY_A, 'A'); key(GLFW_KEY_D, 'D');

    static bool pF = 0, pU = 0, pI = 0, pO = 0, pP = 0;
    bool F = glfwGetKey(w, GLFW_KEY_F1) == GLFW_PRESS;
    bool U = glfwGetKey(w, GLFW_KEY_U) == GLFW_PRESS;
    bool I = glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS;
    bool O = glfwGetKey(w, GLFW_KEY_O) == GLFW_PRESS;
    bool P = glfwGetKey(w, GLFW_KEY_P) == GLFW_PRESS;

    if (F && !pF) { cursorFree = !cursorFree;glfwSetInputMode(w, GLFW_CURSOR, cursorFree ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED); } pF = F;
    if (U && !pU)isDoorOpen = !isDoorOpen; pU = U;
    if (I && !pI)windowOpen = !windowOpen; pI = I;
    if (O && !pO)isLampOn = !isLampOn; pO = O;
    if (P && !pP)isWallLampOn = !isWallLampOn; pP = P;
}

int main()
{
    // Inicjalizacja GLFW i OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
    GLFWwindow* win = glfwCreateWindow(1280, 720, "Projekt - Pokoj", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, fbSizeCB);
    glfwSetCursorPosCallback(win, mouseCB);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shadery i tekstury
    Shader prog("vertex.glsl", "fragment.glsl");
    unsigned int texWall = loadTexture("resources/textures/wall.png");
    unsigned int texFloor = loadTexture("resources/textures/floor.png");
    unsigned int texGlass = loadTexture("resources/textures/glass.png");
    unsigned int texDoor = loadTexture("resources/textures/door.png");
    unsigned int texBed = loadTexture("resources/textures/bed.png");
    unsigned int texDesk = loadTexture("resources/textures/desk.png");

    // Model lozka (OBJ)
    Model bedModel("resources/objects/bed/Full_Size_Bed_with_White_Sheets_Black_V1.obj");

    prog.use();
    prog.setInt("texSampler", 0);

    // Wymiary pokoju
    const float L = -3.f, R = 3.f, B = 0.f, T = 3.f, ZB = -4.f, ZF = 0.f;
    const float winZ_start = -3.f, winZ_end = -1.f, winY_start = 1.f, winY_end = 2.2f;
    const float doorX_start = -0.5f, doorX_end = 0.5f, doorY_end = 2.1f;

    // Geometria pokoju
    float room_vertices[] = { L,B,ZB,0,0,0,0,1, doorX_start,B,ZB,1,0,0,0,1, doorX_start,T,ZB,1,1,0,0,1, L,T,ZB,0,1,0,0,1, doorX_end,B,ZB,0,0,0,0,1, R,B,ZB,1,0,0,0,1, R,T,ZB,1,1,0,0,1, doorX_end,T,ZB,0,1,0,0,1, doorX_start,doorY_end,ZB,0,0,0,0,1, doorX_end,doorY_end,ZB,1,0,0,0,1, doorX_end,T,ZB,1,1,0,0,1, doorX_start,T,ZB,0,1,0,0,1, L,B,ZB,0,1,0,1,0, R,B,ZB,1,1,0,1,0, R,B,ZF,1,0,0,1,0, L,B,ZF,0,0,0,1,0, R,B,ZB,0,0,-1,0,0, R,B,ZF,1,0,-1,0,0, R,winY_start,ZF,1,1,-1,0,0, R,winY_start,ZB,0,1,-1,0,0, R,winY_end,ZB,0,0,-1,0,0, R,winY_end,ZF,1,0,-1,0,0, R,T,ZF,1,1,-1,0,0, R,T,ZB,0,1,-1,0,0, R,winY_start,ZB,0,0,-1,0,0, R,winY_start,winZ_start,1,0,-1,0,0, R,winY_end,winZ_start,1,1,-1,0,0, R,winY_end,ZB,0,1,-1,0,0, R,winY_start,winZ_end,0,0,-1,0,0, R,winY_start,ZF,1,0,-1,0,0, R,winY_end,ZF,1,1,-1,0,0, R,winY_end,winZ_end,0,1,-1,0,0 };
    unsigned int room_indices[] = { 0,1,2,2,3,0, 4,5,6,6,7,4, 8,9,10,10,11,8, 12,13,14,14,15,12, 16,17,18,18,19,16, 20,21,22,22,23,20, 24,25,26,26,27,24, 28,29,30,30,31,28 };
    unsigned int roomVAO, roomVBO, roomEBO;
    glGenVertexArrays(1, &roomVAO); glGenBuffers(1, &roomVBO); glGenBuffers(1, &roomEBO);
    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO); glBufferData(GL_ARRAY_BUFFER, sizeof(room_vertices), room_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roomEBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(room_indices), room_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); glEnableVertexAttribArray(2);

    // Drzwi
    float door_width = doorX_end - doorX_start; float door_height = doorY_end;
    float door_vertices[] = { 0.f,0.f,0.f,0,0,0,0,1, door_width,0.f,0.f,1,0,0,0,1, door_width,door_height,0.f,1,1,0,0,1, 0.f,door_height,0.f,0,1,0,0,1 };
    unsigned int door_indices[] = { 0,1,2,2,3,0 };
    unsigned int doorVAO, doorVBO, doorEBO;
    glGenVertexArrays(1, &doorVAO); glGenBuffers(1, &doorVBO); glGenBuffers(1, &doorEBO);
    glBindVertexArray(doorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO); glBufferData(GL_ARRAY_BUFFER, sizeof(door_vertices), door_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, doorEBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(door_indices), door_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); glEnableVertexAttribArray(2);

    // Okno
    float glass_width = winZ_end - winZ_start; float glass_height = winY_end - winY_start;
    float glass_vertices[] = { 0.f,0.f,0.f,0,0,-1,0,0, glass_width,0.f,0.f,1,0,-1,0,0, glass_width,glass_height,0.f,1,1,-1,0,0, 0.f,glass_height,0.f,0,1,-1,0,0 };
    unsigned int glass_indices[] = { 0,1,2,2,3,0 };
    unsigned int glassVAO, glassVBO, glassEBO;
    glGenVertexArrays(1, &glassVAO); glGenBuffers(1, &glassVBO); glGenBuffers(1, &glassEBO);
    glBindVertexArray(glassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, glassVBO); glBufferData(GL_ARRAY_BUFFER, sizeof(glass_vertices), glass_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glassEBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glass_indices), glass_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); glEnableVertexAttribArray(2);

    // Geometria mebli (szescian)
    float cube_vertices[] = { -.5f,-.5f,-.5f,0,0,0,0,-1,.5f,-.5f,-.5f,1,0,0,0,-1,.5f,.5f,-.5f,1,1,0,0,-1,-.5f,.5f,-.5f,0,1,0,0,-1,-.5f,-.5f,.5f,0,0,0,0,1,.5f,-.5f,.5f,1,0,0,0,1,.5f,.5f,.5f,1,1,0,0,1,-.5f,.5f,.5f,0,1,0,0,1,-.5f,.5f,.5f,1,0,-1,0,0,-.5f,.5f,-.5f,1,1,-1,0,0,-.5f,-.5f,-.5f,0,1,-1,0,0,-.5f,-.5f,.5f,0,0,-1,0,0,.5f,.5f,.5f,1,0,1,0,0,.5f,.5f,-.5f,1,1,1,0,0,.5f,-.5f,-.5f,0,1,1,0,0,.5f,-.5f,.5f,0,0,1,0,0,-.5f,-.5f,-.5f,0,1,0,-1,0,.5f,-.5f,-.5f,1,1,0,-1,0,.5f,-.5f,.5f,1,0,0,-1,0,-.5f,-.5f,.5f,0,0,0,-1,0,-.5f,.5f,-.5f,0,1,0,1,0,.5f,.5f,-.5f,1,1,0,1,0,.5f,.5f,.5f,1,0,0,1,0,-.5f,.5f,.5f,0,0,0,1,0 };
    unsigned int cube_indices[] = { 0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12,16,17,18,18,19,16,20,21,22,22,23,20 };
    unsigned int furnitureVAO, furnitureVBO, furnitureEBO;
    glGenVertexArrays(1, &furnitureVAO);glGenBuffers(1, &furnitureVBO);glGenBuffers(1, &furnitureEBO);
    glBindVertexArray(furnitureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, furnitureVBO);glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, furnitureEBO);glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));glEnableVertexAttribArray(2);

    // Glowna petla programu
    float lastTime = 0.f;
    while (!glfwWindowShouldClose(win))
    {
        float now = float(glfwGetTime()), dt = now - lastTime;lastTime = now;
        processInput(win, dt);
        int w, h;glfwGetFramebufferSize(win, &w, &h);
        glm::mat4 proj = glm::perspective(glm::radians(45.f), float(w) / h, 0.1f, 150.f);
        prog.use();
        prog.setMat4("projection", proj);
        prog.setMat4("view", cam.view());
        prog.setVec3("viewPos", cam.getPosition());

        // Oswietlenie
        prog.setVec3("ambientColor", glm::vec3(0.45f));

        // Lampa sufitowa (klawisz O)
        prog.setBool("spotlights[0].isOn", isLampOn);
        prog.setVec3("spotlights[0].position", glm::vec3(0.f, 2.8f, -2.f));
        prog.setVec3("spotlights[0].direction", glm::vec3(0.f, -1.f, 0.f));
        prog.setVec3("spotlights[0].color", glm::vec3(1.f, 0.95f, 0.85f));
        prog.setVec3("spotlights[0].specular", glm::vec3(1.f));
        prog.setFloat("spotlights[0].cutOff", glm::cos(glm::radians(45.f)));
        prog.setFloat("spotlights[0].outerCutOff", glm::cos(glm::radians(65.f)));

        // Lampa scienna (klawisz P)
        prog.setBool("spotlights[1].isOn", isWallLampOn);
        prog.setVec3("spotlights[1].position", glm::vec3(1.5f, 2.2f, -3.9f));
        prog.setVec3("spotlights[1].direction", glm::vec3(0.0f, -1.0f, 0.0f));
        prog.setVec3("spotlights[1].color", glm::vec3(1.0f, 1.0f, 0.85f));
        prog.setVec3("spotlights[1].specular", glm::vec3(1.0f));
        prog.setFloat("spotlights[1].cutOff", glm::cos(glm::radians(30.0f)));
        prog.setFloat("spotlights[1].outerCutOff", glm::cos(glm::radians(55.0f)));

        glClearColor(0.53f, 0.81f, 0.98f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysowanie sceny
        // Pokoj
        prog.setBool("useTexture", true);
        glBindVertexArray(roomVAO);
        prog.setMat4("model", glm::mat4(1.f));
        glBindTexture(GL_TEXTURE_2D, texWall);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, texFloor);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));
        // Okno
        float windowTargetAngle = windowOpen ? -90.f : 0.f;
        windowCurrentAngle += (windowTargetAngle - windowCurrentAngle) * dt * 5.0f;
        glm::mat4 glassModel = glm::mat4(1.f);
        glassModel = glm::translate(glassModel, glm::vec3(R, winY_start, winZ_start));
        glassModel = glm::rotate(glassModel, glm::radians(windowCurrentAngle), glm::vec3(0, 1, 0));
        prog.setMat4("model", glassModel);
        glDepthMask(GL_FALSE);
        glBindVertexArray(glassVAO);
        glBindTexture(GL_TEXTURE_2D, texGlass);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);

        // Oprawy lamp
        prog.setBool("useTexture", false);
        glBindVertexArray(furnitureVAO);

        prog.setInt("fixtureIndex", 0);
        glm::mat4 ceilingLampModel = glm::mat4(1.f);
        ceilingLampModel = glm::translate(ceilingLampModel, glm::vec3(0.f, 2.9f, -2.f));
        ceilingLampModel = glm::scale(ceilingLampModel, glm::vec3(0.4f, 0.2f, 0.4f));
        prog.setMat4("model", ceilingLampModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        prog.setInt("fixtureIndex", 1);
        glm::mat4 wallLampModel = glm::mat4(1.0f);
        wallLampModel = glm::translate(wallLampModel, glm::vec3(1.5f, 2.2f, ZB + 0.1f));
        wallLampModel = glm::scale(wallLampModel, glm::vec3(0.5f, 0.2f, 0.2f));
        prog.setMat4("model", wallLampModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Drzwi
        float doorTargetAngle = isDoorOpen ? 90.f : 0.f; //Jesli isDoorOpen to true (chcemy otworzyc)  -> cel to 90 stopni. Jesli isDoorOpen to false (chcemy zamknac) -> cel to 0 stopni.
        doorCurrentAngle += (doorTargetAngle - doorCurrentAngle) * dt * 5.0f;
        glBindVertexArray(doorVAO);
        glBindTexture(GL_TEXTURE_2D, texDoor);
        glm::mat4 doorModel = glm::mat4(1.f);
        doorModel = glm::translate(doorModel, glm::vec3(doorX_start, B, ZB));
        doorModel = glm::rotate(doorModel, glm::radians(doorCurrentAngle), glm::vec3(0, 1, 0));
        prog.setMat4("model", doorModel);
        prog.setBool("useTexture", true);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Meble
        glBindVertexArray(furnitureVAO);
        glBindTexture(GL_TEXTURE_2D, texDesk);
        glm::mat4 deskTopModel = glm::mat4(1.f);
        deskTopModel = glm::translate(deskTopModel, glm::vec3(2.4f, 0.75f, -2.f));
        deskTopModel = glm::scale(deskTopModel, glm::vec3(1.f, 0.1f, 2.f));
        prog.setMat4("model", deskTopModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glm::mat4 legModel = glm::mat4(1.f);
        legModel = glm::translate(legModel, glm::vec3(2.4f, 0.375f, -2.f - 0.95f));
        legModel = glm::scale(legModel, glm::vec3(1.f, 0.75f, 0.1f));
        prog.setMat4("model", legModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        legModel = glm::mat4(1.f);
        legModel = glm::translate(legModel, glm::vec3(2.4f, 0.375f, -2.f + 0.95f));
        legModel = glm::scale(legModel, glm::vec3(1.f, 0.75f, 0.1f));
        prog.setMat4("model", legModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Lozko (model OBJ)
        glBindTexture(GL_TEXTURE_2D, texBed);
        glm::mat4 bedObjModel = glm::mat4(1.0f);
        bedObjModel = glm::translate(bedObjModel, glm::vec3(L + 1.2f, 0.55f, -2.5f));
        bedObjModel = glm::rotate(bedObjModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        bedObjModel = glm::scale(bedObjModel, glm::vec3(0.012f));
        prog.setMat4("model", bedObjModel);
        bedModel.draw();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}