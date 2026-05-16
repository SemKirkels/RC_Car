#include "healthTask.h"
#include "main.h"
#include "cmsis_os.h"
#include "rc_config.h"
#include "car.h"
#include "mcp9600Sensor.h"
#include "mcp9808Sensor.h"
#include "bms.h"

extern I2C_HandleTypeDef    hi2c1;
extern Car car;

static Mcp9600  motorTemp(&hi2c1, TEMP_MOTOR_ADDR, Mcp9600::ProbeType::K);
static Mcp9808  ambientTemp(&hi2c1, TEMP_AMBIENT_ADDR);
static Bms      bms(&hi2c1);

extern "C" void StartHealthTask(void* argument){
    motorTemp.init();
    ambientTemp.init();
    bms.init();

    for(;;){
        motorTemp.processData();
        ambientTemp.processData();
        bms.processData();

        HealthMap health = {};
        health.motorTempHot       = motorTemp.getHotJunc();
        health.motorTempCold      = motorTemp.getColdJunc();

        health.ambientTemp        = ambientTemp.getTemperature();

        health.cellVoltage[0]     = bms.getCellVoltage(0);
        health.cellVoltage[1]     = bms.getCellVoltage(1);
        health.cellVoltage[2]     = bms.getCellVoltage(2);
        health.totalVoltage       = bms.getTotalVoltage();
        health.current            = bms.getCurrent();
        health.bmsStatus          = bms.getStatus();


        car.setHealth(health);

        osDelay(HEALTH_READOUT_DELAY);
    }
}