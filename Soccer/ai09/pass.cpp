#include "ai09.h"

TVec2 ai09::CalculatePassPos ( int robot_num , const TVec2& target , const TVec2& statPos , float bar )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	//Line ball_line ( 1.0 , -ballLine.getSlope() , -ballLine.getIntercept() );
	
	if (chip_head<180) {
		ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , chip_head );
		cout << "	calcing with static head: " << chip_head << endl;
	}
	
	float angleWithTarget = AngleWith(statPos, target);
	//angleWithTarget = OwnRobot[robot_num].State.Angle;
	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( statPos.X + bar * cosDeg ( angleWithTarget ) , statPos.Y + bar * sinDeg ( angleWithTarget ) ) );
	TVec2 fans = Vec2 (ans.getX() - bar * cosDeg ( angleWithTarget ) , ans.getY() - bar * sinDeg ( angleWithTarget ) );
	AddDebugCircle(fans,90,Cornflower_Blue);
	return fans;
}


void ai09::WaitForPass ( int robot_num , bool chip , TVec2* target , TVec2* statPos )
{
	TVec2 pos = CalculatePassPos(robot_num,target==NULL?Vec2(-side*3025, 0):*target,statPos==NULL?OwnRobot[robot_num].State.Position:*statPos,95);
	
	/*if (target==NULL) {
		target = new TVec2(Vec2(-side*3025, 0));
	}*/
	
	if (target==NULL) {
		OwnRobot[robot_num].target.Angle = calculateOneTouchAngle ( robot_num , pos );
	}
	else {
		OwnRobot[robot_num].target.Angle = AngleWith(OwnRobot[robot_num].State.Position, *target);
	}
	
	Line shoot_line = Line::makeLineFromPositionAndAngle ( VecPosition( pos.X , pos.Y ) , OwnRobot[robot_num].target.Angle);
	Line open_line = Line::makeLineFromPositionAndAngle ( VecPosition ( pos.X , pos.Y ) , calculateOpenAngleToGoal (pos,robot_num).X );
	debugDraw=true;
	AddDebugLine(pos,Vec2(-side*3025, shoot_line.getYGivenX(-side*3025)),Brown);
	AddDebugLine(pos,Vec2(-side*3025, open_line.getYGivenX(-side*3025)),Pink);
	debugDraw=false;

	
	//OwnRobot[robot_num].target.Angle=-90;

	ERRTSetObstacles ( robot_num ,0,1,1,0,0,0);
	ERRTNavigate2Point ( robot_num ,pos,0,100,&VELOCITY_PROFILE_MAMOOLI );
	
	if ( target == NULL )
	{
		if (chip) {
			OwnRobot[robot_num].Chip( 60 );
		}
		else
			OwnRobot[robot_num].Shoot( 150 );
	}
	else
	{
		if (chip) {
			OwnRobot[robot_num].Chip( 0 );
		}
		else
			OwnRobot[robot_num].Shoot( 0 );

	}

	//OwnRobot[robot_num].Dribble( 15 );
}