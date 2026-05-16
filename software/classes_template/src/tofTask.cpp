#include "tofTask.h"
#include "cmsis_os.h"
#include "rc_config.h"
#include "main.h"
#include "car.h"

/* Sensor lib depends on used sensor. Originally designed to use vl53l1x */
#if TOF_SENSOR_TYPE == TOF_SENSOR_VL53L0X
    #include "vl53l0xSensor.h"
    typedef Vl53l0xSensor TofSensor;
#else
    #include "vl53l1xSensor.h"
    typedef Vl53l1xSensor TofSensor;
#endif

extern I2C_HandleTypeDef hi2c1;
extern Car car;

#if TOF_SENSOR_TYPE == TOF_SENSOR_VL53L0X
static Vl53l0xSensor tofFront(&hi2c1, TOF_FRONT_ADDR, XSHUT_FRONT_GPIO_PORT, XSHUT_FRONT_PIN);
static Vl53l0xSensor tofLeft (&hi2c1, TOF_LEFT_ADDR,  XSHUT_LEFT_GPIO_PORT,  XSHUT_LEFT_PIN);
static Vl53l0xSensor tofRight(&hi2c1, TOF_RIGHT_ADDR, XSHUT_RIGHT_GPIO_PORT, XSHUT_RIGHT_PIN);

void StartToFTask(void* argument){
    tofFront.disable();
    tofLeft.disable();
    tofRight.disable();
    osDelay(TOF_STARTUP_DELAY);

    tofFront.enable();
    tofFront.init();

   tofLeft.enable();
    tofLeft.init();

    tofRight.enable();
    tofRight.init();

    for(;;){
        tofFront.processData();
        tofLeft.processData();
        tofRight.processData();

        DistanceMap dist = {
            tofFront.getDistance(),
            tofLeft.getDistance(),
            tofRight.getDistance()
        };

        car.setDistances(dist);
        osDelay(TOF_READOUT_DELAY);
    }
}
#endif

#if TOF_SENSOR_TYPE == TOF_SENSOR_VL53L1X
static Vl53l1xSensor tofFront(&hi2c1, TOF_FRONT_ADDR, XSHUT_FRONT_GPIO_PORT, XSHUT_FRONT_PIN);
static Vl53l1xSensor tofLeft (&hi2c1, TOF_LEFT_ADDR, XSHUT_LEFT_GPIO_PORT,  XSHUT_LEFT_PIN);
static Vl53l1xSensor tofRight(&hi2c1, TOF_RIGHT_ADDR, XSHUT_RIGHT_GPIO_PORT, XSHUT_RIGHT_PIN);

void StartToFTask(void * argument){
    VL53L1X* sensors[] = {
        &tofFront.getSensor(),
        &tofLeft.getSensor(),
        &tofRight.getSensor()
    };
    TOF_BootMultipleSensors(sensors, 3);

    for(;;){
        tofFront.processData();
        tofLeft.processData();
        tofRight.processData();

        car.setDistances(dist);
        osDelay(TOF_READOUT_DELAY);
    }
}
#endif