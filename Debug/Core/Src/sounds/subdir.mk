################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sounds/piezo_driver.c \
../Core/Src/sounds/sound_beep.c \
../Core/Src/sounds/sound_config_mode.c \
../Core/Src/sounds/sound_double_beep.c \
../Core/Src/sounds/sound_engine.c 

OBJS += \
./Core/Src/sounds/piezo_driver.o \
./Core/Src/sounds/sound_beep.o \
./Core/Src/sounds/sound_config_mode.o \
./Core/Src/sounds/sound_double_beep.o \
./Core/Src/sounds/sound_engine.o 

C_DEPS += \
./Core/Src/sounds/piezo_driver.d \
./Core/Src/sounds/sound_beep.d \
./Core/Src/sounds/sound_config_mode.d \
./Core/Src/sounds/sound_double_beep.d \
./Core/Src/sounds/sound_engine.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sounds/%.o Core/Src/sounds/%.su Core/Src/sounds/%.cyclo: ../Core/Src/sounds/%.c Core/Src/sounds/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/sounds" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/effects" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/logic" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sounds

clean-Core-2f-Src-2f-sounds:
	-$(RM) ./Core/Src/sounds/piezo_driver.cyclo ./Core/Src/sounds/piezo_driver.d ./Core/Src/sounds/piezo_driver.o ./Core/Src/sounds/piezo_driver.su ./Core/Src/sounds/sound_beep.cyclo ./Core/Src/sounds/sound_beep.d ./Core/Src/sounds/sound_beep.o ./Core/Src/sounds/sound_beep.su ./Core/Src/sounds/sound_config_mode.cyclo ./Core/Src/sounds/sound_config_mode.d ./Core/Src/sounds/sound_config_mode.o ./Core/Src/sounds/sound_config_mode.su ./Core/Src/sounds/sound_double_beep.cyclo ./Core/Src/sounds/sound_double_beep.d ./Core/Src/sounds/sound_double_beep.o ./Core/Src/sounds/sound_double_beep.su ./Core/Src/sounds/sound_engine.cyclo ./Core/Src/sounds/sound_engine.d ./Core/Src/sounds/sound_engine.o ./Core/Src/sounds/sound_engine.su

.PHONY: clean-Core-2f-Src-2f-sounds

