//#include "QuadMesh.h"

typedef struct Bot
{
	float x;
	float y;
	float z;
	float botAngle;
	float aYaw, aPitch;//arm yaw and pitch
	float hPitch;//hammer pitch
	float wheelAngle;
	float speed;
} Bot;

Bot newBot();
void drawPlayerBot(Bot *b);
void drawAIBot(Bot *b);

void moveBotOnMesh(Bot *b);
double getBotY(Bot *b, QuadMesh *groundMesh, int meshSize);