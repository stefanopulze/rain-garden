#ifdef INFLUXDB_ENABLE
void setupInfluxdb()
{
    influx.setDb(INFLUXDB_DATABASE);
}


InfluxData buildInfluxData(String measure, byte sensorId, float value)
{
    InfluxData row(measure);
    char idPadded[2];
    sprintf(idPadded, "%02d", sensorId);
    row.addTag("sensor", "sensor-" + String(idPadded));
    row.addValue("value", value);
    return row;
}

InfluxData buildInfluxData(String measure, String sensorId, float value)
{
    InfluxData row(measure);
    // char idPadded[2];
    // sprintf(idPadded, "%02d", sensorId);
    // row.addTag("sensor", "sensor-" + String(idPadded));
    row.addTag("sensor", sensorId);
    row.addValue("value", value);
    return row;
}

void sendToInflux(SensorData *data)
{
    InfluxData temperature = buildInfluxData("temperature", "sensor-01", data->temperature);
    influx.prepare(temperature);

    InfluxData humidity = buildInfluxData("humidity", "sensor-01", data->humidity);
    influx.prepare(humidity);

    InfluxData pressure = buildInfluxData("pressure", "sensor-01", data->pressure);
    influx.prepare(pressure);

    InfluxData soil = buildInfluxData("soil", "sensor-01", data->soil);
    influx.prepare(soil);

    // writes all prepared measurements with a single request into db.
    boolean success = influx.write();
}
#endif
