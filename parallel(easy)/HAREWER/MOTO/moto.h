#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"

void moto(int mode);
int Velocity_A(int TargetVelocity, int CurrentVelocity);
int Velocity_B(int TargetVelocity, int CurrentVelocity);
int Velocity_C(int TargetVelocity, int CurrentVelocity);
int Velocity_D(int TargetVelocity, int CurrentVelocity);
void motor_A(int mode);
void motor_B(int mode);
void motor_C(int mode);
void motor_D(int mode);
#endif
