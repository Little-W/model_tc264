################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/User/Encoder.c \
../src/User/Function.c \
../src/User/LQ_CCU6.c \
../src/User/myDuty.c \
../src/User/myUivariables.c \
../src/User/myVariables.c \
../src/User/myctrl.c \
../src/User/myinit.c \
../src/User/pid.c 

COMPILED_SRCS += \
./src/User/Encoder.src \
./src/User/Function.src \
./src/User/LQ_CCU6.src \
./src/User/myDuty.src \
./src/User/myUivariables.src \
./src/User/myVariables.src \
./src/User/myctrl.src \
./src/User/myinit.src \
./src/User/pid.src 

C_DEPS += \
./src/User/Encoder.d \
./src/User/Function.d \
./src/User/LQ_CCU6.d \
./src/User/myDuty.d \
./src/User/myUivariables.d \
./src/User/myVariables.d \
./src/User/myctrl.d \
./src/User/myinit.d \
./src/User/pid.d 

OBJS += \
./src/User/Encoder.o \
./src/User/Function.o \
./src/User/LQ_CCU6.o \
./src/User/myDuty.o \
./src/User/myUivariables.o \
./src/User/myVariables.o \
./src/User/myctrl.o \
./src/User/myinit.o \
./src/User/pid.o 


# Each subdirectory must supply rules for building sources it contributes
src/User/%.src: ../src/User/%.c src/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fF:/9-embeded/infineon/LQ_TC264DA_LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/src\/User\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

src/User/%.o: ./src/User/%.src src/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-User

clean-src-2f-User:
	-$(RM) ./src/User/Encoder.d ./src/User/Encoder.o ./src/User/Encoder.src ./src/User/Function.d ./src/User/Function.o ./src/User/Function.src ./src/User/LQ_CCU6.d ./src/User/LQ_CCU6.o ./src/User/LQ_CCU6.src ./src/User/myDuty.d ./src/User/myDuty.o ./src/User/myDuty.src ./src/User/myUivariables.d ./src/User/myUivariables.o ./src/User/myUivariables.src ./src/User/myVariables.d ./src/User/myVariables.o ./src/User/myVariables.src ./src/User/myctrl.d ./src/User/myctrl.o ./src/User/myctrl.src ./src/User/myinit.d ./src/User/myinit.o ./src/User/myinit.src ./src/User/pid.d ./src/User/pid.o ./src/User/pid.src

.PHONY: clean-src-2f-User

