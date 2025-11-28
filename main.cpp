#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/freeglut.h>

// -------------------------------
// Window / world settings
// -------------------------------
const int   WIN_WIDTH = 800;
const int   WIN_HEIGHT = 700;
const float WORLD_MIN = -400.0f;
const float WORLD_MAX = 400.0f;

// -------------------------------
// Helpers / Display lists
// -------------------------------
const float TWO_PI = 6.2831853f;
GLuint circleDL;    // created in init()
GLuint shipDL;
GLuint eggDL;
GLuint explosionRingDL;

// -------------------------------
// Player / game state
// -------------------------------
int   score = 0;
int   lives = 3;
bool  gameOver = false;
bool  gameWin = false;

// explosion at game over
bool  shipExplosionActive = false;
float shipExplosionX = 0.0f;
float shipExplosionY = 0.0f;
float shipExplosionTimer = 0.0f;   // time in seconds
const float shipExplosionMaxTime = 0.6f;  // total duration (0.6 sec)

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

// scale and pulse for simulating the object moving closer or farther from the camera
float ufoScale = 1.0f;
float ufoTargetScale = 1.2f;
bool  ufoApproach = false;

float ufoPulsePhase = 0.0f;         // sinus phase
float ufoPulseAmount = 0.2f;        // pulse amplitude (how strong the UFO scales in/out)
float ufoLifeStep = 0.2f;           // how much the UFO permanently grows when the player loses a life

// -------------------------------
// Utility
// -------------------------------
// draw a circle with radius r and center at(cx, cy)
void drawCircle(float cx, float cy, float r)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(r, r, 1.0f);
    glCallList(circleDL);
    glPopMatrix();
}

// draw ellipse using different radius on x and y
void drawEllipse(float cx, float cy, float rx, float ry)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(rx, ry, 1.0f);
    glCallList(circleDL);
    glPopMatrix();
}

float toRad(float deg) {
    return deg * 3.1415926f / 180.0f;
}

// -------------------------------
// DRAW FUNCTIONS
// -------------------------------
void drawShip()
{
    glCallList(shipDL);
}

void drawChickenShape()
{
    // body
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.0f, 0.0f, 20.0f);

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
    glCallList(eggDL);
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

// t in [0, 1] – explosion progress (0 = start, 1 = end)
void drawExplosionLines(float t)
{
    int numRings = 3;          // number of concentric circles
    float maxRadius = 80.0f;   // how big the explosion gets at the end

    glLineWidth(2.0f);

    for (int ring = 0; ring < numRings; ++ring) {
        // the radius for the current circle increases by t
        float baseR = maxRadius * (ring + 1) / numRings * t;
        if (baseR <= 0.0f)
            continue;

        glPushMatrix();
        glScalef(baseR, baseR, 1.0f);
        glCallList(explosionRingDL);
        glPopMatrix();
    }
}

// UFO object used to show composed transformations
void drawUFO()
{
    // dome
    glColor3f(0.7f, 0.7f, 1.0f);
    drawEllipse(0.0f, 0.0f, 25.0f, 16.0f);


    // base
    glColor3f(0.5f, 0.5f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(-40, -8);
    glVertex2f(40, -8);
    glVertex2f(50, -16);
    glVertex2f(-50, -16);
    glEnd();

    // small lights
    float lightRadius = 4.0f;
    glColor3f(1.0f, 0.2f, 0.2f); 
    drawCircle(-30.0f, -10.0f, lightRadius);
    glColor3f(0.2f, 1.0f, 0.2f); 
    drawCircle(0.0f, -12.0f, lightRadius);
    glColor3f(0.2f, 0.8f, 1.0f); 
    drawCircle(30.0f, -10.0f, lightRadius);
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
        renderBitmapString(-100, -30, GLUT_BITMAP_8_BY_13, "Press R to restart");
    }
    else if (gameWin) {
        glColor3f(0.2f, 1.0f, 0.2f);
        renderBitmapString(-80, 0, GLUT_BITMAP_HELVETICA_18, "YOU WIN");
        renderBitmapString(-100, -30, GLUT_BITMAP_8_BY_13, "Press R to restart");
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

    shipExplosionActive = false;
    shipExplosionX = 0.0f;
    shipExplosionY = 0.0f;
    shipExplosionTimer = 0.0f;

    // UFO
    ufoAngle = 0.0f;
    ufoOrbit = 0.0f;
    ufoScale = 1.0f;
    ufoTargetScale = 1.0f;
    ufoApproach = false;
    ufoPulsePhase = 0.0f;
    ufoPulseAmount = 0.2f;
    ufoLifeStep = 0.2f;

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

void init()
{
    glClearColor(0.0f, 0.0f, 0.08f, 1.0f);
    glPointSize(2.0f);

    resetGame();

    // --- circleDL: display list for a unit circle (radius = 1.0) ---
    // Used for all circular and elliptical shapes (chickens, UFO dome, lights, etc.)
    circleDL = glGenLists(1);
    glNewList(circleDL, GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
    int numSegments = 64;
    for (int i = 0; i < numSegments; ++i)
    {
        float theta = TWO_PI * (float)i / (float)numSegments;
        float x = cosf(theta);
        float y = sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
    glEndList();

    // --- shipDL: display list for ship model ---
    shipDL = glGenLists(1);
    glNewList(shipDL, GL_COMPILE);

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
    glEndList();

    // --- eggDL: display list for egg shape ---
    eggDL = glGenLists(1);
    glNewList(eggDL, GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 25; ++i) {
        float a = i * TWO_PI / 25.0f;
        float x = 7.0f * cosf(a);
        float y = 11.0f * sinf(a);
        glVertex2f(x, y);
    }
    glEnd();
    glEndList();


    // --- explosionRingDL: display list for short line segments arranged on a unit circle ---
    // Used to draw concentric explosion rings by scaling this pattern
    explosionRingDL = glGenLists(1);
    glNewList(explosionRingDL, GL_COMPILE);

    int segmentsPerRing = 24;   // number of positions on the circle
    glBegin(GL_LINES);
    for (int i = 0; i < segmentsPerRing; ++i) {

        // draw only half of the lines (interrupted lines)
        if (i % 2 == 1) continue;

        float a0 = (float)i * TWO_PI / segmentsPerRing;
        float a1 = (float)(i + 0.4f) * TWO_PI / segmentsPerRing; // short line

        float r0 = 1.0f;
        float r1 = 1.0f + 0.1f;

        float x0 = r0 * cosf(a0);
        float y0 = r0 * sinf(a0);
        float x1 = r1 * cosf(a1);
        float y1 = r1 * sinf(a1);

        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
    }
    glEnd();
    glEndList();
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
    if (!gameWin) {             // player wins, the UFO disappears
        glPushMatrix();
        // vertical translation
        glTranslatef(0.0f, 260.0f, 0.0f);
        // orbit movement
        glTranslatef(ufoOrbitRadiusX * cosf(ufoOrbit),
            ufoOrbitRadiusY * sinf(ufoOrbit),
            0.0f);

        //scale
        float pulse = sinf(ufoPulsePhase) * ufoPulseAmount;
        float ufoFinalScale = ufoScale + pulse;
        glScalef(ufoFinalScale, ufoFinalScale, 1.0f);

        // rotation 
        glRotatef(ufoAngle, 0, 0, 1);
        drawUFO();
        glPopMatrix();
    }

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

    // draw ship (only if game is not over)
    if (!gameOver) {
        glPushMatrix();
        glTranslatef(shipX, shipY, 0.0f);
        drawShip();
        glPopMatrix();
    }

    // explosion at game over
    if (shipExplosionActive) {
        // explosion progress between 0 and 1
        float t = 1.0f - shipExplosionTimer / shipExplosionMaxTime;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;

        glPushMatrix();
        glTranslatef(shipExplosionX, shipExplosionY, 0.0f);

        glColor3f(1.0f, 0.6f, 0.1f);
        drawExplosionLines(t);
        glPopMatrix();
    }

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
        if (ufoOrbit > TWO_PI) ufoOrbit -= TWO_PI;

        // animation (lerp)
        if (ufoApproach) {
            float speed = 0.05f;
            ufoScale += (ufoTargetScale - ufoScale) * speed;

            if (fabs(ufoScale - ufoTargetScale) < 0.01f) {
                ufoScale = ufoTargetScale;
                ufoApproach = false;
            }
        }

        ufoPulsePhase += 0.08f;          // pulse speed
        if (ufoPulsePhase > TWO_PI)
            ufoPulsePhase -= TWO_PI;

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

                // UFO becomes larger
                ufoTargetScale += ufoLifeStep;
                ufoApproach = true;

                if (lives <= 0) {
                    gameOver = true;

                    // at game over, the UFO becomes even larger and stops approaching
                    ufoScale = 2.5f;
                    ufoTargetScale = 2.5f;
                    ufoApproach = false;

                    // start explosion
                    shipExplosionActive = true;
                    shipExplosionX = shipX;
                    shipExplosionY = shipY;
                    shipExplosionTimer = 0.6f;   // 0.6 sec
                    shipExplosionTimer = shipExplosionMaxTime;
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

            // deactivate all eggs when the game is won
            for (int i = 0; i < MAX_EGGS; ++i) {
                eggs[i].active = false;
            }
        }
    }

    // update explosion timer so the effect only lasts for a short period after game over
    if (shipExplosionActive) {
        shipExplosionTimer -= 0.016f;   // approx. 60 FPS
        if (shipExplosionTimer <= 0.0f) {
            shipExplosionActive = false;
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

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialDown);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
