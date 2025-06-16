################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/logic/logic_config.c \
../Core/Src/logic/logic_door_control.c \
../Core/Src/logic/logic_engine.c \
../Core/Src/logic/logic_timer_switch.c \
../Core/Src/logic/logic_toggle.c 

OBJS += \
./Core/Src/logic/logic_config.o \
./Core/Src/logic/logic_door_control.o \
./Core/Src/logic/logic_engine.o \
./Core/Src/logic/logic_timer_switch.o \
./Core/Src/logic/logic_toggle.o 

C_DEPS += \
./Core/Src/logic/logic_config.d \
./Core/Src/logic/logic_door_control.d \
./Core/Src/logic/logic_engine.d \
./Core/Src/logic/logic_timer_switch.d \
./Core/Src/logic/logic_toggle.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/logic/%.o Core/Src/logic/%.su Core/Src/logic/%.cyclo: ../Core/Src/logic/%.c Core/Src/logic/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/sounds" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/effects" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/logic" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-logic

clean-Core-2f-Src-2f-logic:
	-$(RM) ./Core/Src/logic/logic_config.cyclo ./Core/Src/logic/logic_config.d ./Core/Src/logic/logic_config.o ./Core/Src/logic/logic_config.su ./Core/Src/logic/logic_door_control.cyclo ./Core/Src/logic/logic_door_control.d ./Core/Src/logic/logic_door_control.o ./Core/Src/logic/logic_door_control.su ./Core/Src/logic/logic_engine.cyclo ./Core/Src/logic/logic_engine.d ./Core/Src/logic/logic_engine.o ./Core/Src/logic/logic_engine.su ./Core/Src/logic/logic_timer_switch.cyclo ./Core/Src/logic/logic_timer_switch.d ./Core/Src/logic/logic_timer_switch.o ./Core/Src/logic/logic_timer_switch.su ./Core/Src/logic/logic_toggle.cyclo ./Core/Src/logic/logic_toggle.d ./Core/Src/logic/logic_toggle.o ./Core/Src/logic/logic_toggle.su

.PHONY: clean-Core-2f-Src-2f-logic

