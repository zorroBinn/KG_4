#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <soil.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
using namespace std;

//Камера
float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 8.0f;
float cameraAngleHorizontal = 0.0f;
float cameraAngleVertical = 0.0f;
float cameraSpeed = 0.15f;
float lookSpeed = 0.04f;

//Текстуры предметов
GLuint floorTexture, wallTexture, tableTexture, chairTexture, potolokTexture, noutKeyb, noutScreen, noutBack, doorTexture, shkafTexture, windowTexture, clockTexture, kartinaTexture;

//Функция для настройки материалов
void setMaterial(float ambient[4], float diffuse[4], float specular[4], float shininess) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient); //Задание фонового цвета материала
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); //Задание рассеянного цвета материала
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular); //Задание зеркального цвета материала
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); //Задание гладкости поверхности
}

//Функция загрузки текстуры
GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (!image) { //Проверяем успешность загрузки
        cout << "Ошибка загрузки текстуры: " << filename << " - " << SOIL_last_result() << endl;
        return 0; //Возвращаем 0, если загрузка не удалась
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //Загружаем данные изображения в текстуру
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    //Настраиваем параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Освобождаем память, выделенную под изображение
    SOIL_free_image_data(image);

    return texture;
}

//Загрузка текстур
void loadTextures() {
    floorTexture = loadTexture("floor.jpg");
    wallTexture = loadTexture("wall.jpg");
    tableTexture = loadTexture("table.jpg");
    chairTexture = loadTexture("chair.jpg");
    potolokTexture = loadTexture("potolok.jpg");
    noutKeyb = loadTexture("noutKeyb.jpg");
    noutScreen = loadTexture("noutScreen.jpg");
    noutBack = loadTexture("noutBack.jpg");
    doorTexture = loadTexture("door.jpg");
    shkafTexture = loadTexture("shkaf.jpg");
    windowTexture = loadTexture("window.jpg");
    clockTexture = loadTexture("clock.jpg");
    kartinaTexture = loadTexture("kartina.jpg");
}

//Пол
void drawFloor() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(10.0f, 0.0f, 10.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//Потолок
void drawPotolok() {
    glBindTexture(GL_TEXTURE_2D, potolokTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 4.0f, -10.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(10.0f, 4.0f, 10.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, 4.0f, 10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//Стена
void drawWall(float x, float y, float z, float width, float height, float angle) {
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, height, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, height, 0.0f);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

//Картина
void drawPicture() {
    float width = 0.5f, height = 0.6f; //Размеры картины

    glBindTexture(GL_TEXTURE_2D, kartinaTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -9.99f); //Позиция картины на задней стене
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, height / 2, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, height / 2, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, -height / 2, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, -height / 2, 0.0f);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//Параллелепипед с текстурой
void drawTexturedParallelepiped(float width, float height, float depth, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(width, height, depth);

    glBegin(GL_QUADS);
    //Передняя сторона
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    //Задняя сторона
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

    //Верхняя сторона
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    //Нижняя сторона
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

    //Правая сторона
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);

    //Левая сторона
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

//Окно
void drawWindow() {
    float width = 2.5f, height = 2.0f, depth = 0.03f; //Размеры окна

    glPushMatrix();
    glTranslatef(9.99f, 2.0f, 0.0f); //Позиция окна на правой стене
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); //Разворачиваем окно вдоль стены
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    drawTexturedParallelepiped(width, height, depth, windowTexture);

    glPopMatrix();
}

//Часы
void drawClock() {
    float width = 1.0f, height = 1.0f, depth = 0.03f; //Размеры часов

    glPushMatrix();
    glTranslatef(-9.99f, 3.0f, 0.0f); //Позиция часов на левой стене
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    drawTexturedParallelepiped(width, height, depth, clockTexture);
    glPopMatrix();
}

//Стол
void drawTable() {
    float legHeight = 1.0f, legWidth = 0.1f;
    float tableTopHeight = 0.1f, tableWidth = 2.0f, tableDepth = 1.0f;

    //Столешница
    glPushMatrix();
    glTranslatef(0.0f, legHeight + tableTopHeight / 2, 0.0f);
    drawTexturedParallelepiped(tableWidth, tableTopHeight, tableDepth, tableTexture);
    glPopMatrix();

    //Ножки
    for (float dx = -0.9f; dx <= 0.9f; dx += 1.8f) {
        for (float dz = -0.4f; dz <= 0.4f; dz += 0.8f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawTexturedParallelepiped(legWidth, legHeight, legWidth, tableTexture);
            glPopMatrix();
        }
    }
}

//Стул
void drawChair() {
    float legHeight = 0.7f, legWidth = 0.075f;
    float seatHeight = 0.05f, seatWidth = 0.8f, seatDepth = 0.8f;
    float backHeight = 0.7f;

    //Сиденье
    glPushMatrix();
    glTranslatef(0.0f, legHeight + seatHeight / 2, 0.0f);
    drawTexturedParallelepiped(seatWidth, seatHeight, seatDepth, chairTexture);
    glPopMatrix();

    //Ножки
    for (float dx = -0.35f; dx <= 0.35f; dx += 0.7f) {
        for (float dz = -0.35f; dz <= 0.35f; dz += 0.7f) {
            glPushMatrix();
            glTranslatef(dx, legHeight / 2, dz);
            drawTexturedParallelepiped(legWidth, legHeight, legWidth, chairTexture);
            glPopMatrix();
        }
    }

    //Спинка
    glPushMatrix();
    glTranslatef(-seatWidth / 2 + 0.05f, legHeight + seatHeight + backHeight / 2, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawTexturedParallelepiped(backHeight, legWidth, seatDepth - 0.1f, chairTexture);
    glPopMatrix();
}

//Дверь
void drawDoor() {
    float width = 1.5f, height = 3.0f, depth = 0.05f;

    glPushMatrix();
    glTranslatef(0.0f, height / 2 - 0.05, 9.97f); //Положение двери за первоначальной позицией камеры
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    drawTexturedParallelepiped(width, height, depth, doorTexture);
    glPopMatrix();
}

//Ноутбук
void drawLaptop() {
    //Размеры основания (клавиатура)
    float baseWidth = 0.6f; //Ширина основания
    float baseHeight = 0.015f; //Высота основания
    float baseDepth = 0.4f; //Глубина основания

    //Размеры крышки (экран)
    float lidWidth = baseWidth; //Ширина крышки
    float lidHeight = 0.4f; //Высота крышки
    float lidDepth = 0.004f; //Толщина крышки

    //Угол открытия крышки
    float lidAngle = 10.0f;

    //Поворот ноутбука по оси OY
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); //Поворот ноутбука на 180 градусов по оси OY

    //Основание (клавиатура)
    glPushMatrix();
    drawTexturedParallelepiped(baseWidth+0.005, baseHeight, baseDepth, noutKeyb); //Отрисовка основания с текстурой клавиатуры
    glPopMatrix();

    //Крышка (экран + задняя сторона)
    glPushMatrix();
    glTranslatef(0.0f, 0.39f, -baseDepth * 0.68); //Смещение к основанию крышки
    glRotatef(-lidAngle, 1.0f, 0.0f, 0.0f); //Открытие крышки на заданный угол

    //Поворот всей крышки на 180 градусов по оси OZ
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

    //Экран
    glPushMatrix();
    glTranslatef(0.0f, lidHeight / 2, lidDepth / 2); //Смещение для передней стороны (экран)
    drawTexturedParallelepiped(lidWidth, lidHeight, lidDepth, noutScreen); //Отрисовка экрана с текстурой
    glPopMatrix();

    //Задняя сторона
    glPushMatrix();
    glTranslatef(0.0f, lidHeight / 2, -lidDepth / 2); //Смещение для задней стороны
    drawTexturedParallelepiped(lidWidth + 0.001, lidHeight + 0.001, lidDepth + 0.007, noutBack); //Отрисовка задней стороны с текстурой
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

//Шкаф
void drawShkaf(float x, float y, float z, float width, float height, float depth, float angle) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    //Передняя сторона с текстурой шкафа
    glBindTexture(GL_TEXTURE_2D, shkafTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, height, depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, height, depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, 0.0f, depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, 0.0f, depth / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    //Остальные стороны с текстурой стола
    glBindTexture(GL_TEXTURE_2D, tableTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    //Задняя сторона
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(width / 2, 0.0f, -depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-width / 2, 0.0f, -depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-width / 2, height, -depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(width / 2, height, -depth / 2);

    //Левая сторона
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, 0.0f, -depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-width / 2, 0.0f, depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-width / 2, height, depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, height, -depth / 2);

    //Правая сторона
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(width / 2, 0.0f, depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, 0.0f, -depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, height, -depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(width / 2, height, depth / 2);

    //Верхняя сторона
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, height, depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, height, depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, height, -depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, height, -depth / 2);

    //Нижняя сторона
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, 0.0f, depth / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, 0.0f, depth / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, 0.0f, -depth / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, 0.0f, -depth / 2);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

//Источники света - посреди потолка комнаты + экран ноутбука
void initLighting() {
    glEnable(GL_LIGHTING); //Включение освещения
    
    glEnable(GL_LIGHT0); //Источник света 0
    //Цвет освещения
    GLfloat light0Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //Мягкий белый свет
    GLfloat light0Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Блики
    GLfloat light0Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Лёгкий фоновый свет
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f); //Затухание света с расстоянием

    //Источник света 1 (экран ноутбука)
    //glEnable(GL_LIGHT1);
    GLfloat light1Diffuse[] = { 0.4f, 0.6f, 1.0f, 1.0f }; //Холодный свет
    GLfloat light1Specular[] = { 0.5f, 0.7f, 1.0f, 1.0f }; //Блики
    GLfloat light1Ambient[] = { 0.1f, 0.2f, 0.3f, 1.0f }; //Фоновый свет
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f); //Затухание света с расстоянием

    //Настройка материалов сцены
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Блики от источника света
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMateriali(GL_FRONT, GL_SHININESS, 64); //Гладкость поверхности
}

//Отображение сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float lookX = cameraX + cos(cameraAngleVertical) * sin(cameraAngleHorizontal);
    float lookY = cameraY + sin(cameraAngleVertical);
    float lookZ = cameraZ - cos(cameraAngleVertical) * cos(cameraAngleHorizontal);

    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
    
    glPushMatrix();
    //Позиция источника света (центр потолка)
    GLfloat light0Position[] = { 0.0f, 3.9f, 0.0f, 1.0f }; //Точечный свет
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glPopMatrix();

    //Светящийся кубик - для визуализации точки освещения на потолке
    glPushMatrix();
    GLfloat emissionColor[] = { 1.0f, 1.0f, 0.8f, 1.0f }; //Cвечение
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor); //Включаем эмиссию
    glTranslatef(0.0f, 3.9f, 0.0f); //Позиция источника света
    glutSolidCube(0.1f); //Кубик размером 0.3
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission); //Отключаем эмиссию
    glPopMatrix();

    //Устанавливаем позицию света на экране ноутбука
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f); //Положение экрана ноутбука
    GLfloat light1Position[] = { 0.0f, 0.39f, -0.5f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    GLfloat light1Direction[] = { 0.0f, 1.0f, -1.0f }; //Направление: вверх и назад (для камеры вперёд)
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1Direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0f); //Угол освещения
    glPopMatrix();

    ////Светящийся кубик - для визуализации точки освещения на экране
    //glPushMatrix();
    //GLfloat emission1Color[] = { 0.4f, 0.6f, 1.0f, 1.0f }; //Cвечение
    //glMaterialfv(GL_FRONT, GL_EMISSION, emission1Color); //Включаем эмиссию
    //glTranslatef(0.0f, 1.3f, 0.2f); //Позиция источника света
    //glutSolidCube(0.1f); //Кубик размером 0.3
    //GLfloat noEmission1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //glMaterialfv(GL_FRONT, GL_EMISSION, noEmission1); //Отключаем эмиссию
    //glPopMatrix();

    //Материал для пола
    float floorAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float floorDiffuse[] = { 0.6f, 0.5f, 0.4f, 1.0f };
    float floorSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    setMaterial(floorAmbient, floorDiffuse, floorSpecular, 10.0f);
    drawFloor(); //Пол
    
    //Материал для потолка
    float potolokAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float potolokDiffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    float potolokSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    setMaterial(potolokAmbient, potolokDiffuse, potolokSpecular, 5.0f);
    drawPotolok(); //Потолок
    
    //Материал для двери
    float doorAmbient[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    float doorDiffuse[] = { 0.7f, 0.5f, 0.3f, 1.0f };
    float doorSpecular[] = { 0.2f, 0.1f, 0.05f, 1.0f };
    setMaterial(doorAmbient, doorDiffuse, doorSpecular, 15.0f);
    drawDoor(); //Дверь
    
    //Материал для картины
    float pictureAmbient[] = { 0.3f, 0.3f, 0.2f, 1.0f };
    float pictureDiffuse[] = { 0.8f, 0.7f, 0.6f, 1.0f };
    float pictureSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    setMaterial(pictureAmbient, pictureDiffuse, pictureSpecular, 10.0f);
    drawPicture(); //Картина
    
    //Материал для окна
    float windowAmbient[] = { 0.2f, 0.3f, 0.4f, 1.0f };
    float windowDiffuse[] = { 0.6f, 0.7f, 0.9f, 1.0f };
    float windowSpecular[] = { 0.2f, 0.2f, 0.3f, 1.0f };
    setMaterial(windowAmbient, windowDiffuse, windowSpecular, 15.0f);
    drawWindow(); //Окно
    
    //Материал для часов
    float clockAmbient[] = { 0.4f, 0.3f, 0.2f, 1.0f };
    float clockDiffuse[] = { 0.7f, 0.5f, 0.4f, 1.0f };
    float clockSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    setMaterial(clockAmbient, clockDiffuse, clockSpecular, 20.0f);
    drawClock(); //Часы
    
    //Материал для стен
    float wallAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    float wallDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float wallSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    setMaterial(wallAmbient, wallDiffuse, wallSpecular, 20.0f);
    //Стены
    drawWall(0.0f, 0.0f, -10.0f, 20.0f, 4.0f, 0.0f);
    drawWall(0.0f, 0.0f, 10.0f, 20.0f, 4.0f, 180.0f);
    drawWall(-10.0f, 0.0f, 0.0f, 20.0f, 4.0f, 90.0f);
    drawWall(10.0f, 0.0f, 0.0f, 20.0f, 4.0f, -90.0f);
    
    //Материал для мебели
    float tableAmbient[] = { 0.2f, 0.15f, 0.1f, 1.0f };
    float tableDiffuse[] = { 0.6f, 0.4f, 0.2f, 1.0f };
    float tableSpecular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    setMaterial(tableAmbient, tableDiffuse, tableSpecular, 25.0f);
    drawTable(); //Стол

    //Материал для стула
    float chairAmbient[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    float chairDiffuse[] = { 0.7f, 0.5f, 0.3f, 1.0f };
    float chairSpecular[] = { 0.4f, 0.3f, 0.2f, 1.0f };
    setMaterial(chairAmbient, chairDiffuse, chairSpecular, 20.0f);
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, -1.5f);
    drawChair(); //Стул
    glPopMatrix();

    //Материал для шкафов
    float shcafAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float shcafDiffuse[] = { 0.5f, 0.4f, 0.3f, 1.0f };
    float shcafSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    setMaterial(shcafAmbient, shcafDiffuse, shcafSpecular, 15.0f);
    //Левые шкафы в дальнем углу
    drawShkaf(-8.9f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(-6.8f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(-4.7f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(-2.6f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    //Правые шкафы в дальнем углу
    drawShkaf(2.6f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(4.7f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(6.8f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);
    drawShkaf(8.9f, 0.0f, -9.4f, 2.0f, 3.0f, 1.0f, 0.0f);

    //Материал для ноутбука
    float laptopAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float laptopDiffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
    float laptopSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    setMaterial(laptopAmbient, laptopDiffuse, laptopSpecular, 30.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.11f, 0.0f);
    drawLaptop(); //Ноутбук
    glPopMatrix();

    glutSwapBuffers();

}

//Настройка проекции
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h; //Для масштабирования сцены - соотношение сторон окна
    glViewport(0, 0, w, h); //Устанавливаем область вывода на экран в пикселях
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); //Устанавливаем перспективную проекцию с углом обзора 45, соотношением сторон aspect, границами видимости (0, 100]
    glMatrixMode(GL_MODELVIEW);
}

//Обработка клавиш
void keyboard(unsigned char key, int x, int y) {
    float nextX = cameraX, nextY = cameraY, nextZ = cameraZ;
    bool reset = false;
    switch (key) {
    case 'w': //Вперед
        nextX += cameraSpeed * sin(cameraAngleHorizontal);
        nextZ -= cameraSpeed * cos(cameraAngleHorizontal);
        break;
    case 's': //Назад
        nextX -= cameraSpeed * sin(cameraAngleHorizontal);
        nextZ += cameraSpeed * cos(cameraAngleHorizontal);
        break;
    case 'a': //Влево
        nextX -= cameraSpeed * cos(cameraAngleHorizontal);
        nextZ -= cameraSpeed * sin(cameraAngleHorizontal);
        break;
    case 'd': //Вправо
        nextX += cameraSpeed * cos(cameraAngleHorizontal);
        nextZ += cameraSpeed * sin(cameraAngleHorizontal);
        break;
    case 'q': //Поворот влево
        cameraAngleHorizontal -= lookSpeed;
        break;
    case 'e': //Поворот вправо
        cameraAngleHorizontal += lookSpeed;
        break;
    case 'r': //Сброс камеры
        cameraX = 0.0f, cameraY = 2.0f, cameraZ = 8.0f;
        cameraAngleHorizontal = 0.0f; cameraAngleVertical = 0.0f;
        reset = true;
        break;
    case 27: //Выход
        exit(0);
    }

    //Проверяем, не выходит ли камера за пределы комнаты
    if (nextX >= -9.9f && nextX <= 9.9f && !reset) cameraX = nextX;
    if (nextZ >= -8.6f && nextZ <= 9.9f && !reset) cameraZ = nextZ;
    glutPostRedisplay();
}

//Обработка стрелок
void keyboardArrows(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: //Поднять камеру
        if (cameraY + cameraSpeed <= 4.0f) cameraY += cameraSpeed;
        break;
    case GLUT_KEY_DOWN: //Опустить камеру
        if (cameraY - cameraSpeed >= 0.0f) cameraY -= cameraSpeed;
        break;
    case GLUT_KEY_LEFT: //Взгляд вверх
        if (cameraAngleVertical + lookSpeed <= M_PI_2) cameraAngleVertical += lookSpeed;
        break;
    case GLUT_KEY_RIGHT: //Взгляд вниз
        if (cameraAngleVertical - lookSpeed >= -M_PI_2) cameraAngleVertical -= lookSpeed;
        break;
    }
    glutPostRedisplay();
}

//Главная функция
int main(int argc, char** argv) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Управление камерой:" << endl;
    cout << "w - вперёд" << endl;
    cout << "s - назад" << endl;
    cout << "a - влево" << endl;
    cout << "d - вправо" << endl;
    cout << "q - взгляд влево" << endl;
    cout << "e - взгляд вправо" << endl;
    cout << "Стрелка вверх - поднять камеру" << endl;
    cout << "Стрелка вниз - опустить камеру" << endl;
    cout << "Стрелка влево - взгляд вверх" << endl;
    cout << "Стрелка вправо - взгляд вниз" << endl;
    cout << "r - сброс камеры" << endl;
    cout << "Esc - выход" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Кабинет");

    glEnable(GL_DEPTH_TEST);
    loadTextures();
    initLighting(); //Инициализируем освещение
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardArrows);

    glutMainLoop();
    return 0;
}
