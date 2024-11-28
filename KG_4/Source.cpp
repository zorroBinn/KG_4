#include <GL/freeglut.h>

// Камера
float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 8.0f;
float lookX = 0.0f, lookY = 1.0f, lookZ = 0.0f;
float cameraSpeed = 0.2f;

// Функция для создания параллелепипеда
void drawParallelepiped(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Стол (4 ножки + столешница)
void drawTable() {
    float legHeight = 1.0f, legWidth = 0.1f;
    float tableTopHeight = 0.1f, tableWidth = 2.0f, tableDepth = 1.0f;

    glColor3f(0.7f, 0.5f, 0.3f);

    glPushMatrix();
    glTranslatef(0.0f, legHeight + tableTopHeight / 2, 0.0f);
    drawParallelepiped(tableWidth, tableTopHeight, tableDepth);
    glPopMatrix();

    for (float dx = -0.9f; dx <= 0.9f; dx += 1.8f) {
        for (float dz = -0.4f; dz <= 0.4f; dz += 0.8f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawParallelepiped(legWidth, legHeight, legWidth);
            glPopMatrix();
        }
    }
}

// Стул (4 ножки + сиденье + спинка)
void drawChair() {
    float legHeight = 0.8f, legWidth = 0.1f;
    float seatHeight = 0.1f, seatWidth = 0.8f, seatDepth = 0.8f;
    float backHeight = 1.0f;

    glColor3f(0.3f, 0.3f, 0.8f);

    // Сиденье
    glPushMatrix();
    glTranslatef(0.0f, legHeight + seatHeight / 2, 0.0f);
    drawParallelepiped(seatWidth, seatHeight, seatDepth);
    glPopMatrix();

    // Ножки
    for (float dx = -0.35f; dx <= 0.35f; dx += 0.7f) {
        for (float dz = -0.35f; dz <= 0.35f; dz += 0.7f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawParallelepiped(legWidth, legHeight, legWidth);
            glPopMatrix();
        }
    }

    // Спинка (повернута на 90 градусов по OY)
    glPushMatrix();
    glTranslatef(-seatWidth / 2 + 0.05f, legHeight + seatHeight + backHeight / 2, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Поворот спинки
    drawParallelepiped(backHeight, legWidth, seatDepth - 0.1f);
    glPopMatrix();
}

// Ноутбук
void drawLaptop() {
    float baseWidth = 1.0f, baseHeight = 0.05f, baseDepth = 0.6f;
    float lidWidth = 1.0f, lidHeight = 0.05f, lidDepth = 0.6f;

    // Основание ноутбука
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, baseHeight / 2, 0.0f);
    drawParallelepiped(baseWidth, baseHeight, baseDepth);
    glPopMatrix();

    // Крышка ноутбука
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 6 * baseHeight, baseDepth / 2 + 0.1); // Расположить возле длинного края
    glRotatef(110.0f, 1.0f, 0.0f, 0.0f); // Угол открытия крышки
    drawParallelepiped(lidWidth, lidHeight, lidDepth);
    glPopMatrix();
}

// Пол
void drawFloor() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawParallelepiped(10.0f, 0.1f, 10.0f);
    glPopMatrix();
}

// Источники света
void initLighting() {
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    GLfloat light0Position[] = { 0.0f, 4.0f, 0.0f, 1.0f };
    GLfloat light0Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);

    glEnable(GL_LIGHT1);
    GLfloat light1Position[] = { 0.8f, 1.2f, 0.0f, 1.0f };
    GLfloat light1Diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);

    glEnable(GL_LIGHT2);
    GLfloat light2Position[] = { -4.5f, 2.5f, 0.0f, 1.0f };
    GLfloat light2Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);

    glEnable(GL_COLOR_MATERIAL);
}

// Отображение сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    initLighting();

    drawFloor();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    drawTable();
    glTranslatef(-1.5f, 0.0f, -1.5f);
    drawChair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.1f, 0.2f);
    drawLaptop();
    glPopMatrix();

    glutSwapBuffers();
}

// Настройка проекции
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

// Обработка клавиш
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': cameraZ -= cameraSpeed; break;
    case 's': cameraZ += cameraSpeed; break;
    case 'a': cameraX -= cameraSpeed; break;
    case 'd': cameraX += cameraSpeed; break;
    case 'q': cameraY += cameraSpeed; break;
    case 'e': cameraY -= cameraSpeed; break;
    case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}

// Главная функция
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cabinet Scene");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
