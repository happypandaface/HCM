
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define RADIUS           70.f
#define DIST_BALL       (RADIUS * 2.f + RADIUS * 0.1f)
#define GRID_SIZE       (RADIUS * 4.5f)
#define VIEW_SCENE_DIST (DIST_BALL * 3.f + 200.f)

/* Prototypes */
void init( void );
void display( void );
void reshape( GLFWwindow* window, int w, int h );
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );
void DrawBoingBall( void );
void BounceBall( double dt );
void DrawBoingBallBand( GLfloat long_lo, GLfloat long_hi );
void DrawGrid( void );

/* Vertex type */
typedef struct {float x; float y; float z;} vertex_t;

double  t;
double  t_old = 0.f;
double  dt;

/* PI */
#ifndef M_PI
 #define M_PI 3.1415926535897932384626433832795
#endif


/*****************************************************************************
 * Truncate a degree.
 *****************************************************************************/
GLfloat TruncateDeg( GLfloat deg )
{
   if ( deg >= 360.f )
      return (deg - 360.f);
   else
      return deg;
}

/*****************************************************************************
 * Convert a degree (360-based) into a radian.
 * 360' = 2 * PI
 *****************************************************************************/
double deg2rad( double deg )
{
   return deg / 360 * (2 * M_PI);
}

/*****************************************************************************
 * 360' sin().
 *****************************************************************************/
double sin_deg( double deg )
{
   return sin( deg2rad( deg ) );
}

/*****************************************************************************
 * 360' cos().
 *****************************************************************************/
double cos_deg( double deg )
{
   return cos( deg2rad( deg ) );
}

/*****************************************************************************
 * Compute a cross product (for a normal vector).
 *
 * c = a x b
 *****************************************************************************/
void CrossProduct( vertex_t a, vertex_t b, vertex_t c, vertex_t *n )
{
   GLfloat u1, u2, u3;
   GLfloat v1, v2, v3;

   u1 = b.x - a.x;
   u2 = b.y - a.y;
   u3 = b.y - a.z;

   v1 = c.x - a.x;
   v2 = c.y - a.y;
   v3 = c.z - a.z;

   n->x = u2 * v3 - v2 * v3;
   n->y = u3 * v1 - v3 * u1;
   n->z = u1 * v2 - v1 * u2;
}


GLfloat PerspectiveAngle( GLfloat size,
                          GLfloat dist )
{
   GLfloat radTheta, degTheta;

   radTheta = 2.f * (GLfloat) atan2( size / 2.f, dist );
   degTheta = (180.f * radTheta) / (GLfloat) M_PI;
   return degTheta;
}

void init( void )
{
	/*
	* Clear background.
	*/
	glClearColor( 0.55f, 0.55f, 0.55f, 0.f );

	glShadeModel( GL_FLAT );
}
void display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

	//drawBallHow = DRAW_BALL_SHADOW;
	//DrawBoingBall();

	DrawGrid();

	//drawBallHow = DRAW_BALL;
	//DrawBoingBall();
	

	glPopMatrix();
	glFlush();
}
/*****************************************************************************
 * reshape()
 *****************************************************************************/
void reshape( GLFWwindow* window, int w, int h )
{
   glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluPerspective( PerspectiveAngle( RADIUS * 2, 200 ),
                   (GLfloat)w / (GLfloat)h,
                   1.0,
                   VIEW_SCENE_DIST );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   gluLookAt( 0.0, 0.0, VIEW_SCENE_DIST,/* eye */
              0.0, 0.0, 0.0,            /* center of vision */
              0.0, -1.0, 0.0 );         /* up vector */
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void DrawGrid( void )
{
	int              row, col;
	const int        rowTotal    = 12;                   /* must be divisible by 2 */
	const int        colTotal    = rowTotal;             /* must be same as rowTotal */
	const GLfloat    widthLine   = 2.0;                  /* should be divisible by 2 */
	const GLfloat    sizeCell    = GRID_SIZE / rowTotal;
	const GLfloat    z_offset    = -40.0;
	GLfloat          xl, xr;
	GLfloat          yt, yb;

	glPushMatrix();
	glDisable( GL_CULL_FACE );

	/*
	* Another relative Z translation to separate objects.
	*/
	glTranslatef( 0.0, 0.0, DIST_BALL );

	/*
	* Draw vertical lines (as skinny 3D rectangles).
	*/
	for ( col = 0; col <= colTotal; col++ )
	{
		/*
		* Compute co-ords of line.
		*/
		xl = -GRID_SIZE / 2 + col * sizeCell;
		xr = xl + widthLine;

		yt =  GRID_SIZE / 2;
		yb = -GRID_SIZE / 2 - widthLine;

		glBegin( GL_POLYGON );

		glColor3f( 0.6f, 0.1f, 0.6f );               /* purple */

		glVertex3f( xr, yt, z_offset );       /* NE */
		glVertex3f( xl, yt, z_offset );       /* NW */
		glVertex3f( xl, yb, z_offset );       /* SW */
		glVertex3f( xr, yb, z_offset );       /* SE */

		glEnd();
	}

	/*
	* Draw horizontal lines (as skinny 3D rectangles).
	*/
	for ( row = 0; row <= rowTotal; row++ )
	{
		/*
		* Compute co-ords of line.
		*/
		yt = GRID_SIZE / 2 - row * sizeCell;
		yb = yt - widthLine;

		xl = -GRID_SIZE / 2;
		xr =  GRID_SIZE / 2 + widthLine;

		glBegin( GL_POLYGON );

		glColor3f( 0.6f, 0.1f, 0.6f );               /* purple */

		glVertex3f( xr, yt, z_offset );       /* NE */
		glVertex3f( xl, yt, z_offset );       /* NW */
		glVertex3f( xl, yb, z_offset );       /* SW */
		glVertex3f( xr, yb, z_offset );       /* SE */

		glEnd();
	}

	glPopMatrix();

	return;
}

int main()
{
	GLFWwindow* window;
	int width, height;

	/* Init GLFW */
	if( !glfwInit() )
		exit( EXIT_FAILURE );

	glfwWindowHint(GLFW_DEPTH_BITS, 16);

	window = glfwCreateWindow( 400, 400, "Boing (classic Amiga demo)", NULL, NULL );
	if (!window)
	{
	   glfwTerminate();
	   exit( EXIT_FAILURE );
	}

	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );

	glfwGetFramebufferSize(window, &width, &height);
	reshape(window, width, height);

	glfwSetTime( 0.0 );
   
	init();
	
	for (;;)
	{
       t = glfwGetTime();
       dt = t - t_old;
       t_old = t;

       /* Draw one frame */
       display();

       /* Swap buffers */
       glfwSwapBuffers(window);
       glfwPollEvents();
	   
		if (glfwWindowShouldClose(window))
			break;
	}

	glfwTerminate();
	exit( EXIT_SUCCESS );
}