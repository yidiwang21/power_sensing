/*
 * One arduino board for one GPU power sensing
 */

#include <Adafruit_INA260.h>
#include <SoftwareSerial.h>

const int sensor_count = 2;

typedef struct sensor_t {
	Adafruit_INA260 ina260;
	float current = 0;
	float voltage = 0;
	float power = 0;
}sensor_t;

typedef struct overall_data_t {
	float current = 0;
	float voltage = 0;
	float power = 0;
}overall_data_t;

sensor_t sensor[sensor_count];
overall_data_t overall_data;

void setup() {
	Serial.begin(115200);
	// Wait until serial port is opened
	while (!Serial) { delay(10); }

	for (int i = 0; i < sensor_count; i++) {
		sensor[i].ina260 = Adafruit_INA260();
	}

	sensor[0].ina260.begin(0x41);
	sensor[1].ina260.begin(0x40);
	
	// output csv headers
	// unit: mA, mV, mW
	for (int i = 0; i < sensor_count; i++) {
		Serial.print("s");
		Serial.print(i);
		Serial.print(".current");
		Serial.print("\t");
		Serial.print("s");
		Serial.print(i);
		Serial.print(".voltage");
		Serial.print("\t");
		Serial.print("s");
		Serial.print(i);
		Serial.print(".power");
		Serial.print("\t");
	}
	Serial.print("overall.current");
	Serial.print("\t");
	Serial.print("overall.voltage");
	Serial.print("\t");
	Serial.println("overall.power");
}

int row_count = 0;

void loop() {
	if (row_count < 500) {
		// read sensor data
		// compute overall data
		overall_data.current = 0;
		overall_data.voltage = 0;
		overall_data.power = 0;
		for (int i = 0; i < sensor_count; i++) {
			sensor[i].current = sensor[i].ina260.readCurrent();
			sensor[i].voltage = sensor[i].ina260.readBusVoltage();
			sensor[i].power = sensor[i].ina260.readPower();

			overall_data.current += sensor[i].current;
			overall_data.voltage += sensor[i].voltage;
			overall_data.power += sensor[i].power;
		}	

		// print in csv format
		for (int i = 0; i < sensor_count; i++){
			Serial.print(sensor[i].current);
			Serial.print("\t");
			Serial.print(sensor[i].voltage);
			Serial.print("\t");
			Serial.print(sensor[i].power);
			Serial.print("\t");
		}
		Serial.print(overall_data.current);
		Serial.print("\t");
		Serial.print(overall_data.voltage);
		Serial.print("\t");
		Serial.println(overall_data.power);

		// Serial.println();
		delay(1000);
		row_count++;
	}
}