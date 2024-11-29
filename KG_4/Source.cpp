#include <GL/freeglut.h>
#include <soil.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
using namespace std;

//������
float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 8.0f;
float cameraAngleHorizontal = 0.0f;
float cameraAngleVertical = 0.0f;
float cameraSpeed = 0.2f;
float lookSpeed = 0.05f;

//��������
GLuint floorTexture, wallTexture, tableTexture, chairTexture, potolokTexture, noutKeyb, noutScreen, noutBack;

//������� �������� ��������
GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (!image) { //��������� ���������� ��������
        cout << "������ �������� ��������: " << filename << " - " << SOIL_last_result() << endl;
        return 0; //���������� 0, ���� �������� �� �������
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //��������� ������ ����������� � ��������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    //����������� ��������� ��������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //����������� ������, ���������� ��� �����������
    SOIL_free_image_data(image);

    return texture;
}

//�������� �������
void loadTextures() {
    floorTexture = loadTexture("floor.jpg");
    wallTexture = loadTexture("wall.jpg");
    tableTexture = loadTexture("table.jpg");
    chairTexture = loadTexture("chair.jpg");
    potolokTexture = loadTexture("potolok.jpg");
    noutKeyb = loadTexture("noutKeyb.jpg");
    noutScreen = loadTexture("noutScreen.jpg");
    noutBack = loadTexture("noutBack.jpg");
}

//��� � ���������
void drawTexturedFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(10.0f, 0.0f, 10.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//������� � ���������
void drawTexturedPotolok() {
    glBindTexture(GL_TEXTURE_2D, potolokTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 4.0f, -10.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(10.0f, 4.0f, 10.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, 4.0f, 10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//����� � ���������
void drawTexturedWall(float x, float y, float z, float width, float height, float angle) {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, height, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, height, 0.0f);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

//�������������� � ���������
void drawTexturedParallelepiped(float width, float height, float depth, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(width, height, depth);

    glBegin(GL_QUADS);
    //�������� �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    //������ �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

    //������� �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    //������ �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

    //������ �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);

    //����� �������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

//���� � ���������
void drawTexturedTable() {
    float legHeight = 1.0f, legWidth = 0.1f;
    float tableTopHeight = 0.1f, tableWidth = 2.0f, tableDepth = 1.0f;

    //����������
    glPushMatrix();
    glTranslatef(0.0f, legHeight + tableTopHeight / 2, 0.0f);
    drawTexturedParallelepiped(tableWidth, tableTopHeight, tableDepth, tableTexture);
    glPopMatrix();

    //�����
    for (float dx = -0.9f; dx <= 0.9f; dx += 1.8f) {
        for (float dz = -0.4f; dz <= 0.4f; dz += 0.8f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawTexturedParallelepiped(legWidth, legHeight, legWidth, tableTexture);
            glPopMatrix();
        }
    }
}

//���� � ���������
void drawTexturedChair() {
    float legHeight = 0.8f, legWidth = 0.1f;
    float seatHeight = 0.1f, seatWidth = 0.8f, seatDepth = 0.8f;
    float backHeight = 1.0f;

    //�������
    glPushMatrix();
    glTranslatef(0.0f, legHeight + seatHeight / 2, 0.0f);
    drawTexturedParallelepiped(seatWidth, seatHeight, seatDepth, chairTexture);
    glPopMatrix();

    //�����
    for (float dx = -0.35f; dx <= 0.35f; dx += 0.7f) {
        for (float dz = -0.35f; dz <= 0.35f; dz += 0.7f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawTexturedParallelepiped(legWidth, legHeight, legWidth, chairTexture);
            glPopMatrix();
        }
    }

    //������
    glPushMatrix();
    glTranslatef(-seatWidth / 2 + 0.05f, legHeight + seatHeight + backHeight / 2, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawTexturedParallelepiped(backHeight, legWidth, seatDepth - 0.1f, chairTexture);
    glPopMatrix();
}



//��������� �����
void initLighting() {
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    GLfloat light0Position[] = { 0.0f, 3.9f, 0.0f, 1.0f };
    GLfloat light0Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);

    glEnable(GL_COLOR_MATERIAL);
}

//����������� �����
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float lookX = cameraX + cos(cameraAngleVertical) * sin(cameraAngleHorizontal);
    float lookY = cameraY + sin(cameraAngleVertical);
    float lookZ = cameraZ - cos(cameraAngleVertical) * cos(cameraAngleHorizontal);

    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    //���
    drawTexturedFloor();
    //�������
    drawTexturedPotolok();
    //�����
    drawTexturedWall(0.0f, 0.0f, -10.0f, 20.0f, 4.0f, 0.0f);
    drawTexturedWall(0.0f, 0.0f, 10.0f, 20.0f, 4.0f, 0.0f);
    drawTexturedWall(-10.0f, 0.0f, 0.0f, 20.0f, 4.0f, 90.0f);
    drawTexturedWall(10.0f, 0.0f, 0.0f, 20.0f, 4.0f, -90.0f);

    //������
    drawTexturedTable();
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, -1.5f);
    drawTexturedChair();
    glPopMatrix();
    //�������
    glPushMatrix();
    glTranslatef(0.0f, 1.1f, 0.0f);
    //drawLaptop();
    glPopMatrix();

    glutSwapBuffers();

}

//��������� ��������
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

//��������� ������
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cameraX += cameraSpeed * sin(cameraAngleHorizontal);
        cameraZ -= cameraSpeed * cos(cameraAngleHorizontal);
        break;
    case 's':
        cameraX -= cameraSpeed * sin(cameraAngleHorizontal);
        cameraZ += cameraSpeed * cos(cameraAngleHorizontal);
        break;
    case 'a':
        cameraX -= cameraSpeed * cos(cameraAngleHorizontal);
        cameraZ -= cameraSpeed * sin(cameraAngleHorizontal);
        break;
    case 'd':
        cameraX += cameraSpeed * cos(cameraAngleHorizontal);
        cameraZ += cameraSpeed * sin(cameraAngleHorizontal);
        break;
    case 'q':
        cameraAngleHorizontal -= lookSpeed;
        break;
    case 'e':
        cameraAngleHorizontal += lookSpeed;
        break;
    case 'r':
        cameraX = 0.0f, cameraY = 2.0f, cameraZ = 8.0f;
        cameraAngleHorizontal = 0.0f; cameraAngleVertical = 0.0f;
        break;
    case 27: exit(0);
    }
    glutPostRedisplay();
}

//��������� ����������� ������
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: //������� ������
        cameraY += cameraSpeed;
        break;
    case GLUT_KEY_DOWN: //�������� ������
        cameraY -= cameraSpeed;
        break;
    case GLUT_KEY_LEFT: //������ �����
        cameraAngleVertical += lookSpeed;
        break;
    case GLUT_KEY_RIGHT: //������ ����
        cameraAngleVertical -= lookSpeed;
        break;
    }
    glutPostRedisplay();
}

//������� �������
int main(int argc, char** argv) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "���������� �������:" << endl;
    cout << "w - �����" << endl;
    cout << "s - �����" << endl;
    cout << "a - �����" << endl;
    cout << "d - ������" << endl;
    cout << "q - ������ �����" << endl;
    cout << "e - ������ ������" << endl;
    cout << "������� ����� - ������� ������" << endl;
    cout << "������� ���� - �������� ������" << endl;
    cout << "������� ����� - ������ �����" << endl;
    cout << "������� ������ - ������ ����" << endl;
    cout << "r - ����� ������" << endl;
    cout << "Esc - �����" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cabinet Scene");

    glEnable(GL_DEPTH_TEST);
    loadTextures();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
