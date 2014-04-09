#include "ai09.h"
#include <math.h>

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

static int my_hys=0;
void ai09::GKHi ( int robot_num , int defence_num , bool stop )
{
	gkIntercepting = false;

	//side = -side;
	if ((ballIsGoaling())&&(DIS(ball.Position, OwnRobot[robot_num].State.Position)/ball.velocity.magnitude<1)&&(!stop))
	{
		//WaitForPass ( robot_num , true );
		GK_shirje(robot_num);
		my_hys = 10;
	}
	else if (( my_hys > 0 )&&(!stop))
	{
		//WaitForPass ( robot_num , true );
		GK_shirje(robot_num);
		my_hys --;
	}
	
	else
	{
		//OwnRobot[robot_num].Chip(50);
		
		my_hys = 0;
	
		ERRTSetObstacles ( robot_num , false , true , false , false , false );
		if ( ( IsInObstacle ( Vec2 ( world2mapX(ball.Position.X),world2mapY(ball.Position.Y) ) ) ) && ( ball.velocity.magnitude < 1000 ) && (!stop) && (side*ball.Position.X<3025) && (timer.time()>1.0f) && (fabs(ball.Position.Y)<1200.0f) )
		{
			gkIntercepting = true;

			ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 0 , 0 );
			//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
			tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * 3133 , 0 ) ) ,0,500,false,0,0,0);
		}
		else
		{
			TVec2 target = GK_Ghuz();
			
			OwnRobot[robot_num].face(ball.Position);
			ERRTSetObstacles(robot_num, stop , false, false, false, false);
			ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
			
		}
	}
	//side = -side;
}

void ai09::GK_shirje ( int robot_num )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) );
	OwnRobot[robot_num].face(ball.Position);
	TVec2 fans = Vec2(ans.getX(), ans.getY());
	fans = ((fans-OwnRobot[robot_num].State.Position)*2.0f)+OwnRobot[robot_num].State.Position;
	ERRTSetObstacles ( robot_num ,0,0,0,0,0,0);
	ERRTNavigate2Point(robot_num,fans , 1, 100, &VELOCITY_PROFILE_KHARAKI);
	OwnRobot[robot_num].Chip(500);
}