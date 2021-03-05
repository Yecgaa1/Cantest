//
// Created by 星星 on 2021/2/26.
//

#ifndef SEND_CANSENT_H
#define SEND_CANSENT_H
#include "stm32f1xx_hal_can.h"
CAN_TxHeaderTypeDef Can_TxHeader;
static void CAN_Filter_Config(void)
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
/*CAN 筛选器初始化 */
    CAN_FilterInitStructure.FilterBank=0;
//筛选器组 0
    CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK; //工作在掩码模式
    CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT;//筛选器位宽为单个 32 位。
/*
使能筛选器，按照标志的内容进行比对筛选，扩展 ID 不是如下的就抛弃掉，是的话，会存
入 FIFO0。 */
    CAN_FilterInitStructure.FilterIdHigh= ((((uint32_t)0x1314<<3)|
                                            CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;
//要筛选的 ID 高位
    CAN_FilterInitStructure.FilterIdLow= (((uint32_t)0x1314<<3)|
                                          CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //要筛选的 ID 低位
    CAN_FilterInitStructure.FilterMaskIdHigh= 0xFFFF;//筛选器高 16 位每位必须匹配
    CAN_FilterInitStructure.FilterMaskIdLow= 0xFFFF;//筛选器低 16 位每位必须匹配
    CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0;//筛选器被关联到 FIFO0
    CAN_FilterInitStructure.FilterActivation=ENABLE;
//使能筛选器
    HAL_CAN_ConfigFilter(&hcan,&CAN_FilterInitStructure);
}

static void CANFilterConfig_Scale16_IdMask(void)
{

    CAN_FilterTypeDef  sFilterConfig;
    uint16_t StdIdArray1[10] ={0x7D1,0x7D2,0x7D3,0x7D4,0x7D5,	//定义第一组标准CAN ID
                               0x7D6,0x7D7,0x7D8,0x7D9,0x7DA};
    uint16_t StdIdArray2[10] ={0x751,0x752,0x753,0x754,0x755,	//定义第二组标准CAN ID
                               0x756,0x757,0x758,0x759,0x75A};
    uint16_t      mask,tmp,i,num;

    sFilterConfig.FilterBank = 0;					//使用过滤器5
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;			//配置为掩码模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;		//设为16位宽

    //配置第一个过滤对
    sFilterConfig.FilterIdLow =StdIdArray1[0]<<5;			//设置第一个验证码
    mask =0x7ff;
    num =sizeof(StdIdArray1)/sizeof(StdIdArray1[0]);
    for(i =0; i<num; i++)							//计算第一个屏蔽码
    {
        tmp =StdIdArray1[i] ^ (~StdIdArray1[0]);
        mask &=tmp;
    }
    sFilterConfig.FilterMaskIdLow =(mask<<5)|0x10;    //只接收数据帧

    //配置第二个过滤对
    sFilterConfig.FilterIdHigh = StdIdArray2[0]<<5;	//设置第二个验证码
    mask =0x7ff;
    num =sizeof(StdIdArray2)/sizeof(StdIdArray2[0]);
    for(i =0; i<num; i++)					//计算第二个屏蔽码
    {
        tmp =StdIdArray2[i] ^ (~StdIdArray2[0]);
        mask &=tmp;
    }
    sFilterConfig.FilterMaskIdHigh = (mask<<5)|0x10;  //只接收数据帧


    sFilterConfig.FilterFIFOAssignment = 0;		//通过的CAN 消息放入到FIFO0中
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
void CAN_SetMsg(void)
{

    uint8_t ubCounter = 0;
    Can_TxHeader.StdId=0xFFFF;
    Can_TxHeader.ExtId=0x1314;
//使用的扩展 ID
    Can_TxHeader.IDE=CAN_ID_EXT;
//扩展模式
    Can_TxHeader.RTR=CAN_RTR_DATA;
//发送的是数据
    Can_TxHeader.DLC=8;
//数据长度为 8 字节
/* 设置要发送的数据 0-7*/

}
#endif //SEND_CANSENT_H
