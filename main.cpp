#include<windows.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

#define PI 3.1416

struct Point
{
    float x,y;
};

bool srandCalled = false; // Flag to ensure srand is called only once

struct Point centerE_A = {130, 162}; //Enemy A Center Coordinate
struct Point centerE_B = {100, 162}; //Enemy B Center Coordinate
struct Point centerAgentA = {0, 0}; //AgentA Center Coordinate
struct Point centerAgentB = {0, 0}; //AgentB Center Coordinate

void initializeRandom()
{
    if (!srandCalled)
    {
        srand(time(NULL)); // Initialize the random number generator
        srandCalled = true;

        //randomly generating AgentA Center Coordinates
        centerAgentA.x = 23 + (rand() % 154);
        centerAgentA.y = 8 + (rand() % 154);

        //randomly generating AgentB Center Coordinates
        centerAgentB.x = 23 + (rand() % 154);
        centerAgentB.y = 8 + (rand() % 154);
    }
}

float radiusE = 3; //Enemy Radius
float radiusAgent = 4; //Agent Radius
float velocity = 0.07;

float rightBorder = 180;
float leftBorder = 20;
float topBorder = 165;
float bottomBorder = 5;

bool isAgentADestroyed = false;
bool isAgentBDestroyed = false;
bool isGameFinished = false;
bool stage1complete = true;
bool stage2complete = false;
bool stage3complete = false;
bool stage4complete = false;

int score = 0; //Game score
int enemyAState = 0; //Enemy A states 0,1,2,3
int enemyBState = 0; //Enemy B states 0,1,2,3


void drawCircleWithPolygon(float cx, float cy, float radius, int slices)
{
    int i;
    float angle, x, y;
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = ((2 * PI)/slices) * i;
            x = cx + radius * cos(angle);
            y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    //Border
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2d(leftBorder,topBorder);
        glVertex2d(rightBorder,topBorder);
        glVertex2d(rightBorder,bottomBorder);
        glVertex2d(leftBorder,bottomBorder);
    }
    glEnd();

    //EnemyA
    glColor3f(1,0,0);
    glPushMatrix();
    {
        drawCircleWithPolygon(centerE_A.x, centerE_A.y, radiusE, 40);
    }
    glPopMatrix();

    //EnemyB
    glColor3f(1,0,0);
    glPushMatrix();
    {
        drawCircleWithPolygon(centerE_B.x, centerE_B.y, radiusE, 40);
    }
    glPopMatrix();

    //AgentA
    if(!isAgentADestroyed)
    {
        glColor3f(0,1,0);//green color
        glPushMatrix();
        {
            drawCircleWithPolygon(centerAgentA.x, centerAgentA.y, radiusAgent, 40);
        }
        glPopMatrix();
    }

    //AgentB
    if(!isAgentBDestroyed)
    {
        glColor3f(0,0,1);//blue color
        glPushMatrix();
        {
            drawCircleWithPolygon(centerAgentB.x, centerAgentB.y, radiusAgent, 40);
        }
        glPopMatrix();
    }

    glFlush();
}

void keyboard_action(unsigned char key, int x, int y)
{
    if(key == 'a' && centerAgentB.x > (leftBorder+radiusAgent))
    {
        centerAgentB.x -= 2;
    }
    else if(key == 'd' && centerAgentB.x < (rightBorder-radiusAgent))
    {
        centerAgentB.x += 2;
    }
    else if(key == 'w' && centerAgentB.y < (topBorder-radiusAgent))
    {
        centerAgentB.y += 2;
    }
    else if(key == 's' && centerAgentB.y > (bottomBorder+radiusAgent))
    {
        centerAgentB.y -= 2;
    }
    glutPostRedisplay();
}

void special_action(int key, int x, int y)
{
    if(!isGameFinished)
    {
        if(key == GLUT_KEY_LEFT && centerAgentA.x > (leftBorder+radiusAgent))
        {
            centerAgentA.x -= 2;
        }
        else if(key == GLUT_KEY_RIGHT && centerAgentA.x < (rightBorder-radiusAgent))
        {
            centerAgentA.x += 2;
        }
        else if(key == GLUT_KEY_UP && centerAgentA.y < (topBorder-radiusAgent))
        {
            centerAgentA.y += 2;
        }
        else if(key == GLUT_KEY_DOWN && centerAgentA.y > (bottomBorder+radiusAgent))
        {
            centerAgentA.y -= 2;
        }
    }

    glutPostRedisplay();
}

void animate()
{
    if(!isGameFinished)
    {
        score++;
        double factorA;
        double factorB;

        if(score > 5000 && stage1complete)
        {
            velocity = velocity*1.20;//velocity increases by a factor of 1.20 if score > 5000
            stage1complete = false;
            stage2complete = true;
        }
        else if(score > 10000 && stage2complete)
        {
            velocity = velocity*1.25;//velocity increases by a factor of 1.25 if score > 5000
            stage2complete = false;
            stage3complete = true;
        }
        else if(score > 15000 && stage3complete)
        {
            velocity = velocity*1.35;//velocity increases by a factor of 1.35 if score > 5000
            stage3complete = false;
            stage4complete = true;
        }
        else if(score > 20000 && stage4complete)
        {
            velocity = velocity*1.50;//velocity increases by a factor of 1.50 if score > 5000
            stage4complete = false;
        }

        //Enemy A path logic
        if(enemyAState == 0)
        {
            factorA = 1;
            centerE_A.x += velocity;
            centerE_A.y -= factorA*velocity;
            if(centerE_A.x > 177 || centerE_A.y < 112)
            {
                enemyAState = 1;
            }
        }
        else if(enemyAState == 1)
        {
            factorA = 0.305;
            centerE_A.x -= velocity;
            centerE_A.y += factorA*velocity;
            if(centerE_A.x < 23 || centerE_A.y > 162)
            {
                enemyAState = 2;
            }
        }
        else if(enemyAState == 2)
        {
            factorA = 1;
            centerE_A.x += velocity;
            centerE_A.y -= factorA*velocity;
            if(centerE_A.x > 177 || centerE_A.y < 8)
            {
                enemyAState = 3;
            }
        }
        else if(enemyAState == 3)
        {
            factorA = 3.35;
            centerE_A.x -= velocity;
            centerE_A.y += factorA*velocity;
            if(centerE_A.x < 130 || centerE_A.y > 162)
            {
                enemyAState = 0;
                centerE_A = {130, 162};
            }
        }

        //Enemy B path logic
        if(enemyBState == 0)
        {
            factorB = 0;
            centerE_B.x += factorB*velocity;
            centerE_B.y -= velocity;
            if(centerE_B.y < 8)
            {
                enemyBState = 1;
            }
        }
        else if(enemyBState == 1)
        {
            factorB = 1;
            centerE_B.x += velocity;
            centerE_B.y += factorB*velocity;
            if(centerE_B.x > 177 || centerE_B.y > 85)
            {
                enemyBState = 2;
            }
        }
        else if(enemyBState == 2)
        {
            factorB = 0;
            centerE_B.x -= velocity;
            centerE_B.y -= factorB*velocity;
            if(centerE_B.x < 23)
            {
                enemyBState = 3;
            }
        }
        else if(enemyBState == 3)
        {
            factorB = 1;
            centerE_B.x += velocity;
            centerE_B.y += factorB*velocity;
            if(centerE_B.x > 100 || centerE_B.y > 162)
            {
                enemyBState = 0;
            }
        }

        if(!isAgentADestroyed)
        {
            //distances between enemies and agentA
            double enemyA_agentA = sqrt(pow((centerE_A.x-centerAgentA.x),2) + pow((centerE_A.y-centerAgentA.y),2));
            double enemyB_agentA = sqrt(pow((centerE_B.x-centerAgentA.x),2) + pow((centerE_B.y-centerAgentA.y),2));

            //check for collision between enemies and agentA
            if(enemyA_agentA <= (radiusE+radiusAgent))
            {
                isAgentADestroyed = true;
            }
            if(enemyB_agentA <= (radiusE+radiusAgent))
            {
                isAgentADestroyed = true;
            }
        }

        if(!isAgentBDestroyed)
        {
            //distances between enemies and agentB
            double enemyA_agentB = sqrt(pow((centerE_A.x-centerAgentB.x),2) + pow((centerE_A.y-centerAgentB.y),2));
            double enemyB_agentB = sqrt(pow((centerE_B.x-centerAgentB.x),2) + pow((centerE_B.y-centerAgentB.y),2));

            //check for collision between enemies and agentA
            if(enemyA_agentB <= (radiusE+radiusAgent))
            {
                isAgentBDestroyed = true;
            }
            if(enemyB_agentB <= (radiusE+radiusAgent))
            {
                isAgentBDestroyed = true;
            }
        }

        //Game finish logic
        if(isAgentADestroyed && isAgentBDestroyed)
        {
            isGameFinished = true;
        }
    }
    else
    {
        printf("Score : %d\n", score);
        exit(0);
    }

    glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glOrtho(-2.0, 200.0, -2.0, 200.0, -25.0, 25.0);
}

int main()
{
    initializeRandom(); // Call this function to initialize random numbers
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Dodge Game");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard_action);
    glutSpecialFunc(special_action);
    glutMainLoop();
    return 0;
}
