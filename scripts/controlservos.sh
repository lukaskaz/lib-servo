#!/bin/bash

SERVOMIN=1
SERVOMAX=4
#SERVOPOSMINNS=600000
SERVOPOSMINNS=800000
SERVOPOSMAXNS=2000000
SERVOPOSMAX=15
SERVONUM=$1
SERVOPOS=$2
INFO=$3
PWMNUM=$(($SERVONUM-1))
#PWMDRIVER=/sys/class/pwm/pwmchip2
PWMDRIVER=/sys/class/pwm/pwmchip0
PWMTOUPD=$PWMDRIVER/pwm${PWMNUM}
PERIODNS=20000000
PROCENTMAX=100
DUTYCYCLENS=$(((SERVOPOSMAXNS-SERVOPOSMINNS)/PROCENTMAX*$SERVOPOS+SERVOPOSMINNS))

function _initializepwm()
{
	echo $PWMNUM > $PWMDRIVER/export
	if [ $? -ne 0 ]; then
		echo "[ERROR] Cannot initialize pwm/servo: $PWMNUM/$SERVONUM"
		exit 3
	fi
	sleep 0.5
	echo $PERIODNS > $PWMTOUPD/period
}

function _setpwm()
{
	#echo $PERIODINNS > $PWMTOUPD/period
	echo $DUTYCYCLENS > $PWMTOUPD/duty_cycle
	#echo 1 > $PWMTOUPD/duty_cycle
}

function _showpwms()
{
	echo "=========="
	echo "Info about enabled PWMs"
	cat /sys/kernel/debug/pwm | grep -i sysfs
	echo "=========="
}

## main ##

if [ -z "$SERVONUM" -o -z "$SERVOPOS" ]; then
	echo -e "Usage:\n./$(basename $0) SERVONUM[$SERVOMIN-$SERVOMAX] SERVOPOSPERC[0-100]"
	exit 1
fi

if [ $SERVOPOS -lt 0 -o $SERVOPOS -gt 100 ]; then
	echo "[ERROR] Servos position is in range [0-100]%, given: $SERVOSPOS"
	exit 2
elif [ ! -e $PWMDRIVER ]; then
	echo "[ERROR] Servos driver not working [$PWMDRIVER]"
	exit 3
elif [ $SERVONUM -lt $SERVOMIN -o $SERVONUM -gt $SERVOMAX ]; then
	echo "[ERROR] Servos to control are [$SERVOMIN - $SERVOMAX], given: $SERVONUM"
	exit 4
elif [ ! -e $PWMTOUPD ]; then
	_initializepwm
fi
_setpwm
if [ -n "$INFO" ]; then
	_showpwms
	echo "Set servo #${SERVONUM} to position ${SERVOPOS}%->${DUTYCYCLENS}[ns]"
	echo "[OK] Completed"
fi

#end


