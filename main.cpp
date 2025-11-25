//#include <windows.h>
//#include <math.h>
//#include <GL/freeglut.h>
//
//float chickenAngle = 0.0f;
//float chickenX = 0;
//
//float shipX = 0;
//float followSpeed = 0.04f;
//
//float eggFall = 0.0f;
//float eggX = 0;
//
//float bulletY = -200;
//float bulletX = 0;
//bool bulletActive = false;
//
//float eggFallSpeed = 4.0f;
//float bulletSpeed = 8.0f;
//
//float avoidPower = 2.0f;   // how strong the ship avoids the egg
//
//// UFO animation
//float ufoAngle = 0.0f;     // rotation of UFO
//float ufoOrbit = 0.0f;     // orbit movement of UFO
//
///* ---------------------------------- */
///* DRAW FUNCTIONS                     */
///* ---------------------------------- */
//void drawShip() {
//    glColor3f(0.2f, 0.6f, 1.0f);
//    glBegin(GL_TRIANGLES);
//    glVertex2f(0, 30);
//    glVertex2f(-20, -20);
//    glVertex2f(20, -20);
//    glEnd();
//
//    glColor3f(1.0f, 0.2f, 0.2f);
//    glBegin(GL_QUADS);
//    glVertex2f(-10, -20);
//    glVertex2f(10, -20);
//    glVertex2f(10, -40);
//    glVertex2f(-10, -40);
//    glEnd();
//}
//
//void drawChicken() {
//    glColor3f(1.0f, 1.0f, 0.0f);
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(0, 0);
//    for (int i = 0; i <= 20; i++) {
//        float a = i * 2.0f * 3.1416f / 20.0f;
//        glVertex2f(25.0f * cosf(a), 25.0f * sinf(a));
//    }
//    glEnd();
//
//    glColor3f(1.0f, 0.3f, 0.3f);
//    glBegin(GL_TRIANGLES);
//    glVertex2f(0, 25);
//    glVertex2f(-10, 40);
//    glVertex2f(10, 40);
//    glEnd();
//}
//
//void drawEgg() {
//    glColor3f(1, 1, 1);
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(0, 0);
//    for (int i = 0; i <= 20; i++) {
//        float a = i * 2.0f * 3.1416f / 20.0f;
//        glVertex2f(8.0f * cosf(a), 12.0f * sinf(a));
//    }
//    glEnd();
//}
//
//void drawBullet() {
//    glColor3f(1, 0, 0);
//    glBegin(GL_QUADS);
//    glVertex2f(-4, -4);
//    glVertex2f(4, -4);
//    glVertex2f(4, 4);
//    glVertex2f(-4, 4);
//    glEnd();
//}
//
//void drawStar() {
//    glColor3f(1, 1, 1);
//    glBegin(GL_POINTS);
//    glVertex2f(0, 0);
//    glEnd();
//}
//
//// UFO shape
//void drawUFO() {
//    // upper dome
//    glColor3f(0.7f, 0.7f, 1.0f);
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(0, 0);
//    for (int i = 0; i <= 40; i++) {
//        float a = i * 2.0f * 3.1416f / 40.0f;
//        glVertex2f(25.0f * cosf(a), 12.0f * sinf(a));
//    }
//    glEnd();
//
//    // lower platform
//    glColor3f(0.5f, 0.5f, 0.9f);
//    glBegin(GL_QUADS);
//    glVertex2f(-40, -8);
//    glVertex2f(40, -8);
//    glVertex2f(50, -16);
//    glVertex2f(-50, -16);
//    glEnd();
//}
//
///* ---------------------------------- */
///* COLLISION CHECK                    */
///* ---------------------------------- */
//bool bulletHitsChicken() {
//    return fabs(bulletX - chickenX) < 25 && fabs(bulletY - 150) < 25;
//}
//
//bool eggWillHitShipSoon() {
//    float eggY = 150 - eggFall;
//
//    // check only when egg is near ship vertically
//    if (eggY < -140 && eggY > -260) {
//        if (fabs(eggX - shipX) < 40) {
//            return true;
//        }
//    }
//    return false;
//}
//
///* ---------------------------------- */
///* DISPLAY                            */
///* ---------------------------------- */
//void displayFcn(void) {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // stars background
//    glPointSize(3);
//    for (int i = 0; i < 20; i++) {
//        glPushMatrix();
//        glTranslatef(-300 + rand() % 600, -300 + rand() % 600, 0);
//        drawStar();
//        glPopMatrix();
//    }
//
//    // UFO with composed transformations (translations + rotation)
//    glPushMatrix();
//
//    // translation 1: base height of UFO above scene
//    glTranslatef(0, 200, 0);
//
//    // translation 2: orbit movement (composition of translations)
//    glTranslatef(120.0f * cosf(ufoOrbit), 40.0f * sinf(ufoOrbit), 0);
//
//    // rotation of UFO around its own center
//    glRotatef(ufoAngle, 0, 0, 1);
//
//    drawUFO();
//
//    glPopMatrix();
//
//    // chicken
//    glPushMatrix();
//    glTranslatef(chickenX, 150, 0);
//    drawChicken();
//    glPopMatrix();
//
//    // falling egg
//    glPushMatrix();
//    glTranslatef(eggX, 150 - eggFall, 0);
//    drawEgg();
//    glPopMatrix();
//
//    // bullet
//    if (bulletActive) {
//        glPushMatrix();
//        glTranslatef(bulletX, bulletY, 0);
//        drawBullet();
//        glPopMatrix();
//    }
//
//    // player ship
//    glPushMatrix();
//    glTranslatef(shipX, -200, 0);
//    drawShip();
//    glPopMatrix();
//
//    glFlush();
//}
//
///* ---------------------------------- */
///* ANIMATION                          */
///* ---------------------------------- */
//void timerFcn(int value)
//{
//    // chicken moves left-right (sinus)
//    chickenAngle += 0.03f;
//    chickenX = 150.0f * sinf(chickenAngle);
//
//    // egg follows chicken horizontally and falls down
//    eggX = chickenX;
//    eggFall += eggFallSpeed;
//    if (eggFall > 350) eggFall = 0;
//
//    // ship follows chicken if egg is not dangerous
//    if (!eggWillHitShipSoon()) {
//        shipX += (chickenX - shipX) * followSpeed;
//    }
//    else {
//        // avoidance movement
//        if (shipX < eggX)
//            shipX -= avoidPower;
//        else
//            shipX += avoidPower;
//    }
//
//    // automatic bullet firing
//    if (!bulletActive) {
//        bulletActive = true;
//        bulletX = shipX;
//        bulletY = -160;
//    }
//
//    if (bulletActive) {
//        bulletY += bulletSpeed;
//
//        if (bulletHitsChicken()) {
//            bulletActive = false;
//        }
//
//        if (bulletY > 400)
//            bulletActive = false;
//    }
//
//    // UFO animation: spin + orbit
//    ufoAngle += 2.0f;
//    if (ufoAngle > 360.0f) ufoAngle -= 360.0f;
//
//    ufoOrbit += 0.01f;
//    if (ufoOrbit > 6.2832f) ufoOrbit -= 6.2832f;
//
//    glutPostRedisplay();
//    glutTimerFunc(16, timerFcn, 1);
//}
//
///* ---------------------------------- */
//void reshapeFcn(int w, int h)
//{
//    glViewport(0, 0, w, h);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(-350, 350, -350, 350);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
///* ---------------------------------- */
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitWindowSize(700, 700);
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//    glutCreateWindow("Chicken Invaders - Transformations and UFO");
//
//    glClearColor(0, 0, 0.1f, 1);
//
//    glutDisplayFunc(displayFcn);
//    glutReshapeFunc(reshapeFcn);
//    glutTimerFunc(16, timerFcn, 1);
//
//    glutMainLoop();
//    return 0;
//}
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/freeglut.h>

// -------------------------------
// Window / world settings
// -------------------------------
const int   WIN_WIDTH = 800;
const int   WIN_HEIGHT = 800;
const float WORLD_MIN = -400.0f;
const float WORLD_MAX = 400.0f;

// -------------------------------
// Player / game state
// -------------------------------
int   score = 0;
int   lives = 3;
bool  gameOver = false;
bool  gameWin = false;

// -------------------------------
// Ship (player)
// -------------------------------
float shipX = 0.0f;
float shipY = -300.0f;
float shipSpeed = 6.0f;

// -------------------------------
// Chickens
// -------------------------------
const int NUM_CHICKENS = 8;

struct Chicken {
    float baseX;
    float baseY;
    bool  alive;
} chickens[NUM_CHICKENS];

float chickenWaveAngle = 0.0f;      // for sinusoidal movement
float chickenWaveSpeed = 0.015f;
float chickenWaveAmp = 80.0f;     // amplitude

// -------------------------------
// Bullets (from player)
// -------------------------------
const int MAX_BULLETS = 10;

struct Bullet {
    float x, y;
    bool  active;
} bullets[MAX_BULLETS];

float bulletSpeed = 10.0f;

// -------------------------------
// Eggs (from chickens)
// -------------------------------
const int MAX_EGGS = 5;

struct Egg {
    float x, y;
    bool  active;
} eggs[MAX_EGGS];

float eggSpeed = 4.5f;
int   eggCooldown = 0;              // simple timer for egg spawns

// -------------------------------
// UFO (object with composed transforms)
// -------------------------------
float ufoAngle = 0.0f;              // spin
float ufoOrbit = 0.0f;              // orbit angle
float ufoOrbitRadiusX = 200.0f;
float ufoOrbitRadiusY = 60.0f;

// -------------------------------
// Utility
// -------------------------------
float toRad(float deg) {
    return deg * 3.1415926f / 180.0f;
}

// -------------------------------
// DRAW FUNCTIONS
// -------------------------------
void drawShip()
{
    // body
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 40);
    glVertex2f(-25, -25);
    glVertex2f(25, -25);
    glEnd();

    // engine
    glColor3f(1.0f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-12, -25);
    glVertex2f(12, -25);
    glVertex2f(12, -45);
    glVertex2f(-12, -45);
    glEnd();
}

void drawChickenShape()
{
    // body
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 25; ++i) {
        float a = i * 2.0f * 3.1415926f / 25.0f;
        glVertex2f(20.0f * cosf(a), 20.0f * sinf(a));
    }
    glEnd();

    // comb
    glColor3f(1.0f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 20);
    glVertex2f(-8, 30);
    glVertex2f(8, 30);
    glEnd();
}

void drawEggShape()
{
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 25; ++i) {
        float a = i * 2.0f * 3.1415926f / 25.0f;
        float x = 7.0f * cosf(a);
        float y = 11.0f * sinf(a);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawBulletShape()
{
    glColor3f(1, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-3, -6);
    glVertex2f(3, -6);
    glVertex2f(3, 6);
    glVertex2f(-3, 6);
    glEnd();
}

void drawStar()
{
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
}

void drawExplosion()
{
    glColor3f(1.0f, 0.7f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 20; ++i) {
        float a = i * 2.0f * 3.1415926f / 20.0f;
        float r = 25.0f + 10.0f * sinf(5.0f * a);
        glVertex2f(r * cosf(a), r * sinf(a));
    }
    glEnd();
}

// UFO object used to show composed transformations
void drawUFO()
{
    // dome
    glColor3f(0.7f, 0.7f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 40; ++i) {
        float a = i * 2.0f * 3.1415926f / 40.0f;
        glVertex2f(25.0f * cosf(a), 12.0f * sinf(a));
    }
    glEnd();

    // base
    glColor3f(0.5f, 0.5f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(-40, -8);
    glVertex2f(40, -8);
    glVertex2f(50, -16);
    glVertex2f(-50, -16);
    glEnd();

    // small lights
    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.2f, 0.2f); glVertex2f(-30, -10);
    glColor3f(0.2f, 1.0f, 0.2f); glVertex2f(0, -12);
    glColor3f(0.2f, 0.8f, 1.0f); glVertex2f(30, -10);
    glEnd();
}

// -------------------------------
// HUD / TEXT
// -------------------------------
void renderBitmapString(float x, float y, void* font, const char* string)
{
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string);
        ++string;
    }
}

void drawHUD()
{
    char buffer[64];

    glColor3f(1, 1, 1);
    sprintf_s(buffer, "Score: %d", score);
    renderBitmapString(WORLD_MIN + 20, WORLD_MAX - 40, GLUT_BITMAP_8_BY_13, buffer);

    sprintf_s(buffer, "Lives: %d", lives);
    renderBitmapString(WORLD_MIN + 20, WORLD_MAX - 60, GLUT_BITMAP_8_BY_13, buffer);

    if (gameOver) {
        glColor3f(1, 0.2f, 0.2f);
        renderBitmapString(-80, 0, GLUT_BITMAP_HELVETICA_18, "GAME OVER");
        renderBitmapString(-160, -30, GLUT_BITMAP_8_BY_13, "Press R to restart");
    }
    else if (gameWin) {
        glColor3f(0.2f, 1.0f, 0.2f);
        renderBitmapString(-80, 0, GLUT_BITMAP_HELVETICA_18, "YOU WIN");
        renderBitmapString(-160, -30, GLUT_BITMAP_8_BY_13, "Press R to restart");
    }
}

// -------------------------------
// COLLISIONS
// -------------------------------
bool checkCollisionCircle(float x1, float y1, float r1,
    float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dist2 = dx * dx + dy * dy;
    float r = r1 + r2;
    return dist2 <= r * r;
}

// -------------------------------
// INITIALIZATION
// -------------------------------
void resetGame()
{
    score = 0;
    lives = 3;
    gameOver = false;
    gameWin = false;

    shipX = 0.0f;
    shipY = -300.0f;

    // chickens in 2 rows
    for (int i = 0; i < NUM_CHICKENS; ++i) {
        chickens[i].alive = true;
        int row = i / 4;    // 0 or 1
        int col = i % 4;
        chickens[i].baseX = -180.0f + col * 120.0f;
        chickens[i].baseY = 200.0f + row * 80.0f;
    }

    // bullets
    for (int i = 0; i < MAX_BULLETS; ++i)
        bullets[i].active = false;

    // eggs
    for (int i = 0; i < MAX_EGGS; ++i)
        eggs[i].active = false;

    eggCooldown = 0;
}

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.08f, 1.0f);
    glPointSize(2.0f);

    resetGame();
}

// -------------------------------
// DISPLAY
// -------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // background stars (randomized each frame for a simple effect)
    glColor3f(1, 1, 1);
    for (int i = 0; i < 80; ++i) {
        glPushMatrix();
        float sx = WORLD_MIN + (rand() % (int)(WORLD_MAX - WORLD_MIN));
        float sy = WORLD_MIN + (rand() % (int)(WORLD_MAX - WORLD_MIN));
        glTranslatef(sx, sy, 0);
        drawStar();
        glPopMatrix();
    }

    // UFO with composed transformations:
    // T1 (height) * T2 (orbit) * R (spin) * model
    glPushMatrix();
    // base vertical position
    glTranslatef(0.0f, 260.0f, 0.0f);
    // orbit movement
    glTranslatef(ufoOrbitRadiusX * cosf(ufoOrbit),
        ufoOrbitRadiusY * sinf(ufoOrbit),
        0.0f);
    // rotation around its own center
    glRotatef(ufoAngle, 0, 0, 1);
    drawUFO();
    glPopMatrix();

    // draw chickens (with small local rotation for animation)
    for (int i = 0; i < NUM_CHICKENS; ++i) {
        if (!chickens[i].alive) continue;

        glPushMatrix();
        float waveX = chickenWaveAmp * sinf(chickenWaveAngle);
        glTranslatef(chickens[i].baseX + waveX,
            chickens[i].baseY,
            0.0f);
        glRotatef(5.0f * sinf(chickenWaveAngle * 3.0f + i), 0, 0, 1);
        drawChickenShape();
        glPopMatrix();
    }

    // draw eggs
    for (int i = 0; i < MAX_EGGS; ++i) {
        if (!eggs[i].active) continue;

        glPushMatrix();
        glTranslatef(eggs[i].x, eggs[i].y, 0.0f);
        drawEggShape();
        glPopMatrix();
    }

    // draw bullets
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].active) continue;

        glPushMatrix();
        glTranslatef(bullets[i].x, bullets[i].y, 0.0f);
        drawBulletShape();
        glPopMatrix();
    }

    // draw ship
    glPushMatrix();
    glTranslatef(shipX, shipY, 0.0f);
    drawShip();
    glPopMatrix();

    // HUD
    drawHUD();

    glFlush();
}

// -------------------------------
// UPDATE (TIMER)
// -------------------------------
void update(int value)
{
    if (!gameOver && !gameWin) {

        // chicken global wave
        chickenWaveAngle += chickenWaveSpeed;

        // UFO animation
        ufoAngle += 1.5f;
        if (ufoAngle > 360.0f) ufoAngle -= 360.0f;

        ufoOrbit += 0.01f;
        if (ufoOrbit > 6.2832f) ufoOrbit -= 6.2832f;

        // bullets movement and collisions
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (!bullets[i].active) continue;

            bullets[i].y += bulletSpeed;
            if (bullets[i].y > WORLD_MAX + 20) {
                bullets[i].active = false;
                continue;
            }

            // check collision with chickens
            for (int j = 0; j < NUM_CHICKENS; ++j) {
                if (!chickens[j].alive) continue;

                float waveX = chickenWaveAmp * sinf(chickenWaveAngle);
                float cx = chickens[j].baseX + waveX;
                float cy = chickens[j].baseY;
                if (checkCollisionCircle(bullets[i].x, bullets[i].y, 8.0f,
                    cx, cy, 22.0f)) {
                    bullets[i].active = false;
                    chickens[j].alive = false;
                    score += 10;
                    break;
                }
            }
        }

        // eggs movement and collisions
        for (int i = 0; i < MAX_EGGS; ++i) {
            if (!eggs[i].active) continue;

            eggs[i].y -= eggSpeed;
            if (eggs[i].y < WORLD_MIN - 30) {
                eggs[i].active = false;
                continue;
            }

            // collision with ship
            if (checkCollisionCircle(eggs[i].x, eggs[i].y, 10.0f,
                shipX, shipY, 25.0f)) {
                eggs[i].active = false;
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                }
            }
        }

        // random egg spawn from random alive chicken
        if (eggCooldown > 0) eggCooldown--;
        if (eggCooldown == 0) {
            // find a free egg slot
            int slot = -1;
            for (int i = 0; i < MAX_EGGS; ++i) {
                if (!eggs[i].active) {
                    slot = i;
                    break;
                }
            }
            if (slot != -1) {
                // choose random chicken
                int tries = 0;
                while (tries < 10) {
                    int c = rand() % NUM_CHICKENS;
                    if (chickens[c].alive) {
                        float waveX = chickenWaveAmp * sinf(chickenWaveAngle);
                        eggs[slot].x = chickens[c].baseX + waveX;
                        eggs[slot].y = chickens[c].baseY - 20.0f;
                        eggs[slot].active = true;
                        break;
                    }
                    tries++;
                }
                eggCooldown = 50 + rand() % 60; // delay until next egg
            }
        }

        // check win condition
        bool anyAlive = false;
        for (int i = 0; i < NUM_CHICKENS; ++i) {
            if (chickens[i].alive) {
                anyAlive = true;
                break;
            }
        }
        if (!anyAlive) {
            gameWin = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// -------------------------------
// INPUT
// -------------------------------
bool keyStates[256] = { false };

void keyDown(unsigned char key, int x, int y)
{
    keyStates[key] = true;

    if (key == 27) { // ESC
        exit(0);
    }

    if (key == 'r' || key == 'R') {
        resetGame();
    }

    if (key == ' ') {
        // fire bullet
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (!bullets[i].active) {
                bullets[i].active = true;
                bullets[i].x = shipX;
                bullets[i].y = shipY + 30.0f;
                break;
            }
        }
    }
}

void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

void specialDown(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) {
        shipX -= shipSpeed;
    }
    if (key == GLUT_KEY_RIGHT) {
        shipX += shipSpeed;
    }

    // clamp ship inside world
    if (shipX < WORLD_MIN + 40) shipX = WORLD_MIN + 40;
    if (shipX > WORLD_MAX - 40) shipX = WORLD_MAX - 40;
}

// -------------------------------
// RESHAPE
// -------------------------------
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(WORLD_MIN, WORLD_MAX, WORLD_MIN, WORLD_MAX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// -------------------------------
// MAIN
// -------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Chicken Invaders 2D");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialDown);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
