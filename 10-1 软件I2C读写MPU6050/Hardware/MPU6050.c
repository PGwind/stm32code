#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS); // 设备地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress); // 寄存器地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data); // 数据
	MyI2C_ReceiveAck();
	MyI2C_Stop(); 
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress) // 读取
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS); // 设备地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress); // 寄存器地址
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 读指令
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1); // 读取一个字节，不给从机应答
	MyI2C_Stop();
	
	return Data;
}

// 读取多个字节
/*
		uint8_t Data[length]; // 声明一个数组用于存储读取的多个字节数据
		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS); // 设备地址
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress); // 寄存器地址
		MyI2C_ReceiveAck();

		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 读指令
		MyI2C_ReceiveAck();

		for (int i = 0; i < length; i++) {
				Data[i] = MyI2C_ReceiveByte();
				if (i == length - 1) {
						MyI2C_SendAck(1); // 最后一个字节，不给从机应答，即停止读取
				} else {
						MyI2C_SendAck(0); // 给从机应答，继续读取下一个字节
				}
			}

		MyI2C_Stop();
*/


void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 电源管理寄存器1 解除睡眠、选择X轴陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 采用分频10
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06); // 配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪配置寄存器 最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度 最大量程
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
												int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	// 加速度寄存器 XYZ轴
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); // 高8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); // 低8位
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L); 
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); 
	*AccZ = (DataH << 8) | DataL;
	
	// 陀螺仪
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L); 
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L); 
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L); 
	*GyroZ = (DataH << 8) | DataL;
}
