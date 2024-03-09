#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void get_operations(void **operations);

sensor create_sensor(enum sensor_type sensor_type,
void *sensor_data, int *operations_idxs, int nr_operations)
{
	sensor sensor;
	sensor.sensor_type = sensor_type;
	sensor.sensor_data = sensor_data;
	sensor.operations_idxs = operations_idxs;
	sensor.nr_operations = nr_operations;
	return sensor;
}
// Afisam senzor de tip tire
void print_tire_sensor(tire_sensor *sensor)
{
	printf("Tire Sensor\n");
	printf("Pressure: %.2f\n", sensor->pressure);
	printf("Temperature: %.2f\n", sensor->temperature);
	printf("Wear Level: %d%%\n", sensor->wear_level);
	if (sensor->performace_score == 0)
		printf("Performance Score: Not Calculated\n");
	else
		printf("Performance Score: %d\n", sensor->performace_score);
}
// Afisam senzor de tip power management unit
void print_pmu_sensor(power_management_unit *sensor)
{
	printf("Power Management Unit\n");
	printf("Voltage: %.2f\n", sensor->voltage);
	printf("Current: %.2f\n", sensor->current);
	printf("Power Consumption: %.2f\n", sensor->power_consumption);
	printf("Energy Regen: %d%%\n", sensor->energy_regen);
	printf("Energy Storage: %d%%\n", sensor->energy_storage);
}
// Verificam tipul de senzor
void print_sensor(sensor *sensor)
{
	if (sensor->sensor_type == TIRE) {
		print_tire_sensor(sensor->sensor_data);
	} else if (sensor->sensor_type == PMU) {
		print_pmu_sensor((power_management_unit *)sensor->sensor_data);
	}
}
// "Sortam" vectorul de senzori
int *sort_index_sensor_data(sensor *sensors, int num_sensors)
{
	int *idx = malloc(num_sensors * sizeof(int));
	int count = 0;
	for (int i = 0; i < num_sensors; i++) {
		//intai luam in ordine toti senzorii de tip PMU
		if (sensors[i].sensor_type == PMU) {
			idx[count] = i;
			count++;
		}
	}
	for (int i = 0; i < num_sensors; i++) {
		//ii adaugam in continuare pe cei de tip TIRE
		if (sensors[i].sensor_type == TIRE) {
			idx[count] = i;
			count++;
		}
	}
	return idx;
}
// Se efectueaza operatiile senzorului in ordine
void process_sensor_operations(sensor **sensors, int n, int idx, int *idx_array)
{
	void **operations = malloc(8 * sizeof(void *));
	get_operations(operations);
	int i = idx_array[idx];
	sensor *current_sensor = *sensors + i;
	for (int j = 0; j < current_sensor->nr_operations; j++) {
	void (*op_func)(void *) = operations[current_sensor->operations_idxs[j]];
		op_func(current_sensor->sensor_data);
	}
	free(operations);
}
// Stergem toti senzorii care contin valori eronate
void delete_invalid_sensors(sensor **sensors, int *nr_sensors)
{
	int i = 0;
	while (i < *nr_sensors) {
		sensor *sensorr = (*sensors) + i;
		if (sensorr->sensor_type == TIRE) {
			tire_sensor *tire_sensorr = sensorr->sensor_data;
			if (tire_sensorr->pressure < 19 || tire_sensorr->pressure > 28 ||
				tire_sensorr->temperature < 0 ||
				tire_sensorr->temperature > 120 ||
				tire_sensorr->wear_level < 0 ||
				tire_sensorr->wear_level > 100) {
				free(sensorr->operations_idxs);
				free(sensorr->sensor_data);
				for (int j = i; j < *nr_sensors - 1; j++)
					*(*sensors + j) = *((*sensors) + j + 1);
				(*nr_sensors)--;
				continue;
			}
		} else if (sensorr->sensor_type == PMU) {
			power_management_unit *pmu_sensor =
			(power_management_unit *)sensorr->sensor_data;
			if (pmu_sensor->voltage < 10 || pmu_sensor->voltage > 20 ||
				pmu_sensor->current < -100 ||
				pmu_sensor->current > 100 ||
				pmu_sensor->power_consumption < 0 ||
				pmu_sensor->power_consumption > 1000 ||
				pmu_sensor->energy_regen < 0 ||
				pmu_sensor->energy_regen > 100 ||
				pmu_sensor->energy_storage < 0 ||
				pmu_sensor->energy_storage > 100) {
				free(sensorr->operations_idxs);
				free(sensorr->sensor_data);
				for (int j = i; j < *nr_sensors - 1; j++)
					*((*sensors) + j) = *((*sensors) + j + 1);
				(*nr_sensors)--;
				continue;
			}
		}
		i++;
	}
	*sensors = realloc(*sensors, *nr_sensors * sizeof(sensor));
}

void close_program(sensor **sensors, int nr_sensors, int **idxx)
{
	// eliberam memoria alocata senzorilor
	free(*idxx);
	for (int i = 0; i < nr_sensors; i++) {
		free((*sensors+i)->sensor_data);
		free((*sensors+i)->operations_idxs);
	}
	free(*sensors);
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	fp = fopen(argv[1], "rb");

	if (fp == NULL) {
		printf("Error: could not open file\n");
		return 1;
	}

	// citim numarul de senzori
	int nr_sensors;
	fread(&nr_sensors, sizeof(int), 1, fp);

	// alocam memorie pentru vectorul de senzori
	sensor *sensors = malloc(nr_sensors * sizeof(sensor));

	// citim fiecare senzor
	for (int i = 0; i < nr_sensors; i++) {
		// citim tipul senzorului
		enum sensor_type sensor_type;
		fread(&sensor_type, sizeof(sensor_type), 1, fp);

		// citim datele senzorului
		void *sensor_data;
		if (sensor_type == TIRE) {
			tire_sensor *tire_sensorr =
			(tire_sensor *)malloc(sizeof(tire_sensor));
			fread(tire_sensorr, sizeof(tire_sensor), 1, fp);
			sensor_data = tire_sensorr;
		} else if (sensor_type == PMU) {
			power_management_unit *pmu_sensor =
			(power_management_unit *)malloc(sizeof(power_management_unit));
			fread(pmu_sensor, sizeof(power_management_unit), 1, fp);
			sensor_data = pmu_sensor;
		}

		// citim numarul de operatii si vectorul de indecsi ale operatiilor
		int nr_operations;
		fread(&nr_operations, sizeof(int), 1, fp);
		int *operations_idxs = malloc(nr_operations * sizeof(int));
		fread(operations_idxs, sizeof(int), nr_operations, fp);
		// cream senzorul si il adaugam la vector
		sensors[i] = create_sensor(sensor_type, sensor_data,
		operations_idxs, nr_operations);
	}

	int *idxx = sort_index_sensor_data(sensors, nr_sensors);
	char *c = (char *)malloc(10 * sizeof(char));
	scanf("%s", c);
	while (strcmp(c, "exit") != 0) {
		if (strcmp(c, "print") == 0) {
			int idx;
			scanf("%d", &idx);
			if (idx < 0 || idx >= nr_sensors)
				printf("Index not in range!\n");
			else
				print_sensor(&sensors[idxx[idx]]);
		} else if (strcmp(c, "analyze") == 0) {
			int idx;
			scanf("%d", &idx);
			if (idx < 0 || idx >= nr_sensors)
				printf("Index not in range!\n");
			else
				process_sensor_operations(&sensors, nr_sensors, idx, idxx);
		} else if (strcmp(c, "clear") == 0) {
			delete_invalid_sensors(&sensors, &nr_sensors);
			free(idxx);
			idxx = sort_index_sensor_data(sensors, nr_sensors);
		}
		scanf("%s", c);
	}
	free(c);
	close_program(&sensors, nr_sensors, &idxx);
	fclose(fp);
	return 0;
}
