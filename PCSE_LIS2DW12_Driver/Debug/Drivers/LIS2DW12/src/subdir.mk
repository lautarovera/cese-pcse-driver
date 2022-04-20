################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LIS2DW12/src/lis2dw12.c \
../Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.c 

OBJS += \
./Drivers/LIS2DW12/src/lis2dw12.o \
./Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.o 

C_DEPS += \
./Drivers/LIS2DW12/src/lis2dw12.d \
./Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LIS2DW12/src/%.o Drivers/LIS2DW12/src/%.su: ../Drivers/LIS2DW12/src/%.c Drivers/LIS2DW12/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lautaro/CESE/2_Bimestre/PCSE/cese-pcse-driver/PCSE_LIS2DW12_Driver/Drivers/LIS2DW12/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LIS2DW12-2f-src

clean-Drivers-2f-LIS2DW12-2f-src:
	-$(RM) ./Drivers/LIS2DW12/src/lis2dw12.d ./Drivers/LIS2DW12/src/lis2dw12.o ./Drivers/LIS2DW12/src/lis2dw12.su ./Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.d ./Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.o ./Drivers/LIS2DW12/src/lis2dw12_stm32f429xx_port.su

.PHONY: clean-Drivers-2f-LIS2DW12-2f-src

