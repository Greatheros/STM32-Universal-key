# 哆啦A梦的STM32按键驱动库文档  

## 1. 快速开始  
### 1.1 引入头文件  
```c
#include "Key.h"
```  

### 1.2 创建按键  
```c
Create_Key(0, GPIOA, GPIO_Pin_0);  // 创建ID=0的按键(PA0)
Create_Key(1, GPIOB, GPIO_Pin_1);  // 创建ID=1的按键(PB1)
```  

### 1.3 初始化按键系统  
```c
TIM2_ITInit();      // 启动定时器2(1ms中断)
```  

### 1.4 注册回调函数  
```c
void my_callback(KEY_STATE State) {
    if (State == KEY_SINGLE) {
        printf("单击\n");
    }
}

Key_Register(0, my_callback);  // 注册到按键0
```  

### 1.5 配置按键特性（可选）  
```c
Button_Setrep(0, keyTRUE);     // 启用重复触发
Button_RepTime(0, 150);        // 设置重复间隔150ms
```  

### 1.6 回调函数模板  
```c
void Key_Callback(KEY_STATE State) {
    switch (State) {
        case KEY_SINGLE:
            // 单击处理逻辑
            break;
        case KEY_DOUBLE:
            // 双击处理逻辑
            break;
        case KEY_LONG:
            // 长按处理逻辑
            break;
        case KEY_REPEAT:
            // 重复触发处理逻辑
            break;
        default:
            break;
    }
}
```  

### 1.7 定时器中断处理  
```c
// 在 stm32f10x_it.c 中添加以下代码
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        Key_Tick();             // 调用按键扫描函数
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```  


## 2. 功能特点  
- 目前仅适配STM32F103C8标准库环境
- 每个按键可注册多个回调函数  
- 支持共阴和共阳两种按键连接方式，默认共阴极连接
- 支持无限数量按键，默认最多支持3个按键（可通过`KEY_MAX`宏定义修改）  
- 检测多种按键状态：    
  - 单击（`KEY_SINGLE`）  
  - 双击（`KEY_DOUBLE`）  
  - 长按（`KEY_LONG`）  
  - 重复触发（`KEY_REPEAT`）  
- 可配置参数：  
  - 消抖时间（`KEY_Filter`，默认20ms）  
  - 长按时间（`KEY_Longtime`，默认1500ms）  
  - 双击间隔时间（`KEY_Doubletime`，默认300ms）  
- 支持按键重复触发功能（长按状态下周期性触发）  


## 3. 关键定义与结构  
### 3.1 一些关键宏定义  
```c
#define KEY_MAX  3           // 最大按键数量
#define KEY_Press    1        // 按键按下
#define KEY_NoPress  0        // 按键未按下
#define KEY_Filter   20       // 消抖时长(ms)
#define KEY_Longtime 1500     // 按键长按时间(ms)
#define KEY_Doubletime 300    // 按键双击间隔(ms)
#define keyTRUE  1
#define keyFALSE 0
#define Common_Yin 1          // 按键连接方式，1为共阴，0为共阳
```  

### 3.2 按键状态枚举  
```c
typedef enum {
    KEY_IDLE,        // 空闲
    KEY_PRESS,       // 按下
    KEY_RELEASE,     // 释放
    
    /* 可外部使用的状态 */
    KEY_SINGLE,      // 单击
    KEY_DOUBLE,      // 双击
    KEY_LONG,        // 长按
    KEY_REPEAT,      // 重复
} KEY_STATE;
```  


## 4. 使用示例  
```c
#include "stm32f10x.h"
#include "Key.h"
#include "serial.h"

/* id为0按键的回调函数1 */
void Key0_callback1(KEY_STATE State) {
    if (State == KEY_SINGLE) {
        printf("这是KEY0单击1\r\n");    
    }
    if (State == KEY_DOUBLE) {
        printf("这是KEY0双击1\r\n");    
    }
    if (State == KEY_LONG) {
        printf("这是KEY0长按1\r\n");    
    }
    if (State == KEY_REPEAT) {
        printf("这是KEY0重复1\r\n");    
    }
}

/* id为0按键的回调函数2 */
void Key0_callback2(KEY_STATE State) {
    if (State == KEY_SINGLE) {
        printf("这是KEY0单击2\r\n");    
    }
    if (State == KEY_DOUBLE) {
        printf("这是KEY0双击2\r\n");    
    }
    if (State == KEY_LONG) {
        printf("这是KEY0长按2\r\n");    
    }
    if (State == KEY_REPEAT) {
        printf("这是KEY0重复2\r\n");    
    }
}

/* id为2按键的回调函数 */
void Key2_callback(KEY_STATE State) {
    if (State == KEY_SINGLE) {
        printf("这是KEY2单击\r\n");    
    }
    if (State == KEY_DOUBLE) {
        printf("这是KEY2双击\r\n");    
    }
    if (State == KEY_LONG) {
        printf("这是KEY2长按\r\n");    
    }
    if (State == KEY_REPEAT) {
        printf("这是KEY2重复\r\n");    
    }
}

int main(void) {
    Serial_Init();                // 初始化串口
    
    /* 创建按键 */
    Create_Key(0, GPIOA, GPIO_Pin_0);  // 创建id为0的按键
    Create_Key(2, GPIOB, GPIO_Pin_1);  // 创建id为2的按键
    
    TIM2_ITInit();                  // 初始化定时器2
    
    /* 配置按键功能 */
    Button_Setrep(2, keyTRUE);      // 设置id为2的按键启用重复触发
    
    /* 注册回调函数 */
    Key_Register(0, Key0_callback1); // 为id=0按键注册回调函数1
    Key_Register(0, Key0_callback2); // 为id=0按键注册回调函数2
    Key_Register(2, Key2_callback);  // 为id=2按键注册回调函数
    
    printf("按键驱动初始化完成\r\n");
    
    while (1) {
        // 主循环中可处理其他任务
    }
}

/* 定时器2中断处理函数，需在stm32f10x_it.c中实现 */
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        Key_Tick();             // 调用按键扫描函数
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```  


## 5. 注意事项  
1. **按键数量限制**：最大按键数量由`KEY_MAX`宏定义决定，默认值为3，如需增加请修改此宏。  
2. **共阴/共阳设置**：共阴连接时`#define Common_Yin 1`（默认），共阳连接时`#define Common_Yin 0`。  
3. **定时器配置**：`Key_Tick()`必须在1ms定时器中断中调用，示例使用TIM2，如需更换定时器请修改`TIM2_ITInit()`。  
4. **回调函数执行顺序**：多个回调函数按注册顺序执行，后注册的函数先执行（链表头插法）。  
5. **按键状态处理**：长按状态下若启用重复触发（`Button_Setrep(id, keyTRUE)`），可通过`Button_RepTime(id, time)`设置间隔。  
  


## 6. 宏定义配置说明  
| 宏定义          | 默认值 | 说明                          |  
|-----------------|--------|-------------------------------|  
| `KEY_MAX`       | 3      | 最大按键数量                  |  
| `KEY_Filter`    | 20     | 按键消抖时间(ms)              |  
| `KEY_Longtime`  | 1500   | 长按判定时间(ms)              |  
| `KEY_Doubletime`| 300    | 双击间隔时间(ms)              |  
| `Common_Yin`    | 1      | 按键连接方式（1=共阴，0=共阳） |  


## 7. 版本说明  
**当前版本：V1.0.0**  
- 初始版本，支持单击、双击、长按、重复触发四种状态检测。  
- 实现多回调函数注册机制及共阴/共阳连接方式支持。  

 

