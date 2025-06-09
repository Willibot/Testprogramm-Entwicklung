################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/effects/color_utils.c \
../Core/Src/effects/led_driver.c \
../Core/Src/effects/led_effect_blink.c \
../Core/Src/effects/led_effect_breathe.c \
../Core/Src/effects/led_effect_chase_left.c \
../Core/Src/effects/led_effect_chase_right.c \
../Core/Src/effects/led_effect_config_mode.c \
../Core/Src/effects/led_effect_engine.c \
../Core/Src/effects/led_effect_flash_alt.c \
../Core/Src/effects/led_effect_single_led.c \
../Core/Src/effects/led_effect_solid.c 

OBJS += \
./Core/Src/effects/color_utils.o \
./Core/Src/effects/led_driver.o \
./Core/Src/effects/led_effect_blink.o \
./Core/Src/effects/led_effect_breathe.o \
./Core/Src/effects/led_effect_chase_left.o \
./Core/Src/effects/led_effect_chase_right.o \
./Core/Src/effects/led_effect_config_mode.o \
./Core/Src/effects/led_effect_engine.o \
./Core/Src/effects/led_effect_flash_alt.o \
./Core/Src/effects/led_effect_single_led.o \
./Core/Src/effects/led_effect_solid.o 

C_DEPS += \
./Core/Src/effects/color_utils.d \
./Core/Src/effects/led_driver.d \
./Core/Src/effects/led_effect_blink.d \
./Core/Src/effects/led_effect_breathe.d \
./Core/Src/effects/led_effect_chase_left.d \
./Core/Src/effects/led_effect_chase_right.d \
./Core/Src/effects/led_effect_config_mode.d \
./Core/Src/effects/led_effect_engine.d \
./Core/Src/effects/led_effect_flash_alt.d \
./Core/Src/effects/led_effect_single_led.d \
./Core/Src/effects/led_effect_solid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/effects/%.o Core/Src/effects/%.su Core/Src/effects/%.cyclo: ../Core/Src/effects/%.c Core/Src/effects/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/sounds" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/effects" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-effects

clean-Core-2f-Src-2f-effects:
	-$(RM) ./Core/Src/effects/color_utils.cyclo ./Core/Src/effects/color_utils.d ./Core/Src/effects/color_utils.o ./Core/Src/effects/color_utils.su ./Core/Src/effects/led_driver.cyclo ./Core/Src/effects/led_driver.d ./Core/Src/effects/led_driver.o ./Core/Src/effects/led_driver.su ./Core/Src/effects/led_effect_blink.cyclo ./Core/Src/effects/led_effect_blink.d ./Core/Src/effects/led_effect_blink.o ./Core/Src/effects/led_effect_blink.su ./Core/Src/effects/led_effect_breathe.cyclo ./Core/Src/effects/led_effect_breathe.d ./Core/Src/effects/led_effect_breathe.o ./Core/Src/effects/led_effect_breathe.su ./Core/Src/effects/led_effect_chase_left.cyclo ./Core/Src/effects/led_effect_chase_left.d ./Core/Src/effects/led_effect_chase_left.o ./Core/Src/effects/led_effect_chase_left.su ./Core/Src/effects/led_effect_chase_right.cyclo ./Core/Src/effects/led_effect_chase_right.d ./Core/Src/effects/led_effect_chase_right.o ./Core/Src/effects/led_effect_chase_right.su ./Core/Src/effects/led_effect_config_mode.cyclo ./Core/Src/effects/led_effect_config_mode.d ./Core/Src/effects/led_effect_config_mode.o ./Core/Src/effects/led_effect_config_mode.su ./Core/Src/effects/led_effect_engine.cyclo ./Core/Src/effects/led_effect_engine.d ./Core/Src/effects/led_effect_engine.o ./Core/Src/effects/led_effect_engine.su ./Core/Src/effects/led_effect_flash_alt.cyclo ./Core/Src/effects/led_effect_flash_alt.d ./Core/Src/effects/led_effect_flash_alt.o ./Core/Src/effects/led_effect_flash_alt.su ./Core/Src/effects/led_effect_single_led.cyclo ./Core/Src/effects/led_effect_single_led.d ./Core/Src/effects/led_effect_single_led.o ./Core/Src/effects/led_effect_single_led.su ./Core/Src/effects/led_effect_solid.cyclo ./Core/Src/effects/led_effect_solid.d ./Core/Src/effects/led_effect_solid.o ./Core/Src/effects/led_effect_solid.su

.PHONY: clean-Core-2f-Src-2f-effects

