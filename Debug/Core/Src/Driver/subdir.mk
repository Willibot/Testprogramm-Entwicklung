################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Driver/cy8cmbr3108.c 

OBJS += \
./Core/Src/Driver/cy8cmbr3108.o 

C_DEPS += \
./Core/Src/Driver/cy8cmbr3108.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Driver/%.o Core/Src/Driver/%.su Core/Src/Driver/%.cyclo: ../Core/Src/Driver/%.c Core/Src/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/sounds" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/effects" -I"E:/CAD/NX12/Eigene Projekte/Sensortaster/Programme/Testprogramm Entwicklung/Core/Inc/logic" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Driver

clean-Core-2f-Src-2f-Driver:
	-$(RM) ./Core/Src/Driver/cy8cmbr3108.cyclo ./Core/Src/Driver/cy8cmbr3108.d ./Core/Src/Driver/cy8cmbr3108.o ./Core/Src/Driver/cy8cmbr3108.su

.PHONY: clean-Core-2f-Src-2f-Driver

